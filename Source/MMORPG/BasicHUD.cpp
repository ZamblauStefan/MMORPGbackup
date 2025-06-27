// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicHUD.h"
#include "Components/Widget.h"
#include "InventoryPanel.h"
#include "ThirdPersonMPCharacter.h"
#include "InventoryComponent.h"
#include "EquipmentPanel.h"
#include "EquipmentComponent.h"
#include "Kismet/GameplayStatics.h"


void UBasicHUD::NativeConstruct()
{
	Super::NativeConstruct();
	//InitializeInventory();
}

void UBasicHUD::ToggleCharacterDetails()
{
	if (!CharacterDetailsPanel)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterDetailsPanel is null!"));
		return;
	}

		// switch between visible and collapsed
		const bool bIsVisible = CharacterDetailsPanel->GetVisibility() == ESlateVisibility::Visible;
		CharacterDetailsPanel->SetVisibility(bIsVisible ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

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
	InventoryPanel->SetVisibility(bIsVisible ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

	// Equipment Panel
	if (EquipmentPanel)
	{
		const bool bEqIsVisible = EquipmentPanel->GetVisibility() == ESlateVisibility::Visible;
		EquipmentPanel->SetVisibility(bEqIsVisible ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

		if (bEqIsVisible == false)
		{
			AThirdPersonMPCharacter* Player = Cast<AThirdPersonMPCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			if (Player && Player->GetEquipmentComponent())
			{
				EquipmentPanel->UpdatePanel(Player->GetEquipmentComponent());
			}
		}
	}


}

void UBasicHUD::BindInventoryToUI(UInventoryComponent* InventoryComponent)
{
	if (InventoryComponent)
	{
		// Facem legatura dintre inventory si panel
		//InventoryPanel->BindToInventory(InventoryComponent);
	}
}

void UBasicHUD::ToggleQuestLog()
{
	UE_LOG(LogTemp, Warning, TEXT("Apel: ABasicHUD::ToggleQuests()"));

	if (!QuestWidget && QuestWidgetClass)
	{
		QuestWidget = CreateWidget<UUserWidget>(GetWorld(), QuestWidgetClass);
		if (QuestWidget)
		{
			QuestWidget->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("[BasicHUD]Widget-ul a fost adaugat in Viewport"));
		}
	}
	else if (QuestWidget)
	{
		const bool bVisible = QuestWidget->IsVisible();
		QuestWidget->SetVisibility(bVisible ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

		if (!bVisible)
		{
			if (UFunction* RefreshFunc = QuestWidget->FindFunction(FName("Refresh")))
			{
				QuestWidget->ProcessEvent(RefreshFunc, nullptr);
				UE_LOG(LogTemp, Warning, TEXT("[BasicHUD]Apel Refresh() dupa toggle"));
			}
		}

	}



}


