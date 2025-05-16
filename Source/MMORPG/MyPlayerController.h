// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicHUD.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API AMyPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;

protected:
    /** Set this in Blueprint: Widget Blueprint subclass of UBasicHUD */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UBasicHUD> BasicHUDClass;

private:
    /** Instance of the HUD widget */
    UPROPERTY()
    UBasicHUD* BasicHUDWidget;

public:
    UFUNCTION()
    void ToggleCharacterDetails();

    UFUNCTION()
    void ToggleInventory();

};