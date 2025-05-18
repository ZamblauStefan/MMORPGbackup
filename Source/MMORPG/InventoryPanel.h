#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryPanel.generated.h"

class UUniformGridPanel;
class UInventoryComponent;
class UInventoryItem;
class UInventoryItemWidget;


UCLASS()
class MMORPG_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void InitializePanel(UInventoryComponent* InventoryComp);

protected:

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* GridPanel;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UInventoryItemWidget> ItemWidgetClass;
	
private:

	// referinta catre inventory
	UPROPERTY()
	UInventoryComponent* LinkedInventory;

	void RefreshInventory();

	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<UInventoryItem> InventoryItemClass;

	UFUNCTION()
	void PopulateInventory();
	void BindToInventory(UInventoryComponent* InventoryComponent);

	void NativeDestruct();


protected:

	virtual void NativeConstruct() override;
	*/
};
