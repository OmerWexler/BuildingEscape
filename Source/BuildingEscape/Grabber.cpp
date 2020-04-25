// All reserved to Omer Wexler. Yay.

#pragma once 

#include "Grabber.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Engine/EngineTypes.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#define OUT 

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{	
	Super::BeginPlay();
	
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	InputComponent = GetOwner()->InputComponent;

	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandle equipped on %s!"), *GetOwner()->GetName())
	}

	if (!InputComponent) {
		UE_LOG(LogTemp, Error, TEXT("No InputComponent equipped on %s!"), *GetOwner()->GetName())
	} else {
		InputComponent->BindAction(FName{TEXT("ToggleGrab")}, EInputEvent::IE_Pressed, this, &UGrabber::Grab);
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// UpdateReach();
	// DrawDebugLine(GetWorld(), CurrentStartOfReach, CurrentEndOfReach, FColor{0, 255, 0}, false, 0.f, 0, 10);
	if (!PhysicsHandle) { return; }

	if (bIsSomethingGrabbed) {
		UpdateReach();
		PhysicsHandle->SetTargetLocation(CurrentEndOfReach);
		PhysicsHandle->SetTargetRotation(GetOwner()->GetTransform().Rotator());
	}
}

void UGrabber::UpdateReach() {
	FRotator ViewpointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT CurrentStartOfReach, 
		OUT ViewpointRotation
	);

	CurrentEndOfReach = CurrentStartOfReach + (ViewpointRotation.Vector() * Reach);
} 

void UGrabber::Grab() 
{
	if (!PhysicsHandle) { return; }
	UpdateReach();
	
	FHitResult ObjectHit;
	bool Hit = GetWorld()->LineTraceSingleByObjectType(
		OUT ObjectHit,
		CurrentStartOfReach,
		CurrentEndOfReach,
		FCollisionObjectQueryParams{ECC_PhysicsBody},
		FCollisionQueryParams{    
			GetOwner()->GetFName(),
			false,
			GetOwner()
		}
	);

	if (PhysicsHandle->GetGrabbedComponent()) { 
		PhysicsHandle->ReleaseComponent();
		bIsSomethingGrabbed = false;

	} else if (Hit) {
		PhysicsHandle->GrabComponentAtLocation(ObjectHit.GetComponent(), ObjectHit.MyBoneName, ObjectHit.TraceEnd);
		bIsSomethingGrabbed = true;
	}
}