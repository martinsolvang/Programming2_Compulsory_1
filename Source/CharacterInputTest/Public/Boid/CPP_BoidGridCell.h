// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


/**
 * 
 */

USTRUCT()
struct FCPP_BoidGridCell
{
	GENERATED_BODY()
public:
	
	UPROPERTY()
	TArray<class ACPP_BoidActor*> BoidInCell;

	UPROPERTY()
	bool bIsObstructed = false;

	void AddBoid(class ACPP_BoidActor* Boid);

	void RemoveBoid(class ACPP_BoidActor* Boid);

	TArray<ACPP_BoidActor*> GetBoids() const;
	
};
