#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorButton.generated.h"

UENUM(BlueprintType)
enum class ETarget : uint8
{
	S_None		UMETA(DisplayName = "None"),
	S_Portal	UMETA(DisplayName = "Portal"),
	S_Lasers	UMETA(DisplayName = "Lasers"),
	S_Fans		UMETA(DisplayName = "Fans"),
};

UCLASS()
class TELEBUBBIES_API AFloorButton : public AActor
{
	GENERATED_BODY()
	
public:	
	AFloorButton();
	UFUNCTION() void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION(BlueprintImplementableEvent) void ToggleLasers();
	UFUNCTION(BlueprintImplementableEvent) void ToggleFans();

	UPROPERTY(EditAnywhere, Category = Body)	class UStaticMeshComponent* MainMesh;
	UPROPERTY(EditAnywhere, Category = Sound)	class USoundCue* PressButtonSound;
												class UAudioComponent* AudioComponent;
												
	UPROPERTY(EditAnywhere, Category = Target)	ETarget ButtonTarget;
	UPROPERTY(EditAnywhere, Category = Pressed) TSubclassOf<AActor> BP_Teleport;
	UPROPERTY(EditAnywhere, Category = Pressed) FVector SpawnPosition;

protected:
	virtual void BeginPlay() override;
	void SpawnPortal();
	void Unpress();
	void OnPressed();

	FTimerHandle UnpressTime;
	FVector FullyPressedPosition = FVector(0.F);
	bool bPressed = false;

};
