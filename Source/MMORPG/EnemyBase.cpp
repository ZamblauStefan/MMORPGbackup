// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ThirdPersonMPCharacter.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Runtime/AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"





// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIController::StaticClass();

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;


}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (Player)
	{
		ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		AAIController* AIController = Cast<AAIController>(GetController());

		if (Player && AIController)
		{
			FAIMoveRequest MoveRequest(Player); // setezi aici goal-ul
			MoveRequest.SetAcceptanceRadius(100.0f);

			FNavPathSharedPtr NavPath;
			AIController->MoveTo(MoveRequest, &NavPath);
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

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("HP: %f"), CurrentHealth));

	return DamageAmount;
}

void AEnemyBase::Die()
{
	// animatie si efect
	Destroy();
}
