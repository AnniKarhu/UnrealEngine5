// LeaveMeAlone - all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/Components/LMAHealthComponent.h"
#include "Core/Components/LMAStaminaComponent.h"

#include "LMADefaultCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UDecalComponent;
class UMaterialInterface;
class ULMAHealthComponent;
class ULMStaminaComponent;
class ULMAWeaponComponent;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Weapon")
	ULMAWeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Stamina")
	ULMAStaminaComponent* StaminaComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprinting")
	bool TrySprinting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprinting")
	float SprintingSpeed = 1000.f; //заменить на BlueprintRead VisibleAnywhere

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprinting")
	float WalkingSpeed = 300.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprinting")
	bool IsSprinting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprinting")
	float SprintStaminaCost = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprinting")
	float StaminaRecoverCost = 2;

private:

	float YRotation = -75.0f; //поворот камеры по оси Y
	float ArmLength = 1400.0f; //длина штатива.
	float FOV = 55.0f; //поле зрения камеры	

	/*METHODS*/

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnHealthChanged(float NewHealth);
	void OnStaminaChanged(float NewStamina);

	UFUNCTION()
	ULMAHealthComponent* GetHealthComponent() const { return HealthComponent; }	

	UFUNCTION()
	ULMAStaminaComponent* GetStaminaComponent() const { return StaminaComponent; }
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	
	void StartSprint();
	void StopSprint();

	void ZoomCamera(float Value);

private:
	void OnDeath(); //	функция, которая будет реагировать на сообщение от делегата, о том, что персонаж умер

	void RotationPlayerOnCursor();

	bool CanSprint() const;

};
