// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Components/ActorComponent.h"
#include "Public/CollisionQueryParams.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsHandleComponent.h"
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
	FindPhysicsHandelComponent();		//	UE_LOG(LogTemp, Warning, TEXT("Grabber Reporting For Duty!"));
	SetupInputComponent();
}

	//look for attached InputComponent
void UGrabber::SetupInputComponent()
{

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component found on %s!"), *GetOwner()->GetName());
		//bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input Component NOT FOUND on %s!"), *GetOwner()->GetName());
	}
}

///look for attached Physics Handle
void UGrabber::FindPhysicsHandelComponent()
{
	
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		//If Physics handle is found don't do anything
	}
	else
	{
		//log out the error
		UE_LOG(LogTemp, Error, TEXT("PhysicsHandle NOT FOUND on %s!"), *GetOwner()->GetName());
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// get the player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	//UE_LOG(LogTemp, Warning, TEXT ("Location is: %s. Rotation is %s."),*PlayerViewPointLocation.ToString(),*PlayerViewPointRotation.ToString());

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;

	//If the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		//MOVE THE OBJECT WE'RE HOLDING
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
		

	
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key pressed"));

	//LineTrace and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	//If we hit something then attach a physics handle
	if (ActorHit != nullptr)
	{
	PhysicsHandle->GrabComponentAtLocationWithRotation
		(
			ComponentToGrab, //ComponentToGrab
			NAME_None, //grab what bone name, if any
			ComponentToGrab->GetOwner()->GetActorLocation(), //grab location
			ComponentToGrab->GetOwner()->GetActorRotation() //grab rotation
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key released"));

	// Release Physics Handle
	PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// get the player view point this tick
	 FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	//UE_LOG(LogTemp, Warning, TEXT ("Location is: %s. Rotation is %s."),*PlayerViewPointLocation.ToString(),*PlayerViewPointRotation.ToString());

	////draw a red trace to visualize
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;

	//FVector(0.0f, 0.0f, 100.0f);<----This was our test vector
	//DrawDebugLine(
	//	GetWorld(),
	//	PlayerViewPointLocation,
	//	LineTraceEnd,
	//	FColor(255, 0, 0),
	//	false,
	//	0.0f,
	//	0.0f,
	//	10.0f
	//);

	//setup query parameters
	FCollisionQueryParams TraceParamaters(FName(TEXT("")), false, GetOwner());

	//Ray-cast out to reach distance. will need variable to set that distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType
	(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParamaters
	);
	//see what we hit
	//FString WhatsHit = Hit.GetActor()->GetName(); <----my first try
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is Hit!"), (*ActorHit->GetName()));
	}
	return Hit;
}

