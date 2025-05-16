//// Quest.h
//#pragma once
//
//#include "CoreMinimal.h"
//#include "Quest.generated.h"
//
//UENUM(BlueprintType)
//enum class EQuestStatus : uint8
//{
//    NotStarted     UMETA(DisplayName = "Not Started"),
//    InProgress     UMETA(DisplayName = "In Progress"),
//    Completed      UMETA(DisplayName = "Completed")
//};
//
//USTRUCT(BlueprintType)
//struct FQuest
//{
//    GENERATED_BODY()
//
//public:
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
//    int32 QuestID;
//
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
//    FString QuestName;
//
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
//    FString QuestDescription;
//
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
//    EQuestStatus QuestStatus;
//
//    // Constructor implicit
//    FQuest()
//        : QuestID(0)
//        , QuestName(TEXT("Default Quest"))
//        , QuestDescription(TEXT("No description"))
//        , QuestStatus(EQuestStatus::NotStarted)
//    {
//    }
//};
