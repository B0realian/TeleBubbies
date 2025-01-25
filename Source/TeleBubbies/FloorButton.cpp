#include "FloorButton.h"
#define D_BUG(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(text), fstring))
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "TeleBubbiesCharacter.h"


AFloorButton::AFloorButton()
{
	PrimaryActorTick.bCanEverTick = true;
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(MainMesh);
	Collider = CreateDefaultSubobject<UCapsuleComponent>("Collider");
	Collider->SetupAttachment(RootComponent);
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComponent->SetupAttachment(RootComponent);
}

void AFloorButton::BeginPlay()
{
	Super::BeginPlay();
	MainMesh->OnComponentHit.AddDynamic(this, &AFloorButton::OnHit);
	this->SetActorEnableCollision(true);
	FullyPressedPosition = FVector(GetActorLocation());
	FullyPressedPosition.Z -= 10.f;
}

void AFloorButton::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bPressed) return;
	if (ATeleBubbiesCharacter* Player = Cast<ATeleBubbiesCharacter>(OtherActor))
	{
		bPressed = true;
		this->SetActorEnableCollision(false);

		if (AudioComponent)
		{
			AudioComponent->SetSound(PressButtonSound);
			AudioComponent->Play(0.f);
		}

		FActorSpawnParameters SpawnInfo;
		AActor* Portal = GetWorld()->SpawnActor<AActor>(BP_Teleport, SpawnPosition, FRotator(0.F), SpawnInfo);

		float Time = 0.f;
		int i = 0;

		while (i < 20)
		{
			FVector CurrentPosition = GetActorLocation();

			Time += GetWorld()->GetDeltaSeconds();
			if (Time >= 0.1f)
			{
				OnPressed(CurrentPosition, i);
				Time = 0.f;
			}
		}
	}
}

void AFloorButton::OnPressed(FVector ActorLocation, int& i)
{
	if (i < 19)
	{
		FVector NewPosition = FVector(ActorLocation.X, ActorLocation.Y, ActorLocation.Z - 1.f);
		SetActorLocation(NewPosition);
	}
	else
		SetActorLocation(FullyPressedPosition);

	i++;
}
