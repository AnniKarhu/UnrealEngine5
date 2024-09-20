// LeaveMeAlone - all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/Components/LMAHealthComponent.h"
#include "LMADefaultCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UDecalComponent;
class UMaterialInterface;
class ULMAHealthComponent;
class UAnimMontage;


UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALMADefaultCharacter();

	/*PROPERTIES*/
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UCameraComponent* Camera;

	UPROPERTY()
	UDecalComponent* CursorDecal; //UDecalComponent* CurrentCursor = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial; //UMaterialInterface* CursorMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f); 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraZoom")
	float CameraZoomScale = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraZoom")
	float CameraMinDistance = 250;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraZoom")
	float CameraMaxDistance = 5000;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
	ULMAHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprinting")
	bool IsSprinting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprinting")
	float SprintingSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprinting")
	float WalkingSpeed = 300.f;

private:

	float YRotation = -75.0f; //������� ������ �� ��� Y
	float ArmLength = 1400.0f; //����� �������.
	float FOV = 55.0f; //���� ������ ������

	/*METHODS*/

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnHealthChanged(float NewHealth);

	UFUNCTION()
	ULMAHealthComponent* GetHealthComponent() const { return HealthComponent; }

	UFUNCTION(BlueprintCallable)
	bool GetIsSprinting() const { return IsSprinting; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	
	void StartSprint();
	void StopSprint();

	void ZoomCamera(float Value);

private:
	void OnDeath(); //	�������, ������� ����� ����������� �� ��������� �� ��������, � ���, ��� �������� ����

	void RotationPlayerOnCursor();

};
