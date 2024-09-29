// LeaveMeAlone - all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LMAHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float) 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedBP, float, NewHealth);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEAVEMEALONE_API ULMAHealthComponent : public UActorComponent
{
    GENERATED_BODY() 

/*Fields*/
private:
    float Health = 0.0f; //������� ��������� ��������

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float MaxHealth = 100.0f; //�����������t ��������

public:
  
	//UPROPERTY(BlueprintAssignable, Category = "Default")
    FOnDeath OnDeath;

	//UPROPERTY(BlueprintAssignable, Category = "Default")
    FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Default")
    FOnHealthChangedBP OnHealthChangedBP;

/*Methods*/
public:	
	// Sets default values for this component's properties
	ULMAHealthComponent();

	UFUNCTION(BlueprintCallable)
	float GetHealth() const 
	{ 
		return Health; 
	}	

	UFUNCTION(BlueprintCallable)
	bool IsDead() const;

	bool AddHealth(float NewHealth); // ���������� ���������� ������ � ���������� true, ���� �������� ���� ������� ����������.
	bool IsHealthFull() const; // ��������������� �������, ������� ��������,    ������ ��  �������� � ��������� �����.

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UFUNCTION()
    void OnTakeAnyDamage(
	AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser); // ������z, ������� ����� ���������� ��� ��������� �����

	void BroadcastOnHealthChanged();

		
};
