#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Logging/LogMacros.h"
#include "ThirdPersonMPCharacter.h"
#include "Components/Widget.h"

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT(">> MyPlayerController::BeginPlay() called"));


    if (IsLocalController() && BasicHUDClass)
    {
        // Creeaza widget-ul de HUD folosind controller-ul curent ca owner
        BasicHUDWidget = CreateWidget<UBasicHUD>(this, BasicHUDClass);
        if (BasicHUDWidget)
        {
            BasicHUDWidget->AddToViewport();

            // Ascunde panoul de detalii la început
            if (UWidget* DetailsPanel = BasicHUDWidget->GetWidgetFromName(TEXT("CharacterDetailsPanel")))
            {
                DetailsPanel->SetVisibility(ESlateVisibility::Collapsed);
            }
            else
            {
                UE_LOG(LogTemplateCharacter, Warning, TEXT("DetailsPanel nu a fost gasit in HUD-ul creat."));
            }
        }
        else
        {
            UE_LOG(LogTemplateCharacter, Warning, TEXT("Failed to create BasicHUDWidget in PlayerController."));
        }
    }
}

void AMyPlayerController::ToggleCharacterDetails()
{
    if (BasicHUDWidget)
    {
        BasicHUDWidget->ToggleCharacterDetails();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("MyPC: BasicHUDWidget e null in ToggleCharacterDetails"));
    }
}

void AMyPlayerController::ToggleInventory()
{
    if (BasicHUDWidget)
    {
        BasicHUDWidget->ToggleInventory();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("MyPC: BasicHUDWidget e null in ToggleInventory"));
    }
}
