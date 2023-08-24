// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickable/Treasure.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/AttributeComponent.h"
#include "HUD/PlayerOverlay.h"

void ATreasure::OverlapBegin(UPrimitiveComponent* ComponentOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{

	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);

	if (player)
	{
		if (PickSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickSound, GetActorLocation());

			int32 CurrentGoldAmount = player->GetAttributeComponent()->AddGold(GoldValue);

			player->GetPlayerOverlay()->SetGold(CurrentGoldAmount);
		}

		Destroy();
	}
}
