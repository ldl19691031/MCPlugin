#include "MCpluginPrivatePCH.h"
#include "MCComponent.h"
#include "Tables.h"

UMCComponent::UMCComponent()
	:Super()
{
	SVOTree = MakeShareable(new FSVOTree());	
}
void UMCComponent::AddVoxel(FVector Point){
	SVOTree->AddAVoxel(Point);
}
void UMCComponent::UpdateMesh(){

}
void UMCComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner() != nullptr)
		SVOTree->World = GetOwner()->GetWorld();
}