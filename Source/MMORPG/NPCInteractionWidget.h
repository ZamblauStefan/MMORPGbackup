// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NPCInteractionWidget.generated.h"

class UButton;
class ABaseNPC;
class UTextBlock;
class InventoryComponent;

UCLASS()
class MMORPG_API UNPCInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

    UFUNCTION()
    void OnTalkPressed();

    UFUNCTION()
    void OnShopPressed();

    UFUNCTION()
    void OnRepairPressed();

    UFUNCTION()
    void OnExitPressed();

    UPROPERTY(meta = (BindWidget))
    UTextBlock* DialogTextBlock;
    UPROPERTY(meta = (BindWidget))
    UButton* TalkButton;
    UPROPERTY(meta = (BindWidget))
    UButton* ShopButton;
    UPROPERTY(meta = (BindWidget))
    UButton* RepairButton;
    UPROPERTY(meta = (BindWidget))
    UButton* ExitButton;

    void SetOwningNPC(ABaseNPC* NPC);

protected:
    ABaseNPC* OwningNPC;
	
	
};
