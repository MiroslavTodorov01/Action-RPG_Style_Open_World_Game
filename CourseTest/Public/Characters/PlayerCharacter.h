// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/CharacterTypes.h"
#include "Characters/BaseCharacter.h"
#include "Interfaces/PickupInterface.h"
#include "PlayerCharacter.generated.h"

class UGroomComponent;
class AItem;
class UAnimMontage;
class UPlayerOverlay;

UCLASS()
class COURSETEST_API APlayerCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

	void InitializePlayerOverlay();

	virtual void EnableInputFromAnimation() override;
	virtual bool CanAttack() override;
	virtual void Die() override;

	UFUNCTION(BlueprintCallable)
	void DodgeEnd();

	UFUNCTION(BlueprintCallable)
	void WeaponAttachToSocket();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void SetOverlapingItem(class AItem* AItem) override;
	bool IfItemIsAttached() const;
	virtual void AddHealingPotion() override;
	virtual void AddGold(class ATreasure* Treasure) override;
	virtual void AddSouls(class ASoul* Soul) override;

	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; };
	FORCEINLINE EActionState GetActionState() const { return ActionState; };
	FORCEINLINE UPlayerOverlay* GetPlayerOverlay() const { return PlayerOverlay; };

private:

	EActionState ActionState = EActionState::EAS_Unoccupied;
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY()
	UPlayerOverlay* PlayerOverlay;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Hair;

	UPROPERTY(VisibleAnywhere)
	UGroomComponent* Eyebrows;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* DashNS;

	UPROPERTY(EditDefaultsOnly)
	FTimerHandle TimeHandle;

	UPROPERTY(VisibleAnywhere)
	AItem* Item;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* EquiptUnequiptMontage;

	UPROPERTY(EditAnywhere)
	double DashDamage;

	TArray<AActor*> ActorsToIgnor;

	float ValueToSpawnAfterDash;
	
	virtual void AttackEnd() override;

	/* Input */
	void MoveForwardBackward(float Value);
	void MoveLeftRight(float Value);
	void Turn(float Value);
	void LookUpDown(float Value);
	void Walk();
	void DisableWalk();
	void Jump();
	void Dodge();
	void Heal();

	void Dash();
	void CalculateDash();
	void PickUp();
	virtual void Attack() override;
	void HeavyAttack();
	void EquiptOrUnequipt();
	void DealDamageFromDash(const TArray<FHitResult>& HitPoints);
	bool IfActorAlreadyDemaged(const AActor* CurrentActor);
};
