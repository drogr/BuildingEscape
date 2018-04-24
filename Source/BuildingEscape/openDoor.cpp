// Fill out your copyright notice in the Description page of Project Settings.

#include "openDoor.h"
#include "GameFramework/Actor.h"
#include "Math/Rotator.h"
#include <Components/ActorComponent.h>



// Sets default values for this component's properties
UopenDoor::UopenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UopenDoor::BeginPlay()
{
	Super::BeginPlay();

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	//find the owning actor
	Owner = GetOwner();
}

void UopenDoor::OpenDoor()
{
	//set the door #rotation
	Owner->SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
}

void UopenDoor::CloseDoor()
{
	//set the door #rotation
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

// Called every frame
void UopenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the TriggerVolume every frame
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	//Poll every frame to see if the doors have been open for more than time set in DoorCloseDelay. Compare current time to LastdoorOpen
	if (GetWorld()->GetTimeSeconds()-LastDoorOpenTime>DoorCloseDelay)
	{
		CloseDoor();
	}


}

