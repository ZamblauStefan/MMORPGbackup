// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "InventoryItemWidget.generated.h"

class UItemBase;
class UImage;
class UTextBlock;
class UItemTooltip;
class UItemData;

/**
 * 
 */
UCLASS()
class MMORPG_API UInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
 
    // Initializeaza widget-ul cu datele item-ului
    UFUNCTION(BlueprintCallable)
    void InitItem(UItemBase* Item);

    UPROPERTY()
    class UItemBase* ItemData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UItemTooltip> TooltipClass;

    UItemTooltip* CurrentTooltip;

    virtual void NativePreConstruct() override;
    virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    void UseItem();


protected:
    UPROPERTY(meta = (BindWidget))
    UImage* ItemIcon;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Quantity;
	
	
};
