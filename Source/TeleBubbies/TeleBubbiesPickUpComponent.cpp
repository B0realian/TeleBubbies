// Copyright Epic Games, Inc. All Rights Reserved.

#include "TeleBubbiesPickUpComponent.h"

UTeleBubbiesPickUpComponent::UTeleBubbiesPickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UTeleBubbiesPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UTeleBubbiesPickUpComponent::OnSphereBeginOverlap);
}

void UTeleBubbiesPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	ATeleBubbiesCharacter* Character = Cast<ATeleBubbiesCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
