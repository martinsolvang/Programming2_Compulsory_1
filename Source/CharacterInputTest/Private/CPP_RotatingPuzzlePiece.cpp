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

}

void ACPP_RotatingPuzzlePiece::OnInteract_Implementation()
{
	
}

void ACPP_RotatingPuzzlePiece::Rotate()
{
	
}


