#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentPanel.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API UEquipmentPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(meta = (BindWidget))
    class UEquipmentSlot* WeaponSlot;

    UPROPERTY(meta = (BindWidget))
    class UEquipmentSlot* HelmetSlot;

    UPROPERTY(meta = (BindWidget))
    class UEquipmentSlot* ArmorSlot;

    UPROPERTY(meta = (BindWidget))
    class UEquipmentSlot* GauntletsSlot;

    UPROPERTY(meta = (BindWidget))
    class UEquipmentSlot* BootsSlot;

    UFUNCTION(BlueprintCallable)
    void UpdatePanel(class UEquipmentComponent* Equipment);
	
	
};
