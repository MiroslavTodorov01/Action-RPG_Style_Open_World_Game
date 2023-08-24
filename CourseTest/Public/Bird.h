// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Bird.generated.h"

UCLASS()
class COURSETEST_API ABird : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABird();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* capsule;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* skeletalMesh;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	class UCameraComponent* Camera;

	//UPROPERTY(EditAnywhere)
	//float Speed;

	UPROPERTY(EditAnywhere)
	float CameraSpeed;

	void MoveForwardBackward(float Value);

	void UpDown(float Value);

	void LeftRight(float Value);

	void Roll(float Value);

	void MoveLeftRight(float Value);

	void FlyUpDown(float Value);
};
