#include "MCpluginPrivatePCH.h"
#include "MCComponent.h"
#include "Tables.h"
#include "DrawDebugHelpers.h"
UMCComponent::UMCComponent()
	:Super()
{
	SVOTree = MakeShareable(new FSVOTree());	
}
void UMCComponent::AddVoxel(FVector Point){
	SVOTree->AddAVoxel(Point);
	UpdateMesh();
}
void UMCComponent::UpdateMesh(){
	for (auto Cell : SVOTree->CellsList)
	{
		DrawDebugPoint(GetOwner()->GetWorld(), Cell->Center, 10, FColor::White, true, 120.0f);
		DrawDebugBox(GetOwner()->GetWorld(), Cell->Center, FVector(Cell->Size), FColor::Black, true);
		int count = 0;
		unsigned int flag = 1;
		//通过移位判断哪一位为1，然后找出对应的边
		while (flag)
		{
			if (EdgeMagicTable[Cell->CellType] & flag)
			{
				//因为数组下标比count要少1，所以先计算后增加count
				DrawDebugPoint(GetOwner()->GetWorld(), Cell->Center + EdgeDeltaList[count] * Cell->Size, 10, FColor::Yellow, true, 120.0f);
				GEngine->AddOnScreenDebugMessage(0, 1.0, FColor::Blue, FString::FromInt(count));
				
			}
			count++;
			flag = flag << 1;
		}
	}
}
void UMCComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner() != nullptr)
		SVOTree->World = GetOwner()->GetWorld();
}