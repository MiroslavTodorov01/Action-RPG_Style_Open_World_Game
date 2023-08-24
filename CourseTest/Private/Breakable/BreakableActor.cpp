// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Pickable/Treasure.h"
#include "Components/CapsuleComponent.h"

ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	GeometrycalComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Break style"));
	SetRootComponent(GeometrycalComponent);
	GeometrycalComponent->SetGenerateOverlapEvents(true);
	GeometrycalComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometrycalComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collision"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

}


void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (bBroken || TreasuresClass.Num() == 0)
	{
		return;
	}

	bBroken = true;

	int32 indexOfTreasure = FMath::RandRange(0, TreasuresClass.Num() - 1);

	FVector LocatonToSpawn = GetActorLocation();

	LocatonToSpawn.Z += 74.f;

	Treasure = GetWorld()->SpawnActor<ATreasure>(TreasuresClass[indexOfTreasure], LocatonToSpawn, FRotator(0.f));
	
}


