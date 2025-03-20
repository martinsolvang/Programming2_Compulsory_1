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
	
	bool IsLocationNearObstacles(const FVector& Location, float CheckDistance);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	float GridCellSize = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	FVector RoomMin = FVector(-500.0f, -500.0f, -500.0f);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	FVector RoomMax = FVector(500.0f, 500.0f, 500.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid Settings")
	int32 CalculatedGridCellCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Grid Settings")
	float TimerInterval = 0.1f;
	
private:

	FCriticalSection GridLock;
	
	TMap<FIntVector, UCPP_BoidGridCell*> GridCells;

	TMap<FIntVector, bool> ObstacleCells;

	TMap<ACPP_BoidActor*, FIntVector> BoidToGridKeyMap;

	void InitializeObstacleGrid();

	FVector GridOrigin;

	FTimerHandle RunTimerHandle;

	FVector ComputeBoidAlgorithm(ACPP_BoidActor* Boid, const TArray<ACPP_BoidActor*>& Neighbors);

	void BatchProcessBoidAlgorithm();	

	void DebugDrawGrid();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
