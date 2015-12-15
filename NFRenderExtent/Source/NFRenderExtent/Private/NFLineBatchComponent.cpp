#include "NFRenderExtentPrivatePCH.h"
#include "NFLineBatchComponent.h"
#include "PrimitiveSceneProxy.h"
#include "DynamicMeshBuilder.h"



/** Vertex Buffer */
class FCustomMeshVertexBuffer : public FVertexBuffer
{
public:
	TArray<FDynamicMeshVertex> Vertices;

	virtual void InitRHI() override
	{
		FRHIResourceCreateInfo CreateInfo;
		VertexBufferRHI = RHICreateVertexBuffer(Vertices.Num() * sizeof(FDynamicMeshVertex), BUF_Static, CreateInfo);

		// Copy the vertex data into the vertex buffer.
		void* VertexBufferData = RHILockVertexBuffer(VertexBufferRHI, 0, Vertices.Num() * sizeof(FDynamicMeshVertex), RLM_WriteOnly);
		FMemory::Memcpy(VertexBufferData, Vertices.GetData(), Vertices.Num() * sizeof(FDynamicMeshVertex));
		RHIUnlockVertexBuffer(VertexBufferRHI);
	}

};

/** Index Buffer */
class FCustomMeshIndexBuffer : public FIndexBuffer
{
public:
	TArray<int32> Indices;

	virtual void InitRHI() override
	{
		FRHIResourceCreateInfo CreateInfo;
		IndexBufferRHI = RHICreateIndexBuffer(sizeof(int32), Indices.Num() * sizeof(int32), BUF_Static, CreateInfo);

		// Write the indices to the index buffer.
		void* Buffer = RHILockIndexBuffer(IndexBufferRHI, 0, Indices.Num() * sizeof(int32), RLM_WriteOnly);
		FMemory::Memcpy(Buffer, Indices.GetData(), Indices.Num() * sizeof(int32));
		RHIUnlockIndexBuffer(IndexBufferRHI);
	}
};

/** Vertex Factory */
class FCustomMeshVertexFactory : public FLocalVertexFactory
{
public:

	FCustomMeshVertexFactory()
	{}

	/** Init function that should only be called on render thread. */
	void Init_RenderThread(const FCustomMeshVertexBuffer* VertexBuffer)
	{
		check(IsInRenderingThread());

		DataType NewData;
		NewData.PositionComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, Position, VET_Float3);
		NewData.TextureCoordinates.Add(
			FVertexStreamComponent(VertexBuffer, STRUCT_OFFSET(FDynamicMeshVertex, TextureCoordinate), sizeof(FDynamicMeshVertex), VET_Float2)
			);
		NewData.TangentBasisComponents[0] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, TangentX, VET_PackedNormal);
		NewData.TangentBasisComponents[1] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, TangentZ, VET_PackedNormal);
		NewData.ColorComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, Color, VET_Color);

		SetData(NewData);
	}

	/** Initialization */
	void Init(const FCustomMeshVertexBuffer* VertexBuffer)
	{
		if (IsInRenderingThread())
		{
			Init_RenderThread(VertexBuffer);
		}
		else
		{
			ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
				InitCustomMeshVertexFactory,
				FCustomMeshVertexFactory*, VertexFactory, this,
				const FCustomMeshVertexBuffer*, VertexBuffer, VertexBuffer,
				{
					VertexFactory->Init_RenderThread(VertexBuffer);
				});
		}
	}
};

/** Represents a LineBatchComponent to the scene manager. */
class FLineBatcherSceneProxy : public FPrimitiveSceneProxy
{
public:
	FLineBatcherSceneProxy(const UNFEllipseDrawerComponent* InComponent)
		: FPrimitiveSceneProxy(InComponent)
		, MaterialRelevance(InComponent->GetMaterialRelevance(GetScene().GetFeatureLevel()))
	{
		bWillEverBeLit = false;

		for (int32 lineId = 0; lineId < InComponent->BatchedLines.Num(); ++lineId)
		{
			const FBatchedLine& line = InComponent->BatchedLines[lineId];

			FDynamicMeshVertex Vert0;
			Vert0.Position = line.Start;
			Vert0.Color = line.Color.ToFColor(false);
			int32 VIndex = VertexBuffer.Vertices.Add(Vert0);
			IndexBuffer.Indices.Add(VIndex);

			FDynamicMeshVertex Vert1;
			Vert1.Position = line.End;
			Vert1.Color = line.Color.ToFColor(false);
			VIndex = VertexBuffer.Vertices.Add(Vert1);
			IndexBuffer.Indices.Add(VIndex);

		}

		// Init vertex factory
		VertexFactory.Init(&VertexBuffer);

		// Enqueue initialization of render resource
		BeginInitResource(&VertexBuffer);
		BeginInitResource(&IndexBuffer);
		BeginInitResource(&VertexFactory);

		// Grab material
		Material = InComponent->GetMaterial(0);
		if (Material == nullptr)
		{
			Material = UMaterial::GetDefaultMaterial(MD_Surface);
		}
	}

	virtual ~FLineBatcherSceneProxy()
	{
		VertexBuffer.ReleaseResource();
		IndexBuffer.ReleaseResource();
		VertexFactory.ReleaseResource();
	}

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_CustomMeshSceneProxy_GetDynamicMeshElements);

		const bool bWireframe = AllowDebugViewmodes() && ViewFamily.EngineShowFlags.Wireframe;

		auto WireframeMaterialInstance = new FColoredMaterialRenderProxy(
			GEngine->WireframeMaterial ? GEngine->WireframeMaterial->GetRenderProxy(IsSelected()) : NULL,
			FLinearColor(0, 0.5f, 1.f)
			);

		Collector.RegisterOneFrameMaterialProxy(WireframeMaterialInstance);

		FMaterialRenderProxy* MaterialProxy = NULL;
		if (bWireframe)
		{
			MaterialProxy = WireframeMaterialInstance;
		}
		else
		{
			MaterialProxy = Material->GetRenderProxy(IsSelected());
		}

		for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
		{
			if (VisibilityMap & (1 << ViewIndex))
			{
				const FSceneView* View = Views[ViewIndex];
				// Draw the mesh.
				FMeshBatch& Mesh = Collector.AllocateMesh();
				FMeshBatchElement& BatchElement = Mesh.Elements[0];
				BatchElement.IndexBuffer = &IndexBuffer;
				Mesh.bWireframe = bWireframe;
				Mesh.VertexFactory = &VertexFactory;
				Mesh.MaterialRenderProxy = MaterialProxy;
				BatchElement.PrimitiveUniformBuffer = CreatePrimitiveUniformBufferImmediate(GetLocalToWorld(), GetBounds(), GetLocalBounds(), true, UseEditorDepthTest());
				BatchElement.FirstIndex = 0;
				BatchElement.NumPrimitives = IndexBuffer.Indices.Num() / 2;
				BatchElement.MinVertexIndex = 0;
				BatchElement.MaxVertexIndex = VertexBuffer.Vertices.Num() - 1;
				Mesh.ReverseCulling = IsLocalToWorldDeterminantNegative();
				Mesh.Type = PT_LineList;
				Mesh.DepthPriorityGroup = SDPG_World;
				Mesh.bCanApplyViewModeOverrides = false;
				Collector.AddMesh(ViewIndex, Mesh);
			}
		}
	}


	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) override
	{
		FPrimitiveViewRelevance Result;
		Result.bDrawRelevance = IsShown(View);
		Result.bShadowRelevance = false;
		Result.bDynamicRelevance = true;
		MaterialRelevance.SetPrimitiveViewRelevance(Result);
		return Result;
	}

	virtual bool CanBeOccluded() const override
	{
		return !MaterialRelevance.bDisableDepthTest;
	}

	virtual uint32 GetMemoryFootprint(void) const override { return(sizeof(*this) + GetAllocatedSize()); }

	uint32 GetAllocatedSize(void) const { return(FPrimitiveSceneProxy::GetAllocatedSize()); }


private:

	UMaterialInterface* Material;
	FCustomMeshVertexBuffer VertexBuffer;
	FCustomMeshIndexBuffer IndexBuffer;
	FCustomMeshVertexFactory VertexFactory;

	FMaterialRelevance MaterialRelevance;

};
//------------------------------------------------------------------------
UNFEllipseDrawerComponent::UNFEllipseDrawerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoActivate = true;
	bTickInEditor = true;
	PrimaryComponentTick.bCanEverTick = true;

	SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	bUseEditorCompositing = true;
	bGenerateOverlapEvents = false;
	DefaultLifeTime = -1;
}

//------------------------------------------------------------------------
FPrimitiveSceneProxy* UNFEllipseDrawerComponent::CreateSceneProxy()
{
	if (BatchedLines.Num() > 0)
	{
		return new FLineBatcherSceneProxy(this);
	}
	return nullptr;
}

//------------------------------------------------------------------------
FBoxSphereBounds UNFEllipseDrawerComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	const FVector BoxExtent(HALF_WORLD_MAX);
	return FBoxSphereBounds(FVector::ZeroVector, BoxExtent, BoxExtent.Size());
}

//------------------------------------------------------------------------
void UNFEllipseDrawerComponent::Update()
{
	if (BatchedLines.Num() > 0 || BatchedPoints.Num() > 0 || BatchedMeshes.Num() > 0)
	{
		BatchedLines.Empty();
		BatchedPoints.Empty();
		BatchedMeshes.Empty();
	}

	const float	AngleDelta = 2.0f * PI / NumSides;
	FVector	LastVertex = XDir * SemiMajor;

	for (int32 SideIndex = 0; SideIndex < NumSides; SideIndex++)
	{
		const FVector Vertex = 
			YDir * SemiMinor * FMath::Sin(AngleDelta * (SideIndex + 1))
			+ XDir * SemiMajor * FMath::Cos(AngleDelta * (SideIndex + 1));
		new(BatchedLines) FBatchedLine(LastVertex, Vertex, Color, -1.0f, 0.0f, DepthPriority);
		LastVertex = Vertex;
	}

	MarkRenderStateDirty();
}

//------------------------------------------------------------------------
void UNFEllipseDrawerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}



//------------------------------------------------------------------------
