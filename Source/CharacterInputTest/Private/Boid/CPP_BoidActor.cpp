// Fill out your copyright notice in the Description page of Project Settings.

#include "DistanceFieldAtlas.h"
#include "Boid/CPP_BoidActor.h"
#include "DrawDebugHelpers.h"
#include "MaterialHLSLTree.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/MapErrors.h"

// Sets default values
ACPP_BoidActor::ACPP_BoidActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CohesionFactor = 1;
	SeparationFactor = 1;
	AlignmentFactor = 1;
	CurrentVector = FVector::ZeroVector;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

bool ACPP_BoidActor::ShouldPerformObstacleAvoidance()
{
	if (!GridManager)
	{
		// Find the grid manager if not set
		return true;
	}
    
	bool nearObstacleCell = GridManager->IsLocationNearObstacles(GetActorLocation(), 200.0f);
	
	return nearObstacleCell;
}

void ACPP_BoidActor::ScheduleObstacleAvoidance()
{
	// Only schedule obstacle avoidance if needed
	if (ShouldPerformObstacleAvoidance())
	{
		// For fish near obstacles, check more frequently
		float ObstacleCheckInterval = 0.1f;
        
		if (!GetWorldTimerManager().IsTimerActive(ObstacleAvoidanceTimerHandle))
		{
			GetWorldTimerManager().SetTimer(
				ObstacleAvoidanceTimerHandle,
				this,
				&ACPP_BoidActor::ObstacleAvoidance,
				ObstacleCheckInterval,
				true
			);
		}
	}
	else
	{
		// For fish far from obstacles, we can disable obstacle avoidance
		if (GetWorldTimerManager().IsTimerActive(ObstacleAvoidanceTimerHandle))
		{
			GetWorldTimerManager().ClearTimer(ObstacleAvoidanceTimerHandle);
		}
	}
}

// Called when the game starts or when spawned
void ACPP_BoidActor::BeginPlay()
{
	Super::BeginPlay();

	if (!StaticMeshComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("StaticMeshComponent is not initialized for %s. Please initialize it."), *GetName());
		return;
	}

	if (!GridManager)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPP_BoidGridManager::StaticClass(), FoundActors);
		if (FoundActors.Num() > 0)
		{
			GridManager = Cast<ACPP_BoidGridManager>(FoundActors[0]);
		}
	}
    
	// Register with grid manager
	if (GridManager)
	{
		GridManager->RegisterBoid(this);
	}
    
	PreviousLocation = GetActorLocation();
    
	// Set up timer for obstacle avoidance
	// GetWorld()->GetTimerManager().SetTimer(
	// 	ObstacleAvoidanceTimerHandle,
	// 	this,
	// 	&ACPP_BoidActor::ObstacleAvoidance,
	// 	ObstacleAvoidanceInterval,
	// 	true
	// );
	
	CurrentVector = GetActorForwardVector().GetSafeNormal()*MovementSpeed;
	//CurrentVector = FVector::ZeroVector;
}

void ACPP_BoidActor::ObstacleAvoidance()
{
    const float RaycastDistance = 200.0f;
    const float EmergencyDistance = 50.0f;
    float DeltaTime = GetWorld()->GetDeltaSeconds();
    
    FVector CurrentLocation = GetActorLocation();
    FVector CurrentDirection = CurrentVector.GetSafeNormal();
    
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    
    // 5 rays: forward, left, right, up, down
    FVector Directions[5];
    Directions[0] = CurrentDirection;
    
    // Horizontal rays
    FVector RightVector = FVector::CrossProduct(CurrentDirection, FVector::UpVector).GetSafeNormal();
    Directions[1] = (CurrentDirection + RightVector * 0.5f).GetSafeNormal();
    Directions[2] = (CurrentDirection - RightVector * 0.5f).GetSafeNormal();
    
    // Vertical rays
    FVector UpVector = FVector::CrossProduct(RightVector, CurrentDirection).GetSafeNormal();
    Directions[3] = (CurrentDirection + UpVector * 0.5f).GetSafeNormal();
    Directions[4] = (CurrentDirection - UpVector * 0.5f).GetSafeNormal();
    
    bool bNeedsAvoidance = false;
    FVector AvoidanceVector = FVector::ZeroVector;
    float ClosestHitDistance = RaycastDistance;
    
    for (int32 i = 0; i < 5; i++)
    {
        FHitResult Hit;
        bool bHit = GetWorld()->LineTraceSingleByChannel(
            Hit,
            CurrentLocation,
            CurrentLocation + Directions[i] * RaycastDistance,
            ECC_WorldStatic,
            QueryParams
        );
        
        if (bHit)
        {
            bNeedsAvoidance = true;
            
            // Progressive weight - more influence as we get closer
            float AvoidWeight = 1.0f - (Hit.Distance / RaycastDistance);
            
            // Forward ray gets more weight
            if (i == 0) AvoidWeight *= 1.3f;
            
            AvoidanceVector += Hit.Normal * AvoidWeight;
            
            if (Hit.Distance < ClosestHitDistance)
            {
                ClosestHitDistance = Hit.Distance;
            }
        }
    }
    
    if (bNeedsAvoidance && !AvoidanceVector.IsNearlyZero())
    {
        AvoidanceVector.Normalize();
        
        // Dynamic avoidance strength based on distance
        float AvoidanceStrength = 1.0f;
        if (ClosestHitDistance < EmergencyDistance)
        {
            // Scale up gradually as we get closer to emergency distance
            float EmergencyFactor = 1.0f - (ClosestHitDistance / EmergencyDistance);
            AvoidanceStrength = FMath::Lerp(1.0f, 2.5f, EmergencyFactor);
        }
        
        // Blend factor increases gradually as we get closer
        float BlendFactor = FMath::Clamp(1.0f - (ClosestHitDistance / RaycastDistance), 0.2f, 0.8f);
        
        FVector DesiredDirection = FMath::Lerp(
            CurrentDirection,
            AvoidanceVector,
            BlendFactor * AvoidanceStrength
        ).GetSafeNormal();
        
        // Calculate the force needed
        FVector DesiredForce = (DesiredDirection * CurrentVector.Size()) - CurrentVector;
        
        // Apply smooth interpolation - more responsive than original but not instant
        CurrentAvoidanceDirection = FMath::VInterpTo(
            CurrentAvoidanceDirection,
            DesiredForce,
            DeltaTime,
            // Adjust this value to control responsiveness
            // Lower = smoother but slower, Higher = quicker but potentially jerky
            3.0f 
        );
    }
    else
    {
        // Gradually decrease avoidance when no obstacles
        CurrentAvoidanceDirection = FMath::VInterpTo(
            CurrentAvoidanceDirection,
            FVector::ZeroVector,
            DeltaTime,
            2.0f
        );
    }
}

void ACPP_BoidActor::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);

	
	FVector OldPosition = GetActorLocation();

	
	// Handle obstacle avoidance
	ScheduleObstacleAvoidance();

	FScopeLock Lock(&VectorLock);
	if (bVectorBufferReady) {
		CurrentVector = NextVector;
		bVectorBufferReady = false;
	}
	
	if (!CurrentAvoidanceDirection.IsNearlyZero())
	{
		// Apply avoidance with DeltaTime to make it framerate independent
		CurrentVector += CurrentAvoidanceDirection * DeltaTime * AvoidanceFactor;
    
		// Maintain max speed
		CurrentVector = CurrentVector.GetClampedToMaxSize(MovementSpeed);
	}
    
	// Apply the movement vector
	FVector NewLocation = GetActorLocation() + CurrentVector * DeltaTime;
    
	SetActorLocation(NewLocation);
    
	// Set rotation to face direction of travel
	if (!CurrentVector.IsNearlyZero())
	{
		FRotator NewRotation = CurrentVector.Rotation();
		SetActorRotation(NewRotation);
	}
	
	if (GridManager && !OldPosition.Equals(GetActorLocation(), GridManager->GridCellSize * 0.5f))
	{
		GridManager->RegisterBoid(this);
	}
	
}