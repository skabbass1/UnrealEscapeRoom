// Fill out your copyright notice in the Description page of Project Settings.

#include "DoorOpen.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UDoorOpen::UDoorOpen()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true; 

	// ...
}


// Called when the game starts
void UDoorOpen::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

}

void UDoorOpen::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.f, -OpenAngle, 0.f));
	LastDoorOpenTime = GetWorld()->GetTimeSeconds();
}

void UDoorOpen::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.f, 0.0f, 0.f));
	LastDoorOpenTime = -1.0;
}


// Called every frame
void UDoorOpen::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate->IsOverlappingActor(ActorThatOpens)) {
		OpenDoor();
	}
	else {
		// Ensure door is open
		if (LastDoorOpenTime > 0.0) {
			if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime >= CloseDoorAfterElapsedSeconds) {
				CloseDoor();
			}
		}
		
	}

	
}

