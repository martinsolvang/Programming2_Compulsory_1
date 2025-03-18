// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_BoidGridCell.h"
#include "CPP_BoidGridManager.generated.h"

UCLASS()
class CHARACTERINPUTTEST_API ACPP_BoidGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_BoidGridManager();

	void InitializeGrid(float CellSize);

	void RegisterBoid(ACPP_BoidActor* Boid);

	TArray<ACPP_BoidActor*> GetNeighbors(ACPP_BoidActor* Boid);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	float GridCellSize = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	FVector RoomMin = FVector(-500.0f, -500.0f, -500.0f);
;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	FVector RoomMax = FVector(500.0f, 500.0f, 500.0f);
;
private:	
	TMap<FIntVector, CPP_BoidGridCell*> GridCells;

	TMap<ACPP_BoidActor*, FIntVector> BoidToGridKeyMap;

	FVector GridOrigin;

	void DebugDrawGrid();

	virtual void OnConstruction(const FTransform& Transform) override;
};
