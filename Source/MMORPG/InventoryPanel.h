// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryPanel.generated.h"


class UTextBlock;
class UImage;
class UWB_ItemTooltip;
class UItemBase;
struct FItemData;

/**
 * 
 */
UCLASS()
class MMORPG_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemQuantity;

	UPROPERTY()
	UItemBase* ItemData;

	UPROPERTY()
	UWB_ItemTooltip* Tooltip;

	void SetItemData(UItemBase* NewItem);


protected:


	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	
};
