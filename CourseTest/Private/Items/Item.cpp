// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Kismet/GameplayStatics.h"
#include "CourseTest/DrawMacro.h"
#include "Interfaces/PickupInterface.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	SetRootComponent(root);
	root->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollision->SetupAttachment(root);

	NiagaraSpawn = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara particle"));
	NiagaraSpawn->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AItem::OverlapBegin);

	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AItem::OverlapEnd);

	IfIsPickedUp = false;
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	impulsValue += DeltaTime;

	if (!IfIsPickedUp)
	{
		FVector DeltaLocation = FVector(0.f, 0.f, TransformedSin());

		FRotator DeltaRotation = FRotator(0.f, 1.f, 0.f);

		AddActorLocalOffset(DeltaLocation);

		AddActorLocalRotation(DeltaRotation);
	}
}

float AItem::TransformedSin() 
{
	return amplitude * FMath::Sin(impulsValue * timeOfHalfAmplitude);
}

float AItem::TransformedCos() 
{
	return amplitude * FMath::Cos(impulsValue * timeOfHalfAmplitude);
}

void AItem::SetPickUpState(bool State)
{
	IfIsPickedUp = State;
}

void AItem::SetNiagaraActivate(bool State)
{
	if (!NiagaraSpawn)
	{
		return;
	}

	if (State)
	{
		NiagaraSpawn->Activate(true);
	}
	else
	{
		NiagaraSpawn->Deactivate();
	}
}

void AItem::SpawnPickupEffect()
{
	if (PickUpEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PickUpEffect, GetActorLocation());
	}
}

void AItem::PlayPickupSound()
{
	if (PickUpSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickUpSound, GetActorLocation());
	}
}

void AItem::OverlapBegin(UPrimitiveComponent* ComponentOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);

	if (PickupInterface)
	{
		PickupInterface->SetOverlapingItem(this);
	}
}

void AItem::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);

	if (PickupInterface)
	{
		PickupInterface->SetOverlapingItem(nullptr);
	}
}

void AItem::SetCollisionOnSphereComponent(ECollisionEnabled::Type CollisionType)
{

	if (SphereCollision)
	{
		SphereCollision->SetCollisionEnabled(CollisionType);
	}
}