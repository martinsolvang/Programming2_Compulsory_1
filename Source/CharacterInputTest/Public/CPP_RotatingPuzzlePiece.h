// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interact_Interface.h"
#include "CPP_RotatingPuzzlePiece.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRotationChanged, ACPP_RotatingPuzzlePiece*, RotatedPiece);

UCLASS()
class CHARACTERINPUTTEST_API ACPP_RotatingPuzzlePiece : public AActor, public IInteract_Interface
{
	GENERATED_BODY()

	USceneComponent* SceneRoot;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "True"))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "True"))
	class UBoxComponent* CollisionBox;
	
	bool bIsRotating = false;
	float CurrentTime= 0.0f;
	float RotationTime = 1.0f;
	
	UPROPERTY(EditAnywhere, Category = "Rotation")
	float RotationSpeed = 10.0f;
	
	FRotator StartRotation;
	FRotator TargetRotation;
	
public:
	// This delegate will trigger when RotationPosition is updated
	UPROPERTY(BlueprintAssignable, Category = "Puzzle Events")
	FOnRotationChanged OnRotationChanged;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Position")
	int32 RotationPosition = 0;
	
	// Sets default values for this actor's properties
	ACPP_RotatingPuzzlePiece();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnInteract_Implementation() override;

	void Rotate(float DeltaTime);
	
	

};
