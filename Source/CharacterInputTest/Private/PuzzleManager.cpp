// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleManager.h"


void APuzzleManager::CheckPuzzleState()
{
	bool bIsSolved = true;

	// Check if puzzle pieces match the solution
	for (int32 i = 0; i < PuzzlePieces.Num(); i++)
	{
		if (PuzzlePieces[i]->RotationPosition != PuzzlePiecesSolution[i])
		{
			bIsSolved = false;
			break;
		}
	}

	// If solved, notify listeners
	if (bIsSolved)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Puzzle Solved!"));
		OnPuzzleSolved.Broadcast(); // Trigger the delegate
		PrimaryActorTick.bCanEverTick = false;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Puzzle Not Solved Yet."));
	}

}

void APuzzleManager::OnPuzzlePieceRotated(ACPP_RotatingPuzzlePiece* RotatedPiece)
{
	UE_LOG(LogTemp, Log, TEXT("Puzzle Piece Rotated: %s"), *RotatedPiece->GetName());

	// Call CheckPuzzleState whenever a piece's rotation changes
	CheckPuzzleState();
}


// Sets default values
APuzzleManager::APuzzleManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APuzzleManager::BeginPlay()
{
	Super::BeginPlay();
	// Bind each puzzle piece's delegate to the CheckPuzzleState function
	for (ACPP_RotatingPuzzlePiece* Piece : PuzzlePieces)
	{
		if (Piece)
		{
			// Bind CheckPuzzleState to the OnRotationChanged delegate
			Piece->OnRotationChanged.AddDynamic(this, &APuzzleManager::OnPuzzlePieceRotated);
		}
	}

}

// Called every frame
void APuzzleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

