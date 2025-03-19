// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_BoidDataAsset.h"
#include "GameFramework/Actor.h"
#include "CPP_BoidActor.generated.h"

UCLASS()
class CHARACTERINPUTTEST_API ACPP_BoidActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_BoidActor();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boid")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boid")
	UCPP_BoidDataAsset* BoidData;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boid")
	float CohesionFactor = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boid")
	float SeparationFactor = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boid")
	float AlignmentFactor = 1;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Boid")
	FVector CurrentVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boid")
	float GetSeparationDistance = 50;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Boid")
	float AvoidanceFactor = 1;

	float MovementSpeed = 150.0f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void ObstacleAvoidance(float DeltaTime);

	TSet<AActor*> OverlappingActors;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
