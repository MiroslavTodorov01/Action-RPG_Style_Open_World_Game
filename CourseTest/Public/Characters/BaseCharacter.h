// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterTypes.h"
#include "BaseCharacter.generated.h"

class UAnimMontage;
class UAttributeComponent;

UCLASS()
class COURSETEST_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	float GetCurrentHealthPercentage();
	float GetCurrentStaminaPercentage();

	FORCEINLINE UAnimMontage* GetAttackMontage() const { return AttackMontage; };
	FORCEINLINE AActor* GetCombatTarget() const { return CombatTarget; };
	FORCEINLINE UAttributeComponent* GetAttributeComponent() const { return Attributes; };
	FORCEINLINE void SetCombatTarget(AActor* Target) { CombatTarget = Target; };
	FORCEINLINE TEnumAsByte<EDeathState> GetDeathState() const { return DeathState.GetValue(); };

protected:
	
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathState> DeathState;

	UPROPERTY(VisibleAnywhere)
	class AWeapon* Weapon;

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double WarpTargetDistance = 75.f;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void EnableInputFromAnimation();

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollision(ECollisionEnabled::Type CollisionType);

	void DisableCapsulCollision();

	virtual bool IsAlive();

	/* Combat */
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	virtual void Attack();
	virtual bool CanAttack();
	virtual void Die();
	void PlaySectionFromMontage(UAnimMontage* Montage, const FName& SectionName);
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();
	virtual void PlayeDodgeMontage();
	int32 PlayHitReatctionMontage(const FName& SectionToPlay);
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitPatricles(const FVector& ImpactPoint);
	void StopAttackMontage();

	virtual void HandleDamage(double DamageAmount);

	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();

	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	/* Theta */
	FName GetSectionOnHitCorespondingToTheta(double Theta);
	double CalculateTheta(const FVector& HitVector);


private:
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* HitFleshSound;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* BloodParitcle;

	UPROPERTY(EditAnywhere)
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere)
	TArray<FName> DeadMontageSections;

	int32 PlayRandomSectionFromMontage(UAnimMontage* Montage, const TArray<FName> MontageSections);
};
