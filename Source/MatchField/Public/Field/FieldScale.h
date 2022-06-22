#pragma once

#include "CoreMinimal.h"

#include "FieldScale.generated.h"

USTRUCT(BlueprintType)
struct FFieldScale
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere)
    float Width = 3.5f;
    
    UPROPERTY(EditAnywhere)
    float Length = 1.5f;

    UPROPERTY(EditAnywhere)
    float Thickness = 0.01f;

public:
    float GetWidth() const {return 0.01f * Width;}
    float GetLength() const {return 0.01f * Length;}
    float GetThickness() const {return Thickness;}
    FVector GetScale(float BaseWidth, float BaseLength) const
    {
        const float W = GetWidth() * BaseWidth;
        const float L = GetLength() * BaseLength;
        const float T = GetThickness();
        return FVector(W, L, T);
    }
};
