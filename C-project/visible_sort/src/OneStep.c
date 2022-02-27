#include<header.h> 

//用来控制下一步 
int32_t next_step(int direction)
{
	//在初始状态继续往前，则不做处理 
	if (m_lNowStep==0 && direction==-1)
	{
		return 0;
	}
	//在结束状态继续往后，则不做处理 
	if (m_lNowStep==m_lStepPointer && direction==1)
	{
		continue_to_move=0;
		return 0;
	}
	
	int32_t rep;//用于循环的变量 
	//在第一步的状态下后退，则使所有柱子都失去高亮 
	if (m_lNowStep==1 && direction==-1)
	{
		--m_lNowStep;
		for (rep=0;rep<m_lN;++rep) m_blocks[rep].lComparing=0;
		compareCount=changeCount=0;
		return 1;
	}
	
	//正常地进行下一步 
	if (direction==1) 
	{
		++m_lNowStep;
		compareCount+=m_alSteps[m_lNowStep].compare;
		changeCount+=m_alSteps[m_lNowStep].change;
		int nowx=m_alSteps[m_lNowStep].object[0], nowy=m_alSteps[m_lNowStep].object[1];
		
		//下一步的类型是柱子变色 
		if (m_alSteps[m_lNowStep].kind==1)
		{
			Task.isComparing = 1;
			for (rep=0;rep<m_lN;++rep) m_blocks[rep].lComparing=0;
			
			//根据排序算法的类型，控制柱子变色的方式 
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
		
		//下一步的类型是柱子的移动 
		else
		{
			Task.isMoving = 1;
			//根据排序算法的类型，控制柱子移动的方式 
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
	
	//后退一步，根据上面的部分做一些镜像操作就好了，不详述 
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
			//这里也要做一下反向高亮 
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

//归并排序一次移动结束后，进行状态重置、静态方块的处理
void endOneStepMerge() 
{
	int32_t rep;
	int32_t left = m_alSteps[m_lNowStep].LR[0][0];
	int32_t right = m_alSteps[m_lNowStep].LR[1][1];
	Block tempBlocks[MAXN] = {0};//暂存各方块的状态，下面重新读取 
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

//进行一步归并排序的移动 
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
			if(direction == 1)   //next，读取目标位置和当前位置 
			{
				SetMovingBlocks( rep, target, MBPointer);
			}
			else     //last，运动正好与next相反 
			{
				SetMovingBlocks( target, rep, MBPointer);
			}
			MBPointer += 1;
		}
	}
}

//除了归并排序外，其他排序一次移动结束后，进行状态重置、静态方块的处理，对象均为两个方块 
void endOneStep() 
{
	int32_t a = m_MovingBlocks[0].startBlock;
	int32_t b = m_MovingBlocks[1].startBlock;
	
	//改变状态，变回静态方块 
	m_blocks[a].lState = 0;
	m_blocks[b].lState = 0;	
	
	
	//交换两个方块的各个参数 
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
