#include "Boid/CPP_BoidGridCell.h"

UCPP_BoidGridCell::UCPP_BoidGridCell()
{
	bIsObstructed = false;
}


void UCPP_BoidGridCell::AddBoid(ACPP_BoidActor* Boid)
{
	BoidInCell.Add(Boid);

}

void UCPP_BoidGridCell::RemoveBoid(ACPP_BoidActor* Boid)
{
	BoidInCell.Remove(Boid);
}

TArray<TWeakObjectPtr<ACPP_BoidActor>> UCPP_BoidGridCell::GetBoids() const
{
	return BoidInCell;
}
