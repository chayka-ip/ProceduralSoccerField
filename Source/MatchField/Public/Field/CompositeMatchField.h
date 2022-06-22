// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FieldMeshParams.h"
#include "FieldScale.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "GameFramework/Actor.h"
#include "VisualMarkup/MatchFieldMarkup.h"
#include "CompositeMatchField.generated.h"

UCLASS()
class MATCHFIELD_API ACompositeMatchField : public AActor
{
	GENERATED_BODY()
	
public:
	ACompositeMatchField();
	void InitMarkup();
	void InitField();
		
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent* Markup;

	// used to provide ground collision
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent* Field;

	// FIELD MESH
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category=GroundMesh)
	USplineComponent* FieldGroundMeshSpline;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=GroundMesh)
	bool bDrawGroundMesh = true;

	// FIELD BOX
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* FieldBoxSplineLongTop;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* FieldBoxSplineLongDown;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* FieldBoxSplineShortLeft;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* FieldBoxSplineShortRight;

	// LEFT PENALTY AREA

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* LeftPenaltyAreaLongRight;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* LeftPenaltyAreaShortTop;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* LeftPenaltyAreaShortDown;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* LeftPenaltyCircle;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* LeftNearGatesAreaLongRight;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* LeftNearGatesAreaShortTop;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* LeftNearGatesAreaShortDown;

	// RIGHT PENALTY AREA

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* RightPenaltyAreaLongLeft;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* RightPenaltyAreaShortTop;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* RightPenaltyAreaShortDown;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* RightPenaltyCircle;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* RightNearGatesAreaLongLeft;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* RightNearGatesAreaShortTop;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* RightNearGatesAreaShortDown;

	// CENTER
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* CentralLineSpline;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* CentralCircleSpline;

	// CORNERS

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* CornerCircleTopLeft;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* CornerCircleTopRight;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* CornerCircleDownLeft;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* CornerCircleDownRight;

	// BALL SPOTS
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* CenterBallSpot;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* LeftPenaltyBallSpot;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USplineComponent* RightPenaltyBallSpot;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Settings)
	FMatchFieldMarkup MarkupData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Settings)
	float MarkupMeshHeight = 0.005f;
	
	UPROPERTY(EditAnywhere, Category=Settings)
	UMaterial* MarkupMaterial;
	
	UPROPERTY(EditAnywhere, Category=Settings)
	FFieldScale FieldScale;

	UPROPERTY(EditAnywhere, Category=Settings)
	FFieldMeshParams FieldMeshParams;
	
protected:
	virtual void BeginPlay() override;

public:
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
#endif
	
public:
	FVector CalculateFieldGroundScale() const;
	FVector CalculateFieldGroundSizeInCm() const;
	void ScaleField() const;
	
	static void RecalculateSpline(USplineComponent* S, TArray<FVector> Points);
	static void ClearSplinePoints(USplineComponent* S);
	void RecalculateAllSplines();
	void RecalculateFieldMeshSpline() const;
	void RecalculateCenter() const;
	void RecalculateFieldBoxSpline() const;
	void RecalculatePenaltyAreaSpline() const;
	void RecalculateCorners() const;
	void RecalculateBallSpots() const;
	void AddSplinePointsOffset(USplineComponent* S) const;
	
	UFUNCTION(BlueprintCallable)
	void SetupSplineMeshComponent(const USplineComponent* Spline, USplineMeshComponent* Mesh, int PointIndex);

	FVector GetSplinePointOffset(USplineComponent* Spline) const;
	
public:
	UFUNCTION(BlueprintPure)
	TArray<USplineComponent*> GetAllSplines();
	TArray<USplineComponent*> GetMarkupSplines();
	bool IsMarkupSpline(const USplineComponent* S);
	bool IsFieldMeshSpline(const USplineComponent* S) const;
	
	UFUNCTION(BlueprintPure)
	FVector2D GetMarkupLineMeshScale() const;
	FVector2D GetFieldMeshSegmentScale() const;

};
