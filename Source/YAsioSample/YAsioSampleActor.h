// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "YAsioSampleActor.generated.h"

namespace yasio {
	namespace inet {
		class io_service;
	}
}

UCLASS()
class YASIOSAMPLE_API AYAsioSampleActor : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* VisualMesh;

	yasio::inet::io_service* service;
public:	
	// Sets default values for this actor's properties
	AYAsioSampleActor();
    ~AYAsioSampleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // start yasio::io_service
	void StartYAsioService();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
