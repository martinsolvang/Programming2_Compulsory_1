#pragma once

#include "CoreMinimal.h"
#include "CPP_BoidGridCell.generated.h"

class ACPP_BoidActor;

UCLASS()
class CHARACTERINPUTTEST_API UCPP_BoidGridCell : public UObject
{
GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<TWeakObjectPtr<ACPP_BoidActor>> BoidInCell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid")
	bool bIsObstructed = false;

	UCPP_BoidGridCell();
	
	UFUNCTION(BlueprintCallable, Category = "Boid")
	void AddBoid(ACPP_BoidActor* Boid);

	UFUNCTION(BlueprintCallable, Category = "Boid")
	void RemoveBoid(ACPP_BoidActor* Boid);

	UFUNCTION()
	TArray<TWeakObjectPtr<ACPP_BoidActor>> GetBoids() const;

};
