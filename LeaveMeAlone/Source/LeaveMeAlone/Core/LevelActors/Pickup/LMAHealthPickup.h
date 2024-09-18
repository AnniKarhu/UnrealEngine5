// LeaveMeAlone - all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Core/Characters/LMADefaultCharacter.h"

#include "LMAHealthPickup.generated.h"

UCLASS()
class LEAVEMEALONE_API ALMAHealthPickup : public AActor
{
    GENERATED_BODY() 

/*Fields*/

protected:

    UPROPERTY(VisibleAnywhere, Category = "Pickup")
    USphereComponent* SphereComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    float RespawnTime = 5.0f; //время     респауна актора

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "5.0f", ClampMax = "100.0f"))
    float HealthFromPickup = 100.0f; //количество жизней, которое прибавит пикап


    


/*Methods*/

public:	
	// Sets default values for this actor's properties
	ALMAHealthPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override; //переопределяем метод из базовом классе Актора -  реагирует на пересечение сетки коллизии с другими Акторами

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool GivePickup(ALMADefaultCharacter* Character); //метод отвечает за то, взяли ли мы пикап
	void PickupWasTaken(); //запускать таймер респауна и скрывать пикап на временной промежуток, после взятия пикапа.
	void RespawnPickup(); //возвращает видимость пикапа по прошествии определенного количества времени

};
