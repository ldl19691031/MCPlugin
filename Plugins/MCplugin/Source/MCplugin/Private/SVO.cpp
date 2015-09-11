#include "MCpluginPrivatePCH.h"
#include "DrawDebugHelpers.h"
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
	if (World!=nullptr)
	{
		DrawDebugBox(World, CurrentNode->Center, FVector(CurrentNode->Size, CurrentNode->Size, CurrentNode->Size), FColor::Red,true,100.0f);
	}


	//�жϵ�ǰ�ڵ��Ƿ�Ϊ�ף����Ϊ�������Ϊ��
	if (CurrentNode->NodeType==White)
	{
		CurrentNode->NodeType = Grey;
	}

	//�жϵ�ǰ�ڵ��Ƿ񵽴�������Сֵ����������򷵻أ�����ϸ��
	if (CurrentNode->IsLessThanEdge(VoxelSize))
	{
		CurrentNode->NodeType = Black;
		DrawDebugBox(World, CurrentNode->Center, FVector(CurrentNode->Size, CurrentNode->Size, CurrentNode->Size), FColor::Blue, true, 100.0f);
		//��ӵ�ǰVoxel������Ӷ�Ӧ��Cell
		CreateAVoxel(CurrentNode);
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
	for (int i = 0; i < 8;i++){
		DrawDebugBox(World, CurrentNode->Children[i]->Center, FVector(CurrentNode->Children[i]->Size, CurrentNode->Children[i]->Size, CurrentNode->Children[i]->Size), FColor::Red, true, 100.0f);
		GEngine->AddOnScreenDebugMessage(0, 10.0, FColor::White, CurrentNode->Children[i]->Center.ToString());
		if (CurrentNode->Children[i]->IsInNode(Location)){
			AddAVoxelRecursion(CurrentNode->Children[i], Location);
			return;
		}
			
	}
}
bool FSVOTree::HasVertex(FVector Location){
	return RootNode->DoVoxelCheck(Location);
}
void FSVOTree::CreateAVoxel(OctreeNode<FVoxel>* Node){

	//@todo �Ż���һ�δ��룬����Ҫÿ�α�������Cell��
	//��������һ��Voxel��Ϣ
	Node->Data = MakeShareable(new FVoxel());
	//���ɰ˸�Cell
	bool bIsAlreadyInList = false;
	FCell* Cell;
	//ǰ����
	Cell = new FCell(Node->Center + FVector(-Node->Size, -Node->Size, -Node->Size), Node->Size);
	Cell->CellType |= 0x40;//7�Ŷ���
	for (auto c:CellsList)
	{
		if (*c==*Cell)//��ǰ������Cell�Ѿ�����
		{
			Node->Data->ContainedCells[0] = c;
			c->CellType |= Cell->CellType;
			bIsAlreadyInList = true;
			delete Cell;
			break;
		}
	}
	//û�д��ڣ���List����µ�Cell
	if (!bIsAlreadyInList)
	{
		Node->Data->ContainedCells[0] = MakeShareable(Cell);
		CellsList.Add(Node->Data->ContainedCells[0]);
	}
	//ǰ����
	Cell = new FCell(Node->Center + FVector(-Node->Size, Node->Size, -Node->Size), Node->Size);
	Cell->CellType |= 0x80;
	for (auto c : CellsList)
	{
		if (*c == *Cell)//��ǰ������Cell�Ѿ�����
		{
			Node->Data->ContainedCells[1] = c;
			c->CellType |= Cell->CellType;
			bIsAlreadyInList = true;
			delete Cell;
			break;
		}
	}
	//û�д��ڣ���List����µ�Cell
	if (!bIsAlreadyInList)
	{
		Node->Data->ContainedCells[1] = MakeShareable(Cell);
		CellsList.Add(Node->Data->ContainedCells[1]);
	}
	//ǰ����
	Cell = new FCell(Node->Center + FVector(-Node->Size, -Node->Size, Node->Size), Node->Size);
	Cell->CellType |= 0x20;
	for (auto c : CellsList)
	{
		if (*c == *Cell)//��ǰ������Cell�Ѿ�����
		{
			Node->Data->ContainedCells[2] = c;
			c->CellType |= Cell->CellType;
			bIsAlreadyInList = true;
			delete Cell;
			break;
		}
	}
	//û�д��ڣ���List����µ�Cell
	if (!bIsAlreadyInList)
	{
		Node->Data->ContainedCells[2] = MakeShareable(Cell);
		CellsList.Add(Node->Data->ContainedCells[2]);
	}
	//ǰ����
	Cell = new FCell(Node->Center + FVector(-Node->Size, Node->Size, Node->Size), Node->Size);
	Cell->CellType |= 0x10;
	for (auto c : CellsList)
	{
		if (*c == *Cell)//��ǰ������Cell�Ѿ�����
		{
			Node->Data->ContainedCells[3] = c;
			c->CellType |= Cell->CellType;
			bIsAlreadyInList = true;
			delete Cell;
			break;
		}
	}
	//û�д��ڣ���List����µ�Cell
	if (!bIsAlreadyInList)
	{
		Node->Data->ContainedCells[3] = MakeShareable(Cell);
		CellsList.Add(Node->Data->ContainedCells[3]);
	}

	//������
	Cell = new FCell(Node->Center + FVector(Node->Size, -Node->Size, -Node->Size), Node->Size);
	Cell->CellType |= 0x04;
	for (auto c : CellsList)
	{
		if (*c == *Cell)//��ǰ������Cell�Ѿ�����
		{
			Node->Data->ContainedCells[4] = c;
			c->CellType |= Cell->CellType;
			bIsAlreadyInList = true;
			delete Cell;
			break;
		}
	}
	//û�д��ڣ���List����µ�Cell
	if (!bIsAlreadyInList)
	{
		Node->Data->ContainedCells[4] = MakeShareable(Cell);
		CellsList.Add(Node->Data->ContainedCells[4]);
	}
	//������
	Cell = new FCell(Node->Center + FVector(Node->Size, Node->Size, -Node->Size), Node->Size);
	Cell->CellType |= 0x08;
	for (auto c : CellsList)
	{
		if (*c == *Cell)//��ǰ������Cell�Ѿ�����
		{
			Node->Data->ContainedCells[5] = c;
			c->CellType |= Cell->CellType;
			bIsAlreadyInList = true;
			delete Cell;
			break;
		}
	}
	//û�д��ڣ���List����µ�Cell
	if (!bIsAlreadyInList)
	{
		Node->Data->ContainedCells[5] = MakeShareable(Cell);
		CellsList.Add(Node->Data->ContainedCells[5]);
	}
	//������
	Cell = new FCell(Node->Center + FVector(Node->Size, -Node->Size, Node->Size), Node->Size);
	Cell->CellType |= 0x02;
	for (auto c : CellsList)
	{
		if (*c == *Cell)//��ǰ������Cell�Ѿ�����
		{
			Node->Data->ContainedCells[6] = c;
			c->CellType |= Cell->CellType;
			bIsAlreadyInList = true;
			delete Cell;
			break;
		}
	}
	//û�д��ڣ���List����µ�Cell
	if (!bIsAlreadyInList)
	{
		Node->Data->ContainedCells[6] = MakeShareable(Cell);
		CellsList.Add(Node->Data->ContainedCells[6]);
	}
	//������
	Cell = new FCell(Node->Center + FVector(Node->Size, Node->Size, Node->Size), Node->Size);
	Cell->CellType |= 0x01;
	for (auto c : CellsList)
	{
		if (*c == *Cell)//��ǰ������Cell�Ѿ�����
		{
			Node->Data->ContainedCells[7] = c;
			c->CellType |= Cell->CellType;
			bIsAlreadyInList = true;
			delete Cell;
			break;
		}
	}
	//û�д��ڣ���List����µ�Cell
	if (!bIsAlreadyInList)
	{
		Node->Data->ContainedCells[7] = MakeShareable(Cell);
		CellsList.Add(Node->Data->ContainedCells[7]);
	}
}