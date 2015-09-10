#include "MCpluginPrivatePCH.h"
#include "SVO.h"
const float  FSVOTree::VoxelSize = 100.0f;
FSVOTree::FSVOTree(){
	RootNode = new OctreeNode<FVoxel>(FVector(0,0,0),102400.0f,nullptr);
	//���ؿռ������Сֵ
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
	////������Χ��,����Ԥ�ȹ涨�˴�С�������Բ��÷���
	//CurrentNode->ScaleBoundBox(Location);

	//�жϵ�ǰ�ڵ��Ƿ�Ϊ�ף����Ϊ�������Ϊ��
	if (CurrentNode->NodeType==White)
	{
		CurrentNode->NodeType = Grey;
	}

	//�жϵ�ǰ�ڵ��Ƿ񵽴�������Сֵ����������򷵻أ�����ϸ��
	if (CurrentNode->IsLessThanEdge(VoxelSize))
	{
		CurrentNode->NodeType = Black;
		return;
	}


	//ϸ��                
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

	//�ݹ����ϸ��
	for (auto p : CurrentNode->Children){
		FSVOTree::AddAVoxelRecursion(p, Location);
	}
}