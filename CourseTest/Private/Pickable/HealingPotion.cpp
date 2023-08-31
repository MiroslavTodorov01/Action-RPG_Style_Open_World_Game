#include "Pickable/HealingPotion.h"
#include "Interfaces/PickupInterface.h"

void AHealingPotion::OverlapBegin(UPrimitiveComponent* ComponentOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);

	if (PickupInterface)
	{
		PickupInterface->AddHealingPotion();

		SpawnPickupEffect();
		PlayPickupSound();

		Destroy();
	}
}

