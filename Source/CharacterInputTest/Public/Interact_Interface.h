

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Interact_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UInteract_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CHARACTERINPUTTEST_API IInteract_Interface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void OnInteract();
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

	
	
};
