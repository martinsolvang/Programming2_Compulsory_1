// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Puzzles/CPP_PuzzleBase.h"
#include "CPP_SequencePuzzlePiece.generated.h"


UCLASS()
class CHARACTERINPUTTEST_API ACPP_SequencePuzzlePiece : public ACPP_PuzzleBase
{
	GENERATED_BODY()

	bool bIsTriggered;

	bool bCanMove;

	FVector EndingLocation;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Piece")
	float MovementSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Piece")
	int32 Idenfifier;

	ACPP_SequencePuzzlePiece();

	virtual void Tick(float DeltaTime) override;
	
	virtual void Activate() override;

	void Move(float DeltaTime);
	
};
