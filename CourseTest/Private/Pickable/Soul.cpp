// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickable/Soul.h"
#include "Interfaces/PickupInterface.h"

void ASoul::OverlapBegin(UPrimitiveComponent* ComponentOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult) 
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	
	if (PickupInterface)
	{
		PickupInterface->AddSouls(this);

		SpawnPickupEffect();
		PlayPickupSound();

		Destroy();
	}
}