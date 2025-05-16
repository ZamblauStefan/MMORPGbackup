#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItem.generated.h"


class UImage;
class UTextBlock;
class UItemTooltip;
class UItemBase;

UCLASS()
class MMORPG_API UInventoryItem : public UUserWidget
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
	UItemTooltip* Tooltip;

	void SetItemData(UItemBase* NewItem);

protected:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;


};
	
	

