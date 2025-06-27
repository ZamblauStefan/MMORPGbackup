// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BasicHUD.generated.h"

class UInventoryPanel;
class UInventoryComponent;
class UEquipmentPanel;

UCLASS()
class MMORPG_API UBasicHUD : public UUserWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UUserWidget* CharacterDetailsPanel;

	UPROPERTY(meta = (BindWidget))
	UInventoryPanel* InventoryPanel;

	UPROPERTY(meta = (BindWidget))
	UEquipmentPanel* EquipmentPanel;

	UFUNCTION(BlueprintCallable)
	void ToggleCharacterDetails();
	UFUNCTION(BlueprintCallable)
	void ToggleInventory();

	UPROPERTY()
	UUserWidget* QuestWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> QuestWidgetClass;

	UFUNCTION(BlueprintCallable)
	void ToggleQuestLog();


	// Functie pentru legarea delegate-ului
	void BindInventoryToUI(UInventoryComponent* InventoryComponent);

	void NativeConstruct();


};
