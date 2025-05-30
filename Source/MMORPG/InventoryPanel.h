#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryPanel.generated.h"

class UUniformGridPanel;
class UInventoryComponent;
//class UInventoryItem;
class UInventoryItemWidget;
class UItemBase;



UCLASS()
class MMORPG_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:

	//void InitializePanel(UInventoryComponent* InventoryComp);

	UFUNCTION(BlueprintCallable)
	void RefreshInventory(const TArray<class UItemBase*>& Items);

	void NativeConstruct();

	UFUNCTION(BlueprintCallable)
	void BindToInventory(UInventoryComponent* InventoryComponent);

protected:

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* GridPanel;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UInventoryItemWidget> ItemWidgetClass;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	//TSubclassOf<UInventoryItem> InventoryItemClass;

private:

	 // referinta catre inventory
	UPROPERTY()
	UInventoryComponent* LinkedInventory;


	//UFUNCTION()
	//void PopulateInventory(const TArray<UItemBase*>& Items);

	//void NativeDestruct();


//protected:


};
