// LeaveMeAlone - all rights reserved

#include "Core/Components/LMAWeaponComponent.h"
#include "Core/Animations/LMAReloadFinishedAnimNotify.h"
#include "core/LMABaseWeapon.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(WeaponComponentLog, All, All);

// Sets default values for this component's properties
ULMAWeaponComponent::ULMAWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
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
	}
    }
}

void ULMAWeaponComponent::Fire()
{
    if (Weapon && !AnimReloading)
    {
	Weapon->Fire();
    }
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
    }
}

bool ULMAWeaponComponent::CanReload() const
{
    return !AnimReloading;
}

void ULMAWeaponComponent::Reload()
{
    UE_LOG(WeaponComponentLog, Display, TEXT("enter reload"));
	
	if (!CanReload())
	return;

	UE_LOG(WeaponComponentLog, Display, TEXT("enter reload 2"));
    Weapon->ChangeClip();
    AnimReloading = true;
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    UE_LOG(WeaponComponentLog, Display, TEXT("before play montage"));
    Character->PlayAnimMontage(ReloadMontage);
    UE_LOG(WeaponComponentLog, Display, TEXT("after play montage"));
}