// Fill out your copyright notice in the Description page of Project Settings.

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

	void SetItemDetails(UItemBase* Item);

protected:

	virtual void NativeConstruct() override;
	

};
