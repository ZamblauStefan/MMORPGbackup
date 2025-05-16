// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BasicHUD.generated.h"
/**
 * 
 */
UCLASS()
class MMORPG_API UBasicHUD : public UUserWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UUserWidget* CharacterDetailsPanel;

	UPROPERTY(meta = (BindWidget))
	UUserWidget* InventoryPanel;

	UFUNCTION(BlueprintCallable)
	void ToggleCharacterDetails();
	UFUNCTION(BlueprintCallable)
	void ToggleInventory();

};
