// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CPP_BoidDataAsset.generated.h"

UENUM(BLueprintType)
enum EBoidType : uint8
{
	Small UMETA(DisplayName = "Small"),
	Medium UMETA(DisplayName = "Medium"),	
	Large UMETA(DisplayName = "Large")	
};

UCLASS(BLueprintType)
class CHARACTERINPUTTEST_API UCPP_BoidDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boid")
	UStaticMesh* BoidMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boid")
	TEnumAsByte<EBoidType> BoidType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boid")
	float MovementSpeed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boid")
	bool FollowPlayer = false;
};
