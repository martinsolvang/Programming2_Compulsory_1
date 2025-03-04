// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interact_Interface.h"
#include "CPP_RotatingPuzzlePiece.generated.h"

UCLASS()
class CHARACTERINPUTTEST_API ACPP_RotatingPuzzlePiece : public AActor, public IInteract_Interface
{
	GENERATED_BODY()

	USceneComponent* SceneRoot;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "True"))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "True"))
	class UBoxComponent* CollisionBox;
public:	
	// Sets default values for this actor's properties
	ACPP_RotatingPuzzlePiece();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnInteract_Implementation() override;

	void Rotate();
	
	

};
