// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerAnim.h"
#include "Characters/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
	{
		MovmentComponent = Player->GetCharacterMovement();

	}
}

void UPlayerAnim::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MovmentComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(MovmentComponent->Velocity);
		IsFalling = MovmentComponent->IsFalling();

		CharacterState = Player->GetCharacterState();
		ActionState = Player->GetActionState();
		DeathState = Player->GetDeathState();
	}
}