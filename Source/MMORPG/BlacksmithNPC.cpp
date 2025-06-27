

#include "BlacksmithNPC.h"
#include "NPCInteractionWidget.h"


ABlacksmithNPC::ABlacksmithNPC()
{
	NPCName = FText::FromString("Blacksmith");
}

ABlacksmithNPC::~ABlacksmithNPC()
{

}



void ABlacksmithNPC::Interact()
{
    Super::Interact();

    if (GEngine)
    {
        FString Message = FString::Printf(TEXT("Interactiune cu Blacksmith!"));
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, Message);
    }

    // adaugare quests/shop etc
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC || !NPCInteractionWidgetClass) return;

    UNPCInteractionWidget* Widget = CreateWidget<UNPCInteractionWidget>(PC, NPCInteractionWidgetClass);
    if (Widget)
    {
        Widget->AddToViewport();
        Widget->SetOwningNPC(this);

        // control pe UI
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(Widget->TakeWidget());
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = true;
    }

}

