// LeaveMeAlone - all rights reserved


#include "Core/Characters/LMADefaultCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ALMADefaultCharacter::ALMADefaultCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetRootComponent());
	

	SpringArm->TargetArmLength = ArmLength - 1000;

	//структура FRotator хранит аргументы в следующей последовательности : Pitch, Yaw, Roll.
	// Так как нам необходимо определить значения по оси Y, мы 	устанавливаем Pitch аргумент
	SpringArm->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f)); 
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true; //плавное перемещение, исключение дерганий камеры при быстрых хаотичных движениях персонажа

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	Camera->SetFieldOfView(FOV); //угол обзора камеры
	Camera->bUsePawnControlRotation = false; //данное условие запрещаем камере вращаться относительно SpringArmComponent

	//запретить персонажу поворачиваться в сторону камеры
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>("HealthComponent");
	StaminaComponent = CreateDefaultSubobject<ULMAStaminaComponent>("StaminaComponent");

}

// Called when the game starts or when spawned
void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(CursorMaterial) && GetWorld())
	{
		CursorDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector::Zero());
	}

	HealthComponent->OnDeath.AddUObject(this, &ALMADefaultCharacter::OnDeath); //подписка на делега, информирующий о смерти персонажа

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnHealthChanged.AddUObject(this, &ALMADefaultCharacter::OnHealthChanged);

	OnStaminaChanged(StaminaComponent->GetStamina());
	StaminaComponent->OnStaminaChanged.AddUObject(this, &ALMADefaultCharacter::OnStaminaChanged);	
	
}

void ALMADefaultCharacter::MoveForward(float Value) 
{
	AddMovementInput(GetActorForwardVector(), Value);

	//float temp = -1;
	//temp = FVector::DotProduct(GetVelocity(), GetActorRotation().Vector());
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Speed = %f"), temp));

    if ((IsSprinting)   &&(!(FMath::IsNearlyEqual(Value, 0))))
    {		
		StaminaComponent->DecreaseStamina(SprintStaminaCost);
		if (!CanSprint())
		{
			StopSprint();
		}
    }
    else
    {
		StaminaComponent->IncreaseStamina(StaminaRecoverCost);	
    }
    
	
}

void ALMADefaultCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void ALMADefaultCharacter::StartSprint()
{ 	
	if (CanSprint())
	{
	    IsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}		
}

bool ALMADefaultCharacter::CanSprint() const
{
    return (StaminaComponent->EnoughStaminaForSprint());
}

void ALMADefaultCharacter::StopSprint()
{
    IsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;

	//запустить таймер, который каждую секунду будет добавлять по 10 тамина и перезапускать сам себя, если стамина меньше максимальной
	//таймер нужно отключать при начале спринта
}

void ALMADefaultCharacter::ZoomCamera(float Value)
{
	
	//UE_LOG(LogTemp, Display, TEXT("Value = : %f"), Value);
	float TempDistance = SpringArm->TargetArmLength + Value* CameraZoomScale;

	if ((TempDistance < CameraMinDistance) || (TempDistance > CameraMaxDistance))
	{
		return;
	}	
	
	SpringArm->TargetArmLength = TempDistance;
	
	//UE_LOG(LogTemp, Display, TEXT("TargetArmLength = : %f"), SpringArm->TargetArmLength);
}

// Called every frame
void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!(HealthComponent->IsDead()))
	{
	    RotationPlayerOnCursor();
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC) //если контроллер задан
	{
		//проверяем трассировку курсора
		
		//структура FHitResult хранит всю информацию о результате столкновения-пересечения
		//хранит поля с информацией о столкновении с различными объектами, дастанцию, место начала / конца трейса и т.д.		
		FHitResult ResultHit; 

		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit); //ECC_GameTraceChannel1 - это техническое название MouseCursor - тех. название можно посмотреть в файле DefaultEngine.ini в папке в config
		float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
		SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f))); //повернуть игрока на полученный угол FindRotatorResultYaw
		if (CursorDecal)
		{
			CursorDecal->SetWorldLocation(ResultHit.Location);
		}
	}

}

// Called to bind functionality to input
void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ALMADefaultCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ALMADefaultCharacter::StopSprint);

	

	PlayerInputComponent->BindAxis("ZoomCamera", this, &ALMADefaultCharacter::ZoomCamera);
}

void ALMADefaultCharacter::OnDeath()
{
    CursorDecal->DestroyRenderState_Concurrent();
	
	PlayAnimMontage(DeathMontage);  //проигрывание  анимации монтажа - анимация смерти
    GetCharacterMovement()->DisableMovement(); // запрет движения после смерти
    SetLifeSpan(5.0f); //время,     через которое объект будет уничтожен

	if (Controller)
    {
		Controller->ChangeState(NAME_Spectating);
    }
}

void ALMADefaultCharacter::OnHealthChanged(float NewHealth)
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Health = %f"), NewHealth));
}

void ALMADefaultCharacter::OnStaminaChanged(float NewStamina) 
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Stamina = %f"), NewStamina));
}

void ALMADefaultCharacter::RotationPlayerOnCursor() 
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
		FHitResult ResultHit;
		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
		float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
		SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));
		if (CursorDecal)
		{
			CursorDecal->SetWorldLocation(ResultHit.Location);
		}
    }

}

