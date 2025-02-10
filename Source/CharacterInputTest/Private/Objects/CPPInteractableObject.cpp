// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/CPPInteractableObject.h"

// Sets default values
ACPPInteractableObject::ACPPInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StaticMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACPPInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACPPInteractableObject::OnInteract_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "Interacted With Object");
	Destroy();
}


