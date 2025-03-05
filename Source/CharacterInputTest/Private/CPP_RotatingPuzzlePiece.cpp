// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_RotatingPuzzlePiece.h"

#include "Components/BoxComponent.h"

// Sets default values
ACPP_RotatingPuzzlePiece::ACPP_RotatingPuzzlePiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("Scene Root");
	SceneRoot->SetupAttachment(RootComponent);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StaticMesh->SetupAttachment(SceneRoot);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>("Collision Box");
	CollisionBox->SetupAttachment(SceneRoot);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	
}

// Called when the game starts or when spawned
void ACPP_RotatingPuzzlePiece::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_RotatingPuzzlePiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsRotating)
	{
		Rotate(DeltaTime);
	}
	

}

void ACPP_RotatingPuzzlePiece::OnInteract_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Rotated");
	//Starts rotation function if it's not already rotating
	if (!bIsRotating)
	{
		bIsRotating = true;
		CurrentTime = 0.0f;
		StartRotation = GetActorRotation();
		TargetRotation = StartRotation + FRotator(0, 90, 0);
		
	}
}

void ACPP_RotatingPuzzlePiece::Rotate(float DeltaTime)
{
	// Interpolate using RInterpConstantTo for constant rotation speed
	FRotator NewRotation = FMath::RInterpConstantTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed);

	// Apply the new rotation
	SetActorRotation(NewRotation);

	// Stop rotating when close enough to the target
	if (NewRotation.Equals(TargetRotation, 1.0f)) // 1 degree tolerance
	{
		bIsRotating = false;
		SetActorRotation(TargetRotation); // Ensure exact match
		RotationPosition++;
		if (RotationPosition > 3)
		{
			RotationPosition = 0;
		}

		OnRotationChanged.Broadcast(this);

		
	}


		
}


