#include<stdio.h>
#include<string.h>
#include <windows.h>
#include <winuser.h>

#include"graphics.h"
#include"extgraph.h"
#include"imgui.h"
#include"header.h"

void SetBlockColor(int32_t lComparing);//����block�д����״̬��ѡ�񷽿����ɫ
void drawBlock(int32_t lComparing, double lX, double lHeight, double lNumber);//��һ������
void drawBlocks();//������ֹ�ķ��� 
void drawMovingBlocks();//�����˶��еķ��� 
void drawStatistic();//��ʾͳ������
 
double m_startpointY = 0.4; 

void SetBlockColor(int32_t lComparing) 
{
	switch (lComparing)
	{
		case 1:SetPenColor("Erica"); //����ɫ����ʾ����������ڱ��Ƚϻ��ƶ� 
		break;
		case 2:SetPenColor("Beni"); //ǳ��ɫ����ʾ����������ڱ��Ƚϻ��ƶ� 
		break;
		case 0:SetPenColor("Jadeite"); //���ɫ����ʾ������ӵ�ǰû�б������㷨�õ� 
		break;
		case 3:SetPenColor("Fog"); //����ɫ����ʾ��������ڿ�������ĵ�ǰ�����ڣ���Ŀǰ��û�б��Ƚϻ��ƶ� 
		break;
	}	
}


void drawBlock(int32_t lComparing, double lX, double lHeight, double lNumber)
{
	SetBlockColor(lComparing);
	char numberString[10];
	//������ת��ΪString���������ַ����� 
	
	//��������������������ͱ���������ʾ��������һλС����ʾ 
	if (DataType == INT) 
	{
		sprintf(numberString, "%d", (int32_t)lNumber);
	}
	else
	{
		sprintf(numberString, "%.1lf", lNumber);
	}
	//���ݸ��������һ������ 
	drawBox(lX, m_startpointY, m_blocks[0].lWidth, lHeight, 1, numberString, 'C', "White");
}


void drawBlocks() 
{
	int32_t lI;
	for(lI = 0; lI < m_lN; lI++) {
		//�������о�ֹ״̬������ 
		if(m_blocks[lI].lState == 0) 
		{
			drawBlock(m_blocks[lI].lComparing, m_blocks[lI].lX, m_blocks[lI].lHeight, m_blocks[lI].lNumber);
		}
	}
}

//�����ƶ�״̬������ 
void drawMovingBlocks() {
	int32_t rep;
	for(rep = 0; rep < MBPointer; rep++) 
	{
		//MBPointer�д洢����Ҫ�ƶ��ķ����� 
		int32_t b1 = m_MovingBlocks[rep].startBlock;	
		drawBlock(m_blocks[b1].lComparing, m_MovingBlocks[rep].Position, m_blocks[b1].lHeight, m_blocks[b1].lNumber);	
	}
}

void drawStatistic()
{
	
	char compareString[5],changeString[5];
	itoa(compareCount, compareString, 10);
	itoa(changeCount,changeString,10); 
	// ��ʾ�Ƚϴ���
	SetPenColor("Amber");
	drawLabel(5.8, 3.35, "Compare��");
	drawRectangle(6.7, 3.2, 0.6, 0.4, 0); 
	drawLabel(6.9, 3.35, compareString);
	 
	// ��ʾ��������
	drawLabel(8.1, 3.35, "Exchange��");
	drawRectangle(9.0, 3.2, 0.6, 0.4, 0); 
	drawLabel(9.2, 3.35, changeString);
} 

//�������ӡ�ͳ�����ݵ���ʾ 
void DisplaySort() {
	drawBlocks();
	if(Task.isMoving == 1)
	{
		drawMovingBlocks(MBPointer);
	}
	drawStatistic();
}
