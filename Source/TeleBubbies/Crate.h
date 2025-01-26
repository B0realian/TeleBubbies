#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Crate.generated.h"

UCLASS()
class TELEBUBBIES_API ACrate : public AActor
{
	GENERATED_BODY()
	
public:	
	ACrate();

	UPROPERTY(EditAnywhere, Category = Body)	class UStaticMeshComponent* MainMesh;
	UPROPERTY(EditAnywhere, Category = Sound)	class USoundCue* CollisionSound;
												class UAudioComponent* AudioComponent;

	UFUNCTION() void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	virtual void BeginPlay() override;

};
