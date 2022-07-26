// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalKgOfActors() const;
	void FindAudioComp(int index);
	void FindPresurePlate();

	bool OpenAudioPlay;
	
		
private:
	
	float InitialYaw;
	float CurrentYaw;

	UPROPERTY(EditAnywhere)
		float TargetYaw = -90.f;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* PresurePlate = nullptr;

	UPROPERTY(EditAnywhere)
		AActor* ActorThatOpen;

	float DoorLastOpened = 0.f;

	UPROPERTY(EditAnywhere)
		float DoorDelay = 0.09;

	UPROPERTY(EditAnywhere)
		float OpenSpeed= 45;
	UPROPERTY(EditAnywhere)
		float CloseSpeed = 155;
	UPROPERTY(EditAnywhere)
		float OKMass = 50.f;
	UPROPERTY()
		UAudioComponent* AudioComp = nullptr;
};
