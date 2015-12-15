#pragma once

#include "Classes/Components/LineBatchComponent.h"
#include "NFLineBatchComponent.generated.h"

/** class UNFEllipseDrawerComponent
*/
UCLASS(Blueprintable, ClassGroup = NFGalaxy, editinlinenew, meta = (BlueprintSpawnableComponent), hideCategories = (Mobility), MinimalAPI)
class UNFEllipseDrawerComponent
	: public UMeshComponent
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FVector XDir;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FVector YDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float SemiMajor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float SemiMinor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	int32 NumSides;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	uint8 DepthPriority;


	/** Buffer of lines to draw */
	TArray<struct FBatchedLine> BatchedLines;
	/** Buffer or points to draw */
	TArray<struct FBatchedPoint> BatchedPoints;
	/** Default time that lines/points will draw for */
	float DefaultLifeTime;
	/** Buffer of simple meshes to draw */
	TArray<struct FBatchedMesh> BatchedMeshes;

public:

	UFUNCTION(BlueprintCallable, Category = "NFGalaxy")
	void Update();
	// Begin UPrimitiveComponent interface.
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	// End UPrimitiveComponent interface.
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

};