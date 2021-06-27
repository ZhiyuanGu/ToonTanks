// Zhiyuan Gu 2020 All Rights Reserved

#include "ProjectileBase.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "ToonTanks/Pawns/PawnTank.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = MovementSpeed;
	ProjectileMovement->MaxSpeed = MovementSpeed;
	ProjectileMovement->SetUpdatedComponent(RootComponent);
	InitialLifeSpan = 3.0f;

	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Trail"));
	ParticleTrail->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());

}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Warning, TEXT("[AProjectileBase::OnHit]Projectile on hit!"));
	AActor* MyOwner = GetOwner();
	if (!MyOwner) {
		UE_LOG(LogTemp, Warning, TEXT("[AProjectileBase::OnHit]Owner not found in method!"));
		return;
	}
	// Avoid hitting myself
	if (OtherActor && OtherActor != this && OtherActor != MyOwner) {
		//UE_LOG(LogTemp, Warning, TEXT("[AProjectileBase::OnHit]Applying damage!"));
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, DamageType);
		// Play explotion effects 
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, GetActorLocation());
		// Only apply damage to pawn tank here
		if (OtherActor->IsA(APawnTank::StaticClass())) {
			GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(HitShake);
		}
		Destroy();
	}
}


