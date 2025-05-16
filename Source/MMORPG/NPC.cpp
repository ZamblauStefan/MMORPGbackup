//// NPC.cpp
//#include "NPC.h"
//#include "Engine/Engine.h"
//
//ANPC::ANPC()
//{
//    // Exemplu: inițializăm o quest simplă pe care NPC-ul o poate oferi
//    FQuest NewQuest;
//    NewQuest.QuestID = 1;
//    NewQuest.QuestName = TEXT("Căutarea Aventurierului");
//    NewQuest.QuestDescription = TEXT("Vorbește cu bătrânul din sat pentru a primi instrucțiuni.");
//    NewQuest.QuestStatus = EQuestStatus::NotStarted;
//
//    AvailableQuests.Add(NewQuest);
//}
//
//void ANPC::OfferQuests()
//{
//    // Exemplu de afișare în log.
//    for (const FQuest& Quest : AvailableQuests)
//    {
//        FString StatusString;
//        switch (Quest.QuestStatus)
//        {
//        case EQuestStatus::NotStarted:
//            StatusString = TEXT("Not Started");
//            break;
//        case EQuestStatus::InProgress:
//            StatusString = TEXT("In Progress");
//            break;
//        case EQuestStatus::Completed:
//            StatusString = TEXT("Completed");
//            break;
//        default:
//            StatusString = TEXT("Unknown");
//            break;
//        }
//        UE_LOG(LogTemp, Log, TEXT("Quest Available: ID=%d, Name=%s, Status=%s"),
//            Quest.QuestID, *Quest.QuestName, *StatusString);
//    }
//}
//
//void ANPC::AcceptQuest(int32 QuestID)
//{
//    for (FQuest& Quest : AvailableQuests)
//    {
//        if (Quest.QuestID == QuestID && Quest.QuestStatus == EQuestStatus::NotStarted)
//        {
//            Quest.QuestStatus = EQuestStatus::InProgress;
//            UE_LOG(LogTemp, Log, TEXT("Quest %s accepted."), *Quest.QuestName);
//            return;
//        }
//    }
//    UE_LOG(LogTemp, Warning, TEXT("Quest with ID %d not found or already accepted."), QuestID);
//}
//
//void ANPC::CompleteQuest(int32 QuestID)
//{
//    for (FQuest& Quest : AvailableQuests)
//    {
//        if (Quest.QuestID == QuestID && Quest.QuestStatus == EQuestStatus::InProgress)
//        {
//            Quest.QuestStatus = EQuestStatus::Completed;
//            UE_LOG(LogTemp, Log, TEXT("Quest %s completed."), *Quest.QuestName);
//            return;
//        }
//    }
//    UE_LOG(LogTemp, Warning, TEXT("Quest with ID %d not found or not in progress."), QuestID);
//}
