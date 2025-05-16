// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestReward.h"
#include "Quest_UStruct.generated.h"



// Enum pentru statusul quest-ului
UENUM(BlueprintType)
enum class EQuestStatus : uint8
{
    NotStarted  UMETA(DisplayName = "Not Started"),
    InProgress  UMETA(DisplayName = "In Progress"),
    Completed   UMETA(DisplayName = "Completed")
};


// Structura ce defineste un quest
USTRUCT(BlueprintType)
struct FQuest
{
    GENERATED_BODY()


public:
    // ID-ul quest-ului
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    int QuestID;

    // Nume quest
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FString QuestName;

    // Descriere quest
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FString QuestDescription;

    // Stare curenta a quest-ului
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    EQuestStatus QuestStatus;

    // Lista de recompense pentru quest
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    TArray<FQuestReward> Rewards;



    // Consstructor
    FQuest()
        :QuestID(0)
        ,QuestName(TEXT("Default Quest"))
        ,QuestDescription(TEXT("No description available"))
        ,QuestStatus(EQuestStatus::NotStarted)
    { }

    // Constructor cu parametri fara rewards
    FQuest(int32 InID, const FString& InName, const FString& InDescription)
        :QuestID(InID)
        ,QuestName(InName)
        ,QuestDescription(InDescription)
        ,QuestStatus(EQuestStatus::NotStarted)
    { }


    // Constructor cu parametri cu rewards
    FQuest(int32 InID, const FString& InName, const FString& InDescription,TArray<FQuestReward> InRewards )
        :QuestID(InID)
        , QuestName(InName)
        , QuestDescription(InDescription)
        , QuestStatus(EQuestStatus::NotStarted)
        , Rewards(InRewards)
    {}

};






/**
 * 
 
class MMORPG_API Quest_UStruct
{
public:
	Quest_UStruct();
	~Quest_UStruct();
};
*/