// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_RotatingPuzzlePiece.h"
#include "GameFramework/Actor.h"
#include "PuzzleManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPuzzleSolved);

UCLASS()
class CHARACTERINPUTTEST_API APuzzleManager : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "PuzzleManager")
	TArray<ACPP_RotatingPuzzlePiece*> PuzzlePieces;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "PuzzleManager")
	TArray<int32> PuzzlePiecesSolution;

	UPROPERTY(BlueprintAssignable, Category = "Puzzle Events")
	FOnPuzzleSolved OnPuzzleSolved;

	
	void CheckPuzzleState();
	
	UFUNCTION()
	void OnPuzzlePieceRotated(ACPP_RotatingPuzzlePiece* RotatedPiece);

	// Sets default values for this actor's properties
	APuzzleManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
