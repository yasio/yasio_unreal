// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "YAsioActor.generated.h"

namespace yasio {
	namespace inet {
		class io_service;
	}
}

UCLASS()
class DEMOUE4_API AYAsioActor : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* VisualMesh;

	yasio::inet::io_service* service;
public:	
	// Sets default values for this actor's properties
	AYAsioActor();
    ~AYAsioActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitYAsio();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
