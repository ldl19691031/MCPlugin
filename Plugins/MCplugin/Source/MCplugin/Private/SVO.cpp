#include "MCpluginPrivatePCH.h"
#include "SVO.h"
const float  FSVOTree::VoxelSize = 100.0f;
FSVOTree::FSVOTree(){
	RootNode = new OctreeNode<FVoxel>(FVector(0,0,0),102400.0f,nullptr);
	//体素空间最大最小值
	/*RootNode->Min = FVector(-102400.0f, -102400.0f, -102400.0f);
	RootNode->Max = FVector(102400.0f, 102400.0f, 102400.0f);*/
}
FSVOTree::~FSVOTree(){
	delete RootNode;
}
void FSVOTree::AddAVoxel(FVector Location){
	if (RootNode->IsInNode(Location)!=true)
	{
		return;
	}
	FSVOTree::AddAVoxelRecursion(RootNode, Location);
}

void FSVOTree::AddAVoxelRecursion(OctreeNode<FVoxel>* CurrentNode, FVector Location){
	////放缩包围盒,由于预先规定了大小限制所以不用放缩
	//CurrentNode->ScaleBoundBox(Location);

	//判断当前节点是否为白，如果为白则调节为灰
	if (CurrentNode->NodeType==White)
	{
		CurrentNode->NodeType = Grey;
	}

	//判断当前节点是否到达体素最小值，如果到达则返回，否则细分
	if (CurrentNode->IsLessThanEdge(VoxelSize))
	{
		CurrentNode->NodeType = Black;
		return;
	}


	//细分                
    //          .;rsis,,,,,,,,,,,,,,,,,,..,i1hMax                 
    //       .;ssi,  :                  :rsi:  1i                 
    //    .iss;,     ;              .;rsi:     hi                 
    // .r11i,   .....i............;ssi,        hi                 
    //:S1i;;;;;;;;;;isi;;;;;;;;;;Si,           hi                 
    //;5             ;           S             hi                 
    //;5             ;           S.            hi                 
    //;5             ;           S.            hi                 
    //;5             ;           S.            hi                 
    //;5             ;           S.            hi                 
    //;5             ;           S.            hi                 
    //;Z             ;           S.            hi                 
    //;5           . ,,,,,,,,,,,,3:,,,,,,,,..:rS:                 
    //;5        .,:,.            S.       ,iss;.                  
    //;5     .X,,.               S.    ,isr;.                     
    //;h  .,,.                   S  :rsr;.                        
    //Min,,;:,,::::::Y:::::::::::9rsr: 
	
	CurrentNode->Children[0] = new OctreeNode<FVoxel>(CurrentNode->Center + FVector(-CurrentNode->Size / 2, -CurrentNode->Size / 2, -CurrentNode->Size / 2), CurrentNode->Size / 2, CurrentNode);
	CurrentNode->Children[1] = new OctreeNode<FVoxel>(CurrentNode->Center + FVector( CurrentNode->Size / 2, -CurrentNode->Size / 2, -CurrentNode->Size / 2), CurrentNode->Size / 2, CurrentNode);
	CurrentNode->Children[2] = new OctreeNode<FVoxel>(CurrentNode->Center + FVector(-CurrentNode->Size / 2, CurrentNode->Size / 2, -CurrentNode->Size / 2), CurrentNode->Size / 2, CurrentNode);
	CurrentNode->Children[3] = new OctreeNode<FVoxel>(CurrentNode->Center + FVector(CurrentNode->Size / 2,	CurrentNode->Size / 2, -CurrentNode->Size / 2), CurrentNode->Size / 2, CurrentNode);
	CurrentNode->Children[4] = new OctreeNode<FVoxel>(CurrentNode->Center + FVector(-CurrentNode->Size / 2, -CurrentNode->Size / 2, CurrentNode->Size / 2), CurrentNode->Size / 2, CurrentNode);
	CurrentNode->Children[5] = new OctreeNode<FVoxel>(CurrentNode->Center + FVector(CurrentNode->Size / 2, -CurrentNode->Size / 2,	CurrentNode->Size / 2), CurrentNode->Size / 2, CurrentNode);
	CurrentNode->Children[6] = new OctreeNode<FVoxel>(CurrentNode->Center + FVector(-CurrentNode->Size / 2, CurrentNode->Size / 2,	CurrentNode->Size / 2), CurrentNode->Size / 2, CurrentNode);
	CurrentNode->Children[7] = new OctreeNode<FVoxel>(CurrentNode->Center + FVector(CurrentNode->Size / 2, CurrentNode->Size / 2,	CurrentNode->Size / 2), CurrentNode->Size / 2, CurrentNode);

	//递归调用细分
	for (auto p : CurrentNode->Children){
		FSVOTree::AddAVoxelRecursion(p, Location);
	}
}