#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorButton.generated.h"

UCLASS()
class TELEBUBBIES_API AFloorButton : public AActor
{
	GENERATED_BODY()
	
public:	
	AFloorButton();
	UFUNCTION() void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, Category = Body)	class UCapsuleComponent* Collider;
	UPROPERTY(EditAnywhere, Category = Body)	class UStaticMeshComponent* MainMesh;
	UPROPERTY(EditAnywhere, Category = Sound)	class USoundCue* PressButtonSound;
												class UAudioComponent* AudioComponent;
												
	UPROPERTY(EditAnywhere, Category = Pressed) TSubclassOf<AActor> BP_Teleport;
	UPROPERTY(EditAnywhere, Category = Pressed) FVector SpawnPosition;

protected:
	virtual void BeginPlay() override;
	void OnPressed(FVector ActorLocation, int& i);

	FVector FullyPressedPosition = FVector(0.F);
	bool bPressed = false;

};
