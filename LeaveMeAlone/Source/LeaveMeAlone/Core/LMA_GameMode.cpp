// LeaveMeAlone - all rights reserved


#include "Core/LMA_GameMode.h"
#include "Characters/LMADefaultCharacter.h"
#include "Characters/LMAPlayerController.h"

ALMA_GameMode::ALMA_GameMode()
{
	DefaultPawnClass = ALMADefaultCharacter::StaticClass();
	PlayerControllerClass = ALMAPlayerController::StaticClass();
}