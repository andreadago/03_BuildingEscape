// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	
	Owner = GetOwner();	
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the Trigger Volume every frame
	//If the ActorThatOpens is in the volume
	if (GetTotalMassOfActorsInPlate() > 30.f) //TODO: make a threshold variable
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	//Check if it's time to close the door
	if ((GetWorld()->GetTimeSeconds() - LastDoorOpenTime) > DoorCloseDelay)
	{
		CloseDoor();
	}
}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}

float UOpenDoor::GetTotalMassOfActorsInPlate()
{
	float TotalMass = 0.f;
	
	TArray<AActor*> OverlappingActors;

	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (const auto* Actor : OverlappingActors)
	{
		UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		
		//Other way to get UPrimitiveComponent
		PrimitiveComponent = Actor->FindComponentByClass<UPrimitiveComponent>();


		UE_LOG(LogTemp, Warning, TEXT("Overlapping actor: %s"), *Actor->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Mass of overlapping actor: %f"), PrimitiveComponent->GetMass());
		TotalMass += PrimitiveComponent->GetMass();
	}

	return TotalMass;
}