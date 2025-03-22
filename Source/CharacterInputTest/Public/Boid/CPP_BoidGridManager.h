// // Fill out your copyright notice in the Description page of Project Settings.
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "GameFramework/Actor.h"
// #include "Tasks/Task.h"
// #include "CPP_BoidGridManager.generated.h"
//
// class UCPP_BoidGridCell;
// class ACPP_BoidActor;
//
// UCLASS()
// class CHARACTERINPUTTEST_API ACPP_BoidGridManager : public AActor
// {
// 	GENERATED_BODY()
//     
// public:    
// 	ACPP_BoidGridManager();
//     
// 	// Grid configuration
// 	UPROPERTY(EditAnywhere, Category = "Grid")
// 	float GridCellSize = 200.0f;
//     
// 	UPROPERTY(EditAnywhere, Category = "Grid")
// 	FVector RoomMin = FVector(-2000, -2000, -2000);
//     
// 	UPROPERTY(EditAnywhere, Category = "Grid")
// 	FVector RoomMax = FVector(2000, 2000, 2000);
//     
// 	UPROPERTY(EditAnywhere, Category = "Grid")
// 	float TimerInterval = 0.1f;
//
// 	// Public methods
// 	void RegisterBoid(ACPP_BoidActor* Boid);
// 	bool IsLocationNearObstacles(const FVector& Location, float CheckDistance);
//     
// 	// Debugging
// 	void DebugDrawGrid();
//
// protected:
// 	virtual void BeginPlay() override;
// 	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
// 	virtual void OnConstruction(const FTransform& Transform) override;
//     
// private:
//
// 	FVector GridOrigin;
// 	int32 CalculatedGridCellCount = 0;
// 	FTimerHandle RunTimerHandle;
//     
// 	// Thread-safe collection for grid cells
// 	FCriticalSection GridLock;
// 	TMap<FIntVector, UCPP_BoidGridCell*> GridCells;
// 	TMap<ACPP_BoidActor*, FIntVector> BoidToGridKeyMap;
//     
// 	void InitializeGrid(float CellSize);
// 	void InitializeObstacleGrid();
// 	void ProcessBoidMovement();
//     
// 	// Boid algorithm components
// 	FVector CalculateCohesion(TWeakObjectPtr<ACPP_BoidActor> Boid, const TArray<TWeakObjectPtr<ACPP_BoidActor>>& Neighbors);
// 	FVector CalculateAlignment(TWeakObjectPtr<ACPP_BoidActor> Boid, const TArray<TWeakObjectPtr<ACPP_BoidActor>>& Neighbors);
// 	FVector CalculateSeparation(TWeakObjectPtr<ACPP_BoidActor> Boid, const TArray<TWeakObjectPtr<ACPP_BoidActor>>& Neighbors);
// };
