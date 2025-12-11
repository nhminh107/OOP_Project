#include "Library.h"
#include <commdlg.h>

// Link thư viện GDI+ và Common Dialog
#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib, "Comdlg32.lib") 

using namespace std;
using namespace Gdiplus;

// --- CÁC BIẾN TOÀN CỤC ---
float scale = 1.0f;
float Rotate = 0.0f;
float scroll_x = 0.0f;
float scroll_y = 0.0f;

// Tên file mặc định (sẽ được ghi đè khi chọn file)
string g_InputFileName = "";

// --- KHAI BÁO HÀM ---
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
bool OpenFileDialog(string& selectedPath); // Hàm phụ trợ để mở hộp thoại

// --- HÀM CHÍNH ---
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    // 1. HIỆN HỘP THOẠI CHỌN FILE
    // Nếu người dùng chọn file thành công thì mới chạy tiếp, nếu Cancel thì thoát luôn.
    if (!OpenFileDialog(g_InputFileName)) {
        return 0; // Thoát chương trình nếu không chọn file
    }

    // 2. KHỞI TẠO GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplustoken;
    GdiplusStartup(&gdiplustoken, &gdiplusStartupInput, nullptr);

    // 3. ĐĂNG KÝ LỚP CỬA SỔ
    WNDCLASSEXW wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"SVGReaderClass";
    wcex.lpszMenuName = nullptr;

    // Load icon small.ico trực tiếp
    HANDLE hIcon = LoadImage(NULL, L"small.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    wcex.hIcon = hIcon ? (HICON)hIcon : LoadIcon(NULL, IDI_APPLICATION);

    HANDLE hIconSm = LoadImage(NULL, L"small.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
    wcex.hIconSm = hIconSm ? (HICON)hIconSm : LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassExW(&wcex)) return FALSE;

    // 4. TẠO CỬA SỔ
    // Tiêu đề cửa sổ sẽ hiện tên file luôn cho chuyên nghiệp
    string title = "SVG Reader - " + g_InputFileName;
    wstring wTitle(title.begin(), title.end());

    HWND hWnd = CreateWindowW(L"SVGReaderClass", wTitle.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // 5. MESSAGE LOOP
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplustoken);
    return (int)msg.wParam;
}

// --- HÀM PHỤ TRỢ: MỞ HỘP THOẠI WINDOWS ---
bool OpenFileDialog(string& selectedPath) {
    OPENFILENAMEA ofn;       // Cấu trúc chứa thông tin hộp thoại (Dùng bản 'A' cho string)
    char szFile[260] = { 0 }; // Bộ đệm lưu tên file

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);

    // Bộ lọc chỉ hiện file SVG hoặc tất cả file
    ofn.lpstrFilter = "SVG Files\0*.svg\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    // Hiển thị hộp thoại Open
    if (GetOpenFileNameA(&ofn) == TRUE) {
        selectedPath = string(szFile);
        return true;
    }
    return false;
}

// --- HÀM XỬ LÝ SỰ KIỆN CỬA SỔ (GIỮ NGUYÊN NHƯ CŨ) ---
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_UP:    scroll_y -= 20; break;
        case VK_DOWN:  scroll_y += 20; break;
        case VK_LEFT:  scroll_x -= 20; break;
        case VK_RIGHT: scroll_x += 20; break;
        case 'I': case 'i': scale *= 1.1f; break;
        case 'O': case 'o': scale *= 0.9f; break;
        case 'R': case 'r': Rotate += 5.0f; break;
        case 'L': case 'l': Rotate -= 5.0f; break;
        case 'D': case 'd': scale = 1.0f; Rotate = 0.0f; scroll_x = 0; scroll_y = 0; break;
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case WM_MOUSEWHEEL:
        if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) scale *= 1.1f;
        else scale *= 0.9f;
        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        image img(g_InputFileName); // File đã được chọn từ hộp thoại
        parser parseTool;
        renderer renderTool;
        viewbox* vb = new viewbox();

        img.parseImage(parseTool, *vb);

        // --- Logic tính toán Scale Fit Window ---
        RECT rect; GetWindowRect(hWnd, &rect);
        float WinW = (float)(rect.right - rect.left - 16);
        float WinH = (float)(rect.bottom - rect.top - 39);
        float svgW = vb->getViewWidth();
        float svgH = vb->getViewHeight();
        float autoScale = 1.0f;

        if (svgW > 0 && svgH > 0 && WinW > 0 && WinH > 0) {
            float sx = WinW / svgW;
            float sy = WinH / svgH;
            autoScale = (sx < sy) ? sx : sy;
        }

        Graphics graphics(hdc);
        graphics.SetSmoothingMode(SmoothingModeAntiAlias);

        graphics.TranslateTransform(scroll_x, scroll_y);
        graphics.RotateTransform(Rotate);
        graphics.ScaleTransform(scale * autoScale, scale * autoScale);

        img.renderImage(renderTool, graphics);

        delete vb;
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}