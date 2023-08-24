// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ABird::ABird()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	
	SetRootComponent(capsule);

	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));

	skeletalMesh->SetupAttachment(capsule);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));

	SpringArm->SetupAttachment(capsule);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 200.f;

	SpringArm->AddWorldRotation(FRotator(-10.f, 0.f, 0.f));

	SpringArm->bInheritRoll = false;

	SpringArm->bInheritPitch = false;

	SpringArm->bInheritYaw = false;
}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Forward/Backward"), this, &ABird::MoveForwardBackward);

	PlayerInputComponent->BindAxis(TEXT("LookUp/Down"), this, &ABird::UpDown);

	PlayerInputComponent->BindAxis(TEXT("LookLeft/Right"), this, &ABird::LeftRight);

	PlayerInputComponent->BindAxis(TEXT("Roll"), this, &ABird::Roll);

	PlayerInputComponent->BindAxis(TEXT("MoveLeft/Right"), this, &ABird::MoveLeftRight);

	PlayerInputComponent->BindAxis(TEXT("FlyUp/Down"), this, &ABird::FlyUpDown);
}
void ABird::MoveForwardBackward(float Value) 
{
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	//UE_LOG(LogTemp, Display, TEXT("%f"), Value * Speed);

	//AddActorLocalOffset(FVector(Value * Speed, 0.f, 0.f), true);

	AddMovementInput(GetActorForwardVector(), Value);
}

void ABird::Roll(float Value) 
{
	
	AddActorLocalRotation(FRotator(0.f, 0.f, Value * CameraSpeed), true);
}

void ABird::UpDown(float Value) 
{
	SpringArm->AddLocalRotation(FRotator(Value * CameraSpeed, 0.f, 0.f));

	FRotator rotation = SpringArm->GetComponentRotation();

	rotation.Roll = 0.f;

	SpringArm->SetWorldRotation(rotation);
}

void ABird::LeftRight(float Value) 
{
	SpringArm->AddLocalRotation(FRotator(0.f, Value * CameraSpeed, 0.f));

	FRotator rotation = SpringArm->GetComponentRotation();

	rotation.Roll = 0.f;

	SpringArm->SetWorldRotation(rotation);
}

void ABird::MoveLeftRight(float Value)
{
	AddActorLocalRotation(FRotator(0.f, Value * 2.f, 0.f));

	//AddMovementInput(GetActorRightVector(), Value);
}

void ABird::FlyUpDown(float Value)
{
	AddMovementInput(GetActorUpVector(), Value);
}