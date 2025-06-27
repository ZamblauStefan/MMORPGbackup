#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemTooltip.generated.h"


class UTextBlock;
class UVerticalBox;
class UItemBase;


UCLASS()
class MMORPG_API UItemTooltip : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* StatsBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameRTB;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemDescriptionRTB;


	void SetItemDetails(UItemBase* Item);

protected:

	virtual void NativeConstruct() override;
	

};
