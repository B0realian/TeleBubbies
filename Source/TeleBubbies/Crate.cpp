#include "Crate.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "TeleBubbiesCharacter.h"


ACrate::ACrate()
{
	PrimaryActorTick.bCanEverTick = true;
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(MainMesh);
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("Audio");
	AudioComponent->SetupAttachment(RootComponent);
}

void ACrate::BeginPlay()
{
	Super::BeginPlay();
	MainMesh->OnComponentHit.AddDynamic(this, &ACrate::OnHit);
	AudioComponent->SetSound(CollisionSound);
}

void ACrate::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (AudioComponent)
		AudioComponent->Play(0.f);

	if (ATeleBubbiesCharacter* Player = Cast<ATeleBubbiesCharacter>(OtherActor))
		AddActorLocalOffset(OtherActor->GetActorForwardVector() * 5.f);
}

