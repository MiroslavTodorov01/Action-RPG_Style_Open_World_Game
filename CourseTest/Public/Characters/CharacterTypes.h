#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8 
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_OneHendedWeaponEquipped UMETA(DisplayName = "One Hended Weapon Equipped"),
	ECS_TwoHendedWeaponEquipped UMETA(DisplayName = "Two Hended Weapon Equipped")
};

UENUM(BlueprintType)
enum class EActionState : uint8 
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Dead UMETA(DisplayName = "Dead"),
	EAS_HitReact UMETA(DisplayName = "HitReact"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquipOrNot UMETA(DisplayName = "EquipOrNot")
};

UENUM(BlueprintType)
enum EDeathState
{
	EDS_Death_1 UMETA(DisplayName = "Death_1"),
	EDS_Death_2 UMETA(DisplayName = "Death_2"),
	
	EDS_MAX UMETA(DisplayName = "DeathMax")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_NoState UMETA(DisplayName = "NoState"),
	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Patrulling UMETA(DisplayName = "Patrolling"),
	EES_Chasing	UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engage UMETA(DisplayName = "Engage")
	
};