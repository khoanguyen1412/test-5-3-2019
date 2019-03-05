/* =============================================================
P03- Simon
Controls:
	Jump: space
	Move Right: Right arrow
	Move Left: Left arrow
	Sit: Down arrow
	Stand-Attack: X
	Sit-Attack: Down arrow + X
================================================================ */

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "Game.h"
#include "GameObject.h"
#include "Textures.h"

#include "Simon.h"

#define WINDOW_CLASS_NAME L"Simon"
#define MAIN_WINDOW_TITLE L"P03-Simon"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(200, 200, 255)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define MAX_FRAME_RATE 90

#define ID_TEX_SIMON 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20

CGame *game;
CSimon *simon;

class CSampleKeyHander : public CKeyEventHandler
{
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander * keyHandler;

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	switch (KeyCode)
	{
	case DIK_SPACE:
		simon->SetState(SIMON_STATE_JUMP);
		break;
	case DIK_X:
		if (simon->lastState == SIMON_STATE_SIT_RIGHT)
			simon->SetState(SIMON_ANI_SIT_HIT_RIGHT);
		else if (simon->lastState == SIMON_STATE_SIT_LEFT)
			simon->SetState(SIMON_ANI_SIT_HIT_LEFT);
		else simon->lastState = 0;
		break;
	}

}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	simon->lastState = 0;
}

void CSampleKeyHander::KeyState(BYTE *states)
{

	if (simon->isHitting) return;
	if (game->IsKeyDown(DIK_RIGHT))
		simon->SetState(SIMON_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		simon->SetState(SIMON_STATE_WALKING_LEFT);

	else if (game->IsKeyDown(DIK_DOWN))
	{
		if (simon->GetDirection()>0)
			simon->SetState(SIMON_STATE_SIT_RIGHT);
		else simon->SetState(SIMON_STATE_SIT_LEFT);
	}
	else if (game->IsKeyDown(DIK_X))
	{
		if (simon->GetDirection()>0)
			simon->SetState(SIMON_STATE_STAND_HIT_RIGHT);
		else simon->SetState(SIMON_STATE_STAND_HIT_LEFT);
	}
	else simon->SetState(SIMON_STATE_IDLE);

}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
Load all game resources
In this example: load textures, sprites, animations and simon object
*/

void LoadResources()
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_SIMON, L"textures\\simon.png", D3DCOLOR_XRGB(176, 224, 248));

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);

	sprites->Add(0, 164, 41, 179, 70, texSimon, 0, 0);

	sprites->Add(1, 206, 40, 217, 70, texSimon, 0, 0);
	sprites->Add(2, 245, 41, 258, 70, texSimon, 0, 0);

	sprites->Add(3, 124, 41, 139, 70, texSimon, 0, 0);

	sprites->Add(4, 86, 40, 97, 70, texSimon, 0, 0);
	sprites->Add(5, 45, 41, 59, 70, texSimon, 0, 0);
	//Sit down
	sprites->Add(6, 284, 37, 299, 66, texSimon, 0, 0);
	sprites->Add(7, 4, 37, 19, 66, texSimon, 0, 0);
	//stand-Hit
	sprites->Add(8, 156, 360, 187, 389, texSimon, -16.0f, 0.0f);
	sprites->Add(9, 196, 361, 227, 390, texSimon, -16.0f, 0.0f);
	sprites->Add(10, 244, 361, 303, 389, texSimon, 0.0f, 0.0f);
	sprites->Add(11, 116, 360, 147, 389, texSimon, 0, 0);
	sprites->Add(12, 76, 361, 107, 390, texSimon, 0, 0);
	sprites->Add(13, 0, 361, 59, 389, texSimon, -44, 0);
	//Sit-Hit
	sprites->Add(14, 156, 520, 187, 550, texSimon, -16, 8);
	sprites->Add(15, 196, 524, 227, 546, texSimon, -16, 0);
	sprites->Add(16, 242, 525, 301, 546, texSimon, 0, 0);
	sprites->Add(17, 116, 520, 147, 550, texSimon, 0, 8);
	sprites->Add(18, 76, 524, 107, 546, texSimon, 0, 4);
	sprites->Add(19, 2, 525, 61, 546, texSimon, -44, 4);


	LPANIMATION ani;
	//Stand-Right
	ani = new CAnimation(100);
	ani->Add(0);
	animations->Add(0, ani);

	//Stand-Left
	ani = new CAnimation(100);
	ani->Add(3);
	animations->Add(1, ani);

	//Walk-Right
	ani = new CAnimation(100);
	ani->Add(0);
	ani->Add(1);
	ani->Add(2);
	animations->Add(2, ani);

	//Walk-Left
	ani = new CAnimation(100);
	ani->Add(3);
	ani->Add(4);
	ani->Add(5);
	animations->Add(3, ani);

	//Sit-Right
	ani = new CAnimation(100);
	ani->Add(6);
	animations->Add(4, ani);

	//Sit-Left
	ani = new CAnimation(100);
	ani->Add(7);
	animations->Add(5, ani);

	//Stand-Hit-Right
	ani = new CAnimation(100);
	ani->Add(8);
	ani->Add(9);
	ani->Add(10);
	animations->Add(6, ani);

	//Stand-Hit-Left
	ani = new CAnimation(100);
	ani->Add(11);
	ani->Add(12);
	ani->Add(13);
	animations->Add(7, ani);

	//Sit-Hit-Right
	ani = new CAnimation(100);
	ani->Add(14);
	ani->Add(15);
	ani->Add(16);
	animations->Add(8, ani);

	//Sit-Hit-Left
	ani = new CAnimation(100);
	ani->Add(17);
	ani->Add(18);
	ani->Add(19);
	animations->Add(9, ani);

	simon = new CSimon();
	CSimon::AddAnimation(0);		// idle right
	CSimon::AddAnimation(1);		// idle left
	CSimon::AddAnimation(2);		// walk right
	CSimon::AddAnimation(3);		// walk left
	CSimon::AddAnimation(4);		// sit right
	CSimon::AddAnimation(5);		// sit left
	CSimon::AddAnimation(6);		// stand hit right
	CSimon::AddAnimation(7);		// stand hit left
	CSimon::AddAnimation(8);		// sit hit right
	CSimon::AddAnimation(9);		//sit hit left

	simon->SetPosition(0.0f, 100.0f);
}

/*
Update world status for this frame
dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	simon->Update(dt);
}

/*
Render a frame
*/
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		simon->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();

			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);


	LoadResources();
	Run();

	return 0;
}