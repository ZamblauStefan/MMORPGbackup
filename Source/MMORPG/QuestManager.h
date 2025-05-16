// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quest_UStruct.h"
#include "QuestManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MMORPG_API UQuestManager : public UObject
{

	GENERATED_BODY()


public:
	UQuestManager();

	// Lista questurilor curente (pentru jucator)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<FQuest> PlayerQuests;

	// Functie de adaugare a unui quest
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void AddQuest(const FQuest& NewQuest);

	// Functie de acceptare a unui quest, identificat prin ID
	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool AcceptQuest(int32 QuestID);

	// Functie de finalizare a unui quest, identificat prin ID
	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool CompleteQuest(int32 QuestID);



	// TODO add new functions (verificare stare, listare questuri)





/*
QuestManager();
~QuestManager();
*/

};
