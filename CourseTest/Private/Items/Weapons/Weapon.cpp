// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"

AWeapon::AWeapon() 
{
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	CollisionBox->SetupAttachment(GetRootComponent());
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	StartPointOfTrace = CreateDefaultSubobject<USceneComponent>(TEXT("Start point"));
	StartPointOfTrace->SetupAttachment(GetRootComponent());

	EndPointOfTrace = CreateDefaultSubobject<USceneComponent>(TEXT("End point"));
	EndPointOfTrace->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay() 
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OverlapBeginBlade);

	IgnorActors.Add(this);
}

void AWeapon::Equip(USceneComponent* Component, FName SocketName, AActor* NewOwner, APawn* NewInstigator)
{
	
	SetWeaponOwner(NewOwner, NewInstigator);
	SetPickUpState(true);

	IgnorActors.AddUnique(NewOwner);

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);

	AttachToComponent(Component, AttachmentRules, SocketName);

	if (PickSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickSound, GetActorLocation());
	}
}

void AWeapon::SetWeaponOwner(AActor* NewOwner, APawn* NewInstigator)
{
	SetCollisionOnSphereComponent(ECollisionEnabled::NoCollision);
	SetNiagaraActivate(false);

	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
}

void AWeapon::RemoveIgnoredActorsAfterAttack()
{
	IgnorActors.Empty();

	IgnorActors.Add(this);
	IgnorActors.Add(GetOwner());
}

bool AWeapon::BoxTrace(FHitResult& Hit)
{
	const FVector TraceStartPoint = StartPointOfTrace->GetComponentLocation();

	const FVector TraceEndPoint = EndPointOfTrace->GetComponentLocation();

	if (SizeOfBoxTrace == 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Box trace size not set!"));
	}

	bool IfHasHit = UKismetSystemLibrary::BoxTraceSingle(this, TraceStartPoint, TraceEndPoint, FVector(SizeOfBoxTrace), StartPointOfTrace->GetComponentRotation(), ETraceTypeQuery::TraceTypeQuery1, false, IgnorActors, ActivateBoxTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, Hit, true);

	IgnorActors.Add(Hit.GetActor());

	return IfHasHit;
}

bool AWeapon::IfActorIsTheSame(AActor* OtherActor)
{
	return GetOwner()->ActorHasTag(FName("Enemy")) && OtherActor->ActorHasTag(FName("Enemy"));
}

void AWeapon::OverlapBeginBlade(UPrimitiveComponent* ComponentOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult) 
{
	if (IfActorIsTheSame(OtherActor))
	{
		return;
	}

	FHitResult Hit;

	bool IfHasHit = BoxTrace(Hit);

	if (IfHasHit)
	{
		if (IfActorIsTheSame(Hit.GetActor()))
		{
			return;
		}

		UGameplayStatics::ApplyDamage(Hit.GetActor(), Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());

		ExecuteGetHit(Hit);

		CreateFieldDestruction(Hit.ImpactPoint);
	}
}

void AWeapon::ExecuteGetHit(FHitResult& Hit)
{
	if (IHitInterface* HitActor = Cast<IHitInterface>(Hit.GetActor()))
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(GetOwner());

		if (player && !player->GetCombatTarget())
		{
			player->SetCombatTarget(Hit.GetActor());
		}

		HitActor->Execute_GetHit(Hit.GetActor(), Hit.ImpactPoint, GetOwner());
	}
}
