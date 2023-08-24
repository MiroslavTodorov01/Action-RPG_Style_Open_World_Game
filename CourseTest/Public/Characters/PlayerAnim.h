// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "PlayerAnim.generated.h"

UCLASS()
class COURSETEST_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class APlayerCharacter* Player;

	UPROPERTY(BlueprintReadOnly, Category = "Movment")
	class UCharacterMovementComponent* MovmentComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Movment")
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movment")
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	TEnumAsByte<EDeathState> DeathState;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	EActionState ActionState;
};
