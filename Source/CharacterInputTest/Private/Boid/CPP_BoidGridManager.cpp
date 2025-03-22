// #include "Boid/CPP_BoidGridManager.h"
// #include "Boid/CPP_BoidActor.h"
// #include "DrawDebugHelpers.h"
// #include "Boid/CPP_BoidGridCell.h"
// #include "Kismet/GameplayStatics.h"
//
//
// ACPP_BoidGridManager::ACPP_BoidGridManager()
// {
//     PrimaryActorTick.bCanEverTick = false;
// }
//
// void ACPP_BoidGridManager::InitializeGrid(float CellSize)
// {
//     GridCells.Empty();  // Clear grid from old simulation
//     
//     GridCellSize = CellSize;
//     GridOrigin = GetActorLocation();
//
//     // Calculate grid dimensions
//     FVector RoomSize = RoomMax - RoomMin;
//     int32 NumCellsX = FMath::CeilToInt(RoomSize.X / CellSize);
//     int32 NumCellsY = FMath::CeilToInt(RoomSize.Y / CellSize);
//     int32 NumCellsZ = FMath::CeilToInt(RoomSize.Z / CellSize);
//     int32 TotalCells = NumCellsX * NumCellsY * NumCellsZ;
//     
//     GridCells.Reserve(TotalCells);
//     CalculatedGridCellCount = GridCells.Num();
//     
//     // Create grid cells
//     FVector WorldRoomMin = GetActorLocation() + RoomMin;
//     FVector WorldRoomMax = GetActorLocation() + RoomMax;
//     
//     for (float x = WorldRoomMin.X; x < WorldRoomMax.X; x += CellSize)
//     {
//         for (float y = WorldRoomMin.Y; y < WorldRoomMax.Y; y += CellSize)
//         {
//             for (float z = WorldRoomMin.Z; z < WorldRoomMax.Z; z += CellSize)
//             {
//                 FIntVector GridKey(
//                     FMath::FloorToInt((x - GridOrigin.X) / CellSize),
//                     FMath::FloorToInt((y - GridOrigin.Y) / CellSize),
//                     FMath::FloorToInt((z - GridOrigin.Z) / CellSize)
//                 );
//                 
//                 UCPP_BoidGridCell* NewCell = NewObject<UCPP_BoidGridCell>(this);
//                 GridCells.Add(GridKey, NewCell);
//             }
//         }
//     }
// }
//
// void ACPP_BoidGridManager::RegisterBoid(ACPP_BoidActor* Boid)
// {
//     // Validate boid pointer before proceeding
//     if (!Boid || !IsValid(Boid))
//     {
//         UE_LOG(LogTemp, Warning, TEXT("Invalid Boid passed to RegisterBoid."));
//         return;
//     }
//
//     // Get location and calculate grid key
//     FVector BoidLocation = Boid->GetActorLocation();
//     FIntVector GridKey(
//         FMath::FloorToInt((BoidLocation.X - GridOrigin.X) / GridCellSize),
//         FMath::FloorToInt((BoidLocation.Y - GridOrigin.Y) / GridCellSize),
//         FMath::FloorToInt((BoidLocation.Z - GridOrigin.Z) / GridCellSize)
//     );
//
//     // Lock access to the grid
//     {
//         FScopeLock Lock(&GridLock);
//
//         // Check if this boid was already registered in a cell
//         FIntVector* OldGridKey = BoidToGridKeyMap.Find(Boid);
//         if (OldGridKey && GridCells.Contains(*OldGridKey))
//         {
//             // Skip if the boid is still in the same grid cell
//             if (*OldGridKey == GridKey)
//             {
//                 return;
//             }
//
//             // Otherwise, safely remove the boid from the old cell
//             UCPP_BoidGridCell* OldCell = GridCells[*OldGridKey];
//             if (OldCell && IsValid(OldCell))
//             {
//                 OldCell->RemoveBoid(Boid);
//             }
//         }
//
//         // Check if the new grid key is valid
//         if (!GridCells.Contains(GridKey))
//         {
//             UE_LOG(LogTemp, Warning, TEXT("Grid key out of bounds in RegisterBoid."));
//             return;
//         }
//
//         // Safely add the boid to the new cell
//         UCPP_BoidGridCell* NewCell = GridCells[GridKey];
//         if (NewCell && IsValid(NewCell))
//         {
//             NewCell->AddBoid(Boid);
//             BoidToGridKeyMap.Add(Boid, GridKey);
//         }
//     }
// }
//
//
// bool ACPP_BoidGridManager::IsLocationNearObstacles(const FVector& Location, float CheckDistance)
// {
//     // Convert location to grid key
//     FIntVector GridKey(
//         FMath::FloorToInt((Location.X - GridOrigin.X) / GridCellSize),
//         FMath::FloorToInt((Location.Y - GridOrigin.Y) / GridCellSize),
//         FMath::FloorToInt((Location.Z - GridOrigin.Z) / GridCellSize)
//     );
//     
//     // Check surrounding cells
//     int32 CellRadius = FMath::CeilToInt(CheckDistance / GridCellSize);
//     
//     for (int32 x = -CellRadius; x <= CellRadius; x++)
//     {
//         for (int32 y = -CellRadius; y <= CellRadius; y++)
//         {
//             for (int32 z = -CellRadius; z <= CellRadius; z++)
//             {
//                 FIntVector CheckKey = GridKey + FIntVector(x, y, z);
//                 
//                 UCPP_BoidGridCell** Cell = GridCells.Find(CheckKey);
//                 if (Cell && *Cell && (*Cell)->bIsObstructed)
//                 {
//                     return true;
//                 }
//             }
//         }
//     }
//     
//     return false;
// }
//
// void ACPP_BoidGridManager::InitializeObstacleGrid()
// {
//     for (auto& Cell : GridCells)
//     {
//         FIntVector GridKey = Cell.Key;
//         UCPP_BoidGridCell* CurrentCell = Cell.Value;
//         
//         // Calculate cell center
//         FVector CellCenter = GridOrigin + FVector(
//             (GridKey.X + 0.5) * GridCellSize,
//             (GridKey.Y + 0.5) * GridCellSize,
//             (GridKey.Z + 0.5) * GridCellSize);
//         
//         // Default to not obstructed
//         CurrentCell->bIsObstructed = false;
//         
//         // Simple raycast checks in 6 main directions
//         TArray<FVector> Directions = {
//             FVector(1, 0, 0), FVector(-1, 0, 0),
//             FVector(0, 1, 0), FVector(0, -1, 0),
//             FVector(0, 0, 1), FVector(0, 0, -1)
//         };
//         
//         float RayLength = GridCellSize * 0.6f;
//         
//         for (const FVector& Dir : Directions)
//         {
//             FHitResult Hit;
//             bool bHit = GetWorld()->LineTraceSingleByChannel(
//                 Hit,
//                 CellCenter,
//                 CellCenter + Dir * RayLength,
//                 ECC_WorldStatic
//             );
//             
//             if (bHit)
//             {
//                 CurrentCell->bIsObstructed = true;
//                 break;
//             }
//         }
//         
//         // Simple debug visualization
//         if (CurrentCell->bIsObstructed)
//         {
//             DrawDebugBox(GetWorld(), CellCenter, FVector(GridCellSize/2), FColor::Red, false, 5.0f);
//         }
//     }
// }
//
// FVector ACPP_BoidGridManager::CalculateCohesion(TWeakObjectPtr<ACPP_BoidActor> Boid, const TArray<TWeakObjectPtr<ACPP_BoidActor>>& Neighbors)
// {
//     // First validate the Boid pointer and return early if invalid
//     if (!Boid.IsValid())
//         return FVector::ZeroVector;
//
//     ACPP_BoidActor* BoidPtr = Boid.Get();
//     if (!BoidPtr || !IsValid(BoidPtr))
//         return FVector::ZeroVector;
//
//     FVector CenterOfMass = FVector::ZeroVector;
//     int32 ValidNeighbors = 0;
//     
//     for (const TWeakObjectPtr<ACPP_BoidActor>& Neighbor : Neighbors)
//     {
//         // Never use the raw Get() call without checking IsValid() first
//         if (!Neighbor.IsValid())
//             continue;
//             
//         if (Neighbor == Boid)
//             continue;
//
//         // Get the pointer after validation and check it again
//         ACPP_BoidActor* NeighborPtr = Neighbor.Get();
//         if (!NeighborPtr || !IsValid(NeighborPtr))
//             continue;
//
//         CenterOfMass += NeighborPtr->GetActorLocation();
//         ValidNeighbors++;
//     }
//     
//     if (ValidNeighbors > 0)
//     {
//         CenterOfMass /= ValidNeighbors;
//         return (CenterOfMass - BoidPtr->GetActorLocation()).GetSafeNormal();
//     }
//
//     return FVector::ZeroVector;
// }
//
// FVector ACPP_BoidGridManager::CalculateAlignment(TWeakObjectPtr<ACPP_BoidActor> Boid, const TArray<TWeakObjectPtr<ACPP_BoidActor>>& Neighbors)
// {
//     // First validate the Boid pointer and return early if invalid
//     if (!Boid.IsValid())
//         return FVector::ZeroVector;
//
//     ACPP_BoidActor* BoidPtr = Boid.Get();
//     if (!BoidPtr || !IsValid(BoidPtr))
//         return FVector::ZeroVector;
//
//     FVector AverageHeading = FVector::ZeroVector;
//     int32 ValidNeighbors = 0;
//     
//     for (const TWeakObjectPtr<ACPP_BoidActor>& Neighbor : Neighbors)
//     {
//         // Proper validation chain
//         if (!Neighbor.IsValid())
//             continue;
//             
//         if (Neighbor == Boid)
//             continue;
//             
//         ACPP_BoidActor* NeighborPtr = Neighbor.Get();
//         if (!NeighborPtr || !IsValid(NeighborPtr))
//             continue;
//             
//         AverageHeading += NeighborPtr->CurrentVector;
//         ValidNeighbors++;
//     }
//     
//     if (ValidNeighbors > 0)
//     {
//         return (AverageHeading / ValidNeighbors).GetSafeNormal();
//     }
//     
//     return FVector::ZeroVector;
// }
//
// FVector ACPP_BoidGridManager::CalculateSeparation(TWeakObjectPtr<ACPP_BoidActor> Boid, const TArray<TWeakObjectPtr<ACPP_BoidActor>>& Neighbors)
// {
//     // First validate the Boid pointer and return early if invalid
//     if (!Boid.IsValid())
//         return FVector::ZeroVector;
//
//     ACPP_BoidActor* BoidPtr = Boid.Get();
//     if (!BoidPtr || !IsValid(BoidPtr))
//         return FVector::ZeroVector;
//
//     FVector Separation = FVector::ZeroVector;
//     
//     for (const TWeakObjectPtr<ACPP_BoidActor>& Neighbor : Neighbors)
//     {
//         // Proper validation chain
//         if (!Neighbor.IsValid())
//             continue;
//             
//         if (Neighbor == Boid)
//             continue;
//             
//         ACPP_BoidActor* NeighborPtr = Neighbor.Get();
//         if (!NeighborPtr || !IsValid(NeighborPtr))
//             continue;
//             
//         FVector Direction = BoidPtr->GetActorLocation() - NeighborPtr->GetActorLocation();
//         float Distance = Direction.Size();
//         
//         if (Distance < BoidPtr->SeparationDistance && Distance > 0)
//         {
//             // Weighted by inverse square of distance for stronger close-range separation
//             Separation += Direction.GetSafeNormal() * (BoidPtr->SeparationDistance / (Distance * Distance));
//         }
//     }
//     
//     return Separation.GetSafeNormal();
// }
//
// void ACPP_BoidGridManager::ProcessBoidMovement()
// {
//     // Temporary map for caching neighbors
//     TMap<FIntVector, TArray<TWeakObjectPtr<ACPP_BoidActor>>> CellNeighborsMap;
//
//     // Lock GridCells during neighbor collection
//     {
//         FScopeLock Lock(&GridLock);
//
//         for (auto& Cell : GridCells)
//         {
//             // Validate the grid cell
//             if (!Cell.Value || !IsValid(Cell.Value))
//             {
//                 continue;
//             }
//
//             // Gather neighbors from the current grid cell and surrounding cells
//             TArray<TWeakObjectPtr<ACPP_BoidActor>> Neighbors;
//
//             FIntVector CellKey = Cell.Key;
//             for (int32 x = -1; x <= 1; x++)
//             {
//                 for (int32 y = -1; y <= 1; y++)
//                 {
//                     for (int32 z = -1; z <= 1; z++)
//                     {
//                         FIntVector NeighborKey = CellKey + FIntVector(x, y, z);
//
//                         if (GridCells.Contains(NeighborKey))
//                         {
//                             UCPP_BoidGridCell* NeighborCell = GridCells[NeighborKey];
//                             if (NeighborCell && IsValid(NeighborCell))
//                             {
//                                 for (TWeakObjectPtr<ACPP_BoidActor> Boid : NeighborCell->GetBoids())
//                                 {
//                                     if (Boid.IsValid() && IsValid(Boid.Get()))
//                                     {
//                                         Neighbors.Add(Boid);
//                                     }
//                                 }
//                             }
//                         }
//                     }
//                 }
//             }
//
//             // Store valid neighbors for the current cell
//             if (Neighbors.Num() > 0)
//             {
//                 CellNeighborsMap.Add(CellKey, Neighbors);
//             }
//         }
//     }
//
//     // Process each boid after unlocking the grid
//     for (auto& CellEntry : CellNeighborsMap)
//     {
//         TArray<TWeakObjectPtr<ACPP_BoidActor>> CurrentNeighbors = CellEntry.Value;
//
//         for (TWeakObjectPtr<ACPP_BoidActor> Boid : CurrentNeighbors)
//         {
//             // Validate boid before accessing
//             if (!Boid.IsValid())
//             {
//                 continue;
//             }
//
//             ACPP_BoidActor* BoidPtr = Boid.Get();
//             if (!BoidPtr || !IsValid(BoidPtr))
//             {
//                 continue;
//             }
//
//             // Perform calculations for cohesion, alignment, and separation
//             FVector Cohesion = CalculateCohesion(Boid, CurrentNeighbors) * BoidPtr->CohesionFactor;
//             FVector Alignment = CalculateAlignment(Boid, CurrentNeighbors) * BoidPtr->AlignmentFactor;
//             FVector Separation = CalculateSeparation(Boid, CurrentNeighbors) * BoidPtr->SeparationFactor;
//
//             // Update the boid's target vector with the combined behaviors
//             FVector CombinedVector = Cohesion + Alignment + Separation;
//             if (!CombinedVector.IsNearlyZero())
//             {
//                 CombinedVector.Normalize();
//             }
//
//             // Apply the combined vector to the boid (thread-safe update)
//             {
//                 FScopeLock Lock(&BoidPtr->VectorLock);
//                 BoidPtr->TargetVector = CombinedVector;
//             }
//         }
//     }
// }
//
//
// void ACPP_BoidGridManager::DebugDrawGrid()
// {
//     for (const auto& Cell : GridCells)
//     {
//         FIntVector GridKey = Cell.Key;
//         FVector CellCenter = GridOrigin + FVector(
//             (GridKey.X + 0.5) * GridCellSize,
//             (GridKey.Y + 0.5) * GridCellSize,
//             (GridKey.Z + 0.5) * GridCellSize);
//             
//         FColor CellColor = Cell.Value->bIsObstructed ? FColor::Red : FColor::Blue;
//         DrawDebugBox(GetWorld(), CellCenter, FVector(GridCellSize/2), CellColor, false, 5.0f);
//     }
// }
//
// void ACPP_BoidGridManager::OnConstruction(const FTransform& Transform)
// {
//     Super::OnConstruction(Transform);
//     
//     FlushPersistentDebugLines(GetWorld());
//     InitializeGrid(GridCellSize);
//     DebugDrawGrid();
// }
//
// void ACPP_BoidGridManager::BeginPlay()
// {
//     Super::BeginPlay();
//     
//     InitializeGrid(GridCellSize);
//     InitializeObstacleGrid();
//     
//     // Set up timer to periodically process boid movement
//     GetWorld()->GetTimerManager().SetTimer(
//         RunTimerHandle,
//         this,
//         &ACPP_BoidGridManager::ProcessBoidMovement,
//         TimerInterval,
//         true
//     );
// }
//
// void ACPP_BoidGridManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
// {
//     // Clear timer first
//     GetWorld()->GetTimerManager().ClearTimer(RunTimerHandle);
//     
//     {
//         FScopeLock Lock(&GridLock);
//         for (auto& Cell : GridCells)
//         {
//             Cell.Value->GetBoids().Empty();  // Clear boid lists within each cell
//         }
//         GridCells.Empty(); // Fully clear grid
//         BoidToGridKeyMap.Empty(); // Clear mapping
//     }
//
//     Super::EndPlay(EndPlayReason);
// }