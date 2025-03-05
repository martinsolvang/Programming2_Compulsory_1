// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzles/CPP_SequencePuzzlePiece.h"


ACPP_SequencePuzzlePiece::ACPP_SequencePuzzlePiece()
{
	bCanMove = false;
	PrimaryActorTick.bCanEverTick = false;
	EndingLocation = GetActorLocation() + FVector(0.0f, 0.0f, -50.0f);
	MovementSpeed = 100.0f;
}

void ACPP_SequencePuzzlePiece::Activate()
{
	Super::Activate();
	bCanMove = true;
	PrimaryActorTick.bCanEverTick = true;
	
}

//Function to move the PuzzlePiece when activated
void ACPP_SequencePuzzlePiece::Move(float DeltaTime)
{
	FVector NewPosition = FMath::VInterpTo(GetActorLocation(), EndingLocation, DeltaTime, MovementSpeed);
	SetActorLocation(NewPosition);
	if (FMath::IsNearlyEqual(NewPosition, EndingLocation, 0.01f))
	{
		SetActorLocation(EndingLocation);
		bCanMove = false;
		PrimaryActorTick.bCanEverTick = false;
		OnStateChanged.Broadcast(this);
	}

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
