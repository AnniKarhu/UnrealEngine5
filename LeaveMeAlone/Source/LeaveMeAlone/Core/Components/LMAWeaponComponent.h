// LeaveMeAlone - all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//#include "core/LMABaseWeapon.h"
#include "LMAWeaponComponent.generated.h"

class ALMABaseWeapon;
class ULMAReloadFinishedAnimNotify;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEAVEMEALONE_API ULMAWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ALMABaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponSocket;

	UPROPERTY()
	ALMABaseWeapon* Weapon = nullptr;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* ReloadMontage;

	bool AnimReloading = false; //анимация перезарядки
	

public:	
	// Sets default values for this component's properties
	ULMAWeaponComponent();
	
	void SpawnWeapon();
	void Fire();

	//перезарядка
	void Reload();
	void InitAnimNotify();
	void OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh);
	bool CanReload() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	
	


	

	
		
};
