#pragma once
#include "ProceduralMeshComponent.h"
#include "SVO.h"
#include "MCComponent.generated.h"

UCLASS(hidecategories = (Object, LOD), meta = (BlueprintSpawnableComponent),  ClassGroup = Custom)
class MCPLUGIN_API UMCComponent :public UProceduralMeshComponent
{
	GENERATED_BODY()
public:
	UMCComponent();

	UFUNCTION(BlueprintCallable, Category = "MCPlugin")
		 void AddVoxel(FVector Point);

	UFUNCTION(BlueprintCallable, Category = "MCPlugin")
		void UpdateMesh();

	virtual void BeginPlay() override;
private:
	TSharedPtr<FSVOTree> SVOTree;
};
