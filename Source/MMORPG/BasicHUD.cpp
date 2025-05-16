// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicHUD.h"
#include "Components/Widget.h"

void UBasicHUD::ToggleCharacterDetails()
{
	if (!CharacterDetailsPanel)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterDetailsPanel is null!"));
		return;
	}

		// switch between visible and collapsed
		const bool bIsVisible = CharacterDetailsPanel->GetVisibility() == ESlateVisibility::Visible;
		CharacterDetailsPanel->SetVisibility(
			bIsVisible ? ESlateVisibility::Collapsed
			: ESlateVisibility::Visible
		);

}

void UBasicHUD::ToggleInventory()
{
	if (!CharacterDetailsPanel)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterDetailsPanel is null!"));
		return;
	}

	// switch between visible and collapsed
	const bool bIsVisible = InventoryPanel->GetVisibility() == ESlateVisibility::Visible;
	InventoryPanel->SetVisibility(
		bIsVisible ? ESlateVisibility::Collapsed
		: ESlateVisibility::Visible
	);
	
}

