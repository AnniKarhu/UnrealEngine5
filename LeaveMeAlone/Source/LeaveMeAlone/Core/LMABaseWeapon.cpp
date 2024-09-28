// LeaveMeAlone - all rights reserved


#include "Core/LMABaseWeapon.h"
DEFINE_LOG_CATEGORY_STATIC(BaseWeaponLog, All, All);

ALMABaseWeapon::ALMABaseWeapon()
{
 	PrimaryActorTick.bCanEverTick = false;

	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponComponent");
	SetRootComponent(WeaponComponent);

}

void ALMABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
    CurrentAmmoWeapon = DefaultAmmoWeapon;	
}

void ALMABaseWeapon::Fire()
{
    if (!WeaponReloaded)
	return;
    
    Shoot();
    
    if (BurstFire)
		 {
		     FTimerHandle BurstFireTimerHandle;
		     GetWorld()->GetTimerManager().SetTimer(BurstFireTimerHandle, this, &ALMABaseWeapon::Fire, BurstFirePause);
		 }	         
}

void ALMABaseWeapon::Shoot()
{
    DecrementBullets();
    
    const FTransform SocketTransform = WeaponComponent->GetSocketTransform(ShootSocket);
    const FVector TraceStart = SocketTransform.GetLocation();
    const FVector ShootDirection = SocketTransform.GetRotation().GetForwardVector();
    const FVector TraceEnd = TraceStart + ShootDirection * TraceDistance;
    
    DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Black, false, 1.0f, 0, 2.0f);
    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
    if (HitResult.bBlockingHit)
    {
	    DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 24, FColor::Red, false, 1.0f);
    }
}

void ALMABaseWeapon::DecrementBullets() 
{
    CurrentAmmoWeapon.Bullets--;
    
    UE_LOG(BaseWeaponLog, Display, TEXT("Bullets = %s"), *FString::FromInt(CurrentAmmoWeapon.Bullets));

    if (IsCurrentClipEmpty())
    {
	    WeaponReloaded = false;
	    BurstFire = false;

	    RunoutOfBulletes.Broadcast(); // оповещение подписанных о пустом магазине
    	    
    }
}

bool ALMABaseWeapon::IsCurrentClipEmpty() const
{
    return CurrentAmmoWeapon.Bullets == 0;    
}

bool ALMABaseWeapon::IsCurrentClipFull() const
{
    return CurrentAmmoWeapon.Bullets == DefaultAmmoWeapon.Bullets;
}

void ALMABaseWeapon::ChangeClip() 
{
    //if *** есть свободные обоймы
    CurrentAmmoWeapon.Bullets = DefaultAmmoWeapon.Bullets;

    //декремент количества обойм DecrementBullets() 
}

bool ALMABaseWeapon::GetBurstFire() const
{
    return BurstFire;
}

void ALMABaseWeapon::ClearBurstFire()
{
   BurstFire = false;
}

void ALMABaseWeapon::TryBurstFire()
{
    // добавить условия установки нового BurstFire = true
    BurstFire = true;
}

void ALMABaseWeapon::FinishReloading() 
{
    ChangeClip();
    WeaponReloaded = true;

    UE_LOG(BaseWeaponLog, Display, TEXT("Reloading finished, Bullets = %s"), *FString::FromInt(CurrentAmmoWeapon.Bullets));
   
}


