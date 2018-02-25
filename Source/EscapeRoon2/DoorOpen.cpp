// Fill out your copyright notice in the Description page of Project Settings.

#include "DoorOpen.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

#define OUT

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

	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s missing Pressure Plate Trigger Volume"), *Owner->GetName());
	}

}

// Called every frame
void UDoorOpen::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PressurePlate) {
		return;
	}
	
	if (GetTotalMassOfOveralppingActors() > MaxPressurePadWeightToOpenDoor) {
		OnOpenRequest.Broadcast();
	}
	else {
		OnCloseRequest.Broadcast();
	}
}

float UDoorOpen::GetTotalMassOfOveralppingActors() const
{	
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	float TotalMass = 0.0f;
	for (const auto* Actor : OverlappingActors) {
	
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

