//// NPC.h
//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/Character.h"
//#include "Quest.h"  // Include definirea questurilor
//#include "NPC.generated.h"
//
//UCLASS()
//class YOURPROJECT_API ANPC : public ACharacter
//{
//    GENERATED_BODY()
//
//public:
//    ANPC();
//
//protected:
//    // Lista de questuri disponibile pe NPC
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
//    TArray<FQuest> AvailableQuests;
//
//public:
//    // Metodă pentru a oferi questurile (de exemplu, pentru a afișa în UI sau în log)
//    UFUNCTION(BlueprintCallable, Category = "Quest")
//    void OfferQuests();
//
//    // Metodă pentru a accepta un quest specific
//    UFUNCTION(BlueprintCallable, Category = "Quest")
//    void AcceptQuest(int32 QuestID);
//
//    // Metodă pentru a marca un quest ca fiind complet
//    UFUNCTION(BlueprintCallable, Category = "Quest")
//    void CompleteQuest(int32 QuestID);
//};
