#include"header.h"

void SortSelector();  //ѡ��Ҫʹ�õ��������� 
void SelectionSort();   //ѡ������ 
void BubbleSort();     //ð������ 
void MergeSort(int32_t left,int32_t right);   //�鲢���� 
void QuickSort(int32_t left, int32_t right);  //�������� 
void ChangeTwoBlocks(int32_t a, int32_t b);   //������������ 
void SetMovingBlocks(int32_t a, int32_t b, int32_t pointer); //�����˶�����Ĳ��� 

//ѡ���������� 
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

//ð����������n-1�֣�ÿ�������������ð�����Ӷ�O(n^2) 
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
				//������������ 
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

//�鲢����ÿ�ν�һ������ֳ����룬�������붼��Ϊ��������O(n)�Ĵ��۽�������ϳ�Ϊһ���������С����Ӷ�O(nlogn) 
void MergeSort(int32_t left,int32_t right)
{
	if (left>=right) 
	{
		return;
	}
	//�ֳ������֣��ݹ����� 
	int32_t lI,lMid=(left+right)/2,lP,lQ,lR;
	lP=lR=left;
	lQ=lMid+1;
	MergeSort(left,lMid);
	MergeSort(lMid+1,right);
	
	//�洢�������� 
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
	
	//�������������кϲ� 
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

//ѡ������ÿ��ѡ��һ����С�����ӷŵ���ǰλ�á����Ӷ�O(n^2) 
void SelectionSort()
{
	int32_t lK=0,lI=0,lSwap,lMax=0;
	for (lK=0;lK<m_lN-1;++lK)
	{
		m_alSteps[++m_lStepPointer].kind=1;
		m_alSteps[m_lStepPointer].object[0]=lK;
		m_alSteps[m_lStepPointer].object[1]=lK;
		lMax=lK;
		//ѡ��һ����С������ 
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
		//��ѡ���������뵱ǰ���ӽ��� 
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

//��������ѡ��һ����׼�����ѱ���С�ķ���ߣ�������ķ��ұߣ�Ȼ��ݹ������˲��������Ӷ�O(nlogn) 
void QuickSort(int32_t left, int32_t right)
{
	if (left>=right)
	{
		return;
	}
	int32_t lI=left,lJ=right,lSwap;
	//ѡ����׼�� 
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
		//����һ��������Ĵ�������Ұ�����С���� 
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
	//�ݹ����� 
	if (left<lJ) QuickSort(left,lJ);
	if (lI<right) QuickSort(lI,right);
}

//���˹鲢�����⣬����������2��������н��������øú��� 
void ChangeTwoBlocks(int32_t a, int32_t b) {
	SetMovingBlocks(a, b, 0);
	SetMovingBlocks(b, a, 1);
}

//�����ƶ����� 
void SetMovingBlocks(int32_t a, int32_t b, int32_t pointer) {
	m_blocks[a].lState = 1;	

	m_MovingBlocks[pointer].Position = m_blocks[a].lX;
	m_MovingBlocks[pointer].distance = m_blocks[b].lX - m_blocks[a].lX;
	m_MovingBlocks[pointer].startBlock = a;
	m_MovingBlocks[pointer].toBlock = b;
}

