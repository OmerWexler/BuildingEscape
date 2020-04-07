// All reserved to Omer Wexler. Yay.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"

#include "OpenDoor.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OpenDoor(float DeltaTime);
	virtual void CloseDoor(float DeltaTime);

private:
	float InitialAngle;
	float TimeWhenDoorWasLastOpened = 0.f;
	AActor* TriggerActor;
	
	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;

	UPROPERTY(EditAnywhere)
	float OpenSpeed = 0.5f;

	UPROPERTY(EditAnywhere)
	float CloseSpeed = 2.f;

	UPROPERTY(EditAnywhere)
	float CloseDelay = 1.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PreasurePlate;
};
