// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "DoorOpen.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEROON2_API UDoorOpen : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoorOpen();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenDoor();
	void CloseDoor();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	
	UPROPERTY(VisibleAnyWhere)
	float OpenAngle = 90.0f;

	UPROPERTY(VisibleAnyWhere)
	float MaxPressurePadWeightToOpenDoor = 100.0f;

	UPROPERTY(EditAnyWhere)
	ATriggerVolume * PressurePlate;

	UPROPERTY(EditAnyWhere)
	float CloseDoorAfterElapsedSeconds = 1.0f;

	AActor* Owner;
	
	float LastDoorOpenTime = -1.0f;

	float GetTotalMassOfOveralppingActors() const;
	
};
