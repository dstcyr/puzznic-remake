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


// LEVEL 1 - 2 : VPP3
// LEVEL 1 - 3 : V33C
// LEVEL 1 - 4 : VJJ7
// LEVEL 1 - 5 : VCCB
// LEVEL 1 - 6 : VRR5
// LEVEL 1 - 7 : V77L
// LEVEL 1 - 8 : VZZW
// LEVEL 1 - 9 : VBBV
// LEVEL 1 - 10: VMM3

// LEVEL 2 - 1 : V55C
// LEVEL 2 - 2 : VFF7
// LEVEL 2 - 3 : VLLB
// LEVEL 2 - 4 : VTT5
// LEVEL 2 - 5 : VWWL
// LEVEL 2 - 6 : 
// LEVEL 2 - 7 : PPV3
// LEVEL 2 - 8 : P3PC
// LEVEL 2 - 9 : 
// LEVEL 2 - 10: 

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
