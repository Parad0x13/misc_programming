#ifndef INITWIN32_H
#define INITWIN32_H

#include "main.h"
#include "InitOGL.h"

// This next variable allows for the Menu Structure to be created with the resource file, or directly here

LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ToolDlgProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

HWND g_hToolbar = NULL;			// For the toolbar

/* If the program is to include ToolDlgProc then It cannot have the DOS box unfortunatly */
BOOL CALLBACK ToolDlgProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_EXIT:
//					PostMessage(hWnd, WM_CLOSE, 0, 0);			// Don't know why this doesnt work
					exit(0);
				break;

				case ID_TOOLBAR_HIDE:
					ShowWindow(g_hToolbar, SW_HIDE);
				break;

				case IDC_BTN2:
					reset();
				break;
			}
		break;

		default:
			return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK AboutDlgProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case WM_INITDIALOG:
			// Apparantly do nothing...
		return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK:
//					MessageBox(hwnd, "Dialog exited with IDOK.", "Notice", MB_OK | MB_ICONINFORMATION);
					printf("OKAY!!! :)\n");
					EndDialog(hWnd, IDOK);
				break;

				case IDCANCEL:
//					MessageBox(hwnd, "Dialog exited with IDCANCEL.", "Notice", MB_OK | MB_ICONINFORMATION);
					printf("NOKAY!!! :(\n");
					EndDialog(hWnd, IDCANCEL);
				break;
			}
		break;

		default:
			return FALSE;
	}
	return TRUE;
}

LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// Added just for fun for now... Can allways be used as a referance later
		case WM_LBUTTONDOWN:
		{
			char szFileName[MAX_PATH];
			HINSTANCE hInstance = GetModuleHandle(NULL);
			GetModuleFileName(hInstance, szFileName, MAX_PATH);
//			MessageBox(hWnd, szFileName, "This program is located at:", MB_OK | MB_ICONINFORMATION);
		}
			break;

		case WM_CREATE:
			g_hToolbar = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_TOOLBAR), hWnd, ToolDlgProc);
			if(g_hToolbar == NULL)
				MessageBox(hWnd, "Toolbar will not able to be created for some reason", "Warning!", MB_OK | MB_ICONINFORMATION);
			return 0;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case ID_FILE_EXIT:
					PostMessage(hWnd, WM_CLOSE, 0, 0);
				break;

				case ID_HELP_ABOUT:
					DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT), hWnd, AboutDlgProc);
				break;

				case ID_TOOLBAR_SHOW:
					ShowWindow(g_hToolbar, SW_SHOW);
				break;

				case ID_TOOLBAR_HIDE:
					ShowWindow(g_hToolbar, SW_HIDE);
				break;

				case ID_MANDELBROT:
					fractal_type=FRACT_MANDELBROT;
					reDisplay=true;
				break;

				case ID_MANDELBAR:
					fractal_type=FRACT_MANDELBAR;
					reDisplay=true;
				break;

				case ID_JULIA:
					fractal_type=FRACT_JULIA;
					reDisplay=true;
				break;

				case ID_HENON:
					fractal_type=FRACT_HENON;
					reDisplay=true;
				break;

				case ID_TINKERBELL:
					fractal_type=FRACT_TINKERBELL;
					reDisplay=true;
				break;

				case ID_LORENZ:
					fractal_type=FRACT_LORENZ;
					reDisplay=true;
				break;

				case ID_RABINOVICHFABRIKANT:
					fractal_type=FRACT_RABINOVICHFABRIKANT;
					reDisplay=true;
				break;

				case ID_HADLEY:
					fractal_type=FRACT_HADLEY;
					reDisplay=true;
				break;

				case ID_ROSSLER:
					fractal_type=FRACT_ROSSLER;
					reDisplay=true;
				break;

				case ID_ACT:
					fractal_type=FRACT_ACT;
					reDisplay=true;
				break;

				case ID_RUCKLIDGE:
					fractal_type=FRACT_RUCKLIDGE;
					reDisplay=true;
				break;

				case ID_IFS_SIERPINSKI:
					fractal_type=FRACT_IFS_SIERPINSKI;
					reDisplay=true;
				break;

				case ID_IFS_TRIANGLES:
					fractal_type=FRACT_IFS_TRIANGLES;
					reDisplay=true;
				break;

				case ID_IFS_PENTAGON:
					fractal_type=FRACT_IFS_PENTAGON;
					reDisplay=true;
				break;

				case ID_ESCAPETIME:
					shadeT=ESCAPETIME;
					reDisplay=true;
				break;

				case ID_NORMALIZE_1:
					shadeT=NORMALIZE_1;
					reDisplay=true;
				break;

				case ID_NORMALIZE_1_V1:
					shadeT=NORMALIZE_1_V1;
					reDisplay=true;
				break;

				case ID_NORMALIZE_1_V2:
					shadeT=NORMALIZE_1_V2;
					reDisplay=true;
				break;

				case ID_NORMALIZE_1_V3:
					shadeT=NORMALIZE_1_V3;
					reDisplay=true;
				break;

				case ID_NORMALIZE_1_V4:
					shadeT=NORMALIZE_1_V4;
					reDisplay=true;
				break;

				case ID_NORMALIZE_2:
					shadeT=NORMALIZE_2;
					reDisplay=true;
				break;
			}
		break;

		case WM_CLOSE:
			PostQuitMessage (0);
			return 0;

		case WM_DESTROY:
			DestroyWindow(g_hToolbar);
			return 0;

		case WM_KEYDOWN:
			switch (wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				return 0;
			case VK_F1:
				KillGLWindow();
				fullscreen=!fullscreen;
				// Recreate Our OpenGL Window
				if(fullscreen==TRUE)
				{
					if (!CreateGLWindow(ProgramHeader, 1280, 800, 0, 0, 32, fullscreen))	// Computer at home
//					if (!CreateGLWindow(ProgramHeader, 1024, 768, 0, 0, 32, fullscreen))
						return 0;					// Quit If Window Was Not Created
				}
				if(fullscreen==FALSE)
				{
					ScreenX=300;
					ScreenY=300;
					if (!CreateGLWindow(ProgramHeader, ScreenX, ScreenY, OffsetX, OffsetY, 16, fullscreen))
						return 0;					// Quit If Window Was Not Created
				}
			break;
			case 'R':
				theta+=5;
				reDisplay=true;
			break;
			case 'F':
				reset();
			break;
			case'T':
				shadeT=NORMALIZE_1;
				reDisplay=true;
			break;
			case 'Y':
				shadeT=NORMALIZE_1_V3;
				reDisplay=true;
			break;
			case 'U':
				shadeT=NORMALIZE_1_V4;
				reDisplay=true;
			break;
			case 'Q':
				changeGraph(1);
			break;
			case 'E':
				changeGraph(2);
			break;
			case 'W':
				changeGraph(3);
			break;
			case 'S':
				changeGraph(4);
			break;
			case 'A':
				changeGraph(5);
			break;
			case 'D':
				changeGraph(6);
			break;
			}
			return 0;
		break;
		case WM_SIZE:										// Resize The OpenGL Window
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));	// LoWord=Width, HiWord=Height
			return 0;

		case WM_HSCROLL:
		{
			int nPos=(short int)HIWORD(wParam);
			printf("Yes?\n");
			exit(0);
		}

		default:
			return DefWindowProc (hWnd, message, wParam, lParam);
			break;
	}
}

#endif
