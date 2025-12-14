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


string g_InputFileName = "";
image* g_pImg = NULL;   
viewbox* g_vb = NULL;  

// --- KHAI BÁO HÀM ---
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK IntroWndProc(HWND, UINT, WPARAM, LPARAM);
void ShowWelcomeWindow(HINSTANCE hInstance);
bool OpenFileDialog(string& selectedPath);


// --- XỬ LÝ CỬA SỔ CHÀO MỪNG (INTRO) - PHIÊN BẢN PRO ---
LRESULT CALLBACK IntroWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        Graphics graphics(hdc);

        // Bật chế độ khử răng cưa để chữ và hình nét mịn
        graphics.SetSmoothingMode(SmoothingModeAntiAlias);
        graphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);

        RECT rect; GetClientRect(hWnd, &rect);
        float winW = (float)rect.right;
        float winH = (float)rect.bottom;

        // --- 1. VẼ ẢNH NỀN (FULL CỬA SỔ) ---
        Image bgImage(L"intro_bg.png");
        if (bgImage.GetLastStatus() == Ok) {
            graphics.DrawImage(&bgImage, 0, 0, rect.right, rect.bottom);
        }
        else {
            // Gradient nền xanh tím hiện đại nếu không có ảnh
            LinearGradientBrush bgBrush(Point(0, 0), Point(winW, winH),
                Color(255, 100, 149, 237), Color(255, 123, 104, 238));
            graphics.FillRectangle(&bgBrush, 0, 0, rect.right, rect.bottom);
        }

        // --- 2. VẼ KHUNG (CARD) Ở GIỮA ---
        // Kích thước khung: Rộng 700px, Cao 500px (Nhỏ hơn cửa sổ chính)
        float cardW = 700.0f;
        float cardH = 550.0f;
        float cardX = (winW - cardW) / 2.0f;
        float cardY = (winH - cardH) / 2.0f;

        // Vẽ bóng đổ cho khung (Shadow) để tạo độ nổi
        SolidBrush shadowBrush(Color(60, 0, 0, 0)); // Màu đen mờ
        graphics.FillRectangle(&shadowBrush, cardX + 10, cardY + 10, cardW, cardH);

        // Vẽ nền khung (Màu trắng hơi trong suốt)
        SolidBrush cardBrush(Color(245, 255, 255, 255));
        graphics.FillRectangle(&cardBrush, cardX, cardY, cardW, cardH);

        // Vẽ viền khung
        Pen borderPen(Color(255, 70, 130, 180), 2);
        graphics.DrawRectangle(&borderPen, cardX, cardY, cardW, cardH);

        // --- 3. ĐỊNH DẠNG FONT CHỮ ---
        FontFamily fontFamily(L"Segoe UI"); // Font chuẩn đẹp của Windows

        Font titleFont(&fontFamily, 32, FontStyleBold, UnitPixel);      // Tiêu đề to, đậm
        Font headerFont(&fontFamily, 20, FontStyleBold, UnitPixel);     // Đầu mục đậm
        Font bodyFont(&fontFamily, 18, FontStyleRegular, UnitPixel);    // Nội dung thường
        Font italicFont(&fontFamily, 18, FontStyleItalic, UnitPixel);   // Nội dung nghiêng
        Font footerFont(&fontFamily, 16, FontStyleItalic | FontStyleBold, UnitPixel); // Footer

        SolidBrush titleBrush(Color(255, 0, 51, 102));    // Xanh đậm
        SolidBrush headerBrush(Color(255, 178, 34, 34));  // Đỏ gạch
        SolidBrush textBrush(Color(255, 30, 30, 30));     // Đen xám
        SolidBrush footerBrush(Color(255, 255, 69, 0));   // Đỏ cam

        StringFormat centerFormat;
        centerFormat.SetAlignment(StringAlignmentCenter);

        // --- 4. VẼ NỘI DUNG VĂN BẢN (Từng dòng một để chỉnh style) ---

        float currentY = cardY + 30; // Bắt đầu vẽ từ trên xuống

        // A. TIÊU ĐỀ
        graphics.DrawString(L"SVG READER - HƯỚNG DẪN", -1, &titleFont,
            RectF(cardX, currentY, cardW, 50), &centerFormat, &titleBrush);
        currentY += 60;

        // B. LỜI CHÀO (In nghiêng)
        graphics.DrawString(L"Xin chào, đây là chương trình đọc ảnh SVG. Dưới đây là hướng dẫn sử dụng.", -1, &italicFont,
            RectF(cardX, currentY, cardW, 30), &centerFormat, &textBrush);
        currentY += 50;

        // Canh lề trái cho phần nội dung chi tiết (Thụt vào 50px)
        float contentLeft = cardX + 80;

        // C. MỤC 1: CAMERA
        graphics.DrawString(L"🎮 ĐIỀU KHIỂN CAMERA:", -1, &headerFont, PointF(contentLeft, currentY), &headerBrush);
        currentY += 30;

        wstring controls = L"• Phím 'I' / 'O': Phóng to / Thu nhỏ\n"
            L"• Phím Mũi tên: Di chuyển (Pan)\n"
            L"• Phím 'R' / 'L': Xoay hình\n"
            L"• Phím 'D': Reset về mặc định";
        graphics.DrawString(controls.c_str(), -1, &bodyFont, PointF(contentLeft + 20, currentY), &textBrush);
        currentY += 100;

        // D. MỤC 2: LƯU Ý
        graphics.DrawString(L"💡 LƯU Ý QUAN TRỌNG:", -1, &headerFont, PointF(contentLeft, currentY), &headerBrush);
        currentY += 30;

        wstring notes = L"• Lăn chuột để Zoom nhanh hơn.\n"
            L"• Click chuột trái vào hình để chọn (Select).";
        graphics.DrawString(notes.c_str(), -1, &bodyFont, PointF(contentLeft + 20, currentY), &textBrush);

        // E. FOOTER (Dưới đáy khung)
        graphics.DrawString(L"--> CLICK CHUỘT BẤT KỲ ĐỂ BẮT ĐẦU <--", -1, &footerFont,
            RectF(cardX, cardY + cardH - 40, cardW, 40), &centerFormat, &footerBrush);

        EndPaint(hWnd, &ps);
    } break;

    case WM_LBUTTONDOWN:
    case WM_KEYDOWN:
        PostMessage(hWnd, WM_CLOSE, 0, 0);
        break;

    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void ShowWelcomeWindow(HINSTANCE hInstance) {
    WNDCLASSEXW wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = IntroWndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.lpszClassName = L"IntroWindowClass";
    RegisterClassExW(&wcex);

    // Kích thước cửa sổ nền TO (Full HD hoặc gần Full màn hình để đẹp)
    // Khung chữ sẽ nằm gọn ở giữa cửa sổ to này.
    int w = 1000;
    int h = 750;

    int x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
    int y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

    HWND hWnd = CreateWindowW(L"IntroWindowClass", L"Welcome", WS_POPUP | WS_VISIBLE,
        x, y, w, h, nullptr, nullptr, hInstance, nullptr);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// =============================================================
// PHẦN 2: CHƯƠNG TRÌNH CHÍNH (MAIN)
// =============================================================

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplustoken;
    GdiplusStartup(&gdiplustoken, &gdiplusStartupInput, nullptr);

    ShowWelcomeWindow(hInstance);

    // 3. CHỌN FILE
    if (!OpenFileDialog(g_InputFileName)) {
        GdiplusShutdown(gdiplustoken);
        return 0; // Thoát nếu không chọn file
    }

    // 4. PARSE FILE SVG (CHỈ 1 LẦN DUY NHẤT TẠI ĐÂY)
    g_vb = new viewbox();
    g_pImg = new image(g_InputFileName);
    parser parseTool;
    g_pImg->parseImage(parseTool, *g_vb);

    // 5. TẠO CỬA SỔ CHÍNH
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

    // Icon
    HANDLE hIcon = LoadImage(NULL, L"favicon.png", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    wcex.hIcon = hIcon ? (HICON)hIcon : LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassExW(&wcex)) return FALSE;

    string title = "SVG Reader - " + g_InputFileName;
    wstring wTitle(title.begin(), title.end());
    HWND hWnd = CreateWindowW(L"SVGReaderClass", wTitle.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // 6. VÒNG LẶP TIN NHẮN CHÍNH
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 7. DỌN DẸP BỘ NHỚ
    if (g_pImg) delete g_pImg;
    if (g_vb) delete g_vb;
    GdiplusShutdown(gdiplustoken);

    return (int)msg.wParam;
}

// =============================================================
// PHẦN 3: XỬ LÝ LOGIC CỬA SỔ CHÍNH
// =============================================================

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Tính toán AutoScale dùng chung
    RECT rect; GetWindowRect(hWnd, &rect);
    float WinW = (float)(rect.right - rect.left - 16);
    float WinH = (float)(rect.bottom - rect.top - 39);
    float svgW = (g_vb) ? g_vb->getViewWidth() : 0;
    float svgH = (g_vb) ? g_vb->getViewHeight() : 0;

    float autoScale = 1.0f;
    if (svgW > 0 && svgH > 0 && WinW > 0 && WinH > 0) {
        float sx = WinW / svgW;
        float sy = WinH / svgH;
        autoScale = (sx < sy) ? sx : sy;
    }
    float totalScale = scale * autoScale;

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
        InvalidateRect(hWnd, NULL, TRUE); // Vẽ lại màn hình
        break;

    case WM_MOUSEWHEEL:
        if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) scale *= 1.1f;
        else scale *= 0.9f;
        InvalidateRect(hWnd, NULL, TRUE);
        break;

        // ĐÃ BỎ CASE WM_LBUTTONDOWN ĐỂ TRÁNH LỖI SELECT

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        if (g_pImg) {
            Graphics graphics(hdc);
            graphics.SetSmoothingMode(SmoothingModeAntiAlias);

            // Áp dụng biến đổi toàn cục
            graphics.TranslateTransform(scroll_x, scroll_y);
            graphics.RotateTransform(Rotate);
            graphics.ScaleTransform(totalScale, totalScale);

            renderer renderTool;
            g_pImg->renderImage(renderTool, graphics);
        }
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

// --- HÀM MỞ FILE ---
bool OpenFileDialog(string& selectedPath) {
    OPENFILENAMEA ofn;
    char szFile[260] = { 0 };
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "SVG Files\0*.svg\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    if (GetOpenFileNameA(&ofn) == TRUE) {
        selectedPath = string(szFile);
        return true;
    }
    return false;
}