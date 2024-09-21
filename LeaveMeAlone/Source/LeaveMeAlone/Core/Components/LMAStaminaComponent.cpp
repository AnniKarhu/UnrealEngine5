// LeaveMeAlone - all rights reserved


#include "Core/Components/LMAStaminaComponent.h"

// Sets default values for this component's properties
ULMAStaminaComponent::ULMAStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	
}


// Called when the game starts
void ULMAStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	Stamina = MaxStamina;
	OnStaminaChanged.Broadcast(Stamina);		
}


void ULMAStaminaComponent::SetStamina(float NewStamina) 
{    
    float ToSetStamina;

	ToSetStamina = (NewStamina < MinStamina ? MinStamina : NewStamina);
    ToSetStamina = (NewStamina > MaxStamina ? MaxStamina : NewStamina);
	
	if (Stamina != ToSetStamina)
    {
		Stamina = ToSetStamina;
		OnStaminaChanged.Broadcast(Stamina);
    }	
}

void ULMAStaminaComponent::DecreaseStamina(float DecValue)
{    
   SetStamina(Stamina - DecValue);
}

void ULMAStaminaComponent::IncreaseStamina(float IncValue)
{
   SetStamina(Stamina + IncValue);
}

