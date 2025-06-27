#include "QuestTriggerBox.h"
#include "ThirdPersonMPCharacter.h"


// Sets default values
AQuestTriggerBox::AQuestTriggerBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;

    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    TriggerBox->SetBoxExtent(FVector(200.f));

}

// Called when the game starts or when spawned
void AQuestTriggerBox::BeginPlay()
{
	Super::BeginPlay();
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AQuestTriggerBox::OnOverlapBegin);
	
}

void AQuestTriggerBox::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (AThirdPersonMPCharacter* Player = Cast<AThirdPersonMPCharacter>(OtherActor))
    {
        if (Player->QuestManager)
        {
            Player->QuestManager->CompleteQuest(QuestIDToComplete, true);

            // triggerul se diostruge dupa activare
            Destroy();
        }
    }
}


// Called every frame
/*
void AQuestTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

