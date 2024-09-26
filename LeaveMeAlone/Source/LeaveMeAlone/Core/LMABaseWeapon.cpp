// LeaveMeAlone - all rights reserved


#include "Core/LMABaseWeapon.h"
DEFINE_LOG_CATEGORY_STATIC(BaseWeaponLog, All, All);


// Sets default values
ALMABaseWeapon::ALMABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponComponent");
	SetRootComponent(WeaponComponent);

}

// Called when the game starts or when spawned
void ALMABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
    CurrentAmmoWeapon = DefaultAmmoWeapon;
	
}

void ALMABaseWeapon::Fire()
{
    Shoot();
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
	    ChangeClip();
    }
}

bool ALMABaseWeapon::IsCurrentClipEmpty() const
{
    return CurrentAmmoWeapon.Bullets == 0;
    
}

void ALMABaseWeapon::ChangeClip() 
{
    //if *** есть свободные обоймы
    CurrentAmmoWeapon.Bullets = DefaultAmmoWeapon.Bullets;

    //декремент количества обойм DecrementBullets() 
}


