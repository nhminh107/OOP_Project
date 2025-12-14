#include "Library.h"
#include <commdlg.h>

// Link thư viện GDI+ và Common Dialog
#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib, "Comdlg32.lib") 

using namespace std;
using namespace Gdiplus;


// =============================================================
// PHẦN 2: CHƯƠNG TRÌNH CHÍNH (MAIN)
// =============================================================

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
{
    if (!InitGDI())
        return -1;

    ShowWelcomeWindow(hInstance);

    // ===== GIỮ NGUYÊN THEO YÊU CẦU =====
    if (!FileExplorer::OpenFileDialog(g_InputFileName)) {
        ShutdownGDI();
        return 0;
    }
    // ==================================

    InitSVGData();

    HWND hWnd = CreateMainWindow(hInstance, nCmdShow);
    if (!hWnd) {
        Cleanup();
        ShutdownGDI();
        return -1;
    }

    MessageLoop();

    Cleanup();
    ShutdownGDI();

    return 0;
}



