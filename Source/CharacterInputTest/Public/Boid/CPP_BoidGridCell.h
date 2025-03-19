#pragma once

#include "CoreMinimal.h"
#include "CPP_BoidGridCell.generated.h"

UCLASS()
class CHARACTERINPUTTEST_API UCPP_BoidGridCell : public UObject
{
GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid")
	TArray<class ACPP_BoidActor*> BoidInCell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid")
	bool bIsObstructed = false;

	UCPP_BoidGridCell();
	
	UFUNCTION(BlueprintCallable, Category = "Boid")
	void AddBoid(ACPP_BoidActor* Boid);

	UFUNCTION(BlueprintCallable, Category = "Boid")
	void RemoveBoid(ACPP_BoidActor* Boid);

	UFUNCTION(BlueprintCallable, Category = "Boid")
	TArray<ACPP_BoidActor*> GetBoids() const;

};
