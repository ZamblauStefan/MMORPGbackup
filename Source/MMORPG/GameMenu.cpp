// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMenu.h"
#include "ThirdPersonMPCharacter.h"




void UGameMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGameMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<AThirdPersonMPCharacter>(GetOwningPlayerPawn());
}

bool UGameMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	// cast operation to item drop, ensure player is valid, call drop item on player
}
