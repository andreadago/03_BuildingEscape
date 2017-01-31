// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:

	//How far ahead of the player can we reach cm
	float Reach = 100.f;
	
	//Physics Handle
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	//Input component
	UInputComponent* InputComponent = nullptr;
	
	//Delegate for grab action; ray-cast and grab what's in reach
	void Grab();

	//Delegate for release grab action
	void Release();

	//Find Physics Handler component
	void FindPhysicsHandleComponent();

	//Setup (assumed) attached input component
	void SetupInputComponent();

	//Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();
};
