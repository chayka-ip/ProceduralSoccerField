#pragma once

#include "CoreMinimal.h"
#include "MarkupLine.h"
#include "MarkupRectangle.h"
#include "Kismet/KismetMathLibrary.h"
#include "MatchField/Public/Utils.h"

#include "MatchFieldMarkup.generated.h"

USTRUCT(BlueprintType)
struct FMatchFieldMarkup
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FVector Center = FVector(0, 0, 3);

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FLinearColor Color = FLinearColor::White;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float LineThickness = 12.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float FieldWidthLong = 11000.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float FieldWidthShort = 6600.0f;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float CenterCircleRadius = 910.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CentralCircleAngleStep = 5.0f;
	
    // PENALTY AREA
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float PenaltyAreaParallelWidth = 4050.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float PenaltyArealPerpWidth = 1650.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float PenaltyCircleRadius = 915.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float PenaltyMarkPoint = 1100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PenaltyCircleAngleStep = 5.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PenaltyMarkRadius = 10.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PenaltyCircleIntersectAdditionAngle = 0.5f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MarkupCircleAngleStep = 5.0f;
    
    // NEAR GATES AREA

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float NearGatesAreaParallelWidth = 1850.0f;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float NearGatesAreaPerpWidth = 550.0f;

    // CORNERS

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float CornerCircleRadius = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CornerCircleAngleStep = 5.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CornerCircleBaseNumSegments = 360.0f;
	
public:
    
    float GetOffsetZ() const {return  Center.Z;}
	float HalfLineWidth() const {return 0.5f * LineThickness;}
	float HalfFieldWidthLong() const {return 0.5f * FieldWidthLong;}
    float HalfFieldWidthShort() const {return 0.5f * FieldWidthShort;}
    float HalfNearGatesAreaWidthParallel() const {return 0.5f * NearGatesAreaParallelWidth;}
    float HalfPenaltyAreaWidthParallel() const {return 0.5f * PenaltyAreaParallelWidth;}
    float RightPenaltyPointX() const {return Center.X + HalfFieldWidthLong() - PenaltyMarkPoint;}
    float LeftPenaltyPointX() const {return Center.X - HalfFieldWidthLong() + PenaltyMarkPoint;}
	float GetHalfAnglePenaltyCircleIntersect() const
	{
	    const float PerpSize = PenaltyArealPerpWidth - PenaltyMarkPoint;
    	return UKismetMathLibrary::DegAcos(PerpSize/PenaltyCircleRadius) + PenaltyCircleIntersectAdditionAngle;
    }

	FVector GetGoalFrameCenter(bool bRight) const
	{
	    const float S = bRight ? 1.0f : -1.0f;
	    const float DeltaX =  S * (HalfFieldWidthLong() - HalfLineWidth());
	    const float X = Center.X + DeltaX;
		return FVector(X, Center.Y, Center.Z);    	
    }
	FVector LeftGoalFrameCenter() const {return GetGoalFrameCenter(false);}
	FVector RightGoalFrameCenter() const {return GetGoalFrameCenter(true);}
    FVector GetRightPenaltyPoint() const {return FVector(RightPenaltyPointX(), 0, GetOffsetZ());}
    FVector GetLeftPenaltyPoint() const {return FVector(LeftPenaltyPointX(), 0, GetOffsetZ());}
	FVector CentralCircleLocation() const {return FVector(Center.X, Center.Y, GetOffsetZ());}
	FVector FieldCornerLD() const
    {
    	return FVector(Center.X - HalfFieldWidthLong(), Center.Y - HalfFieldWidthShort(), GetOffsetZ());
    }
	FVector FieldCornerLT() const
    {
    	return FVector(Center.X - HalfFieldWidthLong(), Center.Y + HalfFieldWidthShort(), GetOffsetZ());
    }
	FVector FieldCornerRT() const
    {
    	return FVector(Center.X + HalfFieldWidthLong(), Center.Y + HalfFieldWidthShort(), GetOffsetZ());
    }
	FVector FieldCornerRD() const
    {
    	return FVector(Center.X + HalfFieldWidthLong(), Center.Y - HalfFieldWidthShort(), GetOffsetZ());
    }
	
	float RightPenaltyAreaInnerCoordX() const
    {
    	return Center.X + HalfFieldWidthLong() - PenaltyArealPerpWidth;
    }
	float LeftPenaltyAreaInnerCoordX() const
    {
    	return Center.X - HalfFieldWidthLong() + PenaltyArealPerpWidth;
    }

public:
	TArray<FVector> GetPenaltyCircleSegmentPath(bool bRightPenalty) const
	{
		const float Step = PenaltyCircleAngleStep;
		const float Radius = PenaltyCircleRadius;
		const float HalfAngle = GetHalfAnglePenaltyCircleIntersect();
		const float FullAngle = 2.0f * HalfAngle;

		const FVector center = bRightPenalty ? GetRightPenaltyPoint() : GetLeftPenaltyPoint();
		const float VS = bRightPenalty ? -1.0f : 1.0f;
		const FVector Dir = (VS * FVector::ForwardVector).RotateAngleAxis(-HalfAngle, FVector::UpVector);
		
		return  UUtilsLib::CalculatePointsOnRadialBound(center, Dir, FullAngle, Step, Radius);
	}
	
	TArray<FVector> GetCornerCircleSegmentPath(bool bTop, bool bRight) const
	{
		const float Step = CornerCircleAngleStep;
		const float Radius = CornerCircleRadius;
		constexpr float FullAngle = 90.0f;

		FVector center;
		if(bTop) center = bRight ? FieldCornerRT() : FieldCornerLT();
		else center = bRight ? FieldCornerRD() : FieldCornerLD();

		FVector Dir;
		if(bTop) Dir = bRight ? -FVector::ForwardVector : -FVector::RightVector;
		else Dir = bRight ? FVector::RightVector : FVector::ForwardVector;
		return  UUtilsLib::CalculatePointsOnRadialBound(center, Dir, FullAngle, Step, Radius);
	}
	
	TArray<FVector> GetRightPenaltyCircleSegmentPath() const {return GetPenaltyCircleSegmentPath(true);}
	TArray<FVector> GetLeftPenaltyCircleSegmentPath() const {return GetPenaltyCircleSegmentPath(false);}
	TArray<FVector> GetLeftTopCornerCircleSegmentPath() const {return GetCornerCircleSegmentPath(true, false);}
	TArray<FVector> GetLeftDownCornerCircleSegmentPath() const	{return GetCornerCircleSegmentPath(false, false);}
	TArray<FVector> GetRightTopCornerCircleSegmentPath() const {return GetCornerCircleSegmentPath(true, true);}
	TArray<FVector> GetRightDownCornerCircleSegmentPath() const {return GetCornerCircleSegmentPath(false, true);}
	
public:
	FMarkupRectangle GetFieldBoxRect() const
	{
		FMarkupRectangle Rect;
		Rect.LeftDownCorner = FieldCornerLD();
		Rect.LeftTopCorner = FieldCornerLT();
		Rect.RightTopCorner = FieldCornerRT();
		Rect.RightDownCorner = FieldCornerRD();
		return Rect;
	}
	FMarkupRectangle GetRightPenaltyAreaRect() const
	{
		FMarkupRectangle Rect;
		Rect.LeftDownCorner  = FVector(RightPenaltyAreaInnerCoordX(), Center.Y - HalfPenaltyAreaWidthParallel(), GetOffsetZ());
		Rect.LeftTopCorner   = FVector(RightPenaltyAreaInnerCoordX(), Center.Y + HalfPenaltyAreaWidthParallel(), GetOffsetZ());
		Rect.RightTopCorner  = FVector(Center.X + HalfFieldWidthLong(), Center.Y + HalfPenaltyAreaWidthParallel(), GetOffsetZ());
		Rect.RightDownCorner = FVector(Center.X + HalfFieldWidthLong(), Center.Y - HalfPenaltyAreaWidthParallel(), GetOffsetZ());
		return Rect;
	}
	FMarkupRectangle GetLeftPenaltyAreaRect() const
	{
		FMarkupRectangle Rect;
		Rect.LeftDownCorner  = FVector(Center.X - HalfFieldWidthLong(), Center.Y - HalfPenaltyAreaWidthParallel(), GetOffsetZ());
		Rect.LeftTopCorner   = FVector(Center.X - HalfFieldWidthLong(), Center.Y + HalfPenaltyAreaWidthParallel(), GetOffsetZ());
		Rect.RightTopCorner  = FVector(LeftPenaltyAreaInnerCoordX(), Center.Y + HalfPenaltyAreaWidthParallel(), GetOffsetZ());
		Rect.RightDownCorner = FVector(LeftPenaltyAreaInnerCoordX(), Center.Y - HalfPenaltyAreaWidthParallel(), GetOffsetZ());
		return Rect;
	}
	FMarkupRectangle GetRightNearGatesAreaRect() const
	{
		FMarkupRectangle Rect;
		Rect.LeftDownCorner  = FVector(Center.X + HalfFieldWidthLong() - NearGatesAreaPerpWidth, Center.Y - HalfNearGatesAreaWidthParallel(), GetOffsetZ());
		Rect.LeftTopCorner   = FVector(Center.X + HalfFieldWidthLong() - NearGatesAreaPerpWidth, Center.Y + HalfNearGatesAreaWidthParallel(), GetOffsetZ());
		Rect.RightTopCorner  = FVector(Center.X + HalfFieldWidthLong(), Center.Y + HalfNearGatesAreaWidthParallel(), GetOffsetZ());
		Rect.RightDownCorner = FVector(Center.X + HalfFieldWidthLong(), Center.Y - HalfNearGatesAreaWidthParallel(), GetOffsetZ());
		return Rect;
	}
	FMarkupRectangle GetLeftNearGatesAreaRect() const
	{
		FMarkupRectangle Rect;
		Rect.LeftDownCorner  = FVector(Center.X - HalfFieldWidthLong(), Center.Y - HalfNearGatesAreaWidthParallel(), GetOffsetZ());
		Rect.LeftTopCorner   = FVector(Center.X - HalfFieldWidthLong(), Center.Y + HalfNearGatesAreaWidthParallel(), GetOffsetZ());
		Rect.RightTopCorner  = FVector(Center.X - HalfFieldWidthLong() + NearGatesAreaPerpWidth, Center.Y + HalfNearGatesAreaWidthParallel(), GetOffsetZ());
		Rect.RightDownCorner = FVector(Center.X - HalfFieldWidthLong() + NearGatesAreaPerpWidth, Center.Y - HalfNearGatesAreaWidthParallel(), GetOffsetZ());
		return Rect;
	}
	
	FMarkupLine CentralLine() const
	{
		FMarkupLine Line;

		Line.Start = FVector(Center.X, Center.Y - HalfFieldWidthShort(), GetOffsetZ());
		Line.End = FVector(Center.X, Center.Y + HalfFieldWidthShort(), GetOffsetZ());
		Line.Thickness = LineThickness;
		Line.Color = Color;
		
		return Line;
	}
	TArray<FMarkupLine> GetFieldBoxLines() const
	{
		return GetFieldBoxRect().GetLines(Color, LineThickness);
	}
	TArray<FMarkupLine> GetRightPenaltyAreaLines() const
	{
		return GetRightPenaltyAreaRect().GetLines(Color, LineThickness);
	}
	TArray<FMarkupLine> GetLeftPenaltyAreaLines() const
	{
		return GetLeftPenaltyAreaRect().GetLines(Color, LineThickness);
	}
	TArray<FMarkupLine> GetRightNearGatesAreaLines() const
	{
		return GetRightNearGatesAreaRect().GetLines(Color, LineThickness);
	}
	TArray<FMarkupLine> GetLeftNearGatesAreaLines() const
	{
		return GetLeftNearGatesAreaRect().GetLines(Color, LineThickness);
	}
	TArray<FMarkupLine> GetAllLines() const
	{
		TArray<FMarkupLine> A;
		A.Add(CentralLine());
		A.Append(GetFieldBoxLines());
		A.Append(GetRightPenaltyAreaLines());
		A.Append(GetLeftPenaltyAreaLines());
		A.Append(GetRightNearGatesAreaLines());
		A.Append(GetLeftNearGatesAreaLines());
		return A;
	}

	TArray<FVector> GetCentralCirclePoints() const
	{
		return UUtilsLib::CalculateCirclePoints(Center, FVector::ForwardVector, CentralCircleAngleStep, CenterCircleRadius);
	}
	TArray<FVector> GetCentralLinePoints() const
	{
		auto C = CentralLine();
		return {C.Start, C.End};
	}
	TArray<FVector> GetFieldBoxPoints() const
	{
		return GetFieldBoxRect().GetPoints();
	}
	TArray<FVector> GetRightPenaltyAreaPoints() const
	{
		return GetRightPenaltyAreaRect().GetPoints();
	}
	TArray<FVector> GetLeftPenaltyAreaPoints() const
	{
		return GetLeftPenaltyAreaRect().GetPoints();
	}
	TArray<FVector> GetRightNearGatesAreaPoints() const
	{
		return GetRightNearGatesAreaRect().GetPoints();
	}
	TArray<FVector> GetLeftNearGatesAreaPoints() const
	{
		return GetLeftNearGatesAreaRect().GetPoints();
	}

	TArray<FVector> GetFieldBoxRenderPath() const
	{
		auto P = GetFieldBoxPoints();
		const FVector V = P[0];
		P.Add(V);
		return P;
	}

	TArray<FVector> GetBallSpotPoints(FVector center) const
	{
		auto Points =  UUtilsLib::CalculateCirclePoints(center, FVector::ForwardVector, MarkupCircleAngleStep, PenaltyMarkRadius);
		const int N = Points.Num();
		if(N > 10)
		{
			const int i = Points.Num() / 2;
			const FVector V = Points[i];
			Points.Add(V);
		}
		return Points;
	}
	TArray<FVector> GetCenterSpotPoints() const {return GetBallSpotPoints(Center);}
	TArray<FVector> GetLeftPenaltySpotPoints() const {return GetBallSpotPoints(GetLeftPenaltyPoint());}
	TArray<FVector> GetRightPenaltySpotPoints() const {return GetBallSpotPoints(GetRightPenaltyPoint());}
};