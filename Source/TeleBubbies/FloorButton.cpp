#include "FloorButton.h"
#define D_BUG(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(text), fstring))
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
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
	if (bOverlappingNotHitting)
	{
		Collider->OnComponentBeginOverlap.AddDynamic(this, &AFloorButton::OnOverlap);
		Collider->OnComponentEndOverlap.AddDynamic(this, &AFloorButton::OnOverlapEnd);
	}
	else
		MainMesh->OnComponentHit.AddDynamic(this, &AFloorButton::OnHit);
	Collider->SetGenerateOverlapEvents(bOverlappingNotHitting);

	StartingPosition = FVector(GetActorLocation());
	FullyPressedPosition = StartingPosition;
	FullyPressedPosition.Z -= 10.f;
}

void AFloorButton::SpawnPortal()
{
	if (bPortalSpawned) return;
	
	FActorSpawnParameters SpawnInfo;
	AActor* Portal = GetWorld()->SpawnActor<AActor>(BP_Teleport, SpawnPosition, FRotator(0.F), SpawnInfo);
	bPortalSpawned = true;
}

void AFloorButton::FindTarget(bool bOverlapping)
{
	if (AudioComponent && !AudioComponent->IsPlaying())
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
			if (!bOverlapping)
				GetWorld()->GetTimerManager().SetTimer(UnpressTime, this, &AFloorButton::Unpress, 1.f, false);
			break;
		case ETarget::S_Fans:
			ToggleFans();
			if (!bOverlapping)
				GetWorld()->GetTimerManager().SetTimer(UnpressTime, this, &AFloorButton::Unpress, 1.f, false);
			break;
		default:
			break;
	}
}

void AFloorButton::OnOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	if (!bOverlappingNotHitting || bPressed) return;
	
	if (ATeleBubbiesCharacter* Player = Cast<ATeleBubbiesCharacter>(OtherActor))
	{
		FindTarget(bOverlappingNotHitting);
		OnPressed();
	}
}

void AFloorButton::OnOverlapEnd(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bOverlappingNotHitting || !bPressed) return;
	
	if (ATeleBubbiesCharacter* Player = Cast<ATeleBubbiesCharacter>(OtherActor))
	{
		FindTarget(bOverlappingNotHitting);
		Unpress();
	}
}

void AFloorButton::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bPressed || bOverlappingNotHitting) return;
	if (ATeleBubbiesCharacter* Player = Cast<ATeleBubbiesCharacter>(OtherActor))
	{
		FindTarget(bOverlappingNotHitting);
		OnPressed();
	}
}

void AFloorButton::OnPressed()
{
	bPressed = true;
	//SetActorLocation(FullyPressedPosition);
}

void AFloorButton::Unpress()
{
	bPressed = false;
	//SetActorLocation(StartingPosition);
}

