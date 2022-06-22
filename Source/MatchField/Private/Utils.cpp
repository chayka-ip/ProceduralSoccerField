#include "Utils.h"
#include "MatchField/Public/Utils.h"

TArray<FVector> UUtilsLib::CalculatePointsOnRadialBound(FVector Center, FVector DirectionInPlane, float FullAngle, float AngleStep,
                                                        float Radius)
{
	TArray<FVector> TargetPoints;
	const FVector V = Radius * DirectionInPlane;

	FVector FirstPoint = V + Center;
	FVector LastPoint = V.RotateAngleAxis(FullAngle, FVector::UpVector) + Center;
	
	if(FMath::Abs(FullAngle) <= FMath::Abs(AngleStep))
	{
		TargetPoints = {FirstPoint, LastPoint};
	}
	else
	{
		const float DeltaAngle = FMath::Abs(AngleStep) * FMath::Sign(FullAngle);
		const int NumSteps = FMath::Abs(FullAngle) / AngleStep;
		for (int i = 0; i < NumSteps; ++i)
		{
			FVector P = V.RotateAngleAxis(i * DeltaAngle, FVector::UpVector) + Center;
			TargetPoints.Add(P);
		}
		TargetPoints.Add(LastPoint);
	}
	return TargetPoints;
}

TArray<FVector> UUtilsLib::CalculateCirclePoints(FVector Center, FVector DirectionInPlane, float AngleStep, float Radius)
{
	const auto FullCircleAngleComp = 360.0f + AngleStep;
	return  CalculatePointsOnRadialBound(Center, DirectionInPlane, FullCircleAngleComp, AngleStep, Radius);
}

UStaticMesh* UUtilsLib::GetCubeMesh()
{
	const auto P = TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMeshAsset(P);
	return BaseMeshAsset.Object ? BaseMeshAsset.Object : nullptr;
}
