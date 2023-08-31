// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickable/Treasure.h"
#include "Components/AttributeComponent.h"
#include "Interfaces/PickupInterface.h"

void ATreasure::OverlapBegin(UPrimitiveComponent* ComponentOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{

	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);

	if (PickupInterface)
	{
		PickupInterface->AddGold(this);

		PlayPickupSound();
		Destroy();
	}
}
