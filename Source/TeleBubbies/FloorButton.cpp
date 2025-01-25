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

void AFloorButton::SpawnPortal()
{
	FActorSpawnParameters SpawnInfo;
	AActor* Portal = GetWorld()->SpawnActor<AActor>(BP_Teleport, SpawnPosition, FRotator(0.F), SpawnInfo);
	OnPressed();
}

//void AFloorButton::ToggleLasers()
//{
//	for (const TSubclassOf<AActor> &t : Targets)
//	{
//		if (t = TSubclassOf<AActor> )
//	}
//}

//void AFloorButton::ToggleFans()
//{
//
//}

void AFloorButton::Unpress()
{
	D_BUG("I'm so unpressed.", NULL);
	bPressed = false;
}

void AFloorButton::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bPressed) return;
	if (ATeleBubbiesCharacter* Player = Cast<ATeleBubbiesCharacter>(OtherActor))
	{
		if (AudioComponent)
		{
			AudioComponent->SetSound(PressButtonSound);
			AudioComponent->Play(0.f);
		}

		switch (ButtonTarget)
		{
			case ETarget::S_Portal:
				SpawnPortal();
				break;
			case ETarget::S_Lasers:
				ToggleLasers();
				bPressed = true;
				D_BUG("If a-you press-a me, I press-a yo mama!", NULL);
				GetWorld()->GetTimerManager().SetTimer(UnpressTime, this, &AFloorButton::Unpress, 1.f, false);
				break;
			case ETarget::S_Fans:
				D_BUG("It's FAN-TASTIC!!", NULL);
				ToggleFans();
				bPressed = true;
				GetWorld()->GetTimerManager().SetTimer(UnpressTime, this, &AFloorButton::Unpress, 1.f, false);
				break;
			default:
				break;
		}
	}
}

void AFloorButton::OnPressed()
{
	bPressed = true;
	this->SetActorEnableCollision(false);
	float Time = 0.f;
	int i = 0;

	while (i < 20)
	{
		FVector CurrentPosition = GetActorLocation();

		Time += GetWorld()->GetDeltaSeconds();
		if (Time >= 0.1f)
		{
			FVector NewPosition = FVector(CurrentPosition.X, CurrentPosition.Y, CurrentPosition.Z - 1.f);
			SetActorLocation(NewPosition);
			Time = 0.f;
			i++;
		}
	}
	SetActorLocation(FullyPressedPosition);
}
