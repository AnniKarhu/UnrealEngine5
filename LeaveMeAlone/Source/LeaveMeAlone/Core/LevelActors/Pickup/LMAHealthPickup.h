// LeaveMeAlone - all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Core/Characters/LMADefaultCharacter.h"
#include "LMAHealthPickup.generated.h"

UCLASS()
class LEAVEMEALONE_API ALMAHealthPickup : public AActor
{
	GENERATED_BODY()

/*Fields*/
protected:

	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	//USphereComponent* SphereComponent;
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float RespawnTime = 5.0f; //время 	респауна актора

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "5.0", ClampMax = "100.0")) //, meta = (ClampMin = "5.0f", ClampMax = "100.0f"))
	float HealthFromPickup = 100.0f; //количество жизней, которое прибавит пикап

/*Methods*/	
public:	
	// Sets default values for this actor's properties
	ALMAHealthPickup();

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override; //переопределяем метод базового класса Актора, он реагирует на пересечение сетки коллизии с другими Акторами
	

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

private:
	bool GivePickup(ALMADefaultCharacter* Character); //отвечает за то, взят ли  пикап
	void PickupWasTaken(); //запускает таймер респауна 	и скрывает пикап на временной промежуток, после взятия пикапа
	void RespawnPickup(); //возвращает видимость пикапа по прошествии определенного количества времени

};
