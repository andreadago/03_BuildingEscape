// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

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

	UE_LOG(LogTemp, Warning, TEXT("Grabber is up!"));
	
	///Look for Physics Handle component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		//do nothing
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find UPhysicsHandleComponent in %s"), *GetOwner()->GetName());
	}

	///Look for Input Component
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
		UE_LOG(LogTemp, Warning, TEXT("FOUND InputComponent on %s"), *GetOwner()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find InputComponent in %s"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );



	/// Get the player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	//Draw a red trace
	FVector LineTraceDirection = PlayerViewPointRotation.Vector() * Reach;

	FVector LineTraceEnd = PlayerViewPointLocation + LineTraceDirection;

	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0,
		10.f
	);

	///Line-trace (AKA ray-cast)
	FCollisionQueryParams CollisionQueryParams(FName(TEXT("")), false, GetOwner());
	FHitResult Hit;

	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		CollisionQueryParams
		);

	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit: %s"), *ActorHit->GetName());
	}

		
	
}

