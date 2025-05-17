#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryPanel.generated.h"

class UUniformGridPanel;
class UInventoryComponent;
class UInventoryItem;


UCLASS()
class MMORPG_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* GridPanel;
	
	// referinta catre inventory
	UPROPERTY()
	UInventoryComponent* LinkedInventory;

	UFUNCTION()
	void PopulateInventory();
	void BindToInventory(UInventoryComponent* InventoryComponent);

	void NativeDestruct();


protected:

	virtual void NativeConstruct() override;
	
};
