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
	BroadcastOnHealthChanged();
	//OnHealthChanged.Broadcast(Health);
	//OnHealthChangedBP.Broadcast(Health);

	AActor* OwnerComponent = GetOwner();
	if (OwnerComponent)
	{
	    //�������� �� �������
		//this - ���������� � ���� ������, &ULMAHealthComponent::OnTakeAnyDamage - �������, ������� �����   ����������� ��� ������ ��������
		OwnerComponent->OnTakeAnyDamage.AddDynamic(this, &ULMAHealthComponent::OnTakeAnyDamage);
	}
	
}



void ULMAHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    //������� �������� ��������� ����� ����������� �� ��  ���������� �����, ������� ������ �� ������ A_RadialFire
	
	if (IsDead())
    {
		return;
    }
    
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
    BroadcastOnHealthChanged();
	//OnHealthChanged.Broadcast(Health);
	//OnHealthChangedBP.Broadcast(Health);

    if (IsDead())
    {
		OnDeath.Broadcast(); //���������� ����������� � ������
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
    BroadcastOnHealthChanged();
   // OnHealthChanged.Broadcast(Health);
   // OnHealthChangedBP.Broadcast(Health);
    return true;
}

void ULMAHealthComponent::BroadcastOnHealthChanged()
{
    OnHealthChanged.Broadcast(Health);
    OnHealthChangedBP.Broadcast(Health);
}


