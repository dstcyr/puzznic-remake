#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <conio.h>
#include "StateMachine.h"
#include "MenuState.h"
#include "GameState.h"
#include "Engine.h"
#include <string>
#include "Log.h"
#include "Config.h"

#if CHECK_MEMORY_LEAKS
#pragma comment(lib, "C:\\vld\\lib\\Win64\\vld.lib")
#include "C:\\vld\\include\\vld.h"
#endif

INT WINAPI WinMain(HINSTANCE _In_ hInst, HINSTANCE _In_opt_ hPrev, PSTR _In_ cmd, INT _In_ flag)
{
    std::string title = "Puzznic Remake";

#if CREATE_DEBUG_CONSOLE
    AllocConsole();
    int _r = freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
    if (_r != 0)
    {
        OutputDebugString("error: can't allocate console");
    }

    title += " - DEBUG";
#endif

    StateMachine fsm;
    fsm.Add("menu", new MenuState());
    fsm.Add("game", new GameState());

    Engine::Initialize(title.c_str(), 1024, 962);
#if START_IN_GAME
    fsm.SetState("game");
#else
    fsm.SetState("menu");
#endif

    Engine::EnterGameLoop(&fsm);

#if PAUSE_CONSOLE_WHEN_CLOSING
    LOG(LL_VERBOSE, "Press any key to continue...");
    int r = _getch();
#endif

#if CREATE_DEBUG_CONSOLE
    FreeConsole();
#endif

    return 0;
}
