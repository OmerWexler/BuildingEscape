// All reserved to Omer Wexler. Yay.


#include "OpenDoor.h"

#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/AudioComponent.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	FindAudioComponent();
	FindTriggerActor();
	
	InitialAngle = GetOwner()->GetActorRotation().Yaw;
	OpenAngle += InitialAngle;
	
	if (!PreasurePlate) {
		UE_LOG(LogTemp, Error, TEXT("No PreasurePlate selected on %s."), *GetOwner()->GetName())
	}
}

void UOpenDoor::FindTriggerActor() {
	if (!TriggerActor) {
		TriggerActor = GetWorld()->GetFirstPlayerController()->GetPawn();
	}
}

void UOpenDoor::FindAudioComponent() {
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent) {
		UE_LOG(LogTemp, Error, TEXT("No AudioComponent found on %s"), *GetOwner()->GetName())
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PreasurePlate && TriggerActor){
		if (PreasurePlate->IsOverlappingActor(TriggerActor))
		{
			OpenDoor(DeltaTime);
			TimeWhenDoorWasLastOpened = GetWorld()->GetTimeSeconds();
		} 
		else if (GetWorld()->GetTimeSeconds() - TimeWhenDoorWasLastOpened >= CloseDelay) {
			CloseDoor(DeltaTime);
		}
	}
	
}

void UOpenDoor::OpenDoor(float DeltaTime) 
{
	float CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	GetOwner()->SetActorRotation({0.0, FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime , OpenSpeed), 0.0});
	
	if (bWasLastClosed && AudioComponent) {
		AudioComponent->Play();
		bWasLastOpen = true;
		bWasLastClosed = false;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime) 
{
	float CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	GetOwner()->SetActorRotation({0.0, FMath::FInterpTo(CurrentYaw, InitialAngle, DeltaTime , CloseSpeed), 0.0});
	
	if (bWasLastOpen && AudioComponent) {
		AudioComponent->Play();
		bWasLastClosed = true;
		bWasLastOpen = false;
	}
}