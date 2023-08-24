#include "Characters/BaseCharacter.h"
#include "Components/AttributeComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "CourseTest/DrawMacro.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::SetWeaponCollision(ECollisionEnabled::Type CollisionType)
{

	if (Weapon && Weapon->GetWeaponBoxCollision())
	{
		Weapon->GetWeaponBoxCollision()->SetCollisionEnabled(CollisionType);

		if (CollisionType == ECollisionEnabled::NoCollision)
		{
			Weapon->RemoveIgnoredActorsAfterAttack();
		}
	}
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitFleshSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitFleshSound, ImpactPoint);
	}
}

void ABaseCharacter::SpawnHitPatricles(const FVector& ImpactPoint)
{
	if (BloodParitcle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodParitcle, ImpactPoint);
	}
}

void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.25, AttackMontage);
	}
}

void ABaseCharacter::DisableCapsulCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::HandleDamage(double DamageAmount)
{
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
	}
}

FVector ABaseCharacter::GetTranslationWarpTarget()
{
	if (!CombatTarget)
	{
		return FVector();
	}
	
	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector SelfLocation = GetActorLocation();

	FVector WarpLocation = (SelfLocation - CombatTargetLocation).GetSafeNormal();
	WarpLocation *= WarpTargetDistance;

	return CombatTargetLocation + WarpLocation;
}

FVector ABaseCharacter::GetRotationWarpTarget()
{
	if (CombatTarget)
	{
		return CombatTarget->GetActorLocation();
	}

	return FVector();
}

bool ABaseCharacter::IsAlive()
{
	if (Attributes)
	{
		return !(Attributes->IsDead());
	}

	return false;
}

void ABaseCharacter::PlaySectionFromMontage(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);

		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

int32 ABaseCharacter::PlayRandomSectionFromMontage(UAnimMontage* Montage, const TArray<FName> MontageSections)
{
	if (MontageSections.Num() <= 0 || !Montage) return -1;

	int32 MaxIndexOfSections = MontageSections.Num() - 1;

	int32 Random = FMath::RandRange(0, MaxIndexOfSections);

	PlaySectionFromMontage(Montage, MontageSections[Random]);

	return Random;
}

void ABaseCharacter::EnableInputFromAnimation()
{
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::Attack()
{
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead")))
	{
		CombatTarget = nullptr;
	}

}

bool ABaseCharacter::CanAttack()
{
	return false;
}

int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandomSectionFromMontage(AttackMontage, AttackMontageSections);
}

int32 ABaseCharacter::PlayDeathMontage()
{
	const int32 IndexSection = PlayRandomSectionFromMontage(DeathMontage, DeadMontageSections);

	TEnumAsByte<EDeathState> Pose(IndexSection);

	if (Pose < EDeathState::EDS_MAX)
	{
		DeathState = Pose;
	}

	return IndexSection;
}

FName ABaseCharacter::GetSectionOnHitCorespondingToTheta(double Theta)
{
	if (Theta >= -45.f && Theta < 45.f)
	{
		return FName("FromFront");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		return FName("FromRight");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		return FName("FromLeft");
	}

	return FName("FromBack");
}

int32 ABaseCharacter::PlayHitReatctionMontage(const FName& SectionToPlay)
{
	 
	PlaySectionFromMontage(HitReactMontage, SectionToPlay);

	 return -1;
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (IsAlive() && Hitter)
	{
		const FVector HitterLocation = Hitter->GetActorLocation();

		FVector HitPoint = FVector(HitterLocation.X, HitterLocation.Y, GetActorLocation().Z) - GetActorLocation();

		double Theta = CalculateTheta(HitPoint);

		FName SectionToPlay = GetSectionOnHitCorespondingToTheta(Theta);

		PlayHitReatctionMontage(SectionToPlay);
	}

	PlayHitSound(ImpactPoint);
	SpawnHitPatricles(ImpactPoint);
}

float ABaseCharacter::GetCurrentHealthPercentage()
{
	if (Attributes)
	{
		return Attributes->CalculatePercentageOfHealth();
	}

	return 0.0f;
}

float ABaseCharacter::GetCurrentStaminaPercentage()
{
	if (Attributes)
	{
		return Attributes->CalculatePercentageOfStamina();
	}

	return 0.0f;
}

double ABaseCharacter::CalculateTheta(const FVector& HitVector)
{
	const FVector HitPoint = HitVector.GetSafeNormal();
	const FVector Forward = GetActorForwardVector();

	double CosTheta = FVector::DotProduct(Forward, HitPoint);

	// angle of two vectors
	double Theta = FMath::Acos(CosTheta);

	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProductVector = FVector::CrossProduct(Forward, HitPoint);

	if (CrossProductVector.Z < 0)
	{
		Theta *= -1;
	}

	return Theta;
}

void ABaseCharacter::Die()
{
	Tags.Add(FName("Dead"));

	PlayDeathMontage();
}
