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
    float RespawnTime = 5.0f; //�����     �������� ������

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "5.0f", ClampMax = "100.0f"))
    float HealthFromPickup = 100.0f; //���������� ������, ������� �������� �����


    


/*Methods*/

public:	
	// Sets default values for this actor's properties
	ALMAHealthPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override; //�������������� ����� �� ������� ������ ������ -  ��������� �� ����������� ����� �������� � ������� ��������

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool GivePickup(ALMADefaultCharacter* Character); //����� �������� �� ��, ����� �� �� �����
	void PickupWasTaken(); //��������� ������ �������� � �������� ����� �� ��������� ����������, ����� ������ ������.
	void RespawnPickup(); //���������� ��������� ������ �� ���������� ������������� ���������� �������

};
