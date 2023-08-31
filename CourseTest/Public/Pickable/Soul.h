// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Soul.generated.h"

/**
 * 
 */
UCLASS()
class COURSETEST_API ASoul : public AItem
{
	GENERATED_BODY()

protected:
	virtual void OverlapBegin(UPrimitiveComponent* ComponentOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult) override;

private:

	UPROPERTY(EditAnywhere)
	int32 SoulValue;

public:

	FORCEINLINE int32 GetSoulValue() const { return SoulValue; };
	FORCEINLINE void SetSoulValue(int32 NewSoulValue) { SoulValue = NewSoulValue; };
};
