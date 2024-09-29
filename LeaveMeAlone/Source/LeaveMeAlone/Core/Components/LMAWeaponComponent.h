// LeaveMeAlone - all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LMAWeaponComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponBulletesChanged, int, Bullets);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	ALMABaseWeapon* Weapon = nullptr;

	 UPROPERTY(BlueprintAssignable, Category = "Default")
	FOnWeaponBulletesChanged OnWeaponBulletesChanged;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* ReloadMontage;

	bool AnimReloading = false; //анимация перезарядки

public:	
	// Sets default values for this component's properties
	ULMAWeaponComponent();
	
	void SpawnWeapon();
	void Fire();

	UFUNCTION(BlueprintCallable)
	void StopFire();

	//перезарядка
	void Reload();
	void InitAnimNotify();
	void OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh);
	bool CanReload() const;	

	void BulletsChanged(int NewBullets);

	UFUNCTION(BlueprintCallable)
	bool GetCurrentWeaponAmmo(FAmmoWeapon& AmmoWeapon) const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	
	


	

	
		
};
