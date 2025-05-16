//#include "Bandit.h"
//#include "Engine/Engine.h"
//
//ABandit::ABandit()
//{
//    PrimaryActorTick.bCanEverTick = true;
//
//    // Inițializări implicite
//    MaxHealth = 100.f;
//    CurrentHealth = MaxHealth;
//    AttackDamage = 20.f;
//    Description = TEXT("A cunning bandit, dangerous and quick on his feet.");
//    BanditStatus = EBanditStatus::Idle;
//}
//
//void ABandit::BeginPlay()
//{
//    Super::BeginPlay();
//    // Poți inițializa sau afişa informații despre bandit la pornire
//    if (GEngine)
//    {
//        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Bandit spawned."));
//    }
//}
//
//void ABandit::Attack()
//{
//    BanditStatus = EBanditStatus::Attacking;
//    // Aici poți declanșa animații, sunete sau logica de atac
//    if (GEngine)
//    {
//        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Bandit is attacking!"));
//    }
//}
//
//void ABandit::ReceiveDamage(float DamageAmount)
//{
//    CurrentHealth -= DamageAmount;
//    if (CurrentHealth <= 0.f)
//    {
//        CurrentHealth = 0.f;
//        Die();
//    }
//    else
//    {
//        if (GEngine)
//        {
//            FString Msg = FString::Printf(TEXT("Bandit takes %.1f damage. Health: %.1f"), DamageAmount, CurrentHealth);
//            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Msg);
//        }
//    }
//}
//
//void ABandit::Die()
//{
//    BanditStatus = EBanditStatus::Dead;
//    // Anunță moartea și eventual elimină actorul din lume
//    if (GEngine)
//    {
//        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Bandit has died."));
//    }
//    // Aici poți adăuga logica de distrugere sau de "respawn", după caz
//    Destroy();
//}
