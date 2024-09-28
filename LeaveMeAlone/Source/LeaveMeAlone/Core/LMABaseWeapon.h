// LeaveMeAlone - all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LMABaseWeapon.generated.h"


DECLARE_MULTICAST_DELEGATE(FRunoutOfBulletes) //������� �����������

USTRUCT(BlueprintType)
struct FAmmoWeapon
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 Bullets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 Clips;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool Infinite;
};

UCLASS()
class LEAVEMEALONE_API ALMABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALMABaseWeapon();
    void Fire();

	FRunoutOfBulletes RunoutOfBulletes;


	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FAmmoWeapon CurrentAmmoWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float BurstFirePause = 0.5f; // ����� ����� ����������
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* WeaponComponent;  // WeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FAmmoWeapon DefaultAmmoWeapon{30, 0, true};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TraceDistance = 800.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName ShootSocket;

private:
	bool BurstFire = false;
    bool WeaponReloaded = true;
	

public:
	void ChangeClip();    
    bool GetBurstFire() const;
	void ClearBurstFire();
    void TryBurstFire();
	void FinishReloading();
	bool IsCurrentClipFull() const;

protected:
	void Shoot();

	void DecrementBullets();
	bool IsCurrentClipEmpty() const;
	
	
};
