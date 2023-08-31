#include "Enemies/Enemy.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/HealthBarComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Pickable/Soul.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	HealthBar = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar"));
	HealthBar->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SencingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sence"));
	SencingComponent->SightRadius = 3000.f;
	SencingComponent->SetPeripheralVisionAngle(45.f);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Enemy"));

	HealthBar->SetVisibility(false);

	AIController = Cast<AAIController>(GetController());

	if (PatrulPointToGo)
	{
		MoveToTarget(PatrulPointToGo);
	}

	SencingComponent->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);

	AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);

	Weapon = SpawnedWeapon;

	Weapon->Equip(GetMesh(), FName("WeaponSocket"), this, this);
}

void AEnemy::LoseIntrest()
{
	HealthBar->SetVisibility(false);
	CombatTarget = nullptr;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead())
	{
		return;
	}


	if (EnemyState > EEnemyState::EES_Patrulling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrulTarget();
	}

}

void AEnemy::Attack()
{
	Super::Attack();

	if (!CombatTarget)
	{
		return;
	}

	EnemyState = EEnemyState::EES_Engage;
	
	PlayAttackMontage();
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !IsInTargetDistance(CombatTarget, AttackRadius);
}

bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsInsideAttackRadius()
{
	return IsInTargetDistance(CombatTarget, AttackRadius);
}

bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

bool AEnemy::CanAttack()
{
	return !IsAttacking() && IsInsideAttackRadius() && !IsDead() && !IsEngaged();
}

void AEnemy::HandleDamage(double DamageAmount)
{
	Super::HandleDamage(DamageAmount);

	if (HealthBar)
	{
		HealthBar->SetHealthBarPercentage(GetCurrentHealthPercentage());
	}
}

bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}

bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engage;
}

void AEnemy::ShowHealthBar()
{
	if (HealthBar)
	{
		HealthBar->SetVisibility(true);
	}
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (Target == nullptr) return;

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(60.f);
	AIController->MoveTo(MoveRequest);
}

void AEnemy::CheckCombatTarget()
{
	if (CombatTarget == nullptr)
	{
		StartPatrolling();
		LoseIntrest();
		return;
	}

	if (IsOutsideCombatRadius())
	{
		ClearAttackTimer();
		LoseIntrest();

		if (!IsEngaged())
		{
			StartPatrolling();
		}
	}
	else if (!IsChasing() && IsOutsideAttackRadius())
	{
		ClearAttackTimer();

		if (!IsEngaged())
		{
			StartChasingTarget();
		}
	}
	else if (CanAttack())
	{
		StartAttackTimer();
	}

}

void AEnemy::CheckPatrulTarget()
{
	if (!PatrulPointToGo) return;

	if (IsInTargetDistance(PatrulPointToGo, PatrulRadius))
	{
		PatrulPointToGo = ChoosNextPatrulTarget();
		GetWorldTimerManager().SetTimer(PatrulTimer, this, &AEnemy::FinishedPatrolling, 4.f);
	}
}

void AEnemy::PawnSeen(APawn* Seen)
{
	if (Seen->ActorHasTag(FName("Dead")))
	{
		return;
	}

	const bool bShoudChaseTarget =
		!IsDead() &&
		!IsChasing() &&
		EnemyState < EEnemyState::EES_Attacking &&
		Seen->ActorHasTag(FName("Player"));

	if (bShoudChaseTarget)
	{
		ShowHealthBar();
		CombatTarget = Seen;
		ClearPatrulTimer();
		StartChasingTarget();
	}
}

void AEnemy::Destroyed()
{
	if (Weapon)
	{
		Weapon->Destroy();
	}
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;

	float RangeTime = FMath::RandRange(AttackMin, AttackMax);

	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, RangeTime);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

void AEnemy::ClearPatrulTimer()
{
	GetWorldTimerManager().ClearTimer(PatrulTimer);
}

void AEnemy::FinishedPatrolling()
{
	MoveToTarget(PatrulPointToGo);
}

bool AEnemy::IsInTargetDistance(const AActor* Target, double radius)
{
	if (Target == nullptr) return false;

	float distance = (Target->GetActorLocation() - GetActorLocation()).Size();

	return distance <= radius;
}

AActor* AEnemy::ChoosNextPatrulTarget()
{
	const int32 index = FMath::RandRange(0, PatrulPoints.Num() - 1);

	return PatrulPoints[index];
}

void AEnemy::Die()
{
	Super::Die();

	EnemyState = EEnemyState::EES_Dead;

	CombatTarget = nullptr;

	ClearAttackTimer();
	DisableCapsulCollision();
	HealthBar->SetVisibility(false);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	SetWeaponCollision(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.Z += 40.f;

	if (SoulClass)
	{
		ASoul* Soul = GetWorld()->SpawnActor<ASoul>(SoulClass, SpawnLocation, GetActorRotation());

		if (Soul)
		{
			Soul->SetSoulValue(SoulValue);
		}
	}
}

bool AEnemy::IsOutsideCombatRadius()
{
	return !IsInTargetDistance(CombatTarget, CombatRadius);
}

void AEnemy::StartChasingTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrulling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(PatrulPointToGo);
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	
	ClearPatrulTimer();
	ClearAttackTimer();
	SetWeaponCollision(ECollisionEnabled::NoCollision);
	StopAttackMontage();

	if (IsAlive())
	{
		ShowHealthBar();
	}
	else
	{
		Die();
	}
	
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();
	
	if (IsInsideAttackRadius())
	{
		EnemyState = EEnemyState::EES_Attacking;
	}
	else if (IsOutsideAttackRadius())
	{
		StartChasingTarget();
	}

	return DamageAmount;
}