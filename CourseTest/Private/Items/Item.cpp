// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Kismet/GameplayStatics.h"
#include "CourseTest/DrawMacro.h"
#include "Characters/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"

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

void AItem::OverlapBegin(UPrimitiveComponent* ComponentOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);

	if (player && !player->IfItemIsAttached())
	{
		player->SetItem(this);
	}
}

void AItem::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Green, FString("Actor overlaps end"));
	}

	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);

	if (player)
	{
		player->SetItem(nullptr);
	}
}

void AItem::SetCollisionOnSphereComponent(ECollisionEnabled::Type CollisionType)
{

	if (SphereCollision)
	{
		SphereCollision->SetCollisionEnabled(CollisionType);
	}
}