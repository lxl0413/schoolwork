#include<header.h> 

//����������һ�� 
int32_t next_step(int direction)
{
	//�ڳ�ʼ״̬������ǰ���������� 
	if (m_lNowStep==0 && direction==-1)
	{
		return 0;
	}
	//�ڽ���״̬���������������� 
	if (m_lNowStep==m_lStepPointer && direction==1)
	{
		continue_to_move=0;
		return 0;
	}
	
	int32_t rep;//����ѭ���ı��� 
	//�ڵ�һ����״̬�º��ˣ���ʹ�������Ӷ�ʧȥ���� 
	if (m_lNowStep==1 && direction==-1)
	{
		--m_lNowStep;
		for (rep=0;rep<m_lN;++rep) m_blocks[rep].lComparing=0;
		compareCount=changeCount=0;
		return 1;
	}
	
	//�����ؽ�����һ�� 
	if (direction==1) 
	{
		++m_lNowStep;
		compareCount+=m_alSteps[m_lNowStep].compare;
		changeCount+=m_alSteps[m_lNowStep].change;
		int nowx=m_alSteps[m_lNowStep].object[0], nowy=m_alSteps[m_lNowStep].object[1];
		
		//��һ�������������ӱ�ɫ 
		if (m_alSteps[m_lNowStep].kind==1)
		{
			Task.isComparing = 1;
			for (rep=0;rep<m_lN;++rep) m_blocks[rep].lComparing=0;
			
			//���������㷨�����ͣ��������ӱ�ɫ�ķ�ʽ 
			if (SortType==BUBBLE || SortType==SELECTION)
			{	
				m_blocks[nowx].lComparing=1;
				m_blocks[nowy].lComparing=2;
			}
			else if (SortType==QUICK)
			{
				for (rep=m_alSteps[m_lNowStep].LR[0][0];rep<=m_alSteps[m_lNowStep].LR[0][1];++rep) m_blocks[rep].lComparing=3;
				m_blocks[nowx].lComparing=1;
				m_blocks[nowy].lComparing=2;
			}
			else if (SortType == MERGE)
			{
				for (rep = m_alSteps[m_lNowStep].LR[0][0]; rep <= m_alSteps[m_lNowStep].LR[0][1]; rep++) 
				{
					m_blocks[rep].lComparing = 1;
				}
				for (rep = m_alSteps[m_lNowStep].LR[1][0]; rep <= m_alSteps[m_lNowStep].LR[1][1]; rep++) 
				{
					m_blocks[rep].lComparing = 2;
				}
			}
		}
		
		//��һ�������������ӵ��ƶ� 
		else
		{
			Task.isMoving = 1;
			//���������㷨�����ͣ����������ƶ��ķ�ʽ 
			if (SortType!=MERGE) 
			{
				MBPointer = 2;
				ChangeTwoBlocks(nowx,nowy);
			}
			else
			{
				OneStepMerge(direction); 
			}
		}
	}
	
	//����һ������������Ĳ�����һЩ��������ͺ��ˣ������� 
	else
	{
		compareCount-=m_alSteps[m_lNowStep].compare;
		changeCount-=m_alSteps[m_lNowStep].change;
		int nowx=m_alSteps[m_lNowStep].object[0], nowy=m_alSteps[m_lNowStep].object[1];
		int prex=m_alSteps[m_lNowStep-1].object[0], prey=m_alSteps[m_lNowStep-1].object[1];
		if (m_alSteps[m_lNowStep].kind==1)
		{
			for (rep=0;rep<m_lN;++rep) m_blocks[rep].lComparing=0;
			if (SortType==BUBBLE || SortType==SELECTION)
			{
				m_blocks[prex].lComparing=1;
				m_blocks[prey].lComparing=2;
			}
			else if (SortType==QUICK) 
			{
				for (rep=m_alSteps[m_lNowStep-1].LR[0][0];rep<=m_alSteps[m_lNowStep-1].LR[0][1];++rep)
				{
					m_blocks[rep].lComparing=3;
				}
				m_blocks[prex].lComparing=1;
				m_blocks[prey].lComparing=2;
			}
			//����ҲҪ��һ�·������ 
			else
			{
				for (rep = m_alSteps[m_lNowStep-1].LR[0][0]; rep <= m_alSteps[m_lNowStep-1].LR[0][1]; rep++) 
				{
					m_blocks[rep].lComparing = 1;
				}
				for (rep = m_alSteps[m_lNowStep-1].LR[1][0]; rep <= m_alSteps[m_lNowStep-1].LR[1][1]; rep++) 
				{
					m_blocks[rep].lComparing = 2;
				}
			}
		}
		else
		{
			Task.isMoving = 1;
			if(SortType != MERGE) 
			{
				MBPointer = 2;
				ChangeTwoBlocks(nowx,nowy);
			}
			else
			{ 
				OneStepMerge(direction);		
			} 
		}
		--m_lNowStep;
	}
	return 1;
}

//�鲢����һ���ƶ������󣬽���״̬���á���̬����Ĵ���
void endOneStepMerge() 
{
	int32_t rep;
	int32_t left = m_alSteps[m_lNowStep].LR[0][0];
	int32_t right = m_alSteps[m_lNowStep].LR[1][1];
	Block tempBlocks[MAXN] = {0};//�ݴ�������״̬���������¶�ȡ 
	for(rep = left; rep <= right; rep++) 
	{
		m_blocks[rep].lState = 0;
		tempBlocks[rep].lNumber = m_blocks[rep].lNumber;
		tempBlocks[rep].lHeight = m_blocks[rep].lHeight;
		tempBlocks[rep].lX = m_blocks[rep].lX;
		tempBlocks[rep].lComparing = m_blocks[rep].lComparing;
	}					
	for(rep = 0; rep < MBPointer; rep++) 
	{
		int32_t tB = m_MovingBlocks[rep].toBlock;
		int32_t sB = m_MovingBlocks[rep].startBlock;
		m_blocks[tB].lNumber = tempBlocks[sB].lNumber;
		m_blocks[tB].lHeight = tempBlocks[sB].lHeight;
		m_blocks[tB].lX = tempBlocks[tB].lX;	
		m_blocks[tB].lComparing = tempBlocks[sB].lComparing;
	}
}

//����һ���鲢������ƶ� 
void OneStepMerge(int32_t direction) 
{
	int32_t rep;
	int32_t left = m_alSteps[m_lNowStep].LR[0][0];
	int32_t right = m_alSteps[m_lNowStep].LR[1][1];
	for (rep = left; rep <= right; rep++) 
	{
		int32_t target = m_alSteps[m_lNowStep].position[rep];
		if (target != rep) 
		{
			if(direction == 1)   //next����ȡĿ��λ�ú͵�ǰλ�� 
			{
				SetMovingBlocks( rep, target, MBPointer);
			}
			else     //last���˶�������next�෴ 
			{
				SetMovingBlocks( target, rep, MBPointer);
			}
			MBPointer += 1;
		}
	}
}

//���˹鲢�����⣬��������һ���ƶ������󣬽���״̬���á���̬����Ĵ��������Ϊ�������� 
void endOneStep() 
{
	int32_t a = m_MovingBlocks[0].startBlock;
	int32_t b = m_MovingBlocks[1].startBlock;
	
	//�ı�״̬����ؾ�̬���� 
	m_blocks[a].lState = 0;
	m_blocks[b].lState = 0;	
	
	
	//������������ĸ������� 
	int32_t lT;
	lT=m_blocks[a].lNumber;
	m_blocks[a].lNumber = m_blocks[b].lNumber;
	m_blocks[b].lNumber = lT;
	
	lT=m_blocks[a].lComparing;
	m_blocks[a].lComparing = m_blocks[b].lComparing;
	m_blocks[b].lComparing = lT;
	
	double lH;
	lH = m_blocks[a].lHeight;
	m_blocks[a].lHeight = m_blocks[b].lHeight;
	m_blocks[b].lHeight = lH;	
}
