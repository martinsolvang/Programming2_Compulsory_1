// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzles/CPP_PuzzleBase.h"
#include "Components/BoxComponent.h"


// Sets default values
ACPP_PuzzleBase::ACPP_PuzzleBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>("Scene Root");
	RootComponent = SceneRoot;

	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StaticMesh->SetupAttachment(SceneRoot);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>("Collision Box");
	CollisionBox->SetupAttachment(StaticMesh);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	bIsActive = true;

}

void ACPP_PuzzleBase::Activate()
{
}

// Called when the game starts or when spawned
void ACPP_PuzzleBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_PuzzleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_PuzzleBase::OnInteract_Implementation()
{
	IInteract_Interface::OnInteract_Implementation();
	Activate();
}

