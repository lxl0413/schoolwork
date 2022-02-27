#include<stdio.h>
#include<string.h>
#include <windows.h>
#include <winuser.h>

#include"graphics.h"
#include"extgraph.h"
#include"imgui.h"
#include"header.h"

void SetBlockColor(int32_t lComparing);//根据block中储存的状态，选择方块的颜色
void drawBlock(int32_t lComparing, double lX, double lHeight, double lNumber);//画一个方块
void drawBlocks();//画出静止的方块 
void drawMovingBlocks();//画出运动中的方块 
void drawStatistic();//显示统计数据
 
double m_startpointY = 0.4; 

void SetBlockColor(int32_t lComparing) 
{
	switch (lComparing)
	{
		case 1:SetPenColor("Erica"); //暗红色，表示这根柱子正在被比较或移动 
		break;
		case 2:SetPenColor("Beni"); //浅红色，表示这根柱子正在被比较或移动 
		break;
		case 0:SetPenColor("Jadeite"); //翡翠色，表示这根柱子当前没有被排序算法用到 
		break;
		case 3:SetPenColor("Fog"); //草绿色，表示这根柱子在快速排序的当前区间内，但目前并没有被比较或移动 
		break;
	}	
}


void drawBlock(int32_t lComparing, double lX, double lHeight, double lNumber)
{
	SetBlockColor(lComparing);
	char numberString[10];
	//将数字转化为String，储存在字符串中 
	
	//如果数据类型是整数，就保留整数显示，否则保留一位小数显示 
	if (DataType == INT) 
	{
		sprintf(numberString, "%d", (int32_t)lNumber);
	}
	else
	{
		sprintf(numberString, "%.1lf", lNumber);
	}
	//根据各项参数画一个柱子 
	drawBox(lX, m_startpointY, m_blocks[0].lWidth, lHeight, 1, numberString, 'C', "White");
}


void drawBlocks() 
{
	int32_t lI;
	for(lI = 0; lI < m_lN; lI++) {
		//画出所有静止状态的柱子 
		if(m_blocks[lI].lState == 0) 
		{
			drawBlock(m_blocks[lI].lComparing, m_blocks[lI].lX, m_blocks[lI].lHeight, m_blocks[lI].lNumber);
		}
	}
}

//画出移动状态的柱子 
void drawMovingBlocks() {
	int32_t rep;
	for(rep = 0; rep < MBPointer; rep++) 
	{
		//MBPointer中存储了需要移动的方块数 
		int32_t b1 = m_MovingBlocks[rep].startBlock;	
		drawBlock(m_blocks[b1].lComparing, m_MovingBlocks[rep].Position, m_blocks[b1].lHeight, m_blocks[b1].lNumber);	
	}
}

void drawStatistic()
{
	
	char compareString[5],changeString[5];
	itoa(compareCount, compareString, 10);
	itoa(changeCount,changeString,10); 
	// 显示比较次数
	SetPenColor("Amber");
	drawLabel(5.8, 3.35, "Compare：");
	drawRectangle(6.7, 3.2, 0.6, 0.4, 0); 
	drawLabel(6.9, 3.35, compareString);
	 
	// 显示交换次数
	drawLabel(8.1, 3.35, "Exchange：");
	drawRectangle(9.0, 3.2, 0.6, 0.4, 0); 
	drawLabel(9.2, 3.35, changeString);
} 

//控制柱子、统计数据的显示 
void DisplaySort() {
	drawBlocks();
	if(Task.isMoving == 1)
	{
		drawMovingBlocks(MBPointer);
	}
	drawStatistic();
}
