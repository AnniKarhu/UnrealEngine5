// LeaveMeAlone - all rights reserved

#include "Core/Components/LMAWeaponComponent.h"
#include "Core/Animations/LMAReloadFinishedAnimNotify.h"
#include "core/LMABaseWeapon.h"
#include "GameFramework/Character.h"

//DEFINE_LOG_CATEGORY_STATIC(WeaponComponentLog, All, All);

// Sets default values for this component's properties
ULMAWeaponComponent::ULMAWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;	
}


// Called when the game starts
void ULMAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
	InitAnimNotify();		
}

void ULMAWeaponComponent::SpawnWeapon() 
{
    if (!IsValid(GetWorld()))
		return;
	
	Weapon = GetWorld()->SpawnActor<ALMABaseWeapon>(WeaponClass);
    if (Weapon)
    {
	const auto Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
	    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	    Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponSocket);
	    BulletsChanged(Weapon->CurrentAmmoWeapon.Bullets); 
		Weapon->RunoutOfBulletes.AddUObject(this, &ULMAWeaponComponent::Reload); // подписка на делега, информирующий о закончившихся патронах
	    Weapon->OnBulletesChanged.AddUObject(this, &ULMAWeaponComponent::BulletsChanged); // подписка на делега, информирующий об изменении количества патронов
	}
    }
}

void ULMAWeaponComponent::Fire()
{
  //  UE_LOG(WeaponComponentLog, Display, TEXT("fire"));
	
	if (Weapon && !AnimReloading)
    {
	    Weapon->TryBurstFire();
		Weapon->Fire();	 
    }
}

void ULMAWeaponComponent::StopFire()
{	
//	UE_LOG(WeaponComponentLog, Display, TEXT("stopt fire"));
    Weapon->ClearBurstFire();
}


void ULMAWeaponComponent::InitAnimNotify()
{
    if (!ReloadMontage) 
		return;

    const auto NotifiesEvents = ReloadMontage->Notifies; //массив всех уведомлений, доступных для добавления к анимации
    for (auto NotifyEvent : NotifiesEvents) //перебор - поиск нужного  уведомления
    {
		auto ReloadFinish = Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify);
		if (ReloadFinish)
		{
			ReloadFinish->OnNotifyReloadFinished.AddUObject(this, &ULMAWeaponComponent::OnNotifyReloadFinished); //подписаться на нужный наш делегат на данное уведомление
			break;
		}
	}
}

void ULMAWeaponComponent::OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh) 
{
    const auto Character = Cast<ACharacter>(GetOwner());
    if (Character->GetMesh() == SkeletalMesh)
    {
		AnimReloading = false;
		Weapon->FinishReloading();
    }
}

bool ULMAWeaponComponent::CanReload() const
{
    if (Weapon->IsCurrentClipFull() || Weapon->GetBurstFire())
	return false;
	
	return !AnimReloading;
}

void ULMAWeaponComponent::BulletsChanged(int NewBullets) 
{
    OnWeaponBulletesChanged.Broadcast(NewBullets);
}

void ULMAWeaponComponent::Reload()
{ 
	if (!CanReload())
	return;

	Weapon->ChangeClip();
    AnimReloading = true;
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    Character->PlayAnimMontage(ReloadMontage);
   
}

bool ULMAWeaponComponent::GetCurrentWeaponAmmo(FAmmoWeapon& AmmoWeapon) const
{
    if (Weapon)
    {
	AmmoWeapon = Weapon->GetCurrentAmmoWeapon();
	return true;
    }
    return false;
}