// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TriggerVolume.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "openDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UopenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UopenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenDoor();

	void CloseDoor();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
private:
	UPROPERTY(VisibleAnywhere)
	float OpenAngle = 90.0f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 1.0f;

	float LastDoorOpenTime;

	//UPROPERTY(EditAnywhere)
	//AActor* ActorThatOpens = nullptr; //Remember Pawn INHERITS from Actor

	AActor* Owner = nullptr; //The owning door
	
	//Returns total mass in Kg
	float GetTotalMAssOFActorsOnPlate();
};
