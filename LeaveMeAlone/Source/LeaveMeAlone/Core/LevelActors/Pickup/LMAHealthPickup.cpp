// LeaveMeAlone - all rights reserved


#include "Core/LevelActors/Pickup/LMAHealthPickup.h"

// Sets default values
ALMAHealthPickup::ALMAHealthPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetRootComponent(BoxComponent);

}

void ALMAHealthPickup::PickupWasTaken()
{
    BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    GetRootComponent()->SetVisibility(false, true);
    FTimerHandle RespawnTimerHandle;
    GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ALMAHealthPickup::RespawnPickup, RespawnTime);
}

void ALMAHealthPickup::RespawnPickup()
{
    BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    GetRootComponent()->SetVisibility(true, true);  

}

bool ALMAHealthPickup::GivePickup(ALMADefaultCharacter* Character)
{
    const auto HealthComponent = Character->GetHealthComponent();
    
    if (!HealthComponent)
	    return false;

    return HealthComponent->AddHealth(HealthFromPickup);
}

void ALMAHealthPickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
    const auto Charcter = Cast<ALMADefaultCharacter>(OtherActor);
    if (GivePickup(Charcter))
    {
	PickupWasTaken();
    }
}

