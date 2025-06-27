#include "NPCInteractionWidget.h"
#include "Components/Button.h"
#include "QuestManager.h"
#include <Kismet/GameplayStatics.h>
#include "ThirdPersonMPCharacter.h"
#include "ItemBase.h"
#include "InventoryComponent.h"
#include "Components/TextBlock.h"



void UNPCInteractionWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (TalkButton) TalkButton->OnClicked.AddDynamic(this, &UNPCInteractionWidget::OnTalkPressed);
    if (ShopButton) ShopButton->OnClicked.AddDynamic(this, &UNPCInteractionWidget::OnShopPressed);
    if (RepairButton) RepairButton->OnClicked.AddDynamic(this, &UNPCInteractionWidget::OnRepairPressed);
    if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &UNPCInteractionWidget::OnExitPressed);
}

void UNPCInteractionWidget::OnTalkPressed()
{
    UE_LOG(LogTemp, Warning, TEXT("Talk button pressed"));

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    AThirdPersonMPCharacter* Player = Cast<AThirdPersonMPCharacter>(PC ? PC->GetPawn() : nullptr);

    if (!Player || !Player->QuestManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Talk] Player sau QuestManager invalid"));
        return;
    }

    TArray<FQuest>& Quests = Player->QuestManager->SideQuests_Blacksmith;

    for (FQuest& Quest : Quests)
    {
        if (Quest.QuestStatus == EQuestStatus::InProgress)
        {
            FString Msg;

            switch (Quest.QuestID)
            {
            case 100: // Talk to blacksmith
            {
                Msg = "Bring me some wood!";
                Player->QuestManager->CompleteQuest(100, false); // false = side quest
                break;
            }
            case 101: // Bring 3 logs
            {
                Msg = "Did you bring the logs? Some iron would be great!";
               
                const FName RequiredItemID = "log"; // DataTable check
                const int32 RequiredQuantity = 3;

                int32 TotalFound = 0;

                for (UItemBase* Item : Player->GetInventoryComponent()->Items)
                {
                    if (Item && Item->ItemID == RequiredItemID)
                    {
                        TotalFound += Item->Quantity;
                    }
                }

                if (TotalFound >= RequiredQuantity)
                {
                    // scadem lemnele din inventar
                    int32 RemainingToRemove = RequiredQuantity;
                    TArray<UItemBase*> ToRemove;
                    for (UItemBase* Item : Player->GetInventoryComponent()->Items)
                    {
                        if (Item && Item->ItemID == RequiredItemID)
                        {
                            const int32 RemoveAmount = FMath::Min(Item->Quantity, RemainingToRemove);
                            Player->GetInventoryComponent()->RemoveItem(Item, RemoveAmount);
                            RemainingToRemove -= RemoveAmount;

                            if (RemainingToRemove <= 0)
                            {
                                break;
                            }
                        }
                    }                

                    Msg = "Thank you for the logs!Now bring me iron ore!";
                    Player->QuestManager->CompleteQuest(101, false);
                }
                else
                {
                    Msg = "You dont have the logs? Then come back later";
                }
                break;
            }
            case 102: // Bring 1 iron ore
            {
                Msg = "Good. Well done!";
                break;
            }
            default:
            {
                Msg = "How can I help you ?";
                break;
            }
            }

            if (DialogTextBlock)
            {
                DialogTextBlock->SetText(FText::FromString(Msg));
            }
            break; // doar primul quest activ
        }
    }
}

void UNPCInteractionWidget::OnShopPressed()
{
    UE_LOG(LogTemp, Warning, TEXT("Shop button pressed"));
}

void UNPCInteractionWidget::OnRepairPressed()
{
    UE_LOG(LogTemp, Warning, TEXT("Repair button pressed"));
}

void UNPCInteractionWidget::OnExitPressed()
{
    UE_LOG(LogTemp, Warning, TEXT("Exit button pressed"));
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    // Restore input
    if (PC)
    {
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = false;
    }
    RemoveFromParent();
}

void UNPCInteractionWidget::SetOwningNPC(ABaseNPC* NPC)
{
    OwningNPC = NPC;
}

