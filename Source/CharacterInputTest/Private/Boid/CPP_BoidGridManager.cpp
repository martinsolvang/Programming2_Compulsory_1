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

	//Gets the total amount of cells in the grid and the reserves it in memory
	FVector RoomSize = RoomMax - RoomMin;

	int32 NumCellsX = FMath::CeilToInt(RoomSize.X / CellSize);
	int32 NumCellsY = FMath::CeilToInt(RoomSize.Y / CellSize);
	int32 NumCellsZ = FMath::CeilToInt(RoomSize.Z / CellSize);

	int32 TotalCells = NumCellsX * NumCellsY * NumCellsZ;

	GridCells.Reserve(TotalCells);
	CalculatedGridCellCount = TotalCells;

	
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

	// Calculate Grid Key based on Boid's current location
	FVector BoidLocation = Boid->GetActorLocation();
	FIntVector GridKey(
		FMath::FloorToInt((BoidLocation.X - GridOrigin.X) / GridCellSize),
		FMath::FloorToInt((BoidLocation.Y - GridOrigin.Y) / GridCellSize),
		FMath::FloorToInt((BoidLocation.Z - GridOrigin.Z) / GridCellSize)
	);

	// Check previous grid key
	FIntVector* OldGridKey = BoidToGridKeyMap.Find(Boid);

	// If there is an old key, remove the boid from its previous cell
	if (OldGridKey && GridCells.Contains(*OldGridKey))
	{
		GridCells[*OldGridKey]->RemoveBoid(Boid);
	}

	// Add the boid to the new cell and update mapping
	if (GridCells.Contains(GridKey))
	{
		GridCells[GridKey]->AddBoid(Boid);
		BoidToGridKeyMap.Add(Boid, GridKey);
	}
}


FVector ACPP_BoidGridManager::ComputeBoidAlgorithm(ACPP_BoidActor* Boid, const TArray<ACPP_BoidActor*>& Neighbors)
{
	FVector Cohesion = FVector::ZeroVector;
	FVector Separation = FVector::ZeroVector;
	FVector Alignment = FVector::ZeroVector;
	int32 NeighborCount = 0;

	for (ACPP_BoidActor* Neighbor : Neighbors)
	{
		if (Neighbor == Boid) continue;

		FVector NeighborDistance = Neighbor->GetActorLocation() - Boid->GetActorLocation();
		float Distance = NeighborDistance.Size();

		Cohesion += Neighbor->GetActorLocation();
		Alignment += Neighbor->CurrentVector;

		if (Distance < Boid->GetSeparationDistance)
		{
			Separation -= NeighborDistance.GetSafeNormal()/FMath::Max(Distance, 0.01f);
		}

		NeighborCount++;
	}

	if (NeighborCount > 0)
	{
		Cohesion = (Cohesion / NeighborCount- Boid->GetActorLocation()).GetClampedToSize(0.0f, 1.0f);
		Alignment = (Alignment / NeighborCount).GetClampedToSize(0.0f, 1.0f);
		Separation = Separation.GetClampedToSize(0.0f, 1.0f);
	}

	return (Cohesion * Boid->CohesionFactor) + (Separation * Boid->SeparationFactor) + (Alignment * Boid->AlignmentFactor);
}

void ACPP_BoidGridManager::BatchProcessBoidAlgorithm()
{
	for (auto& Cell : GridCells)
	{
		 FIntVector CellKey = Cell.Key;
		UCPP_BoidGridCell* CurrentCell = Cell.Value;

		UE::Tasks::Launch(TEXT("GridCellBoidAlgorithmTask"),[this, CellKey, CurrentCell]()
		{
			TArray<ACPP_BoidActor*> Neighbors;
			for (int32 x = -1; x <= 1; x++)
			{
				for (int32 y = -1; y <= 1; y++)
				{
					for (int32 z = -1; z <= 1; z++)
					{
						FIntVector NeighborKey = CellKey + FIntVector(x, y, z);
						if (GridCells.Contains(NeighborKey))
						{
							UCPP_BoidGridCell* NeighborCell = GridCells[NeighborKey];
							Neighbors.Append(NeighborCell->GetBoids());
						} 
					}
				}
			}

			for (ACPP_BoidActor* Boid : CurrentCell->GetBoids())
			{
				FVector NewVector = ComputeBoidAlgorithm(Boid, Neighbors);
				Boid->CurrentVector = NewVector;
				RegisterBoid(Boid); 
			}
		});
	}
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

void ACPP_BoidGridManager::BeginPlay()
{
	Super::BeginPlay();

	InitializeGrid(GridCellSize);
	
	for (auto& Cell : GridCells)
	{
		for (ACPP_BoidActor* Boid : Cell.Value->GetBoids())
		{
			RegisterBoid(Boid);
		}
	}

	
	GetWorld()->GetTimerManager().SetTimer(
		RunTimerHandle, 
		this, 
		&ACPP_BoidGridManager::BatchProcessBoidAlgorithm,
		TimerInterval,                               
		true                                        
	);

}

void ACPP_BoidGridManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(RunTimerHandle);

}





