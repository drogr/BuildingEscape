// Fill out your copyright notice in the Description page of Project Settings.

#include "openDoor.h"
#include "GameFramework/Actor.h"
#include "Math/Rotator.h"
#include <Components/ActorComponent.h>
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Core/Public/Containers/Array.h"
#include "Components/PrimitiveComponent.h"
#define OUT



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
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Missing PressurePlate!"), *GetOwner()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input Component NOT FOUND on %s!"), *GetOwner()->GetName());
	}

	//ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!Owner) 
	{ 	//log out the error if Not Owner
		UE_LOG(LogTemp, Error, TEXT("Owner NOT FOUND on %s!"), *GetOwner()->GetName());;
	}
	//find the owning actor
	Owner = GetOwner();
}

void UopenDoor::OpenDoor()
{
	if (!Owner) { return; }
	//set the door #rotation
	Owner->SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
}

void UopenDoor::CloseDoor()
{
	if (!Owner) { return; }
	//set the door #rotation
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

// Called every frame
void UopenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the TriggerVolume every frame
	if (GetTotalMAssOFActorsOnPlate() > 40.0f)
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

float UopenDoor::GetTotalMAssOFActorsOnPlate()
{
	float TotalMass = 0.0f;

	//find all of the overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; } 
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	//iterate through them adding their masses
	for (auto& Actor : OverlappingActors) //each Actor in the array OverlappingActors
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on Pressure Plate"), *Actor->GetName());
	}
	return TotalMass;
}

