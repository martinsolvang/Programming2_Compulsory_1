// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid/CPP_BoidGridManager.h"

#include "IPropertyTable.h"
#include "Boid/CPP_BoidActor.h"

// Sets default values
ACPP_BoidGridManager::ACPP_BoidGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
}

void ACPP_BoidGridManager::InitializeGrid(float CellSize)
{
	GridCells.Empty();

	GridCellSize = CellSize;

	GridOrigin = GetActorLocation();

	FVector WorldRoomMin = GetActorLocation() + RoomMin;
	FVector WorldRoomMax = GetActorLocation() + RoomMax;

	for (float x = WorldRoomMin.X; x < WorldRoomMax.X; x += CellSize)
	{
		for (float y = WorldRoomMin.Y; y < WorldRoomMax.Y; y += CellSize)
		{
			for (float z = WorldRoomMin.Z; z < WorldRoomMax.Z; z += CellSize)
			{
				FIntVector GridKey(
					FMath::FloorToInt((x - GridOrigin.X) / CellSize),
					FMath::FloorToInt((y - GridOrigin.Y) / CellSize),
					FMath::FloorToInt((z - GridOrigin.Z) / CellSize)
				);

				UCPP_BoidGridCell* NewCell = NewObject<UCPP_BoidGridCell>(this);

				GridCells.Add(GridKey, NewCell);
			}
		}
	}
}


void ACPP_BoidGridManager::RegisterBoid(ACPP_BoidActor* Boid)
{
	if (!Boid)
	{
		return;
	}

	FVector BoidLocation = Boid->GetActorLocation();

	FIntVector GridKey(
		FMath::FloorToInt((BoidLocation.X - GridOrigin.X) / GridCellSize),
		FMath::FloorToInt((BoidLocation.Y - GridOrigin.Y) / GridCellSize),
		FMath::FloorToInt((BoidLocation.Z - GridOrigin.Z) / GridCellSize)
		);

	FIntVector* OldGridKey = BoidToGridKeyMap.Find(Boid);
	if (OldGridKey && *OldGridKey == GridKey)
	{
		return; 
	}
	
	if (OldGridKey && GridCells.Contains(*OldGridKey))
	{
		GridCells[*OldGridKey]->RemoveBoid(Boid);
	}

	if (GridCells.Contains(GridKey))
	{
		GridCells[GridKey]->AddBoid(Boid);
		BoidToGridKeyMap.Add(Boid, GridKey);
	}

	
	if (GridCells.Contains(GridKey))
	{
		UCPP_BoidGridCell* Cell = GridCells[GridKey];
		if (!Cell->BoidInCell.Contains(Boid))
		{
			Cell->AddBoid(Boid);
		}
	}
}

TArray<ACPP_BoidActor*> ACPP_BoidGridManager::GetNeighbors(ACPP_BoidActor* Boid)
{
	TArray<ACPP_BoidActor*> Neighbors;

	if (!Boid)
	{
		return Neighbors;
	}

	FVector BoidLocation = Boid->GetActorLocation();
	FIntVector GridKey(
		FMath::FloorToInt((BoidLocation.X - GridOrigin.X) / GridCellSize),
		FMath::FloorToInt((BoidLocation.Y - GridOrigin.Y) / GridCellSize),
		FMath::FloorToInt((BoidLocation.Z - GridOrigin.Z) / GridCellSize)
		);

	for (int32 x = -1; x <= 1; x++)
	{
		for (int32 y = -1; y <= 1; y++)
		{
			for (int32 z = -1; z <= 1; z++)
			{
				FIntVector NeighborKey = GridKey + FIntVector(x, y, z);
				if (GridCells.Contains(NeighborKey))
				{
					UCPP_BoidGridCell* NeighborCell = GridCells[NeighborKey];
					Neighbors.Append(NeighborCell->GetBoids());
				}
			}
		}
	}
	return Neighbors;
}

void ACPP_BoidGridManager::DebugDrawGrid()
{
	for (const auto& Cell : GridCells)
	{
		FIntVector GridKey = Cell.Key;
		FVector CellCenter = GridOrigin + FVector(
			(GridKey.X + 0.5) * GridCellSize,
			(GridKey.Y + 0.5) * GridCellSize,
			(GridKey.Z + 0.5) * GridCellSize);
		DrawDebugBox(GetWorld(), CellCenter, FVector(GridCellSize/2), FColor::Blue, false
			,5.0f);
	}
}

void ACPP_BoidGridManager::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	FlushPersistentDebugLines(GetWorld());
	InitializeGrid(GridCellSize);
	DebugDrawGrid();
}





