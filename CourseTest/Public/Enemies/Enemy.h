#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/CharacterTypes.h"
#include "Characters/BaseCharacter.h"
#include "Enemy.generated.h"

UCLASS()
class COURSETEST_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	/* AActor */
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/* UHitInterface */
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EEnemyState EnemyState = EEnemyState::EES_Patrulling;

	/* AActor */
	virtual void BeginPlay() override;

	/* ABaseCharacter */
	virtual bool CanAttack() override;
	virtual void Attack() override;
	virtual void AttackEnd() override;
	virtual void HandleDamage(double DamageAmount) override;
	virtual void Die() override;

private:

	UPROPERTY()
	class AAIController* AIController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASoul> SoulClass;

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* SencingComponent;

	UPROPERTY(EditDefaultsonly)
	class UHealthBarComponent* HealthBar;

	UPROPERTY(EditAnywhere)
	int32 SoulValue;

	void ShowHealthBar();

	/* AI Properties */
	UPROPERTY(EditInstanceOnly)
	AActor* PatrulPointToGo;

	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> PatrulPoints;

	int32 currentIndexPoint = 0;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
	float PatrulRadius = 150.f;

	float AttackRadius = 150.f;
	float PatrollingSpeed = 225.f;
	float ChasingSpeed = 450.f;

	/* Timers */
	UPROPERTY()
	FTimerHandle AttackTimer;
	
	UPROPERTY(EditAnywhere)
	float AttackMin = 0.5;

	UPROPERTY(EditAnywhere)
	float AttackMax = 1.f;

	UPROPERTY()
	FTimerHandle PatrulTimer;

	void StartAttackTimer();
	void ClearAttackTimer();
	void ClearPatrulTimer();


	/* AI behavior */
	UFUNCTION()
	void PawnSeen(APawn* Seen);

	bool IsInTargetDistance(const AActor* Target, double radius);
	AActor* ChoosNextPatrulTarget();
	void MoveToTarget(AActor* Target);
	void CheckCombatTarget();
	void CheckPatrulTarget();
	void FinishedPatrolling();
	void LoseIntrest();
	void StartPatrolling();
	void StartChasingTarget();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();

	/* AActor */
	virtual void Destroyed() override;
};
