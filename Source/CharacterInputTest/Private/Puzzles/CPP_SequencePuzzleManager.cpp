// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzles/CPP_SequencePuzzleManager.h"

ACPP_SequencePuzzleManager::ACPP_SequencePuzzleManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACPP_SequencePuzzleManager::BeginPlay()
{
	Super::BeginPlay();

	// Log initial PuzzlePiecesCurrent state
	UE_LOG(LogTemp, Warning, TEXT("Initial PuzzlePiecesCurrent size: %d"), PuzzlePiecesCurrent.Num());

	if (PuzzlePieces.Num() == 0 || PuzzlePiecesSolution.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("APuzzleManager::BeginPlay - PuzzlePieces or Solution array is empty! Make sure to populate it in the editor."));
	}

	for (ACPP_SequencePuzzlePiece* Piece : PuzzlePieces)
	{
		if (Piece)
		{
			Piece->OnStateChanged.AddDynamic(this, &ACPP_SequencePuzzleManager::OnSequencePieceMoved);
			UE_LOG(LogTemp, Warning, TEXT("Bound puzzle piece to delegate: %s"), *Piece->GetName());
		}
	}

	// Log PuzzlePiecesSolution for reference
	UE_LOG(LogTemp, Warning, TEXT("PuzzlePiecesSolution:"));
	for (int32 Id : PuzzlePiecesSolution)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d"), Id);
	}
}

void ACPP_SequencePuzzleManager::CheckPuzzleState() 
{
	Super::CheckPuzzleState();


	UE_LOG(LogTemp, Warning, TEXT("CheckPuzzleState() called"));

	// Check if the current sequence size matches the expected solution size
	if (PuzzlePiecesCurrent.Num() != PuzzlePiecesSolution.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Array sizes do not match! Resetting puzzle."));
		ResetPuzzle();
		return;
	}
	
	bool bIsComplete = true;

	// Compare each value in order
	for (int32 i = 0; i < PuzzlePiecesSolution.Num(); i++)
	{
		if (PuzzlePiecesCurrent[i] != PuzzlePiecesSolution[i])
		{
			UE_LOG(LogTemp, Warning, TEXT("Mismatch found at index %d! Resetting puzzle."), i);
			bIsComplete = false;
			break;
		}
	}

	// If solved, notify listeners
	if (bIsComplete)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Puzzle Solved!"));
		OnPuzzleSolved.Broadcast(); // Trigger the delegate
		SetActorTickEnabled(false);
	}

	else
	{
		// Log before resetting
		UE_LOG(LogTemp, Warning, TEXT("Solution incorrect, resetting puzzle."));
		ResetPuzzle();
	}
	
}

//Resets puzzle if solution didn't match
void ACPP_SequencePuzzleManager::ResetPuzzle()
{
	UE_LOG(LogTemp, Warning, TEXT("Resetting puzzle!"));
	for (ACPP_SequencePuzzlePiece* Piece : PuzzlePieces)
	{
		if (Piece && Piece->bIsMoving)
		{
			Piece->ResetMovement();
		}
	}
	PuzzlePiecesCurrent.Empty();
}

//Updates puzzle and/or checks if puzzle is completed
void ACPP_SequencePuzzleManager::OnSequencePieceMoved(ACPP_PuzzleBase* ChangedPiece)
{
	if (ACPP_SequencePuzzlePiece* MovedPiece = Cast<ACPP_SequencePuzzlePiece>(ChangedPiece))
	{
		// Ensure no duplicates
		if (!PuzzlePiecesCurrent.Contains(MovedPiece->Idenfifier))
		{
			PuzzlePiecesCurrent.Add(MovedPiece->Idenfifier);
		}

		// Make sure it's only checking when enough pieces have moved
		if (PuzzlePiecesCurrent.Num() == PuzzlePiecesSolution.Num())
		{
			CheckPuzzleState();
		}
	}
}



