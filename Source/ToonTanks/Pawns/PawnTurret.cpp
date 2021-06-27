// Zhiyuan Gu 2020 All Rights Reserved


#include "PawnTurret.h"
#include "Kismet/GamePlayStatics.h"
#include "PawnTank.h"

// Sets default values
APawnTurret::APawnTurret()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &APawnTurret::CheckFireCondition, FireRate, true);
	PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));

	TEST();
}

void APawnTurret::HandleDestruction()
{
	Super::HandleDestruction();
	//first all base class version, then overidden logic
	Destroy();
}

void APawnTurret::TEST()
{
	// call PawnBase::TEST
	Super::TEST();
	UE_LOG(LogTemp, Warning, TEXT("Turret!"));
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// player needs to be valid & in range
	if (!PlayerPawn || ReturnDistanceToPlayer() > FireRange) {
		return;
	}
	RotateTurret(PlayerPawn->GetActorLocation());
}

void APawnTurret::CheckFireCondition() {
	// player needs to be valid & in range
	if (!PlayerPawn || ReturnDistanceToPlayer() > FireRange || !PlayerPawn->GetIsPlayerAlive()) {
		return;
	}
	Fire();
}

float APawnTurret::ReturnDistanceToPlayer()
{
	if (!PlayerPawn) {
		return 0.0f;
	}
	return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());

}

