#pragma once

#include "CoreMinimal.h"
#include "MarkupLine.generated.h"

USTRUCT(BlueprintType)
struct FMarkupLine
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Start = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector End = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FLinearColor Color = FLinearColor::White;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Thickness = 0.25f;

	FMarkupLine(){}
	FMarkupLine(FVector start, FVector end, FLinearColor color, float thickness)
	{
		Start = start;
		End = end;
		Color = color;
		Thickness = thickness;
	}
};
