// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzles/CPP_SequencePuzzlePiece.h"


ACPP_SequencePuzzlePiece::ACPP_SequencePuzzlePiece()
{
	MovementSpeed = 100.0f;
	bCanMove = false;
	bIsMoving = false;
	bIsActive = true;
	bIsResetting = false;
	PrimaryActorTick.bCanEverTick = true;

}

void ACPP_SequencePuzzlePiece::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);

	StaticMesh->SetMaterial(0,InactiveMaterial);
	StartingLocation = GetActorLocation();
	EndingLocation = StartingLocation + FVector(0.0f, 0.0f, -50.0f);
}

void ACPP_SequencePuzzlePiece::Activate()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Activated Puzzlepiece"));

	Super::Activate();
	if (bIsActive)
	{
		bCanMove = true;
		SetActorTickEnabled(true);

		UE_LOG(LogTemp, Warning, TEXT("bCanMove set to: %d"), bCanMove);

		bIsActive = false;
		StaticMesh->SetMaterial(0, ActiveMaterial);
		EndingLocation = GetActorLocation() + FVector(0.0f, 0.0f, -50.0f);
	}
}

//Function to move the PuzzlePiece when activated
void ACPP_SequencePuzzlePiece::Move(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("Move function executing"));
	FVector NewPosition = FMath::VInterpTo(GetActorLocation(), EndingLocation, DeltaTime, MovementSpeed);
	
	SetActorLocation(NewPosition);
	
	if (FVector::Dist(NewPosition, EndingLocation) <= 0.01f)
	{
		SetActorLocation(EndingLocation);

		if (!bIsResetting)
		{
			OnStateChanged.Broadcast(this);

		}

		bCanMove = false;
		bIsResetting = false;
		bIsMoving = false;
		SetActorTickEnabled(false);
	}
	else
	{
		bIsMoving = true;  
	}
	
}

void ACPP_SequencePuzzlePiece::ResetMovement()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Reset puzzle position"));

	bIsResetting = true;
	bCanMove = true;
	bIsActive = true;

	EndingLocation = StartingLocation;


	SetActorTickEnabled(true);
	
	StaticMesh->SetMaterial(0,InactiveMaterial);

}

// Called every frame
void ACPP_SequencePuzzlePiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bCanMove)
	{
		Move(DeltaTime);
	}

}
