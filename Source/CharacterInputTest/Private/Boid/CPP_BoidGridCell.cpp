// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid/CPP_BoidGridCell.h"

void UCPP_BoidGridCell::AddBoid(class ACPP_BoidActor* Boid)
{
	if (Boid && !BoidInCell.Contains(Boid))
	{
		BoidInCell.Add(Boid);
	}

}

void UCPP_BoidGridCell::RemoveBoid(class ACPP_BoidActor* Boid)
{
	if (Boid)
	{
		BoidInCell.Remove(Boid);
	}

}

TArray<ACPP_BoidActor*> UCPP_BoidGridCell::GetBoids() const
{
	return BoidInCell;
}
