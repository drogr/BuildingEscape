// Fill out your copyright notice in the Description page of Project Settings.

#include "openDoor.h"
#include "GameFramework/Actor.h"
#include "Math/Rotator.h"


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

	//find the owning actor
	AActor* Owner = GetOwner();

	//Create #rotator
	auto NewRotation = FRotator(0.0f,90.0f,0.0f);

	//set the door #rotation
	
	Owner->SetActorRotation(NewRotation);
	
}


// Called every frame
void UopenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

