// Fill out your copyright notice in the Description page of Project Settings.

#include "Field/CompositeMatchField.h"

constexpr auto SplineLocalSpace = ESplineCoordinateSpace::Type::Local;
constexpr auto SplineWorldSpace = ESplineCoordinateSpace::Type::World;

ACompositeMatchField::ACompositeMatchField()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	this->SetRootComponent(Root);
	
	InitField();
	InitMarkup();

	for (const auto Spline : GetMarkupSplines())
	{
		Spline->SetupAttachment(Markup);
	}
}

void ACompositeMatchField::InitMarkup()
{
	Markup = CreateDefaultSubobject<USceneComponent>(TEXT("MARKUP"));
	Markup->SetupAttachment(Root);
	
	FieldBoxSplineLongTop = CreateDefaultSubobject<USplineComponent>(TEXT("Field Box Long Top Spline"));
	FieldBoxSplineLongDown = CreateDefaultSubobject<USplineComponent>(TEXT("Field Box Long Down Spline"));
	FieldBoxSplineShortLeft = CreateDefaultSubobject<USplineComponent>(TEXT("Field Box Short Left Spline"));
	FieldBoxSplineShortRight = CreateDefaultSubobject<USplineComponent>(TEXT("Field Box Short Right Spline"));

	LeftPenaltyAreaLongRight = CreateDefaultSubobject<USplineComponent>(TEXT("Left Penalty Area Long Right Spline"));
	LeftPenaltyAreaShortTop = CreateDefaultSubobject<USplineComponent>(TEXT("Left Penalty Area Short Top Spline"));
	LeftPenaltyAreaShortDown = CreateDefaultSubobject<USplineComponent>(TEXT("Left Penalty Area Short Down Spline"));
	LeftPenaltyCircle = CreateDefaultSubobject<USplineComponent>(TEXT("Left Penalty Circle Spline"));
	LeftNearGatesAreaLongRight = CreateDefaultSubobject<USplineComponent>(TEXT("Left Near Gates Area Long Right Spline"));
	LeftNearGatesAreaShortTop = CreateDefaultSubobject<USplineComponent>(TEXT("Left Near Gates Area Short Top Spline"));
	LeftNearGatesAreaShortDown = CreateDefaultSubobject<USplineComponent>(TEXT("Left Near Gates Area Short Down Spline"));

	RightPenaltyAreaLongLeft = CreateDefaultSubobject<USplineComponent>(TEXT("Right Penalty Area Long Left Spline"));
	RightPenaltyAreaShortTop = CreateDefaultSubobject<USplineComponent>(TEXT("Right Penalty Area Short Top Spline"));
	RightPenaltyAreaShortDown = CreateDefaultSubobject<USplineComponent>(TEXT("Right Penalty Area Short Down Spline"));
	RightPenaltyCircle = CreateDefaultSubobject<USplineComponent>(TEXT("Right Penalty Circle Spline"));
	RightNearGatesAreaLongLeft = CreateDefaultSubobject<USplineComponent>(TEXT("Right Near Gates Area Long Left Spline"));
	RightNearGatesAreaShortTop = CreateDefaultSubobject<USplineComponent>(TEXT("Right Near Gates Area Short Top Spline"));
	RightNearGatesAreaShortDown = CreateDefaultSubobject<USplineComponent>(TEXT("Right Near Gates Area Short Down Spline"));

	CentralLineSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Central Line Spline"));
	CentralCircleSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Central Circle Spline"));

	CornerCircleTopLeft = CreateDefaultSubobject<USplineComponent>(TEXT("Corner Circle Top Left Spline"));
	CornerCircleTopRight = CreateDefaultSubobject<USplineComponent>(TEXT("Corner Circle Top Right Spline"));
	CornerCircleDownLeft = CreateDefaultSubobject<USplineComponent>(TEXT("Corner Circle Down Left Spline"));
	CornerCircleDownRight = CreateDefaultSubobject<USplineComponent>(TEXT("Corner Circle Down Right Spline"));

	CenterBallSpot = CreateDefaultSubobject<USplineComponent>(TEXT("Center Ball Spot Spline"));
	LeftPenaltyBallSpot = CreateDefaultSubobject<USplineComponent>(TEXT("Left Penalty Ball Spot Spline"));
	RightPenaltyBallSpot = CreateDefaultSubobject<USplineComponent>(TEXT("Right Penalty Ball Spot Spline"));
}

void ACompositeMatchField::InitField()
{
	Field = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FIELD"));
	Field->SetupAttachment(Root);
	
	const auto CubeMesh = UUtilsLib::GetCubeMesh();
	Field->SetStaticMesh(CubeMesh);

	FieldGroundMeshSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Field Ground Mesh Spline"));
	FieldGroundMeshSpline->SetupAttachment(Root);
}

void ACompositeMatchField::BeginPlay()
{
	Super::BeginPlay();
}
#if WITH_EDITOR

void ACompositeMatchField::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	RecalculateAllSplines();
	ScaleField();
}

#endif

FVector ACompositeMatchField::CalculateFieldGroundScale() const
{
	return FieldScale.GetScale(MarkupData.FieldWidthShort, MarkupData.FieldWidthLong);
}

FVector ACompositeMatchField::CalculateFieldGroundSizeInCm() const
{
	return CalculateFieldGroundScale() * 100.0f;
}

void ACompositeMatchField::ScaleField() const
{
	if(!Field) return;
	Field->SetWorldScale3D(CalculateFieldGroundScale());
}

void ACompositeMatchField::RecalculateAllSplines()
{
	RecalculateFieldMeshSpline();

	// Markup
	
	RecalculateFieldBoxSpline();
	RecalculatePenaltyAreaSpline();
	RecalculateCenter();
	RecalculateCorners();
	RecalculateBallSpots();

	for (const auto S : GetAllSplines()) AddSplinePointsOffset(S);
}

void ACompositeMatchField::RecalculateFieldMeshSpline() const
{
	if(bDrawGroundMesh)
	{
		const float FieldLength = CalculateFieldGroundSizeInCm().X;
		RecalculateSpline(FieldGroundMeshSpline, FieldMeshParams.GetSegmentCenters(FieldLength));
	}
	else
	{
		ClearSplinePoints(FieldGroundMeshSpline);
	}
}

void ACompositeMatchField::RecalculateFieldBoxSpline() const
{
	auto Box = MarkupData.GetFieldBoxRect();
	RecalculateSpline(FieldBoxSplineLongDown, Box.GetTopLinePoints());
	RecalculateSpline(FieldBoxSplineLongTop, Box.GetDownLinePoints());
	RecalculateSpline(FieldBoxSplineShortLeft, Box.GetLeftLinePoints());
	RecalculateSpline(FieldBoxSplineShortRight, Box.GetRightLinePoints());
}

void ACompositeMatchField::RecalculatePenaltyAreaSpline() const
{
	auto LBox = MarkupData.GetLeftPenaltyAreaRect();
	RecalculateSpline(LeftPenaltyAreaShortDown, LBox.GetTopLinePoints());
	RecalculateSpline(LeftPenaltyAreaShortTop, LBox.GetDownLinePoints());
	RecalculateSpline(LeftPenaltyAreaLongRight, LBox.GetRightLinePoints());
	RecalculateSpline(LeftPenaltyCircle, MarkupData.GetLeftPenaltyCircleSegmentPath());

	auto LBox2 = MarkupData.GetLeftNearGatesAreaRect();
	RecalculateSpline(LeftNearGatesAreaShortDown, LBox2.GetTopLinePoints());
	RecalculateSpline(LeftNearGatesAreaShortTop, LBox2.GetDownLinePoints());
	RecalculateSpline(LeftNearGatesAreaLongRight, LBox2.GetRightLinePoints());
	
	auto RBox = MarkupData.GetRightPenaltyAreaRect();
	RecalculateSpline(RightPenaltyAreaShortDown, RBox.GetTopLinePoints());
	RecalculateSpline(RightPenaltyAreaShortTop, RBox.GetDownLinePoints());
	RecalculateSpline(RightPenaltyAreaLongLeft, RBox.GetLeftLinePoints());
	RecalculateSpline(RightPenaltyCircle, MarkupData.GetRightPenaltyCircleSegmentPath());

	auto RBox2 = MarkupData.GetRightNearGatesAreaRect();
	RecalculateSpline(RightNearGatesAreaShortDown, RBox2.GetTopLinePoints());
	RecalculateSpline(RightNearGatesAreaShortTop, RBox2.GetDownLinePoints());
	RecalculateSpline(RightNearGatesAreaLongLeft, RBox2.GetLeftLinePoints());
}

void ACompositeMatchField::RecalculateCorners() const
{
	RecalculateSpline(CornerCircleTopLeft, MarkupData.GetLeftTopCornerCircleSegmentPath());
	RecalculateSpline(CornerCircleTopRight, MarkupData.GetRightTopCornerCircleSegmentPath());
	RecalculateSpline(CornerCircleDownLeft, MarkupData.GetLeftDownCornerCircleSegmentPath());
	RecalculateSpline(CornerCircleDownRight, MarkupData.GetRightDownCornerCircleSegmentPath());
}

void ACompositeMatchField::RecalculateBallSpots() const
{
	RecalculateSpline(CenterBallSpot, MarkupData.GetCenterSpotPoints());
	RecalculateSpline(LeftPenaltyBallSpot, MarkupData.GetLeftPenaltySpotPoints());
	RecalculateSpline(RightPenaltyBallSpot, MarkupData.GetRightPenaltySpotPoints());
}

void ACompositeMatchField::AddSplinePointsOffset(USplineComponent* S) const
{
	const FVector V = GetSplinePointOffset(S);
	const int NumPoints = S->GetNumberOfSplinePoints();
	for (int i = 0; i < NumPoints; ++i)
	{
		FVector Location = S->GetLocationAtSplinePoint(i, SplineWorldSpace) + V;
		S->SetLocationAtSplinePoint(i, Location, SplineWorldSpace, true);
	}
	S->UpdateSpline();
}

void ACompositeMatchField::RecalculateCenter() const
{
	RecalculateSpline(CentralLineSpline, MarkupData.GetCentralLinePoints());
	RecalculateSpline(CentralCircleSpline, MarkupData.GetCentralCirclePoints());
}

void ACompositeMatchField::RecalculateSpline(USplineComponent* S, TArray<FVector> Points)
{
	if(!S) return;
	S->ClearSplinePoints();
	S->SetSplinePoints(Points, SplineLocalSpace);
}

void ACompositeMatchField::ClearSplinePoints(USplineComponent* S)
{
	if(!S) return;
	S->ClearSplinePoints();
}

void ACompositeMatchField::SetupSplineMeshComponent(const USplineComponent* Spline, USplineMeshComponent* Mesh, int PointIndex)
{
	const bool bMarkupSpline = IsMarkupSpline(Spline);
	const bool bFieldMeshSpline = IsFieldMeshSpline(Spline);
	const int NextPoint = PointIndex + 1;
	if(Mesh)
	{
		const auto CubeMesh = UUtilsLib::GetCubeMesh();
		const FVector Start = Spline->GetLocationAtSplinePoint(PointIndex, SplineLocalSpace);
		const FVector StartTangent = Spline->GetTangentAtSplinePoint(PointIndex, SplineLocalSpace);
		const FVector End = Spline->GetLocationAtSplinePoint(NextPoint, SplineLocalSpace);
		const FVector EndTangent = Spline->GetTangentAtSplinePoint(NextPoint, SplineLocalSpace);

		Mesh->SetStaticMesh(CubeMesh);
		Mesh->SetStartAndEnd(Start, StartTangent, End, EndTangent);
		Mesh->SetMobility(EComponentMobility::Movable);
		Mesh->bCastDynamicShadow = false;
		
		FVector2D Scale;
		UMaterial* Material = nullptr;
		
		if(bMarkupSpline)
		{
			Scale = GetMarkupLineMeshScale();
			Material = MarkupMaterial;
		}
		else if(bFieldMeshSpline)
		{
			Scale = GetFieldMeshSegmentScale();
			Material = FieldMeshParams.GetMaterialForPoint(PointIndex);
		}
		
		if(Material) Mesh->SetMaterial(0, Material);
		Mesh->SetStartScale(Scale);
		Mesh->SetEndScale(Scale);
	}
}

FVector ACompositeMatchField::GetSplinePointOffset(USplineComponent* Spline) const
{
	const TArray<USplineComponent*> MoveDownSplines = {
		FieldBoxSplineLongTop,
		LeftPenaltyAreaShortTop,
		LeftNearGatesAreaShortTop,
		RightPenaltyAreaShortTop,
		RightNearGatesAreaShortTop
	};

	const TArray<USplineComponent*> MoveUpSplines = {
		FieldBoxSplineLongDown,
		LeftPenaltyAreaShortDown,
		LeftNearGatesAreaShortDown,
		RightPenaltyAreaShortDown,
		RightNearGatesAreaShortDown
	};

	const TArray<USplineComponent*> MoveRightSplines = {
		FieldBoxSplineShortLeft,
		RightPenaltyAreaLongLeft,
		RightNearGatesAreaLongLeft
	};

	const TArray<USplineComponent*> MoveLeftSplines = {
		FieldBoxSplineShortRight,
		LeftPenaltyAreaLongRight,
		LeftNearGatesAreaLongRight,
	};

	const float Distance = MarkupData.HalfLineWidth();
	FVector V = FVector::ZeroVector;
	if(MoveUpSplines.Contains(Spline)) V = -FVector::RightVector;
	if(MoveDownSplines.Contains(Spline)) V = FVector::RightVector;
	if(MoveRightSplines.Contains(Spline)) V = FVector::ForwardVector;
	if(MoveLeftSplines.Contains(Spline)) V = -FVector::ForwardVector;

	return Distance * V;
}

TArray<USplineComponent*> ACompositeMatchField::GetAllSplines()
{
	TArray<USplineComponent*> Out = GetMarkupSplines();
	Out.Add(FieldGroundMeshSpline);
	return Out;
}

TArray<USplineComponent*> ACompositeMatchField::GetMarkupSplines()
{
	return
	{
		FieldBoxSplineLongTop, FieldBoxSplineLongDown, FieldBoxSplineShortLeft, FieldBoxSplineShortRight,
		LeftPenaltyAreaLongRight, LeftPenaltyAreaShortTop,LeftPenaltyAreaShortDown, LeftPenaltyCircle,
		LeftNearGatesAreaLongRight, LeftNearGatesAreaShortTop, LeftNearGatesAreaShortDown,
		RightPenaltyAreaLongLeft, RightPenaltyAreaShortTop, RightPenaltyAreaShortDown, RightPenaltyCircle,
		RightNearGatesAreaLongLeft, RightNearGatesAreaShortTop, RightNearGatesAreaShortDown,
		CentralLineSpline, CentralCircleSpline,
		CornerCircleTopLeft, CornerCircleTopRight, CornerCircleDownLeft, CornerCircleDownRight,
		CenterBallSpot, LeftPenaltyBallSpot, RightPenaltyBallSpot,
	};
}

bool ACompositeMatchField::IsMarkupSpline(const USplineComponent* S)
{
	return GetMarkupSplines().Contains(S);
}

bool ACompositeMatchField::IsFieldMeshSpline(const USplineComponent* S) const
{
	return S == FieldGroundMeshSpline;
}

FVector2D ACompositeMatchField::GetMarkupLineMeshScale() const
{
	const float LineThickness = 0.01f * MarkupData.LineThickness;
	return FVector2D(LineThickness, MarkupMeshHeight);
}

FVector2D ACompositeMatchField::GetFieldMeshSegmentScale() const
{
	const float Width =  CalculateFieldGroundScale().X;
	return FVector2D(Width, FieldMeshParams.Thickness);
}

