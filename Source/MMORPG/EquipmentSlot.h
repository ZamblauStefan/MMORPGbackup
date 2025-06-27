#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentComponent.h"
#include "EquipmentSlot.generated.h"

UCLASS()
class MMORPG_API UEquipmentSlot : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
    UFUNCTION(BlueprintCallable)
    void SetSlot(class UItemBase* Item);

    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
    EEquipmentSlot SlotType;

protected:
    UPROPERTY(meta = (BindWidget))
    class UImage* IconImage;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* NameText;

    UPROPERTY()
    class UItemBase* StoredItem;
	
};
