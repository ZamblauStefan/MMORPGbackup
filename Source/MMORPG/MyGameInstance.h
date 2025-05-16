// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "QuestManager.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:
	// Functie ce va fi apelata de butonul "Start"
	UFUNCTION(BlueprintCallable, Category = "Networking")
	void ConnectToServer(const FString& ServerIP);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	UQuestManager* QuestManager;

	virtual void Init() override;
	
	
};
