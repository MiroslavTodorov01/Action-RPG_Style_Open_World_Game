// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Item.h"
#include "Animation/AnimMontage.h"
#include "Items/Weapons/Weapon.h"
#include "Blueprint/UserWidget.h"
#include "HUD/PlayerHUD.h"
#include "HUD/PlayerOverlay.h"
#include "Components/AttributeComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Hair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	ValueToSpawnAfterDash = 1000.f;

	Tags.Add(FName("Player"));

	InitializePlayerOverlay();
}

void APlayerCharacter::InitializePlayerOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());

		if (PlayerHUD)
		{
			PlayerOverlay = PlayerHUD->GetPlayerOverlay();

			if (PlayerOverlay)
			{
				PlayerOverlay->SetHealthBarPercentage(GetCurrentHealthPercentage());
				PlayerOverlay->SetStaminaBarPercentage(GetCurrentStaminaPercentage());
				PlayerOverlay->SetGold(0.f);
				PlayerOverlay->SetSouls(0.f);
			}
		}
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);	

	PlayerInputComponent->BindAxis(TEXT("Forward/Backward"), this, &APlayerCharacter::MoveForwardBackward);

	PlayerInputComponent->BindAxis(TEXT("MoveLeft/Right"), this, &APlayerCharacter::MoveLeftRight);

	PlayerInputComponent->BindAction(TEXT("Walk"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Walk);

	PlayerInputComponent->BindAction(TEXT("Walk"), EInputEvent::IE_Released, this, &APlayerCharacter::DisableWalk);

	PlayerInputComponent->BindAxis(TEXT("LookLeft/Right"), this, &APlayerCharacter::Turn);

	PlayerInputComponent->BindAxis(TEXT("LookUp/Down"), this, &APlayerCharacter::LookUpDown);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Dash);

	PlayerInputComponent->BindAction(TEXT("PickUpItems"), IE_Pressed, this, &APlayerCharacter::PickUp);

	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &APlayerCharacter::Attack);

	PlayerInputComponent->BindAction(TEXT("HeavyAttack"), IE_Pressed, this, &APlayerCharacter::HeavyAttack);

	PlayerInputComponent->BindAction(TEXT("Equipt/Unequipt"), IE_Pressed, this, &APlayerCharacter::EquiptOrUnequipt);
}

void APlayerCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	SetWeaponCollision(ECollisionEnabled::NoCollision);

	if (IsAlive())
	{
		ActionState = EActionState::EAS_HitReact;
	}
	else
	{
		Die();
	}
}

void APlayerCharacter::Die()
{
	Super::Die();
	DisableCapsulCollision();
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECR_Ignore);

	ActionState = EActionState::EAS_Dead;
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);

	if (PlayerOverlay && Attributes)
	{
		PlayerOverlay->SetHealthBarPercentage(Attributes->CalculatePercentageOfHealth());
	}

	return DamageAmount;
}

void APlayerCharacter::SetItem(AItem* item)
{
	Item = item;
}

bool APlayerCharacter::IfItemIsAttached() const
{
	return Item ? true : false;
}

void APlayerCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;

	if (CombatTarget)
	{
		CombatTarget = nullptr;
	}

	//SetWeaponCollision(ECollisionEnabled::NoCollision);
}

void APlayerCharacter::MoveForwardBackward(float Value)
{
	if (ActionState != EActionState::EAS_Unoccupied)
	{
		return;
	}

	if (Controller != nullptr)
	{
		//AddMovementInput(Camera->GetForwardVector(), Value);

		FRotator YawRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);

		FVector direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(direction, Value);
	}
}

void APlayerCharacter::MoveLeftRight(float Value) 
{
	if (ActionState != EActionState::EAS_Unoccupied)
	{
		return;
	}

	if (Controller != nullptr) 
	{
		//AddMovementInput(Camera->GetRightVector(), Value);

		FRotator PitchRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);

		FVector direction = FRotationMatrix(PitchRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(direction, Value);
	}
}

void APlayerCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void APlayerCharacter::LookUpDown(float Value) 
{
	AddControllerPitchInput(Value);
}

void APlayerCharacter::Walk() 
{
	GetCharacterMovement()->MaxWalkSpeed = 300.f;

}

void APlayerCharacter::DisableWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void APlayerCharacter::Jump() 
{
	if (ActionState != EActionState::EAS_Unoccupied)
	{
		return;
	}

	if (CanJump())
	{
		GetCharacterMovement()->DoJump(false);
	}
}

void APlayerCharacter::Dash() 
{

	if (ActionState == EActionState::EAS_Attacking || ActionState == EActionState::EAS_EquipOrNot)
	{
		return;
	}

	if (DashNS)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DashNS, GetActorLocation(), GetActorRotation());

		TArray<FHitResult> HitActors;

		FVector EndPoint = GetActorForwardVector() * 1000 + GetActorLocation();

		bool IfHit = GetWorld()->LineTraceMultiByChannel(HitActors, GetActorLocation(), EndPoint, ECollisionChannel::ECC_GameTraceChannel1);

		if (IfHit)
		{
			ValueToSpawnAfterDash = abs((HitActors[HitActors.Num() - 1].GetActor()->GetActorLocation() - GetActorLocation()).X);

			ValueToSpawnAfterDash -= 80.f;

			if (ValueToSpawnAfterDash < 0.f)
			{
				ValueToSpawnAfterDash = 0.f;
			}
		}
		else
		{
			ValueToSpawnAfterDash = 1000.f;
		}

		DealDamageFromDash(HitActors);

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		SetActorHiddenInGame(true);

		if (Weapon)
		{
			Weapon->SetActorHiddenInGame(true);
		}
		
		DisableInput(PlayerController);

		GetWorld()->GetTimerManager().SetTimer(TimeHandle, this, &APlayerCharacter::CalculateDash, 0.4f);
	}
}

bool APlayerCharacter::IfActorAlreadyDemaged(const AActor* CurrentActor)
{
	for (const AActor* IgnorActor : ActorsToIgnor)
	{
		if (IgnorActor == CurrentActor)
		{
			return true;
		}
	}

	return false;
}

void APlayerCharacter::DealDamageFromDash(const TArray<FHitResult>& HitActors)
{
	ActorsToIgnor.Add(this);

	for (const FHitResult& hits : HitActors)
	{
		if (!hits.bBlockingHit)
		{
			if (IfActorAlreadyDemaged(hits.GetActor()))
			{
				continue;
			}

			UGameplayStatics::ApplyDamage(hits.GetActor(), DashDamage, this->GetInstigatorController(), this, UDamageType::StaticClass());

			ActorsToIgnor.Add(hits.GetActor());

			UE_LOG(LogTemp, Display, TEXT("Damaged: %s"), *(hits.GetActor()->GetName()));
		}
	}
}

void APlayerCharacter::CalculateDash()
{
	FVector DashLocation = FVector(ValueToSpawnAfterDash, 0.f, 0.f);

	SetActorHiddenInGame(false);

	if (Weapon)
	{
		Weapon->SetActorHiddenInGame(false);
	}

	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	AddActorLocalOffset(DashLocation);

	ActorsToIgnor.Empty();
}

void APlayerCharacter::PickUp() 
{
	if (Item)
	{
		Weapon = Cast<AWeapon>(Item);

		if (!Weapon)
		{
			return;
		}

		Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);

		SetWeaponCollision(ECollisionEnabled::NoCollision);

		CharacterState = ECharacterState::ECS_OneHendedWeaponEquipped;
	}
}

bool APlayerCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}



void APlayerCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void APlayerCharacter::Attack()
{
	if (!CanAttack())
	{
		return;
	}
	
	ActionState = EActionState::EAS_Attacking;

	PlayAttackMontage();
	
}

void APlayerCharacter::HeavyAttack()
{
	if (!CanAttack())
	{
		return;
	}

	ActionState = EActionState::EAS_Attacking;

	PlaySectionFromMontage(GetAttackMontage(), FName("AttackCombo1"));
}

void APlayerCharacter::EnableInputFromAnimation() 
{
	ActionState = EActionState::EAS_Unoccupied;
}

void APlayerCharacter::WeaponAttachToSocket()
{
	FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, true);

	if (CharacterState == ECharacterState::ECS_OneHendedWeaponEquipped)
	{
		Weapon->Equip(GetMesh(), FName("IdleSwordHoldSocket"), this, this);

		CharacterState = ECharacterState::ECS_Unequipped;
	}
	else if (CharacterState == ECharacterState::ECS_Unequipped)
	{
		Weapon->Equip(GetMesh(), FName("EquipTheSwordHoldSocket"), this, this);

		Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);

		CharacterState = ECharacterState::ECS_OneHendedWeaponEquipped;
	}
}

void APlayerCharacter::EquiptOrUnequipt() 
{
	if (!Weapon || ActionState != EActionState::EAS_Unoccupied)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!AnimInstance && !EquiptUnequiptMontage)
	{
		return;
	}

	if (CharacterState == ECharacterState::ECS_Unequipped)
	{
		AnimInstance->Montage_Play(EquiptUnequiptMontage);
	}
	else
	{
		AnimInstance->Montage_Play(EquiptUnequiptMontage);

		AnimInstance->Montage_JumpToSection(FName("Unequip"), EquiptUnequiptMontage);
	}
}