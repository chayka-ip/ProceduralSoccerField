#pragma once

#include "CoreMinimal.h"
#include "MarkupLine.h"
#include "MarkupRectangle.generated.h"

USTRUCT(BlueprintType)
struct FMarkupRectangle
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector LeftTopCorner = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector LeftDownCorner = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector RightTopCorner = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector RightDownCorner = FVector::ZeroVector;

	TArray<FMarkupLine> GetLines(FLinearColor Color, float Thickness) const
	{
		TArray<FMarkupLine> A;

		A.Add(FMarkupLine(LeftTopCorner, LeftDownCorner, Color, Thickness));
		A.Add(FMarkupLine(LeftTopCorner, RightTopCorner, Color, Thickness));
		A.Add(FMarkupLine(RightTopCorner, RightDownCorner, Color, Thickness));
		A.Add(FMarkupLine(RightDownCorner, LeftDownCorner, Color, Thickness));
		return A;
	}

	TArray<FVector> GetPoints()
	{
		return {LeftTopCorner, RightTopCorner, RightDownCorner, LeftDownCorner};
	}
	TArray<FVector> GetTopLinePoints() {return {LeftTopCorner, RightTopCorner};}
	TArray<FVector> GetDownLinePoints() {return {LeftDownCorner, RightDownCorner};}
	TArray<FVector> GetLeftLinePoints() {return {LeftTopCorner, LeftDownCorner};}
	TArray<FVector> GetRightLinePoints() {return {RightTopCorner, RightDownCorner};}
	

};
