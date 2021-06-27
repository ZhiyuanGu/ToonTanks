// Zhiyuan Gu 2020 All Rights Reserved


#include "HealthComponent.h"
#include "ToonTanks/GameModes/TankGameModeBase.h"
#include "Kismet/GamePlayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();


	Health = DefaultHealth;
	GameModeRef = Cast<ATankGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	//UE_LOG(LogTemp, Warning, TEXT("Health Comp ower %s"), *GetOwner()->GetName());
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Received damage"));
	if (Damage <= 0.0f || Health <= 0) {
		return;
	}
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth); // make sure in range
	UE_LOG(LogTemp, Warning, TEXT("Takes damage amount %f, health left %f"), Damage, Health);
	if (Health <= 0) {
		if (GameModeRef) {
			GameModeRef->ActorDied(GetOwner());
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Health Component has no reference to GameMode"));
		}
	}
}


