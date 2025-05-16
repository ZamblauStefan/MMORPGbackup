// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"
#include "GameMenu.h"
#include "InteractionWidget.h"

AMainHUD::AMainHUD()
{

}

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	if (GameMenuClass)
	{
		GameMenuWidget = CreateWidget<UGameMenu>(GetWorld(), GameMenuClass);
		GameMenuWidget->AddToViewport(5);
		GameMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}



}

void AMainHUD::DisplayMenu()
{
	if (GameMenuWidget)
	{
		bIsMenuVisible = true;
		GameMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainHUD::HideMenu()
{
	if (GameMenuWidget)
	{
		bIsMenuVisible = true;
		GameMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMainHUD::ShowInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainHUD::HideInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMainHUD::UpdateInteractionWidget(const FInteractableData* InteractableData) const
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);

		}

		 InteractionWidget->UpdateWidget(InteractableData);

	}

}
