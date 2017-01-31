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
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	FindPhysicsHandleComponent();

	SetupInputComponent();
}

///Look for Physics Handle component
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		//do nothing
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find UPhysicsHandleComponent in %s"), *GetOwner()->GetName());
	}

}

///Look for Input Component
void UGrabber::SetupInputComponent()
{
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


	//LINE TRACE and see if we reach any actors with physics body collision channel set
	FHitResult Hit = GetFirstPhysicsBodyInReach();

	if (Hit.GetActor())
	{
		AActor* HitActor = Hit.GetActor();

		UE_LOG(LogTemp, Warning, TEXT("GetActor with name %s"), *Hit.GetActor()->GetName());

		//Attach Physics Handle
		PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

		if (PhysicsHandle)
		{


			//Cast to primitive component
			UPrimitiveComponent* ComponentToGrab = Hit.GetComponent();

			if (ComponentToGrab != NULL)
			{
				UE_LOG(LogTemp, Warning, TEXT("ComponentToGrab not null"));

				//Grab the actor
				PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), ComponentToGrab->GetOwner()->GetActorRotation());
			}
		}
	}

	//TODO attach physics handle
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (PhysicsHandle->GrabbedComponent)
	{
		/// Get the player view point
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;

		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

		//Draw a red trace
		FVector LineTraceDirection = PlayerViewPointRotation.Vector() * Reach;

		FVector LineTraceEnd = PlayerViewPointLocation + LineTraceDirection;

		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}

}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Get the player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	//Draw a red trace
	FVector LineTraceDirection = PlayerViewPointRotation.Vector() * Reach;

	FVector LineTraceEnd = PlayerViewPointLocation + LineTraceDirection;

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

	return Hit;
}