// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPSubmarineTest.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values

// Called when the game starts or when spawned

ACPPSubmarineTest::ACPPSubmarineTest()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SubmarineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SubmarineMesh"));
	RootComponent = SubmarineMesh;

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));


}

void ACPPSubmarineTest::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACPPSubmarineTest::Elevate()
{
}

void ACPPSubmarineTest::DeElevate()
{
}

void ACPPSubmarineTest::InteractWithObject()
{
}

void ACPPSubmarineTest::Move(const FInputActionValue& InputValue)
{
}

void ACPPSubmarineTest::Look(const FInputActionValue& InputValue)
{
}

// Called every frame
void ACPPSubmarineTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPPSubmarineTest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}

	}
	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPPSubmarineTest::Move);

		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPPSubmarineTest::Look);

		Input->BindAction(ElevateAction, ETriggerEvent::Triggered, this, &ACPPSubmarineTest::Elevate);
		Input->BindAction(DeElevateAction, ETriggerEvent::Triggered, this, &ACPPSubmarineTest::DeElevate);

		Input->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACPPSubmarineTest::InteractWithObject);
	}

}

