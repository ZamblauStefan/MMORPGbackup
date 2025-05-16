// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"


class UGameMenu;
class UInteractionWidget;
struct FInteractableData;



UCLASS()
class MMORPG_API AMainHUD : public AHUD
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UGameMenu> GameMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	bool bIsMenuVisible;

	AMainHUD();

	void DisplayMenu();
	void HideMenu();

	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;
	void UpdateInteractionWidget(const FInteractableData* InteractableData) const;


protected:

	UPROPERTY();
	UGameMenu* GameMenuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;


	virtual void BeginPlay() override;
	
};
