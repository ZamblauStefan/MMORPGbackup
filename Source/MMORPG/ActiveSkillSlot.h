// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActiveSkillSlot.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API UActiveSkillSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Slot")
    FString AssignedSkillID;

    UPROPERTY(meta = (BindWidget))
    class UImage* IconImage;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* KeybindText;

    // functie de initializare slot
    UFUNCTION(BlueprintCallable, Category = "Skill Slot")
    void InitSlot(const FString& InKeybind);
	
	
};
