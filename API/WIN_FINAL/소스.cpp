#include "define.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HWND hDlg = NULL;
	HWND hDlg2 = NULL;
	static HDC hdc, memdc, mem1dc;
	static HBITMAP Basebit, Backbit, Magicbit, Thiefbit, Warbit;
	static HBITMAP  Blockbit, Numberbit, Timerbit, Itembit, Menubit;
	static HBITMAP MPbit, MP_Gagebit, Timebit, Time_gagebit, Wavebit;
	static HBITMAP Select_backbit, Modebit, Char_Explainbit, Mode_Explainbit, Manualbit;
	static BITMAP bmp;

	switch (iMsg)
	{
	case WM_CREATE:
		// 사운드
		SoundInit();
		GetClientRect(hwnd, &rect);
		dre.seed((unsigned)time(NULL));
		SetTimer(hwnd, 4, 200, NULL);
		SetTimer(hwnd, 2, 50, NULL);
		SetTimer(hwnd, 3, 50, NULL);
		SetTimer(hwnd, 6, 100, NULL);
		SetTimer(hwnd, 10, 50, NULL);
		iHeroX = 128;
		iHeroY = 301;
		iHero_Turn = 1;
		iBlock_Turn = -1;
		for (int i = 0; i < 4; i++)
		{
			Title_pos[i].left = 150;
			Title_pos[i].right = 350;
			Title_pos[i].top = 275 + i * 100;
			Title_pos[i].bottom = 325 + i * 100;
		}

		break;
	case WM_KEYUP:
		// 아이템삭제 함수.
		if (iMenu == GAME)
		{
			lItem.remove_if([](const OBJECT& a) {
				if (a.mX == 240 && a.mY == 450)
					bGage = TRUE;
				return (a.mX == 240 && a.mY == 450);
			});
			if (bGage)
			{
				if (igage < 160)
				{
					igage += 8;
				}
				bGage = FALSE;
			}
			if (wParam == VK_CONTROL) //왼쪽 줄
			{
				if (!bSkill)
					iHero_frameX = 0;
			}
			else if (wParam == VK_SPACE) //오른쪽 줄
			{
				if (!bSkill)
					iHero_frameX = 0;
			}
		}
		InvalidateRgn(hwnd, NULL, false);

		break;
	case WM_KEYDOWN:
		if (iMenu == GAME)
		{
			if (!bFall)
			{
				lItem.remove_if([](const OBJECT& a) {
					if (a.mX == 240 && a.mY == 450)
						bGage = TRUE;
					return (a.mX == 240 && a.mY == 450);
				});

				if (wParam == VK_CONTROL)
				{
					bKey = FALSE;
					FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Turn, 0, &g_Channel_Turn);
					iScrollY -= 1;
					if (iHero_frameY == 0)
						iScrollX -= 1;
					else
						iScrollX += 1;
					if (iGame_Select == GAME_RECORD)
					{
						if (dTime_length < 395)
							dTime_length += 5;
					}
					if (iScore > 0)
					{
						if (!bCtrl)
						{
							if (bJudge[iScore - 1])
							{
								if (!bCheat_key)
								{
									FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Death, 0, &g_Channel_Death);
									bFall = true;
									SetTimer(hwnd, 1, 50, NULL);
								}
							}
						}
						else
						{
							if (!bJudge[iScore - 1])
							{
								if (!bCheat_key)
								{
									FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Death, 0, &g_Channel_Death);
									bFall = true;
									SetTimer(hwnd, 1, 50, NULL);
								}
							}
						}
					}
					// 0일때 Space를 누르지않으면 무조건
					else if (iScore == 0)
					{
						if (!bCheat_key)
						{
							FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Death, 0, &g_Channel_Death);
							bFall = true;
							SetTimer(hwnd, 1, 50, NULL);
						}
					}
					// 스코어 증가// 도적스킬
					++iScore;
					if (iHero_Select == 2 && bSkill)
					{
						iOne_Digit += 2;
					}
					else
					{
						++iOne_Digit;
					}

					// 캐릭터 좌우 전환
					if (iHero_frameY == 0)
						iHero_frameY = 1;
					else
						iHero_frameY = 0;
					//블록 지우기
					if (iScore > 10)
						dBlock.pop_front();
					if (!bSkill)
						iHero_frameX = 1;
					iHero_Turn *= -1;
					// 블록움직이기
					for (auto& d : dBlock)
					{
						d.mX += 60 * iHero_Turn;
						d.mY += 30;
					}
					// 아이템움직이기
					for (auto& d : lItem)
					{
						d.mX += 60 * iHero_Turn;
						d.mY += 30;
					}
					if (iGamemode == GAME_WAVE)
					{
						if (iScroll_Wave > 100)
							iScroll_Wave -= 10;
					}
					bCtrl = FALSE;
				}
				if (wParam == VK_SPACE)
				{
					bKey = TRUE;
					FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Step, 0, &g_Channel_Step);
					if (!bGame_start && iMenu == GAME)
					{
						SetTimer(hwnd, 5, 100, NULL);
						if (iGamemode == GAME_WAVE)
						{
							SetTimer(hwnd, 6, 30, NULL);
						}
						bGame_start = TRUE;
						if (bGame_start)
							start = clock();
					}
					iScrollY -= 1; //스크롤
					if (iHero_frameY == 0)
						iScrollX -= 1;
					else
						iScrollX += 1;
					if (iGame_Select == GAME_RECORD)
					{
						if (dTime_length < 395)
							dTime_length += 5;
					}
					if (iScore > 0)
					{
						if (!bJudge[iScore - 1])
						{
							if (!bCheat_key)
							{
								FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Death, 0, &g_Channel_Death);
								bFall = true;
								SetTimer(hwnd, 1, 50, NULL);
							}
						}
					}
					// 스코어 증가 // 도적스킬
					++iScore;
					if (iHero_Select == 2 && bSkill)
					{
						iOne_Digit += 2;
					}
					else
					{
						++iOne_Digit;
					}

					//블록지우기
					if (iScore > 10)
						dBlock.pop_front();

					if (!bSkill)
						iHero_frameX = 1;

					// 블록움직이기
					for (auto& d : dBlock)
					{
						d.mX += 60 * iHero_Turn;
						d.mY += 30;
					}
					// 아이템움직이기
					for (auto& d : lItem)
					{
						d.mX += 60 * iHero_Turn;
						d.mY += 30;
					}
					if (iGame_Select == GAME_WAVE)
					{
						if (iScroll_Wave > 100)
							iScroll_Wave -= 10;
					}
				}
			}
			if (wParam == VK_F1)
			{
				if (!bCheat_key)
					bCheat_key = true;
				else
					bCheat_key = false;
			}
		}
		if (iMenu == SELECT)
		{
			if (wParam == VK_LEFT)
			{
				iHero_Select -= 1;
				if (iHero_Select <= 0)
					iHero_Select = 3;
			}
			else if (wParam == VK_RIGHT)
			{
				iHero_Select += 1;
				if (iHero_Select >= 4)
					iHero_Select = 1;
			}
			else if (wParam == VK_UP)
			{
				iGame_Select++;
				iMode_frame++;
				if (iMode_frame > 2)
				{
					iMode_frame = 0;
				}
				if (iGame_Select > 8)
				{
					iGame_Select = 6;
				}
			}
			else if (wParam == VK_DOWN)
			{
				iGame_Select--;
				iMode_frame--;
				if (iGame_Select < 6)
				{
					iGame_Select = 8;
				}
				if (iMode_frame < 0)
				{
					iMode_frame = 2;
				}
			}
			else if (wParam == VK_SPACE)
			{
				if (iGame_Select == GAME_TIME)
				{
					for (int i = 0; i < 200; ++i)
					{
						// 아이템 초기화.
						iItem_Count += (double)ui_ItemPlace(dre);
						if (iItem_Count >= 5.0)
						{
							lItem.emplace_back(iX_Pos, iY_Pos);
							iItem_Count = 0.;
						}
						// 블록 초기화.
						dBlock.emplace_back(iX_Pos, iY_Pos);
						iTemp = (int)nd(dre);

						if (iTemp != 0)
						{
							iBlock_Turn *= -1;
							bJudge[i] = false;      // 판단용.
						}
						else
							bJudge[i] = true;      // 판단용.

						iY_Pos -= 30;
						iX_Pos += 60 * iBlock_Turn;
					}
				}
				else
				{
					for (int i = 0; i < MAX_BLOCK; ++i)
					{
						// 아이템 초기화.
						iItem_Count += (double)ui_ItemPlace(dre);
						if (iItem_Count >= 5.0)
						{
							lItem.emplace_back(iX_Pos, iY_Pos);
							iItem_Count = 0.;
						}
						// 블록 초기화.
						dBlock.emplace_back(iX_Pos, iY_Pos);
						iTemp = (int)nd(dre);

						if (iTemp != 0)
						{
							iBlock_Turn *= -1;
							bJudge[i] = false;      // 판단용.
						}
						else
							bJudge[i] = true;      // 판단용.

						iY_Pos -= 30;
						iX_Pos += 60 * iBlock_Turn;
					}
				}
				iMenu = GAME;
			}

		}
		if (wParam == 'z' || wParam == 'Z')
		{
			if (iGame_Select == GAME_RECORD)
			{
				if (igage >= 160 && !bFall)
				{
					FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Skill, 0, &g_Channel_Skill);
					bSkill = TRUE;
					igage = 0;

					if (iHero_Select == 3)
					{
						KillTimer(hwnd, 3);
					}
				}
			}
		}
		if (wParam == VK_ESCAPE)
		{
			if (iMenu == MANUAL)
				iMenu = MENU;
		}
		InvalidateRgn(hwnd, NULL, false);

		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (iMenu == MENU)
		{
			if (InBox(Title_pos[0].left, Title_pos[0].top, mx, my, 200, 50))
			{
				iMenu = SELECT;
				iHero_frameX = 2;

			}
			if (InBox(Title_pos[1].left, Title_pos[1].top, mx, my, 200, 50))
			{
				iMenu = MANUAL;
			}
			if (InBox(Title_pos[2].left, Title_pos[2].top, mx, my, 200, 50))
			{
				if (!IsWindow(hDlg2))
				{
					hDlg2 = CreateDialog(g_hInst,
						MAKEINTRESOURCE(IDD_DIALOG1), hwnd, Dlg6_2Proc);
					ShowWindow(hDlg2, SW_SHOW);
				}
			}
			if (InBox(Title_pos[3].left, Title_pos[3].top, mx, my, 200, 50))
			{
				PostQuitMessage(0);
			}
		}
		InvalidateRgn(hwnd, NULL, FALSE);
		break;
	case WM_TIMER:
		if (wParam == 1 && iMenu == GAME)
		{
			iHero_frameX = 2;
			iHeroY += 20;
			if (iHeroY > 600)
			{
				if (iHero_Select == 1 && bSkill)//불사신 스킬
				{
					if (!bKey)
						bCtrl = TRUE;
					else
						bCtrl = FALSE;
					iHeroX = 128; iHeroY = 301;
					bFall = FALSE;
					bSkill = FALSE;
					dTimer = 0;
					//떨어지기 전으로 돌려줌
					for (auto& d : dBlock)
					{
						d.mX -= 60 * iHero_Turn;
						d.mY -= 30;
					}
					for (auto& d : lItem)
					{
						d.mX -= 60 * iHero_Turn;
						d.mY -= 30;
					}
					//판별 원래 떨어지고 전으로
					for (int i = MAX_BLOCK - 1; i > -1; --i)
					{
						bJudge_print = bJudge[i - 1]; // 1(2)
						bJudge[i - 1] = bJudge[i]; // 0(1)
						bJudge[i] = bJudge_print;
					}
					KillTimer(hwnd, 1);
				}
				else // 불사신아닐떄.
				{
					if (iGame_Select == GAME_RECORD || iGame_Select == GAME_WAVE)
						GameScore = iOne_Digit;
					else if (iGame_Select == GAME_TIME && iOne_Digit >= 200)
						GameScore = iOne_Time;
					GameType = iGame_Select;
					Reset();
					//Ranking
					hDlg = CreateDialog(g_hInst,
						MAKEINTRESOURCE(IDD_DIALOG2), hwnd, Dlg6_1Proc);
					ShowWindow(hDlg, SW_SHOW);

					iMenu = MENU;
					KillTimer(hwnd, 1); KillTimer(hwnd, 5); KillTimer(hwnd, 6);
				}
			}
		}
		if (wParam == 2 && iMenu == GAME)
		{
			iItem_frame = (iItem_frame + 1) % 5;
		}
		if (wParam == 3 && iMenu == GAME)
		{
			if (iGame_Select == GAME_RECORD)
			{
				if (dTime_length > 0)
				{
					if ((iOne_Digit) < 300)
						dTime_length -= (iOne_Digit) / 100.0; //난이도
					else
						dTime_length -= 3;
				}
				else
				{
					Reset();
					for (int i = 0; i < MAX_BLOCK; ++i)
					{
						// 아이템 초기화.
						iItem_Count += (double)ui_ItemPlace(dre);
						if (iItem_Count >= 5.0)
						{
							lItem.emplace_back(iX_Pos, iY_Pos);
							iItem_Count = 0.;
						}
						// 블록 초기화.
						dBlock.emplace_back(iX_Pos, iY_Pos);
						iTemp = (int)nd(dre);

						if (iTemp != 0)
						{
							iBlock_Turn *= -1;
							bJudge[i] = false;      // 판단용.
						}
						else
							bJudge[i] = true;      // 판단용.

						iY_Pos -= 30;
						iX_Pos += 60 * iBlock_Turn;
					}

					iMenu = MENU;
					KillTimer(hwnd, 1); KillTimer(hwnd, 5); KillTimer(hwnd, 6);
				}
			}
		}
		if (wParam == 4)   // frame
		{
			if (iMenu == MENU)
			{
				iMenu_frame = (iMenu_frame + 1) % 2;
			}
			if (iMenu == SELECT)
			{
				iHero_frameX += 1;
				if (iHero_frameX >= 5)
					iHero_frameX = 2;
			}
		}
		if (wParam == 5 && iMenu == GAME)
		{
			if (iGame_Select == GAME_TIME)
			{
				if (iOne_Digit >= 200) //200점이면 초기화
				{
					if (iGame_Select == GAME_TIME && iOne_Digit >= 200)
						GameScore = iOne_Time;
					GameType = iGame_Select;
					Reset();
					for (int i = 0; i < MAX_BLOCK; ++i)
					{
						// 아이템 초기화.
						iItem_Count += (double)ui_ItemPlace(dre);
						if (iItem_Count >= 5.0)
						{
							lItem.emplace_back(iX_Pos, iY_Pos);
							iItem_Count = 0.;
						}
						// 블록 초기화.
						dBlock.emplace_back(iX_Pos, iY_Pos);
						iTemp = (int)nd(dre);

						if (iTemp != 0)
						{
							iBlock_Turn *= -1;
							bJudge[i] = false;      // 판단용.
						}
						else
							bJudge[i] = true;      // 판단용.

						iY_Pos -= 30;
						iX_Pos += 60 * iBlock_Turn;
					}

					iMenu = MENU;
					hDlg = CreateDialog(g_hInst,
						MAKEINTRESOURCE(IDD_DIALOG2), hwnd, Dlg6_1Proc);
					ShowWindow(hDlg, SW_SHOW);
					KillTimer(hwnd, 1); KillTimer(hwnd, 5); KillTimer(hwnd, 6);

				}

			}
			if (bSkill && iGame_Select == GAME_RECORD)
			{
				dTimer += 0.1;
				iHero_frameX++;
				if (!bSkill_On){
					if (iHero_frameX > 4) //스킬모션
					{
						iHero_frameX = 2;
					}
					if (iHero_frameX == 4)
						bSkill_On = true;
				}
				else
				{
					iHero_frameX = 2;
				}
				if (dTimer > 4) //스킬 지속시간
				{
					bSkill = FALSE;
					bSkill_On = false;
					iHero_frameX = 0;
					dTimer = 0;
					if (iHero_Select == 3)
					{
						SetTimer(hwnd, 3, 50, NULL);
					}
				}
			}
		}
		if (wParam == 6 && iGame_Select == GAME_WAVE && iMenu == GAME)
		{
			iScroll_Wave += (iOne_Digit) / 50.0; //난이도
			if (iScroll_Wave > iHeroY - 20)
			{
				Reset();
				iScroll_Wave = 150;
				for (int i = 0; i < MAX_BLOCK; ++i)
				{
					// 아이템 초기화.
					iItem_Count += (double)ui_ItemPlace(dre);
					if (iItem_Count >= 5.0)
					{
						lItem.emplace_back(iX_Pos, iY_Pos);
						iItem_Count = 0.;
					}
					// 블록 초기화.
					dBlock.emplace_back(iX_Pos, iY_Pos);
					iTemp = (int)nd(dre);

					if (iTemp != 0)
					{
						iBlock_Turn *= -1;
						bJudge[i] = false;      // 판단용.
					}
					else
						bJudge[i] = true;      // 판단용.

					iY_Pos -= 30;
					iX_Pos += 60 * iBlock_Turn;
				}

				iMenu = MENU;
				KillTimer(hwnd, 1); KillTimer(hwnd, 5); KillTimer(hwnd, 6);
			}
		}
		if (wParam == 10)
		{
			FMOD_Channel_IsPlaying(g_Channel_Menu, &isPlaying);
			if (isPlaying == 0){
				FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Menu, 0, &g_Channel_Menu);
			}
			FMOD_System_Update(g_System);
		}
		InvalidateRgn(hwnd, NULL, false);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		Basebit = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		SelectObject(memdc, Basebit);
		mem1dc = CreateCompatibleDC(hdc);
		//

		if (iMenu == MENU)
		{
			Menubit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP17));
			SelectObject(mem1dc, Menubit);
			GetObject(Menubit, sizeof(BITMAP), &bmp);
			TransparentBlt(memdc, 0, 0, rect.right, rect.bottom, mem1dc, (iMenu_frame * WINDOW_WIDTH), 0, WINDOW_WIDTH, WINDOW_HEIGHT, SRCCOPY);
		}
		if (iMenu == MANUAL)
		{
			Manualbit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12));

			SelectObject(mem1dc, Manualbit);
			GetObject(Manualbit, sizeof(BITMAP), &bmp);
			TransparentBlt(memdc, 0, 0, rect.right, rect.bottom, mem1dc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SRCCOPY);
		}
		if (iMenu == SELECT)
		{
			Magicbit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
			Thiefbit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));
			Warbit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));
			Select_backbit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP20));
			Modebit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP19));
			Char_Explainbit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));
			Mode_Explainbit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP18));

			SelectObject(mem1dc, Select_backbit);
			GetObject(Select_backbit, sizeof(BITMAP), &bmp);
			TransparentBlt(memdc, 0, 0, rect.right, rect.bottom, mem1dc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SRCCOPY);

			SelectObject(mem1dc, Modebit);
			GetObject(Modebit, sizeof(BITMAP), &bmp);
			TransparentBlt(memdc, 165, 125, 154, 48, mem1dc, iMode_frame * 100, 0, 100, 56, SRCCOPY);


			SelectObject(mem1dc, Char_Explainbit);
			GetObject(Char_Explainbit, sizeof(BITMAP), &bmp);
			TransparentBlt(memdc, 130, 350, 300, 200, mem1dc, (iHero_Select - 1) * 300, 0, 300, 200, RGB(255, 255, 255));

			SelectObject(mem1dc, Mode_Explainbit);
			GetObject(Mode_Explainbit, sizeof(BITMAP), &bmp);
			TransparentBlt(memdc, 130, 170, 300, 200, mem1dc, iMode_frame * 300, 0, 300, 200, RGB(255, 255, 255));

			if (iHero_Select == 3)
			{
				SelectObject(mem1dc, Magicbit);
				GetObject(Magicbit, sizeof(BITMAP), &bmp);
				TransparentBlt(memdc, 118, 500, 250, 200, mem1dc, (iHero_frameX * 250), 0, 250, 200, RGB(190, 253, 253));
			}
			else if (iHero_Select == 2)
			{
				SelectObject(mem1dc, Thiefbit);
				GetObject(Thiefbit, sizeof(BITMAP), &bmp);
				TransparentBlt(memdc, 118, 500 - 7, 250, 200, mem1dc, (iHero_frameX * 250), 0, 250, 200, RGB(90, 65, 160));
			}
			else if (iHero_Select == 1)
			{
				SelectObject(mem1dc, Warbit);
				GetObject(Warbit, sizeof(BITMAP), &bmp);
				TransparentBlt(memdc, 118, 500, 250, 200, mem1dc, (iHero_frameX * 250), 0, 250, 200, RGB(255, 100, 30));
			}
		}


		if (iMenu == GAME)
		{
			Backbit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
			Magicbit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
			Thiefbit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));
			Warbit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));
			Blockbit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
			Numberbit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
			Itembit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));
			MPbit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP13));
			MP_Gagebit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP14));
			Time_gagebit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP15));
			Timebit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP16));
			Wavebit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP9));
			Timerbit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP10));


			SelectObject(mem1dc, Backbit);
			GetObject(Backbit, sizeof(BITMAP), &bmp);
			StretchBlt(memdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, mem1dc, iScrollX, iScrollY, WINDOW_WIDTH, WINDOW_HEIGHT, SRCCOPY);

			// 블록
			SelectObject(mem1dc, Blockbit);
			GetObject(Blockbit, sizeof(BITMAP), &bmp);
			for (auto& d : dBlock)
				TransparentBlt(memdc, d.mX, d.mY, 60, 30, mem1dc, 0, 0, 60, 30, RGB(255, 0, 255));

			if (iGame_Select == GAME_RECORD)
			{
				// 아이템
				SelectObject(mem1dc, Itembit);
				GetObject(Itembit, sizeof(BITMAP), &bmp);
				for (auto& d : lItem)
					TransparentBlt(memdc, d.mX, d.mY - 30, 50, 35, mem1dc, (iItem_frame * 50), 0, 50, 35, RGB(0, 132, 190));
				//MP 게이지창
				SelectObject(mem1dc, MP_Gagebit);
				GetObject(MPbit, sizeof(BITMAP), &bmp);
				TransparentBlt(memdc, 5, 55, 200, 30, mem1dc, 0, 0, 200, 30, RGB(255, 255, 255));
				//MP
				SelectObject(mem1dc, MPbit);
				GetObject(MP_Gagebit, sizeof(BITMAP), &bmp);
				TransparentBlt(memdc, 41, 60, igage, 18, mem1dc, 0, 0, 17, 18, RGB(255, 255, 255)); //gage =160이 꽉찬거
				//시간창
				SelectObject(mem1dc, Time_gagebit);
				GetObject(Time_gagebit, sizeof(BITMAP), &bmp);
				TransparentBlt(memdc, 40, 5, 400, 40, mem1dc, 0, 0, 300, 40, RGB(255, 255, 255));
				//시간
				SelectObject(mem1dc, Timebit);
				GetObject(Timebit, sizeof(BITMAP), &bmp);
				TransparentBlt(memdc, 40, 5, (int)dTime_length, 40, mem1dc, 0, 0, 300, 40, RGB(255, 255, 255)); //gage =160이 꽉찬거
			}
			else if (iGame_Select == GAME_TIME)
			{
				if (!bFall && bGame_start)
					iOne_Time = (clock() - start) / 1000.;
				// 타이머
				SelectObject(mem1dc, Timerbit);
				GetObject(Timerbit, sizeof(BITMAP), &bmp);
				if (iOne_Time < 10)
					TransparentBlt(memdc, rect.right / 2 - 5, 30, 30, 35, mem1dc, (iOne_Time * 60), 0, 60, 70, RGB(20, 20, 20));
				else if (iOne_Time >= 10)
				{
					TransparentBlt(memdc, rect.right / 2 - 15, 30, 30, 35, mem1dc, (iOne_Time / 10 * 60), 0, 60, 70, RGB(20, 20, 20));
					TransparentBlt(memdc, rect.right / 2 + 5, 30, 30, 35, mem1dc, (iOne_Time % 10 * 60), 0, 60, 70, RGB(20, 20, 20));
				}

			}


			// 캐릭터
			if (iHero_Select == 3)
			{
				SelectObject(mem1dc, Magicbit);
				GetObject(Magicbit, sizeof(BITMAP), &bmp);
				TransparentBlt(memdc, iHeroX, iHeroY, 250, 200, mem1dc, (iHero_frameX * 250), (iHero_frameY * 200), 250, 200, RGB(190, 253, 253));
			}
			else if (iHero_Select == 2)
			{
				SelectObject(mem1dc, Thiefbit);
				GetObject(Thiefbit, sizeof(BITMAP), &bmp);
				TransparentBlt(memdc, iHeroX, iHeroY - 7, 250, 200, mem1dc, (iHero_frameX * 250), (iHero_frameY * 200), 250, 200, RGB(90, 65, 160));
			}
			else if (iHero_Select == 1)
			{
				SelectObject(mem1dc, Warbit);
				GetObject(Warbit, sizeof(BITMAP), &bmp);
				TransparentBlt(memdc, iHeroX, iHeroY, 250, 200, mem1dc, (iHero_frameX * 250), (iHero_frameY * 200), 250, 200, RGB(255, 100, 30));
			}

			// 스코어
			SelectObject(mem1dc, Numberbit);
			GetObject(Numberbit, sizeof(BITMAP), &bmp);
			if (iOne_Digit < 10)
				TransparentBlt(memdc, rect.right / 2 - 25, 85, 60, 70, mem1dc, (iOne_Digit * 60), 0, 60, 70, RGB(20, 20, 20));
			else if (iOne_Digit >= 10 && iOne_Digit <= 99)
			{
				TransparentBlt(memdc, rect.right / 2, 85, 60, 70, mem1dc, ((iOne_Digit % 10) * 60), 0, 60, 70, RGB(20, 20, 20));
				TransparentBlt(memdc, rect.right / 2 - 50, 85, 60, 70, mem1dc, ((iOne_Digit / 10) * 60), 0, 60, 70, RGB(20, 20, 20));
			}
			else if (iOne_Digit >= 100)
			{
				TransparentBlt(memdc, rect.right / 2 + 20, 85, 60, 70, mem1dc, ((iOne_Digit % 10) * 60), 0, 60, 70, RGB(20, 20, 20));
				TransparentBlt(memdc, rect.right / 2 - 25, 85, 60, 70, mem1dc, ((iOne_Digit % 100 / 10) * 60), 0, 60, 70, RGB(20, 20, 20));
				TransparentBlt(memdc, rect.right / 2 - 70, 85, 60, 70, mem1dc, ((iOne_Digit / 100) * 60), 0, 60, 70, RGB(20, 20, 20));
			}

			if (iGame_Select == GAME_WAVE)
			{
				SelectObject(mem1dc, Wavebit);
				GetObject(Wavebit, sizeof(BITMAP), &bmp);
				TransparentBlt(memdc, 0, WINDOW_HEIGHT - iScroll_Wave, WINDOW_WIDTH, iScroll_Wave, mem1dc, 0, 0, WINDOW_WIDTH, 200, RGB(255, 0, 255));
			}
			// 치트키 상태 메시지
			if (bCheat_key)
			{
				char Array[100];
				sprintf(Array, "치트키 ON");
				DrawText(memdc, Array, strlen(Array), &rect, DT_SINGLELINE | DT_CENTER);
			}
			//
		}

		BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, memdc, 0, 0, SRCCOPY);
		DeleteObject(Warbit); DeleteObject(Timerbit); DeleteObject(Wavebit);
		DeleteObject(Menubit); DeleteObject(Basebit); DeleteObject(Backbit); DeleteObject(Thiefbit);
		DeleteObject(Blockbit); DeleteObject(Itembit); DeleteObject(Magicbit); DeleteObject(Numberbit); DeleteObject(MPbit);
		DeleteObject(MP_Gagebit); DeleteObject(Time_gagebit); DeleteObject(Timebit);
		DeleteObject(Select_backbit); DeleteObject(Modebit); DeleteObject(Char_Explainbit); DeleteObject(Mode_Explainbit);
		DeleteObject(Manualbit);
		DeleteDC(memdc);
		DeleteDC(mem1dc);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		KillTimer(hwnd, 1);
		KillTimer(hwnd, 2);
		KillTimer(hwnd, 3);
		KillTimer(hwnd, 4);
		KillTimer(hwnd, 5);
		KillTimer(hwnd, 6);
		KillTimer(hwnd, 10);

		SoundRelease();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void Reset()
{
	dBlock.clear(); lItem.clear();   iHeroX = 128; iHeroY = 301; iHero_Turn = 1; iBlock_Turn = -1; iHero_frameX = 0;
	iHero_frameY = 0; iScore = 0; bFall = FALSE; iX_Pos = 180; iY_Pos = 420; iOrigin = 180; iItem_Count = 0; igage = 0;
	bGage = FALSE; iTitle_count = 0; iOne_Digit = 0; iItem_frame = 0; bCheat_key = false;
	iHero_Select = 1;  iScrollY = 1280; iScrollX = 100; dTimer = 0; bSkill = FALSE; bGame_start = FALSE; iScroll_Wave = 150;
	dTime_length = 400; iOne_Time = 0; iGame_Select = 6; iMode_frame = 0; bSkill_On = false; bCtrl = FALSE;
}


BOOL CALLBACK Dlg6_1Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) // 대화상자 메시지 처리함수
{
	static HINSTANCE d_hInst;

	switch (iMsg)
	{
	case WM_INITDIALOG:
		return true;


	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OK:
			GetDlgItemText(hDlg, IDC_INPUT, szID, 20);
			ID = szID;
			vInput.emplace_back(ID, GameScore);

			if (GameType == GAME_RECORD)
			{
				static ofstream out("Rank_Record.txt", ios::app);

				for (auto&d : vInput)
					out << d.first << " " << d.second << endl;
				out.close();
			}
			if (GameType == GAME_WAVE)
			{
				static ofstream out2("Rank_Wave.txt", ios::app);

				for (auto&d : vInput)
					out2 << d.first << " " << d.second << endl;
				out2.close();
			}
			if (GameType == GAME_TIME)
			{
				static ofstream out3("Rank_Time.txt", ios::app);

				for (auto&d : vInput)
					out3 << d.first << " " << d.second << endl;
				out3.close();
			}

			EndDialog(hDlg, 0);
			break;
		}
		InvalidateRect(hDlg, &rect, FALSE);
		break;
	case WM_DESTROY:
		DestroyWindow(hDlg);
		hDlg = NULL;
		break;
	}
	return false;
}

BOOL CALLBACK Dlg6_2Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) // 대화상자 메시지 처리함수
{
	static HINSTANCE d_hInst;
	static HWND hList1, hList2, hList3;
	//HWND hButton1

	switch (iMsg)
	{
	case WM_INITDIALOG:
		hList1 = GetDlgItem(hDlg, IDC_LIST1);
		hList2 = GetDlgItem(hDlg, IDC_LIST2);
		hList3 = GetDlgItem(hDlg, IDC_LIST3);
		vOutput_Record.clear();
		vOutput_Wave.clear();
		vOutput_Time.clear();

		{
			ifstream in("Rank_Record.txt");
			string strTemp{};
			int iTemp{};
			while (in >> strTemp >> iTemp)
			{
				vOutput_Record.emplace_back(strTemp, iTemp);
			}
			sort(vOutput_Record.begin(), vOutput_Record.end(), [](const RANKDATA& a, const RANKDATA& b) ->bool{
				return a.second > b.second;
			});
			in.close();

			vector<RANKDATA>::iterator iter = vOutput_Record.begin();
			for (; iter != vOutput_Record.end();)
			{
				string a = "Player : " + iter->first + " ,점수 : " + to_string(iter->second);
				SendMessage(hList1, LB_ADDSTRING, 0, (LPARAM)a.data());
				++iter;
			}
		}

	  {
		  ifstream in1("Rank_Time.txt");
		  string strTemp{};
		  int iTemp{};
		  while (in1 >> strTemp >> iTemp)
		  {
			  vOutput_Time.emplace_back(strTemp, iTemp);
		  }
		  sort(vOutput_Time.begin(), vOutput_Time.end(), [](const RANKDATA& a, const RANKDATA& b) ->bool{
			  return a.second < b.second;
		  });
		  in1.close();

		  vector<RANKDATA>::iterator iter = vOutput_Time.begin();
		  for (; iter != vOutput_Time.end();)
		  {
			  string a = "Player : " + iter->first + " ,시간 : " + to_string(iter->second) + "초";
			  SendMessage(hList2, LB_ADDSTRING, 0, (LPARAM)a.data());
			  ++iter;
		  }
	  }

	  {
		  ifstream in2("Rank_Wave.txt");
		  string strTemp{};
		  int iTemp{};
		  while (in2 >> strTemp >> iTemp)
		  {
			  vOutput_Wave.emplace_back(strTemp, iTemp);
		  }
		  sort(vOutput_Wave.begin(), vOutput_Wave.end(), [](const RANKDATA& a, const RANKDATA& b) ->bool{
			  return a.second > b.second;
		  });
		  in2.close();

		  vector<RANKDATA>::iterator iter = vOutput_Wave.begin();
		  for (; iter != vOutput_Wave.end();)
		  {
			  string a = "Player : " + iter->first + " ,점수 : " + to_string(iter->second);
			  SendMessage(hList3, LB_ADDSTRING, 0, (LPARAM)a.data());
			  ++iter;
		  }
	  }
	  return true;



	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case IDCANCEL:   // 창 'X'
			DestroyWindow(hDlg);
			hDlg = NULL;
			EndDialog(hDlg, 0);
			break;
		}
		InvalidateRect(hDlg, &rect, FALSE);
		break;
	case WM_DESTROY:
		DestroyWindow(hDlg);
		hDlg = NULL;
		break;
	}
	return false;
}