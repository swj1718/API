#pragma once
#pragma warning(disable:4996)
#pragma comment(lib,"msimg32.lib")
#pragma comment (lib, "fmodex_vc.lib")

#include<Windows.h>
#include<iostream>
#include<string>
#include<ctime>
#include<cmath>
#include<random>
#include<fstream>
#include<vector>
#include<list>
#include<deque>
#include<algorithm>
#include "inc/fmod.hpp"
#include"resource.h"
using namespace std;
using namespace FMOD;

// 상수 및 구조체
#define PI 3.14
#define MAX_BLOCK 1000
#define MAX_ITEM 200
#define WINDOW_WIDTH 497
#define WINDOW_HEIGHT 720
typedef struct tagOBJEct
{
	int mX;
	int mY;
	tagOBJEct(int _mX, int _mY)
		: mX(_mX), mY(_mY) {}
}OBJECT;

// <변수>
//배경
//Background
static int iScrollY = 1280;
static int iScrollX = 100;
// Hero
static int iHeroX, iHeroY, iHero_Turn;
static int iHero_frameX = 0;
static int iHero_frameY = 0;
static int iScore = 0;
static bool bFall;
static bool bSkill = FALSE;//스킬 사용중인지 판단
static bool bJudge_print;
// Block
static deque<OBJECT> dBlock;
static int iX_Pos = 180;
static int iY_Pos = 420;
static int iBlock_Turn;
static int iTemp;
static int iOrigin = 180;
static bool bJudge[1000];
// Item
static list<OBJECT> lItem;
static double iItem_Count = 0;
static int igage = 0;
static BOOL bGage = FALSE;
//Time
static double dTime_length = 400;
static double dTimer = 0;
//WAVE
static double iScroll_Wave = 150;
// Random
default_random_engine dre;
uniform_int_distribution<int> ui_ItemPlace{ 0, 5 }; // 0~1개
normal_distribution<> nd(0, 1.0);
//Title
RECT Title_pos[4]; //메뉴 위치
static int iTitle_count = 0;
static int iMenu = 0;
static int iGamemode = 6;
static int mx, my;
HBRUSH hBrush, oldBrush;
// Time
double start;
static int iOne_Time = 0;
// etc
static int iOne_Digit = 0;
static bool bCheat_key = false;
static int iItem_frame = 0;
static int iMenu_frame = 0;
static int iHero_Select = 1;
static int iGame_Select = 6;
static int iMode_frame = 0;
static bool bGame_start = FALSE;
static bool bSkill_On = false;
static bool bCtrl = FALSE;
static bool bKey = FALSE; //FALSE면 ctrl
// Rank
using RANKDATA = pair<string, int>; // char(text),double(time)
static vector<RANKDATA> vInput;
static char szID[20];
static string ID;
static int GameScore;
static int GameType;

static vector<RANKDATA> vOutput_Record;
static vector<RANKDATA> vOutput_Wave;
static vector<RANKDATA> vOutput_Time;

//
enum {
	MENU = 0,
	SELECT,
	GAME,
	MANUAL,
	RANK,
	QUIT,
	GAME_RECORD,
	GAME_TIME,
	GAME_WAVE
};
// 
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Dlg6_1Proc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Dlg6_2Proc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

HINSTANCE g_hInst;
RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
LPCTSTR lpszClass = "WIN_FINAL";


// 사운드
FMOD_SYSTEM     *g_System;
FMOD_SOUND      *g_Menu, *g_Step, *g_Turn, *g_Death, *g_Skill;
FMOD_CHANNEL   *g_Channel_Menu, *g_Channel_Step, *g_Channel_Turn, *g_Channel_Death, *g_Channel_Skill;

void SoundInit()
{
	// 시스템 오브젝트 생성과 초기화
	FMOD_System_Create(&g_System);
	FMOD_System_Init(g_System, 32, FMOD_INIT_NORMAL, NULL);

	// 효과 사운드
	FMOD_System_CreateSound(g_System, "Resource/Sound/Menu.mp3", FMOD_DEFAULT, 0, &g_Menu); //
	FMOD_System_CreateSound(g_System, "Resource/Sound/Up.wav", FMOD_DEFAULT, 0, &g_Step); //
	FMOD_System_CreateSound(g_System, "Resource/Sound/Turn.wav", FMOD_DEFAULT, 0, &g_Turn); //
	FMOD_System_CreateSound(g_System, "Resource/Sound/Death.wav", FMOD_DEFAULT, 0, &g_Death); //
	FMOD_System_CreateSound(g_System, "Resource/Sound/Skill.wav", FMOD_DEFAULT, 0, &g_Skill); //

}

void SoundRelease()
{
	FMOD_Sound_Release(g_Menu);
	FMOD_Sound_Release(g_Step);
	FMOD_Sound_Release(g_Turn);
	FMOD_Sound_Release(g_Death);
	FMOD_Sound_Release(g_Skill);

	FMOD_System_Close(g_System);
	FMOD_System_Release(g_System);
}
FMOD_BOOL isPlaying;




// 사용자 함수
BOOL InBox(int x, int y, int mx, int my, int w, int h)//충돌
{
	if ((x <= mx) && ((x + w) >= mx) && (y <= my) && ((y + h) >= my))
		return TRUE;
	else
		return FALSE;
}
void Reset();
//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevlnstance, LPSTR IpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	//윈도우 생성
	hWnd = CreateWindow(lpszClass, lpszClass,
		WS_OVERLAPPEDWINDOW, 400, 000
		, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);

	//윈도우 출력
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//이벤트 루프 처리
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
