// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Interact_Interface.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "CPPSubmarineTest.generated.h"

UCLASS()
class CHARACTERINPUTTEST_API ACPPSubmarineTest : public APawn
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "True"))
	UStaticMeshComponent* SubmarineMesh;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "True"))
	UFloatingPawnMovement* MovementComponent;

public:
	// Sets default values for this pawn's properties
	ACPPSubmarineTest();


protected:
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* ElevateAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* DeElevateAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* InteractAction;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	void Elevate();
	void DeElevate();
	void InteractWithObject();
	void Move(const FInputActionValue& InputValue);

	void Look(const FInputActionValue& InputValue);

};
