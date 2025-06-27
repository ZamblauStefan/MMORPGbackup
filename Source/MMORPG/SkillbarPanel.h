// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillbarPanel.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API USkillbarPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:

    // Sloturile din skillbar
    UPROPERTY(meta = (BindWidget))
    class UActiveSkillSlot* Slot1;

    UPROPERTY(meta = (BindWidget))
    class UActiveSkillSlot* Slot2;

    UPROPERTY(meta = (BindWidget))
    class UActiveSkillSlot* Slot3;

    UPROPERTY(meta = (BindWidget))
    class UActiveSkillSlot* Slot4;

    // initializare skillbar cu keybind-urile default
    UFUNCTION(BlueprintCallable, Category = "Skillbar")
    void InitSkillbar();
	
	
};
