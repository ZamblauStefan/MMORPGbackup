#pragma once

#include "CoreMinimal.h"
#include "BaseNPC.h"
#include "BlacksmithNPC.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API ABlacksmithNPC : public ABaseNPC
{
	GENERATED_BODY()


public:
	ABlacksmithNPC();
	~ABlacksmithNPC();

	virtual void Interact() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UNPCInteractionWidget> NPCInteractionWidgetClass;


};
