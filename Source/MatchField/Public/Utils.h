#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Utils.generated.h"

UCLASS()
class MATCHFIELD_API UUtilsLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static TArray<FVector> CalculatePointsOnRadialBound(FVector Center, FVector DirectionInPlane, float FullAngle, float AngleStep, float Radius);
	static TArray<FVector> CalculateCirclePoints(FVector Center, FVector DirectionInPlane, float AngleStep, float Radius);
	static UStaticMesh* GetCubeMesh();
};

