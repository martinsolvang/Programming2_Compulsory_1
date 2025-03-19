// Fill out your copyright notice in the Description page of Project Settings.

#include "DistanceFieldAtlas.h"
#include "Boid/CPP_BoidActor.h"

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

// Called when the game starts or when spawned
void ACPP_BoidActor::BeginPlay()
{
	Super::BeginPlay();

	if (!StaticMeshComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("StaticMeshComponent is not initialized for %s. Please initialize it."), *GetName());
		return;
	}

	if (BoidData)
	{
		//CurrentVector = CurrentVector*BoidData->MovementSpeed;
	}

	CurrentVector = GetActorForwardVector().GetSafeNormal()*MovementSpeed;
}

void ACPP_BoidActor::ObstacleAvoidance(float DeltaTime)
{
	// Distance for raycast
    const float RaycastDistance = 300.0f;

    FVector Start = GetActorLocation();
    FVector ForwardVector = CurrentVector.GetSafeNormal();

    // Directions: Forward, Right (45), Left (-45)
    FVector ForwardRay = ForwardVector;
    FVector RightRay = ForwardVector.RotateAngleAxis(45.0f, FVector::UpVector);
    FVector LeftRay = ForwardVector.RotateAngleAxis(-45.0f, FVector::UpVector);

    // Raycast results
    FHitResult HitForward, HitRight, HitLeft;
    float DistanceForward = RaycastDistance; // Default to max distance
    float DistanceRight = RaycastDistance;
    float DistanceLeft = RaycastDistance;

    // Perform raycasts
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this); // Ignore self

    // Forward raycast
    GetWorld()->LineTraceSingleByChannel(HitForward, Start, Start + ForwardRay * RaycastDistance, ECC_WorldStatic, QueryParams);
    if (HitForward.bBlockingHit)
    {
        DistanceForward = HitForward.Distance; // Get distance to obstacle
    }

    // Right raycast
    GetWorld()->LineTraceSingleByChannel(HitRight, Start, Start + RightRay * RaycastDistance, ECC_WorldStatic, QueryParams);
    if (HitRight.bBlockingHit)
    {
        DistanceRight = HitRight.Distance;
    }

    // Left raycast
    GetWorld()->LineTraceSingleByChannel(HitLeft, Start, Start + LeftRay * RaycastDistance, ECC_WorldStatic, QueryParams);
    if (HitLeft.bBlockingHit)
    {
        DistanceLeft = HitLeft.Distance;
    }

    // Debug raycasts
    DrawDebugLine(GetWorld(), Start, Start + ForwardRay * RaycastDistance, FColor::Blue, false, 0.1f, 0, 1.0f); // Forward
    DrawDebugLine(GetWorld(), Start, Start + RightRay * RaycastDistance, FColor::Red, false, 0.1f, 0, 1.0f);  // Right
    DrawDebugLine(GetWorld(), Start, Start + LeftRay * RaycastDistance, FColor::Green, false, 0.1f, 0, 1.0f);  // Left

    // Scoring for directions
    float ForwardScore = DistanceForward; // Favor longer distance
    float RightScore = DistanceRight;
    float LeftScore = DistanceLeft;

    // Add bias to prioritize forward motion
    ForwardScore += 50.0f; // Encourage forward movement

    // Bonus for no-hit rays (completely open directions)
    if (DistanceForward == RaycastDistance) { ForwardScore += 100.0f; }
    if (DistanceRight == RaycastDistance) { RightScore += 100.0f; }
    if (DistanceLeft == RaycastDistance) { LeftScore += 100.0f; }

    // Penalize non-parallel movement (optional for "up/down control")
    // if (bPenalizeVerticalMovement)
    // {
    //     // If the direction goes significantly up or down, penalize it.
    //     if (RightRay.Z > 0.5f || RightRay.Z < -0.5f) RightScore -= 25.0f;
    //     if (LeftRay.Z > 0.5f || LeftRay.Z < -0.5f) LeftScore -= 25.0f;
    // }

    // Determine the best direction
    FVector BestDirection = ForwardRay;
    float MaxScore = ForwardScore;

    if (RightScore > MaxScore)
    {
        BestDirection = RightRay;
        MaxScore = RightScore;
    }
    if (LeftScore > MaxScore)
    {
        BestDirection = LeftRay;
        MaxScore = LeftScore;
    }

    // Adjust current vector using the best direction
    CurrentVector += BestDirection * AvoidanceFactor;


}
	


void ACPP_BoidActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ObstacleAvoidance(DeltaTime);

	if (!CurrentVector.IsNearlyZero())
	{
		CurrentVector = CurrentVector.GetSafeNormal() * MovementSpeed;
	}
	
	FVector NewLocation = GetActorLocation() + CurrentVector * DeltaTime;
	SetActorLocation(NewLocation);
	SetActorRotation(CurrentVector.Rotation());
	FVector DebugEnd = GetActorLocation() + (CurrentVector * 100); // Scale the vector
	//DrawDebugLine(GetWorld(), GetActorLocation(), DebugEnd, FColor::Blue, false, 0.1f, 0, 2.0f);

}

