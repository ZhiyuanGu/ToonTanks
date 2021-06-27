// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
APawnTank::APawnTank()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(BaseMesh);
	/*FVector SpringArmRelativeLocation = SpringArm->GetRelativeLocation();
	UE_LOG(LogTemp, Warning, TEXT("Current relative location is %d %d %d"), SpringArmRelativeLocation.X, SpringArmRelativeLocation.Y, SpringArmRelativeLocation.Z);
	SpringArm->SetRelativeLocation(FVector(0, 0, 0));
	UE_LOG(LogTemp, Warning, TEXT("Current relative location is %d %d %d"), SpringArmRelativeLocation.X, SpringArmRelativeLocation.Y, SpringArmRelativeLocation.Z);*/
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);


	//UE_LOG(LogTemp, Warning, TEXT("Tank name %s"), *GetName());


}

// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerRef = Cast<APlayerController>(GetController());

}

void APawnTank::ApplyFireEffects()
{
	GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(FireShake);
}

// Called every frame
void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Rotate();
	Move();
	if (PlayerControllerRef) {
		// rotate to where the mouse points to
		FHitResult HitResult;
		PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		FVector HitLocation = HitResult.ImpactPoint;
		RotateTurret(HitLocation);
	}
}

// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// calculate functions running constantly, value = 0 if not pressed
	PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMoveInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawnTank::CalculateRotateInput);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawnTank::Fire);
}

bool APawnTank::GetIsPlayerAlive() const
{
	return bIsPlayerAlive;
}

void APawnTank::CalculateMoveInput(float Value) {
	DeltaLocation = FVector(Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0);// only x direction
}

void APawnTank::CalculateRotateInput(float Value) {
	float RotateAmount = Value * RotateSpeed * GetWorld()->DeltaTimeSeconds;
	FRotator Rotation = FRotator(0, RotateAmount, 0);// rotate yaw only, TODO: try rotate in Roll to allow two-wheel side driving effect, axis should be side of car
	DeltaRotation = FQuat(Rotation);
}

void APawnTank::Move() {
	AddActorLocalOffset(DeltaLocation, true);// 2rd arg is bsweep, checking collision
}

void APawnTank::Rotate() {
	AddActorLocalRotation(DeltaRotation, true);
}

void APawnTank::HandleDestruction()
{
	Super::HandleDestruction();
	// TODO: hide player
	bIsPlayerAlive = false;
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}
