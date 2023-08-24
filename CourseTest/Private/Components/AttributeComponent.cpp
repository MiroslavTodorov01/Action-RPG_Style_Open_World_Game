#include "Components/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UAttributeComponent::ReceiveDamage(float Amount)
{
	Health = FMath::Clamp(Health - Amount, 0.f, MaxHealth);

	UE_LOG(LogTemp, Display, TEXT("Health: %f"), Health);
}

float UAttributeComponent::CalculatePercentageOfHealth()
{
	return Health / MaxHealth;
}

float UAttributeComponent::CalculatePercentageOfStamina()
{
	return Stamina / MaxStamina;
}

int32 UAttributeComponent::AddGold(int32 Amount)
{
	Gold += Amount;

	return Gold;
}

int32 UAttributeComponent::AddSouls(int32 Amount)
{
	Souls += Amount;

	return Souls;
}

bool UAttributeComponent::IsDead() 
{
	return Health == 0.f;
}