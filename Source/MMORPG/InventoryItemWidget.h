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

protected:
    UPROPERTY(meta = (BindWidget))
    UImage* ItemIcon;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Quantity;

//private:
    //UPROPERTY()
   // UItemBase* ItemData;
	
	
};
