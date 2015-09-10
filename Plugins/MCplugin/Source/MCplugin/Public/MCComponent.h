#pragma once
#include "ProceduralMeshComponent.h"
#include "MCComponent.generated.h"

UCLASS(hidecategories = (Object, LOD), meta = (BlueprintSpawnableComponent),  ClassGroup = Custom)
class MCPLUGIN_API UMCComponent :public UProceduralMeshComponent
{
	GENERATED_BODY()
public:
	UMCComponent();
};
