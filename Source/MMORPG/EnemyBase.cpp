// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ThirdPersonMPCharacter.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Runtime/AIModule/Classes/AITypes.h"
#include "Runtime/AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/AIModule/Classes/Perception/AIPerceptionComponent.h"
#include "Runtime/AIModule/Classes/Perception/AISense_Hearing.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include <Kismet/KismetMathLibrary.h>
#include <Components/TextBlock.h>






// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIController::StaticClass();

	// widget health bar
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(RootComponent);

	HealthBarWidget->SetWidgetSpace(EWidgetSpace::World);
	HealthBarWidget->SetDrawSize(FVector2D(80.f, 50.f)); // dimensiuni
	HealthBarWidget->SetPivot(FVector2D(0.5f, 0.f));       // centru jos
	HealthBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 120.f)); // deasupra capului
	HealthBarWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision); // fara colisiune cu playeri
	HealthBarWidget->SetDrawAtDesiredSize(false);

	// Widget class setat in editor sau aici
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/UI/WBP_EnemyHealthBar"));
	if (WidgetClass.Succeeded())
	{
		HealthBarWidget->SetWidgetClass(WidgetClass.Class);
	}

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) // server-ul seteaza initial health
	{
		CurrentHealth = MaxHealth;
	}

	// Widget 
	UUserWidget* Widget = HealthBarWidget->GetUserWidgetObject();
	if (Widget)
	{
		UTextBlock* NameText = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("EnemyName")));
		if (NameText)
		{
			NameText->SetText(EnemyDisplayName); // FText
		}

		UTextBlock* HealthText = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("HealthText")));
		if (HealthText)
		{
			FString HPString = FString::Printf(TEXT("%d / %d"), FMath::RoundToInt(CurrentHealth), FMath::RoundToInt(MaxHealth));
			HealthText->SetText(FText::FromString(HPString));
		}
	}



}

void AEnemyBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("Enemy Tick!"));

	float CurrentTime = GetWorld()->GetTimeSeconds();
	AAIController* AIController = Cast<AAIController>(GetController());

	// resetare valori de Taunt ( Enemy ataca player-ul cu ce-a mai mare valoare Taunt)
	for (FTauntData& Entry : TauntedPlayers)
	{
		if (Entry.PlayerRef)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, FString::Printf(TEXT("%s Taunt: %.1f"), *Entry.PlayerRef->GetName(), Entry.TauntValue));
		}

		if (CurrentTime - Entry.LastDamageTime > TauntResetTime)
		{
			Entry.TauntValue = 0.f;
		}
	}

	//ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ACharacter* Target = nullptr;
	float HighestTaunt = 0.f;

	// Enemy cauta Player-ul cu valoare Taunt ce-a mai mare
	for (const FTauntData& Entry : TauntedPlayers)
	{
		if (Entry.PlayerRef && Entry.TauntValue > HighestTaunt)
		{
			HighestTaunt = Entry.TauntValue;
			Target = Entry.PlayerRef;
		}
	}

	// daca nu exista Taunt (Taunt = 0) atunci Enemy ataca cel mai apropiat Player
	if (!Target)
	{
		float ClosestDist = TNumericLimits<float>::Max();

		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			APlayerController* PC = It->Get();
			if (!PC) continue;

			ACharacter* Pawn = Cast<ACharacter>(PC->GetPawn());
			if (!Pawn) continue;

			float Dist = FVector::Dist(GetActorLocation(), Pawn->GetActorLocation());
			if (Dist < ClosestDist)
			{
				ClosestDist = Dist;
				Target = Pawn;
			}
		}
	}

		// Enemy a gasit Player, se duce dupa el si ataca
		if (Target && AIController)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Player found!"));
			const float AttackRange = 150.0f;
			const float DetectionRadius = 800.0f;
			float DistanceToTarget = FVector::Dist(GetActorLocation(), Target->GetActorLocation());

			if (DistanceToTarget <= DetectionRadius)
			{
				if (DistanceToTarget > AttackRange)
				{
					FAIMoveRequest MoveRequest;
					MoveRequest.SetGoalActor(Target);
					MoveRequest.SetAcceptanceRadius(100.0f);

					FNavPathSharedPtr NavPath;
					AIController->MoveTo(MoveRequest, &NavPath);


					GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green,
						FString::Printf(TEXT("Distanta fata de player: %f"), DistanceToTarget));


				}
				else
				{
					AIController->StopMovement(); // opreste miscarea
				}
			}
			else
			{
				AIController->StopMovement();
			}

		}


		if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			APawn* LocalPlayerPawn = PC->GetPawn();

			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(HealthBarWidget->GetComponentLocation(),PC->PlayerCameraManager->GetCameraLocation());
			FRotator FinalRotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);
			HealthBarWidget->SetWorldRotation(FinalRotation);

			if (LocalPlayerPawn)
			{
				float Distance = FVector::Dist(GetActorLocation(), LocalPlayerPawn->GetActorLocation());
				bool bShouldShow = Distance <= 3000.f;

				if (HealthBarWidget)
				{
					HealthBarWidget->SetVisibility(bShouldShow);
				}
			}
		}

		if (!bInitializedHealthBar)
		{
			if (HealthBarWidget && HealthBarWidget->GetUserWidgetObject())
			{
				HealthUpdate();
				bInitializedHealthBar = true;
			}
		}



}



// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth -= DamageAmount;

	if (CurrentHealth <= 0)
	{
		Die();
	}

	// debug
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("HP: %f"), CurrentHealth));

	// identificam cine a cauzat damage-ul
	ACharacter* DamagerCharacter = nullptr;

	if (EventInstigator)
	{
		DamagerCharacter = Cast<ACharacter>(EventInstigator->GetPawn());
	}
	else if (DamageCauser)
	{
		DamagerCharacter = Cast<ACharacter>(DamageCauser);
	}

	if (DamagerCharacter)
	{
		AddTaunt(DamagerCharacter, DamageAmount);
	}

	RegisterExpContribution(EventInstigator, DamageAmount);

	HealthUpdate();
	return DamageAmount;
}

void AEnemyBase::Die()
{
	// animatie si efect
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Enemy Dead!")));

	for (const FExpContribution& Entry : ExpContributors)
	{
		if (Entry.Player && Entry.DamageDone > 0.f)
		{
			AThirdPersonMPCharacter* PlayerCharacter = Cast<AThirdPersonMPCharacter>(Entry.Player->GetPawn());
			if (PlayerCharacter)
			{
				PlayerCharacter->GainEXP(ExperienceGiven);
			}
		}
	}


	Destroy();
}


void AEnemyBase::AddTaunt(ACharacter* Damager, float Amount)
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	for (FTauntData& Entry : TauntedPlayers)
	{
		if (Entry.PlayerRef == Damager)
		{
			Entry.TauntValue += Amount;
			Entry.LastDamageTime = CurrentTime;
			return;
		}
	}

	// Nu exista inca => add new
	FTauntData NewEntry;
	NewEntry.PlayerRef = Damager;
	NewEntry.TauntValue = Amount;
	NewEntry.LastDamageTime = CurrentTime;
	TauntedPlayers.Add(NewEntry);
}

void AEnemyBase::HealthUpdate()
{
	UUserWidget* Widget = HealthBarWidget->GetUserWidgetObject();
	if (Widget)
	{
		UProgressBar* HPBar = Cast<UProgressBar>(Widget->GetWidgetFromName(TEXT("HealthBar")));
		if (HPBar)
		{
			float HealthPercent = CurrentHealth / MaxHealth;
			HPBar->SetPercent(HealthPercent);
		}

		UTextBlock* HPText = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("HealthText")));
		if (HPText)
		{
			FString Text = FString::Printf(TEXT("%d / %d"), FMath::RoundToInt(CurrentHealth), FMath::RoundToInt(MaxHealth));
			HPText->SetText(FText::FromString(Text));
		}

		UTextBlock* NameText = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("EnemyName")));
		if (NameText)
		{
			NameText->SetText(EnemyDisplayName);
		}
	}
}

void AEnemyBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEnemyBase, CurrentHealth);
}

void AEnemyBase::OnRep_CurrentHealth()
{
	HealthUpdate(); 
}


void AEnemyBase::RegisterExpContribution(AController* DamageInstigator, float Damage)
{
	APlayerController* PC = Cast<APlayerController>(DamageInstigator);
	if (!PC) return;

	for (FExpContribution& Entry : ExpContributors)
	{
		if (Entry.Player == PC)
		{
			Entry.DamageDone += Damage;
			return;
		}
	}

	// adaugam prima data
	FExpContribution NewEntry;
	NewEntry.Player = PC;
	NewEntry.DamageDone = Damage;
	ExpContributors.Add(NewEntry);
}




