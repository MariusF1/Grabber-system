// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

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
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw; //TargetYaw = InitialYaw + InitialYaw;

	FindPresurePlate();
	OpenAudioPlay = true;
	FindAudioComp(2);
	
}

void UOpenDoor::FindAudioComp(int index)
{
	AudioComp = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (AudioComp)
	{
		
		AudioComp->SetIntParameter("SoundSwitch", index);
		AudioComp->Play();
		
	}
}

void UOpenDoor::FindPresurePlate()
{

	if (!PresurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s has the open door component on it, but no pressure plate!"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if ( TotalKgOfActors() > OKMass)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
		
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorDelay)
		{
			CloseDoor(DeltaTime);
			
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, TargetYaw, DeltaTime, OpenSpeed);

	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
	
	if(AudioComp && OpenAudioPlay)
	{
		FindAudioComp(0);
		OpenAudioPlay = false;
	}
	
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, InitialYaw, DeltaTime, CloseSpeed);

	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	if(AudioComp && !OpenAudioPlay)
	{
		FindAudioComp(1);
		OpenAudioPlay = true;
	}
}

float UOpenDoor::TotalKgOfActors() const
{
	float TotalKg = 0.f;

	//find all overlapping actos
	TArray<AActor*> OverlappActors;
	if (!PresurePlate) { return TotalKg; }
	PresurePlate->GetOverlappingActors(OverlappActors);

	//add up their mass
	for (AActor* Actor : OverlappActors)
	{
		TotalKg += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();

	}

	return TotalKg;
}