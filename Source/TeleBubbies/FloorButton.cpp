#include "FloorButton.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"


AFloorButton::AFloorButton()
{
	PrimaryActorTick.bCanEverTick = true;
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(MainMesh);
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComponent->SetupAttachment(RootComponent);
}

void AFloorButton::BeginPlay()
{
	Super::BeginPlay();
	
	FullyPressedPosition = FVector(GetActorLocation());
	FullyPressedPosition.Z -= 10.f;
}

void AFloorButton::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bPressed) return;

	if (AudioComponent)
	{
		AudioComponent->SetSound(PressButtonSound);
		AudioComponent->Play(0.f);
	}

	FActorSpawnParameters SpawnInfo;
	AActor* Portal = GetWorld()->SpawnActor<AActor>(BP_Teleport, SpawnPosition, FRotator(0.F), SpawnInfo);

	bool FullyPressed = false;
	while (!FullyPressed)
	{
		FVector CurrentPosition = GetActorLocation();
		static float Time;
		Time += GetWorld()->GetDeltaSeconds();
		if (Time >= 0.05f)
		{
			OnPressed(CurrentPosition);
			Time = 0.f;
		}
		
		if (FMath::Abs(CurrentPosition.Z - FullyPressedPosition.Z) < 1.f)
			FullyPressed = true;
	}
}

void AFloorButton::OnPressed(FVector ActorLocation)
{
	FVector NewPosition = FVector(ActorLocation.X, ActorLocation.Y, ActorLocation.Z - 1.f);
	AddActorLocalOffset(NewPosition);
}
