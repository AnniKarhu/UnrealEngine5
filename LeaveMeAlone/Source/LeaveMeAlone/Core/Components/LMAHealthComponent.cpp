// LeaveMeAlone - all rights reserved


#include "Core/Components/LMAHealthComponent.h"

// Sets default values for this component's properties
ULMAHealthComponent::ULMAHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void ULMAHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	OnHealthChanged.Broadcast(Health);

	AActor* OwnerComponent = GetOwner();
	if (OwnerComponent)
	{
	    //подписка на делегат
		//this - вызывается в этом классе, &ULMAHealthComponent::OnTakeAnyDamage - функция, которая будет   срабатывать при вызове делегата
		OwnerComponent->OnTakeAnyDamage.AddDynamic(this, &ULMAHealthComponent::OnTakeAnyDamage);
	}
	
}



void ULMAHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    //текущее здоровье персонажа будет уменьшаться на то  количество урона, которое пришло из класса A_RadialFire
	
	//Health -= Damage;
    if (IsDead())
    {
		return;
    }
    
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);

    if (IsDead())
    {
		OnDeath.Broadcast(); //оповещение подписанных о смерти
    }
}

bool ULMAHealthComponent::IsDead() const
{
    return Health <= 0.0f;    
}

bool ULMAHealthComponent::IsHealthFull() const
{
    return FMath::IsNearlyEqual(Health, MaxHealth);
}

bool ULMAHealthComponent::AddHealth(float NewHealth)
{
    if (IsDead() || IsHealthFull())
	return false;
    Health = FMath::Clamp(Health + NewHealth, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);
    return true;
}


