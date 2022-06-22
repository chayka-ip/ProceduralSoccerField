#pragma once

#include "CoreMinimal.h"
#include "FieldMeshParams.generated.h"

USTRUCT(BlueprintType)
struct FFieldMeshParams
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    TArray<UMaterial*> LineMaterials;

    UPROPERTY(EditAnywhere)
    float SegmentWidth = 500.0f;

    UPROPERTY(EditAnywhere)
    float Thickness = 0.005f;

public:
    float GetHalfSegmentWidth() const {return 0.5f * SegmentWidth;}
    int GetNumSegments(float TotalLength) const {return  FMath::CeilToInt(TotalLength / SegmentWidth);}
    float GetCenterOffset(float TotalLength) const
    {
        const float RealLength = GetNumSegments(TotalLength) * SegmentWidth;
        return 0.5f * RealLength + GetHalfSegmentWidth();
    }
    TArray<FVector> GetSegmentCenters(float TotalLength, FVector Center=FVector::ZeroVector) const
    {
        TArray<FVector> Out;
        const FVector Normal = FVector::ForwardVector;
        const FVector VC = Center - Normal * GetCenterOffset(TotalLength);
        
        for (int i = 0; i < GetNumSegments(TotalLength); ++i)
        {
            const float OffsetDistance = i * SegmentWidth;
            FVector V = VC + Normal * OffsetDistance;
            Out.Add(V);
        }
        
        return Out;
    }
    
public:
    bool HasMaterialNum(int N) const {return LineMaterials.Num() > N;}
    UMaterial* GetFirstMaterial(){return HasMaterialNum(1) ? LineMaterials[0] : nullptr;}
    UMaterial* GetSecondMaterial(){return HasMaterialNum(1) ? LineMaterials[1] : nullptr;}
    UMaterial* GetMaterialForPoint(int Index)
    {
        const bool bOdd = (Index % 2) != 0;
        const auto First = GetFirstMaterial();
        const auto Second = GetSecondMaterial();
        if(First && !Second) return First;
        return bOdd ? First : Second;
    }
};
