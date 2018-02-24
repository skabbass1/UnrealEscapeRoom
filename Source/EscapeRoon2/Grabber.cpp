// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

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

	FindPhysicsHandleComponent();
	BindGrabAndreleaseActionsToInputComponent();

}
void UGrabber::FindPhysicsHandleComponent()
{
	/// Look for attached Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("Physics Handle Component Not attached to: %s"), *GetOwner()->GetName());
	}
}

// Look for attached input handle. Only appears at runtime
void UGrabber::BindGrabAndreleaseActionsToInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent) {

		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Input Component not found on: %s"), *GetOwner()->GetName());
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FPlayerViewPoint ViewPoint = GetPlayerViewPoint();
	FVector MoveToThisLocation = GetFirstPlayerLineTraceEnd();

	// if physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		
		//move the object we are holding
		PhysicsHandle->SetTargetLocation(MoveToThisLocation);
	}
}

void UGrabber::Grab()
{
	///LINE TRACE and see if we reach any actors with Physics Body Collison Channel Set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	/// If we hit something , then attach a physics handle
	if (ActorHit) {
		// attach physics handle
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None, // No bone to grab on since not dealing with skeletons
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
	}
	
}

void UGrabber::Release()
{
	// Drop the held object
	PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	
	FPlayerViewPoint ViewPoint = GetPlayerViewPoint();
	FVector LineTraceEnd = GetFirstPlayerLineTraceEnd();

	///Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Line-Trace (AKA Ray Cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		ViewPoint.PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody)
	);

	return Hit;
}

FVector UGrabber::GetFirstPlayerLineTraceEnd() const
{
	FPlayerViewPoint ViewPoint = GetPlayerViewPoint();
	return ViewPoint.PlayerViewPointLocation + (ViewPoint.PlayerViewPointRotation.Vector()*Reach);
}

UGrabber::FPlayerViewPoint UGrabber::GetPlayerViewPoint() const
{
	FVector PlayerViewPointLocation; // Location of player relative to origin
	FRotator  PlayerViewPointRotation; // Players line of sigth
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return FPlayerViewPoint{ PlayerViewPointLocation , PlayerViewPointRotation };
}

