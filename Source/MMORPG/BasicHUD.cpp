// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicHUD.h"
#include "Components/Widget.h"
#include "InventoryPanel.h"
#include "ThirdPersonMPCharacter.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"

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
	if (!InventoryPanel)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryPanel is null!"));
		UE_LOG(LogTemp, Warning, TEXT("[BasicHUD] Cream instanta de WB_InventoryPanel!"));
		InventoryPanel = CreateWidget<UInventoryPanel>(GetWorld(), UInventoryPanel::StaticClass());
		InventoryPanel->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[BasicHUD] WB_InventoryPanel deja exista, doar toggle!"));
	}

	// switch between visible and collapsed
	const bool bIsVisible = InventoryPanel->GetVisibility() == ESlateVisibility::Visible;
	InventoryPanel->SetVisibility(
		bIsVisible ? ESlateVisibility::Collapsed
		: ESlateVisibility::Visible
	);

	/*
	// Daca deschidem inventory, populam cu iteme
	if (!bIsVisible)
	{
		AThirdPersonMPCharacter* Character = Cast<AThirdPersonMPCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (Character && Character->GetInventoryComponent())
		{
			// Legam delegate-ul pentru a asculta modificarile de inventory
			InventoryPanel->BindToInventory(Character->GetInventoryComponent());

			// Populam grila de iteme
			InventoryPanel->PopulateInventory();
		}
	}
	*/
}

void UBasicHUD::BindInventoryToUI(UInventoryComponent* InventoryComponent)
{
	if (InventoryComponent)
	{
		// Facem legatura dintre inventory si panel
		//InventoryPanel->BindToInventory(InventoryComponent);
	}
}


/*
void UBasicHUD::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("[BasicHUD] NativeConstruct apelat!"));

	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		AThirdPersonMPCharacter* Character = Cast<AThirdPersonMPCharacter>(PC->GetPawn());
		if (Character && Character->GetInventoryComponent())
		{
			UE_LOG(LogTemp, Warning, TEXT("[BasicHUD] InventoryPanel detectat în NativeConstruct. Legăm delegate-ul."));

			if (InventoryPanel)
			{
				InventoryPanel->BindToInventory(Character->GetInventoryComponent());
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[BasicHUD] Character sau InventoryComp este NULL!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[BasicHUD] PlayerController este NULL!"));
	}
}
*/
