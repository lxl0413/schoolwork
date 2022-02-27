#include"header.h"

void SortSelector();  //选择要使用的排序类型 
void SelectionSort();   //选择排序 
void BubbleSort();     //冒泡排序 
void MergeSort(int32_t left,int32_t right);   //归并排序 
void QuickSort(int32_t left, int32_t right);  //快速排序 
void ChangeTwoBlocks(int32_t a, int32_t b);   //交换两个方块 
void SetMovingBlocks(int32_t a, int32_t b, int32_t pointer); //设置运动方块的参数 

//选择排序类型 
void SortSelector() {
	switch(SortType)
	{
		case BUBBLE:
			BubbleSort();
			break;
		case SELECTION:
			SelectionSort();
			break;
		case QUICK:
			QuickSort(0,m_lN-1); 
			break;
		case MERGE: 
			MergeSort(0,m_lN-1); 	
			break;
	}
} 

//冒泡排序，运行n-1轮，每轮里最大的泡向后冒，复杂度O(n^2) 
void BubbleSort()
{
	int32_t lI = 0, lJ = 0, lSwap=0;
	for (lI=0;lI<m_lN-1;lI++)
	{
		for (lJ=0;lJ<m_lN-lI-1;lJ++)
		{
			m_alSteps[++m_lStepPointer].kind=1;
			m_alSteps[m_lStepPointer].object[0]=lJ;
			m_alSteps[m_lStepPointer].object[1]=lJ+1;
			m_alSteps[m_lStepPointer].compare=1;
			if (m_array[lJ]>m_array[lJ+1]) 
			{
				//交换两个柱子 
				m_alSteps[++m_lStepPointer].kind=2;
				m_alSteps[m_lStepPointer].object[0]=lJ;
				m_alSteps[m_lStepPointer].object[1]=lJ+1;
				m_alSteps[m_lStepPointer].change=1;
				lSwap=m_array[lJ];
				m_array[lJ]=m_array[lJ+1];
				m_array[lJ+1]=lSwap;
			}
		}
	}
	return 0;
}

//归并排序，每次将一个区间分成两半，并将两半都排为有序，再用O(n)的代价将这两半合成为一个有序序列。复杂度O(nlogn) 
void MergeSort(int32_t left,int32_t right)
{
	if (left>=right) 
	{
		return;
	}
	//分成两部分，递归排序 
	int32_t lI,lMid=(left+right)/2,lP,lQ,lR;
	lP=lR=left;
	lQ=lMid+1;
	MergeSort(left,lMid);
	MergeSort(lMid+1,right);
	
	//存储高亮区间 
	m_alSteps[++m_lStepPointer].kind=1;
	m_alSteps[m_lStepPointer].LR[0][0]=left;
	m_alSteps[m_lStepPointer].LR[0][1]=lMid;
	m_alSteps[m_lStepPointer].LR[1][0]=lMid+1;
	m_alSteps[m_lStepPointer].LR[1][1]=right;
	
	m_alSteps[++m_lStepPointer].kind=2;
	m_alSteps[m_lStepPointer].LR[0][0]=left;
	m_alSteps[m_lStepPointer].LR[0][1]=lMid;
	m_alSteps[m_lStepPointer].LR[1][0]=lMid+1;
	m_alSteps[m_lStepPointer].LR[1][1]=right;
	
	//把两个有序序列合并 
	while (lP<=lMid && lQ<=right)
	{
		if (m_array[lP]<=m_array[lQ])
		{
			m_MergeArray[lR]=m_array[lP];
			m_alSteps[m_lStepPointer].position[lP]=lR;
			++lR;
			++lP;
		}
		else
		{
			m_MergeArray[lR]=m_array[lQ];
			m_alSteps[m_lStepPointer].position[lQ]=lR;
			++lR;
			++lQ;
		}
	}
	m_alSteps[m_lStepPointer].compare+=lR-left;
	m_alSteps[m_lStepPointer].change+=right-left+1;
	while (lP<=lMid)
	{
		m_MergeArray[lR++]=m_array[lP++];
		m_alSteps[m_lStepPointer].position[lP-1]=lR-1;
	}
	while (lQ<=right)
	{
		m_MergeArray[lR++]=m_array[lQ++];
		m_alSteps[m_lStepPointer].position[lQ-1]=lR-1;
	}
	for (lI=left;lI<=right;++lI)
	{
		m_array[lI]=m_MergeArray[lI];
	}
}

//选择排序，每次选择一个最小的柱子放到当前位置。复杂度O(n^2) 
void SelectionSort()
{
	int32_t lK=0,lI=0,lSwap,lMax=0;
	for (lK=0;lK<m_lN-1;++lK)
	{
		m_alSteps[++m_lStepPointer].kind=1;
		m_alSteps[m_lStepPointer].object[0]=lK;
		m_alSteps[m_lStepPointer].object[1]=lK;
		lMax=lK;
		//选择一个最小的柱子 
		for (lI=lK+1;lI<m_lN;++lI)
		{
			if (m_array[lI]<m_array[lMax])
			{
				m_alSteps[++m_lStepPointer].kind=1;
				m_alSteps[m_lStepPointer].object[0]=lK;
				m_alSteps[m_lStepPointer].object[1]=lI;
				m_alSteps[m_lStepPointer].compare=lI-lMax;
				lMax=lI;
			}
		}
		//将选出的柱子与当前柱子交换 
		if (lK!=lMax)
		{
			m_alSteps[++m_lStepPointer].kind=2;
			m_alSteps[m_lStepPointer].object[0]=lK;
			m_alSteps[m_lStepPointer].object[1]=lMax;
			m_alSteps[m_lStepPointer].change=1;
			lSwap=m_array[lK];
			m_array[lK]=m_array[lMax];
			m_array[lMax]=lSwap;
		}
	}
}

//快速排序，选出一个基准量，把比它小的放左边，比它大的放右边，然后递归继续如此操作。复杂度O(nlogn) 
void QuickSort(int32_t left, int32_t right)
{
	if (left>=right)
	{
		return;
	}
	int32_t lI=left,lJ=right,lSwap;
	//选出基准量 
	double lMid=m_array[(lI+lJ)/2];
	while (lI<=lJ)
	{
		m_alSteps[++m_lStepPointer].kind=1;
		m_alSteps[m_lStepPointer].object[0]=lI;
		m_alSteps[m_lStepPointer].object[1]=lJ;
		m_alSteps[m_lStepPointer].LR[0][0]=left;
		m_alSteps[m_lStepPointer].LR[0][1]=right;
		while (m_array[lI]<lMid)
		{
			++lI;
			m_alSteps[++m_lStepPointer].kind=1;
			m_alSteps[m_lStepPointer].object[0]=lI;
			m_alSteps[m_lStepPointer].object[1]=lJ;
			m_alSteps[m_lStepPointer].compare=1;
			m_alSteps[m_lStepPointer].LR[0][0]=left;
			m_alSteps[m_lStepPointer].LR[0][1]=right;
		}
		while (m_array[lJ]>lMid)
		{
			--lJ;
			m_alSteps[++m_lStepPointer].kind=1;
			m_alSteps[m_lStepPointer].object[0]=lI;
			m_alSteps[m_lStepPointer].object[1]=lJ;
			m_alSteps[m_lStepPointer].compare=1;
			m_alSteps[m_lStepPointer].LR[0][0]=left;
			m_alSteps[m_lStepPointer].LR[0][1]=right;
		}
		//交换一个左半区的大变量和右半区的小变量 
		if (lI<=lJ)
		{
			if (lI!=lJ)
			{
				m_alSteps[++m_lStepPointer].kind=2;
				m_alSteps[m_lStepPointer].object[0]=lI;
				m_alSteps[m_lStepPointer].object[1]=lJ;
				m_alSteps[m_lStepPointer].change=1;
				m_alSteps[m_lStepPointer].LR[0][0]=left;
				m_alSteps[m_lStepPointer].LR[0][1]=right;
				lSwap=m_array[lI];
				m_array[lI]=m_array[lJ];
				m_array[lJ]=lSwap;
			}
			++lI;
			--lJ;
		}
	}
	//递归排序 
	if (left<lJ) QuickSort(left,lJ);
	if (lI<right) QuickSort(lI,right);
}

//除了归并排序外，其他排序都是2个方块进行交换，调用该函数 
void ChangeTwoBlocks(int32_t a, int32_t b) {
	SetMovingBlocks(a, b, 0);
	SetMovingBlocks(b, a, 1);
}

//设置移动柱子 
void SetMovingBlocks(int32_t a, int32_t b, int32_t pointer) {
	m_blocks[a].lState = 1;	

	m_MovingBlocks[pointer].Position = m_blocks[a].lX;
	m_MovingBlocks[pointer].distance = m_blocks[b].lX - m_blocks[a].lX;
	m_MovingBlocks[pointer].startBlock = a;
	m_MovingBlocks[pointer].toBlock = b;
}

