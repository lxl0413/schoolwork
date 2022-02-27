#include<stdio.h> 
#include<stdlib.h>
#include<string.h>
#include"graphics.h"
#include"header.h"

double blockheight(double lNumber, double lmax, double winheight); //控制柱子高度的函数 
void SaveInFile(); //存储当前状态 
void LoadFromFile(); //读取存储的状态 

void Init(int32_t flag) 
{
	//如果flag == 1，则是最开始的初始化，需要初始UI，否则只需要初始化排序 
	if(flag == 1) 
	{
		InitUI();
		InitSort();	
	}
	else
	{
		InitSort();
	}
} 

void InitUI()
{
	//初始化颜色 
	DefineColor("Moss", 0.0078, 0.2039, 0.2470);
	DefineColor("Jadeite", 0.0001, 0.5020, 0.5020);
	DefineColor("Amber", 0.9490, 0.9373, 0.7922);
	DefineColor("Beni", 0.9255, 0.5020, 0.5529);
	DefineColor("Erica", 0.4275, 0.0001, 0.0549);	
	DefineColor("Fog", 0.3020, 0.5, 0.2705);
}

//初始化排序状态 
void InitState()  
{
	able_to_move = 0;
	continue_to_move = 0;
	finish_sorting = 0;
	m_lNowStep = 0;
	Task.time = 0;
	Task.isStop = 0;
	Task.isComparing = 0;
	Task.isMoving = 0;
	changeCount = 0;
	compareCount = 0;
	MBPointer = 0;
	SetTimeSpeed(0.2);
	EditSpeed(0.2); 
}

//初始化排序 
void InitSort() {
	InitState();
	m_lStepPointer=0;
	finish_sorting = 0;
	
	//运用文件来读入数据 
	FILE *fpin;
	char type[10];
	char typemodel[3][10]={"int","float","double"};
	
	fpin=fopen("files/input.in","r");
	fscanf(fpin, "%s",type); 
	
	//读取数据类型，如果不合规范就退出 
	if (strcmp(type,typemodel[0])==0) DataType=INT;
	else if (strcmp(type,typemodel[1])==0) DataType=FLOAT;
	else if (strcmp(type,typemodel[2])==0) DataType=DOUBLE;
	else
	{
		exit(-1);
	}
	m_lN=-1;
	double InputNumber;
	double lMax;
	int32_t lI;
	lMax=-1;
	//读入各个数据并存储 
	while (fscanf(fpin,"%lf",&InputNumber)==1)
	{
		if (InputNumber<0)
		{
			exit(-1);
		}
		m_array[++m_lN]=InputNumber;
		
		//找到最大的数，以此为基准计算其他方块的高度 
		if(InputNumber > lMax) lMax = InputNumber;
		m_blocks[m_lN].lNumber = InputNumber;
	}
	fscanf(fpin,"%lf",&InputNumber);
	if (InputNumber<0)
	{
		exit(-1);
	}
	m_array[++m_lN]=InputNumber;
	if(InputNumber > lMax) lMax = InputNumber;
	m_blocks[m_lN].lNumber = InputNumber;
	
	//如果排序数据太多（超过100个）就退出 
	if( m_lN > MAXN ) 
	{
		exit(-1);
	}
	fclose(fpin);

	
	//设置排序区域的长和宽 
	double AreaWidth = 5;
	double AreaHeight = 5;	
	
	//计算方块和间隙宽度 
	double blockwidth = AreaWidth / (m_lN + 1.0);
	double interwidth = blockwidth / 5.0;
	blockwidth = blockwidth / 5.0 * 4;
	double startpointX = 0.4 + blockwidth / 2.0 + interwidth;	
	
	//初始化柱子和步骤 
	for(lI = 0; lI < m_lN; lI++) 
	{
		m_blocks[lI].lHeight = blockheight(m_blocks[lI].lNumber, lMax, AreaHeight);
		m_blocks[lI].lWidth = blockwidth;
		m_blocks[lI].lX = startpointX + (blockwidth+interwidth) * lI;
		m_blocks[lI].lState = 0;
		m_blocks[lI].lComparing = 0;
	}
	for (lI = 0; lI<MAXN*MAXN*5; lI++)
	{
		m_alSteps[lI].kind=0;
		m_alSteps[lI].object[0]=m_alSteps[lI].object[1]=0;
		m_alSteps[lI].compare=m_alSteps[lI].change=0;
		m_alSteps[lI].LR[0][0]=m_alSteps[lI].LR[0][1]=m_alSteps[lI].LR[1][0]=m_alSteps[lI].LR[1][1]=0;
		int32_t lJ=0;
		for (lJ=0;lJ<MAXN;lJ++)
		{
			m_alSteps[lI].position[lJ]=0;
		}
	}
}

//高度与数值大小成正比 
double blockheight(double lNumber, double lMax, double AreaHeight) {
	return AreaHeight / (lMax + lMax * 0.1) * lNumber;
}

//将关键数据存储进二进制文件 
void SaveInFile()
{
	FILE *fpsave;
	int32_t lI,lJ;
	fpsave=fopen("files/save.txt","wb");
	fwrite(&DataType,1,1,fpsave);
	fwrite(&m_lN,4,1,fpsave);
	fwrite(&SortType,4,1,fpsave);
	fwrite(&m_lStepPointer,4,1,fpsave);
	fwrite(&m_lNowStep,4,1,fpsave);
	fwrite(&MBPointer,4,1,fpsave);
	fwrite(&compareCount,4,1,fpsave);
	fwrite(&changeCount,4,1,fpsave);
	fwrite(&Task.time,8,1,fpsave);
	fwrite(&Task.isMoving,4,1,fpsave);
	fwrite(&Task.isComparing,4,1,fpsave);
	fwrite(&Task.isStop,4,1,fpsave);
	double speed = GetTimeSpeed();
	fwrite(&speed,8,1,fpsave);
	for (lI=0;lI<m_lN;++lI)
	{
		fwrite(&m_blocks[lI].lHeight,8,1,fpsave);
		fwrite(&m_blocks[lI].lNumber,8,1,fpsave);
		fwrite(&m_blocks[lI].lX,8,1,fpsave);
		fwrite(&m_blocks[lI].lWidth,8,1,fpsave);
		fwrite(&m_blocks[lI].lState,4,1,fpsave);
		fwrite(&m_blocks[lI].lComparing,4,1,fpsave);
	}
	for (lI=1;lI<=m_lStepPointer;++lI)
	{
		fwrite(&m_alSteps[lI].kind,4,1,fpsave);
		fwrite(&m_alSteps[lI].object[0],4,1,fpsave);
		fwrite(&m_alSteps[lI].object[1],4,1,fpsave);
		fwrite(&m_alSteps[lI].compare,4,1,fpsave);
		fwrite(&m_alSteps[lI].change,4,1,fpsave);
		fwrite(&m_alSteps[lI].LR[0][0],4,1,fpsave);
		fwrite(&m_alSteps[lI].LR[0][1],4,1,fpsave);
		fwrite(&m_alSteps[lI].LR[1][0],4,1,fpsave);
		fwrite(&m_alSteps[lI].LR[1][1],4,1,fpsave);
		for (lJ=0;lJ<m_lN;++lJ)
		{
			fwrite(&m_alSteps[lI].position[lJ],4,1,fpsave);
		}
	}
	for (lI=0;lI<MBPointer;++lI)
	{
		fwrite(&m_MovingBlocks[lI].Position,8,1,fpsave);
		fwrite(&m_MovingBlocks[lI].distance,8,1,fpsave);
		fwrite(&m_MovingBlocks[lI].startBlock,4,1,fpsave);
		fwrite(&m_MovingBlocks[lI].toBlock,4,1,fpsave);
	}
	fclose(fpsave);
}

//从二进制文件里读取关键数据 
void LoadFromFile()
{
	FILE *fpsave;
	int32_t lI,lJ;
	fpsave=fopen("files/save.txt","rb");
	fread(&DataType,1,1,fpsave);
	fread(&m_lN,4,1,fpsave);
	fread(&SortType,4,1,fpsave);
	fread(&m_lStepPointer,4,1,fpsave);
	fread(&m_lNowStep,4,1,fpsave);
	fread(&MBPointer,4,1,fpsave);
	fread(&compareCount,4,1,fpsave);
	fread(&changeCount,4,1,fpsave);
	fread(&Task.time,8,1,fpsave);
	fread(&Task.isMoving,4,1,fpsave);
	fread(&Task.isComparing,4,1,fpsave);
	fread(&Task.isStop,4,1,fpsave);
	double speed;
	fread(&speed,8,1,fpsave);
	SetTimeSpeed(speed); 
	EditSpeed(speed);
	for (lI=0;lI<m_lN;++lI)
	{
		fread(&m_blocks[lI].lHeight,8,1,fpsave);
		fread(&m_blocks[lI].lNumber,8,1,fpsave);
		fread(&m_blocks[lI].lX,8,1,fpsave);
		fread(&m_blocks[lI].lWidth,8,1,fpsave);
		fread(&m_blocks[lI].lState,4,1,fpsave);
		fread(&m_blocks[lI].lComparing,4,1,fpsave);
	}
	for (lI=1;lI<=m_lStepPointer;++lI)
	{
		fread(&m_alSteps[lI].kind,4,1,fpsave);
		fread(&m_alSteps[lI].object[0],4,1,fpsave);
		fread(&m_alSteps[lI].object[1],4,1,fpsave);
		fread(&m_alSteps[lI].compare,4,1,fpsave);
		fread(&m_alSteps[lI].change,4,1,fpsave);
		fread(&m_alSteps[lI].LR[0][0],4,1,fpsave);
		fread(&m_alSteps[lI].LR[0][1],4,1,fpsave);
		fread(&m_alSteps[lI].LR[1][0],4,1,fpsave);
		fread(&m_alSteps[lI].LR[1][1],4,1,fpsave);
		for (lJ=0;lJ<m_lN;++lJ)
		{
			fread(&m_alSteps[lI].position[lJ],4,1,fpsave);
		}
	}
	for (lI=0;lI<MBPointer;++lI)
	{
		fread(&m_MovingBlocks[lI].Position,8,1,fpsave);
		fread(&m_MovingBlocks[lI].distance,8,1,fpsave);
		fread(&m_MovingBlocks[lI].startBlock,4,1,fpsave);
		fread(&m_MovingBlocks[lI].toBlock,4,1,fpsave);
	}
	fclose(fpsave);
}
