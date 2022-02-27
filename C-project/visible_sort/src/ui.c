#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <winuser.h>

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"
#include"header.h"

#define winwidth 10.0
#define winheight 6.50
static char speedString[15] = "0.2";

void UI_StartPage(); // 绘制开始界面 
void UI_ChooseButton(); // 绘制开始界面的按钮1-7 
void UI_SortPage(); // 绘制排序页面 
void UI_SortButton(); // 绘制排序界面的按钮8-14
double GetEditSpeed();  // 获取当前排序速度 


// 绘制开始界面 
void UI_StartPage()
{
	SetWindowTitle("VisibleSort - Start");
	SetPenColor("Moss");
	drawRectangle(0, 0, winwidth, winheight, 1);
	SetPenColor("Amber");
	drawRectangle(0.4, 5.3, 9.2, 0.3, 0);
	drawLabel(0.6, 5.4, "V     I     S     I     B     L     E            S     O     R     T");
	drawLabel(1.1, 3.9,"|");
	drawLabel(0.63, 2.8, "Choose");
	drawLabel(0.65, 2.6, "Sorting");
	drawLabel(0.5, 2.4, "Algorithm"); 
}

// 绘制开始界面的按钮1-7 
void UI_ChooseButton()
{
	// 按钮1-4：选择4种排序算法类型 
	setButtonColors ("Jadeite", "Amber", "Moss", "Amber", 1);
	
	if( button(GenUIID(0), 1.6, 2.4, 1.625, 1.625, "Selection Sort") )
	{	
		SortType = SELECTION;
	}
	
	if( button(GenUIID(1), 3.725, 2.4, 1.625, 1.625, "Bubble Sort") )
	{	
		SortType = BUBBLE;
	}
	
	if( button(GenUIID(2), 5.85, 2.4, 1.625, 1.625, "Merge Sort") )
	{	
		SortType = MERGE;
	}
	
	if( button(GenUIID(3), 7.975, 2.4, 1.625, 1.625, "Quick Sort") )
	{	
		SortType = QUICK;
	}

	setButtonColors ("Amber", "Moss", "Moss", "Amber", 1);
	
	// 按钮5：开始排序 
	if( button(GenUIID(4), 0.4, 1.0, 3.8, 0.4, "Start") )
	{	
		Task.Page = 1;
		Init(0);
		SortSelector();
		able_to_move = 0 ;
		continue_to_move = 0;
	}
	
	// 按钮6：继续上一次排序 
	if( button(GenUIID(5), 4.5, 1.0, 3.8, 0.4, "Load") )
	{	
		LoadFromFile();
		Task.Page = 1;
		able_to_move = 0 ;
		continue_to_move = 0;
	}
	
	//按钮7：程序退出 
	if( button(GenUIID(6), 8.6, 1.0, 1.0, 0.4, "Quit") )
	{
		exit(-1);
	}		
}

// 绘制排序页面  
void UI_SortPage(){
	
	SetWindowTitle("VisibleSort - Sort");
	SetPenColor("Moss");
	drawRectangle(0, 0, winwidth, winheight, 1);
	SetPenColor("Amber");
	drawRectangle(0, 6.0, 10.0, 0.5, 0);
	drawLabel(3.45, 6.2, "V     I     S     I     B     L     E            S     O     R     T");
	
	// 画排序展示框 
	SetPenColor("Amber");
	drawRectangle(0.4, 0.4, 5, 5, 1);	
	
	// 显示排序算法的名称 
	SetPenColor("Jadeite");
	drawRectangle(5.8, 5.0, 3.8, 0.4, 1); 
	SetPenColor("Amber");
	switch(SortType) 
	{
		case SELECTION: drawLabel(6.0, 5.15, "S E L E C T I O N   S O R T"); break;
		case BUBBLE: drawLabel(6.0, 5.15, "B U B B L E   S O R T"); break;
		case MERGE: drawLabel(6.0, 5.15, "M E R G E   S O R T"); break;
		case QUICK: drawLabel(6.0, 5.15, "Q U I C K   S O R T"); break;
	}
}

// 绘制排序界面的按钮8-14 
void UI_SortButton() 
{
	setButtonColors ("Jadeite", "Amber", "Moss", "Amber", 1);
	
	// 按钮8：保存当前进度 
	if(continue_to_move == 0)
	{ 
		if( button(GenUIID(7), 5.8, 0.4, 1.0, 0.4, "Save") )
		{	
			SaveInFile();
		}
	} 
	
	// 按钮9：返回主页面 
	if( button(GenUIID(8), 7.2, 0.4, 1.0, 0.4, "Back") )
	{
		Task.Page = 0;
	}

	// 按钮10：程序退出
	if( button(GenUIID(9), 8.6, 0.4, 1.0, 0.4, "Quit") )
	{
		exit(-1);
	}
	
	setButtonColors ("Amber", "Moss", "Moss", "Amber", 1);
	
	// 按钮11：重新开始排序 
	if ( button(GenUIID(10), 5.8, 1.8, 0.75, 0.4, "Replay") ) 
		{
			Init(0);
			SortSelector();
			able_to_move = 1;
			continue_to_move = 1;
		}
	
	// 按钮12：开始/暂停	
	if ( button(GenUIID(11), 6.82, 1.8, 0.75, 0.4, continue_to_move ? "Stop":"Play") ) 
	{
		continue_to_move = (continue_to_move == 0) ? 1 : 0;  //选择contiue时，持续运动 
	}
	
	// 暂停后出现按钮13：上一步	
	if(continue_to_move == 0)
	{
		if (button(GenUIID(12), 7.84, 1.8, 0.75, 0.4, "Last"))
		{
			able_to_move = -1;
		}	
	}
	
	// 暂停后出现按钮14：下一步	
	if ( continue_to_move || button(GenUIID(13), 8.85, 1.8, 0.75, 0.4, "Next")) 
	{
		if(Task.isMoving == 0) 
		{
			able_to_move = 1;
		}	
	}
	
	// 显示排序速度
	setTextBoxColors("Amber", "Amber", "Amber", "Amber", 0);
	SetPenColor("Amber");
	drawLabel(5.8, 2.65, "Rate：");
	if( textbox(GenUIID(17), 6.7, 2.5, 1.7, 0.4, speedString, sizeof(speedString)) )
	{
		SetTimeSpeed(GetEditSpeed());
	}
	
	// 按钮15：减速
	if ( button(GenUIID(15), 8.7, 2.5, 0.4 ,0.4, "-") ) 
	{
		SetTimeSpeed(GetTimeSpeed() * 0.833);	
		EditSpeed(GetTimeSpeed());
	}
	
	// 按钮16：加速
	if ( button(GenUIID(16), 9.2, 2.5, 0.4, 0.4, "+") ) 
	{
		SetTimeSpeed(GetTimeSpeed() * 1.2);	
		EditSpeed(GetTimeSpeed());
	}
}

//显示编辑过的速度 
void EditSpeed(double speed)
{
	sprintf(speedString, "%f", speed);
}

//获取改动的速度 
double GetEditSpeed()
{
	double v = 0;
	sscanf(speedString, "%lf", &v);
	return v;
}

void DrawUI() 
{
	// 开始界面 
	if(Task.Page == 0)
	{
		UI_StartPage();
		UI_ChooseButton();
	}
	// 排序界面 
	else
	{
		UI_SortPage();
		UI_SortButton();
	}
}
