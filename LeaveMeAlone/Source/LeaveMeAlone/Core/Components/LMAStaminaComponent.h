// LeaveMeAlone - all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LMAStaminaComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStaminaChanged, float)


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEAVEMEALONE_API ULMAStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

/*Fields*/
 private:
	float Stamina = 0.0f; // текущее состояние выносливости
	
 protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
    float MaxStamina = 1000.0f; // максимальная выносливость

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
    float MinStamina = 0.0f; // Минимальная выносливость

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
    //float MinStaminaForSprinting = 10.0f; // Минимальная выносливость для возможности спринта

	/*Methods*/
public:	
	// Sets default values for this component's properties
	ULMAStaminaComponent();

	FOnStaminaChanged OnStaminaChanged;

	UFUNCTION(BlueprintCallable)
	float GetStamina() const { return Stamina; }
	void SetStamina(float NewStamina);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	
    
	bool EnoughStaminaForSprint() { return (Stamina >= MinStamina); }
   
	void DecreaseStamina(float DecValue);
	void IncreaseStamina(float IncValue);
		
};
