#pragma once

#include "CoreMinimal.h"
#include "Quest.h"
#include "Components/ActorComponent.h"
#include "QuestManager.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MMORPG_API UQuestManager : public UActorComponent
{

	GENERATED_BODY()


public:
	UQuestManager();

protected:
	virtual void BeginPlay() override;

public:

	// Lista questurilor curente (pentru jucator)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<FQuest> MainQuests;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quests")
	TArray<FQuest> SideQuests_Blacksmith;
	// initializare questuri
	UFUNCTION(BlueprintCallable)
	void InitializeMainQuests();
	UFUNCTION(BlueprintCallable)
	void InitializeBlacksmithQuests();

	// Functie de finalizare a unui quest, identificat prin ID
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void CompleteQuest(int32 QuestID, bool bIsMain);

	UFUNCTION(BlueprintCallable)
	FQuest GetActiveQuest();
	UFUNCTION(BlueprintCallable)
	FQuest GetBlacksmithActiveQuest();

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentQuestIndex() const;
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentBlacksmithQuestIndex() const;
	UFUNCTION(BlueprintCallable)
	void RegisterEnemyKill();
	// TODO add new functions (verificare stare, listare questuri)

};

