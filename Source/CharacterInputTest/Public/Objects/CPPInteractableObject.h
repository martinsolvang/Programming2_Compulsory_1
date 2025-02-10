// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interact_Interface.h"
#include "GameFramework/Actor.h"
#include "CPPInteractableObject.generated.h"

UCLASS()
class CHARACTERINPUTTEST_API ACPPInteractableObject : public AActor, public IInteract_Interface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "True"))
	UStaticMeshComponent* StaticMesh;
	
public:	
	// Sets default values for this actor's properties
	ACPPInteractableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void OnInteract_Implementation() override;

};
