// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPCharacter.h"
//#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"



// Sets default values
ACPPCharacter::ACPPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;


}

// Called when the game starts or when spawned
void ACPPCharacter::BeginPlay()
{
	Super::BeginPlay();


	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
		
	}
}

// Called every frame
void ACPPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPPCharacter::Move);

		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPPCharacter::Look);

		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACPPCharacter::Jump);

		Input->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACPPCharacter::InteractWithObject);
	}

}

void ACPPCharacter::Move(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0,Rotation.Yaw, 0);

		const FVector ForwardRotation = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardRotation, InputVector.Y);
		AddMovementInput(RightDirection, InputVector.X);
	}
}

void ACPPCharacter::Look(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		AddControllerYawInput(InputVector.X);
		AddControllerPitchInput(InputVector.Y);
	}

}

void ACPPCharacter::Jump()
{
	ACharacter::Jump();
}

void ACPPCharacter::InteractWithObject()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Emerald, "Pressed E");
	FVector Start = RootComponent->GetComponentLocation();
	FVector End = Start;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->SweepSingleByChannel(HitResult,Start,End,FQuat::Identity,ECC_Visibility,FCollisionShape::MakeSphere(150),Params))
	{
		if (IInteract_Interface* Interact_Interface = Cast<IInteract_Interface>(HitResult.GetActor()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Emerald, "Interacted with interactable object");
			Interact_Interface->OnInteract_Implementation();
		}
	}
	DrawDebugSphere(GetWorld(), Start, 150, 32, FColor::Red, false, 3.f, 0, 1);
		
}




