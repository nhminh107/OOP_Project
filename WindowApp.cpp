#include "Library.h"

//CÁC HÀM CON
static ULONG_PTR g_gdiplusToken;

bool InitGDI() {
    GdiplusStartupInput input;
    return GdiplusStartup(&g_gdiplusToken, &input, nullptr) == Ok;
}

void ShutdownGDI() {
    GdiplusShutdown(g_gdiplusToken);
}
void InitSVGData() {
    g_vb = new viewbox();
    g_pImg = new image(g_InputFileName);
    parser parseTool;
    g_pImg->parseImage(parseTool, *g_vb);
}
HWND CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASSEXW wcex = { sizeof(WNDCLASSEX) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"SVGReaderClass";

    HANDLE hIcon = LoadImage(NULL, L"favicon.png", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    wcex.hIcon = hIcon ? (HICON)hIcon : LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassExW(&wcex)) return NULL;

    string title = "SVG Reader - " + g_InputFileName;
    wstring wTitle(title.begin(), title.end());

    HWND hWnd = CreateWindowW(
        L"SVGReaderClass",
        wTitle.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0,
        CW_USEDEFAULT, 0,
        nullptr, nullptr,
        hInstance, nullptr
    );

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return hWnd;
}
void MessageLoop() {
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
void Cleanup() {
    if (g_pImg) {
        delete g_pImg;
        g_pImg = nullptr;
    }
    if (g_vb) {
        delete g_vb;
        g_vb = nullptr;
    }
}

// ===== ĐỊNH NGHĨA BIẾN TOÀN CỤC =====
float scale = 1.0f;
float Rotate = 0.0f;
float scroll_x = 0.0f;
float scroll_y = 0.0f;
bool g_ShowGrid = false;
string g_InputFileName = "";
image* g_pImg = nullptr;
viewbox* g_vb = nullptr;

// ĐỊNH NGHĨA HÀM
// --- XỬ LÝ CỬA SỔ CHÀO MỪNG (INTRO) - PHIÊN BẢN PRO (UPDATED) ---
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
            LinearGradientBrush bgBrush(Point(0, 0), Point(winW, winH),
                Color(255, 100, 149, 237), Color(255, 123, 104, 238));
            graphics.FillRectangle(&bgBrush, 0, 0, rect.right, rect.bottom);
        }

        // --- 2. VẼ KHUNG (CARD) Ở GIỮA ---
        float cardW = 700.0f;
        float cardH = 600.0f; // <--- TĂNG CHIỀU CAO LÊN 600 ĐỂ CHỨA ĐỦ NỘI DUNG
        float cardX = (winW - cardW) / 2.0f;
        float cardY = (winH - cardH) / 2.0f;

        // Vẽ bóng đổ cho khung
        SolidBrush shadowBrush(Color(60, 0, 0, 0));
        graphics.FillRectangle(&shadowBrush, cardX + 10, cardY + 10, cardW, cardH);

        // Vẽ nền khung
        SolidBrush cardBrush(Color(245, 255, 255, 255));
        graphics.FillRectangle(&cardBrush, cardX, cardY, cardW, cardH);

        // Vẽ viền khung
        Pen borderPen(Color(255, 70, 130, 180), 2);
        graphics.DrawRectangle(&borderPen, cardX, cardY, cardW, cardH);

        // --- 3. ĐỊNH DẠNG FONT CHỮ ---
        FontFamily fontFamily(L"Segoe UI");

        Font titleFont(&fontFamily, 32, FontStyleBold, UnitPixel);
        Font headerFont(&fontFamily, 20, FontStyleBold, UnitPixel);
        Font bodyFont(&fontFamily, 18, FontStyleRegular, UnitPixel);
        Font italicFont(&fontFamily, 18, FontStyleItalic, UnitPixel);
        Font footerFont(&fontFamily, 16, FontStyleItalic | FontStyleBold, UnitPixel);

        SolidBrush titleBrush(Color(255, 0, 51, 102));
        SolidBrush headerBrush(Color(255, 178, 34, 34));
        SolidBrush textBrush(Color(255, 30, 30, 30));
        SolidBrush footerBrush(Color(255, 255, 69, 0));

        StringFormat centerFormat;
        centerFormat.SetAlignment(StringAlignmentCenter);

        // --- 4. VẼ NỘI DUNG VĂN BẢN ---

        float currentY = cardY + 30;

        // A. TIÊU ĐỀ
        graphics.DrawString(L"SVG READER - HƯỚNG DẪN", -1, &titleFont,
            RectF(cardX, currentY, cardW, 50), &centerFormat, &titleBrush);
        currentY += 60;

        // B. LỜI CHÀO
        graphics.DrawString(L"Xin chào, đây là chương trình đọc ảnh SVG. Dưới đây là hướng dẫn sử dụng.", -1, &italicFont,
            RectF(cardX, currentY, cardW, 30), &centerFormat, &textBrush);
        currentY += 50;

        float contentLeft = cardX + 80;

        // C. MỤC 1: CAMERA (Giữ nguyên)
        graphics.DrawString(L"\u2699 ĐIỀU KHIỂN CAMERA:", -1, &headerFont, PointF(contentLeft, currentY), &headerBrush);
        currentY += 30;

        wstring controls = L"• Phím 'I' / 'O': Phóng to / Thu nhỏ\n"
            L"• Phím Mũi tên: Di chuyển (Pan)\n"
            L"• Phím 'R' / 'L': Xoay hình\n"
            L"• Phím 'D': Reset về mặc định";
        graphics.DrawString(controls.c_str(), -1, &bodyFont, PointF(contentLeft + 20, currentY), &textBrush);
        currentY += 100;

        // D. MỤC 2: CÔNG CỤ & TIỆN ÍCH (MỚI THÊM VÀO) <--- NEW CODE
        graphics.DrawString(L"\u2605 CÔNG CỤ & TIỆN ÍCH:", -1, &headerFont, PointF(contentLeft, currentY), &headerBrush);
        currentY += 30;

        wstring tools = L"• Phím 'G': Bật / Tắt lưới tọa độ vô tận\n"
            L"• Ctrl + S: Xuất ảnh hiện tại ra file PNG";
        graphics.DrawString(tools.c_str(), -1, &bodyFont, PointF(contentLeft + 20, currentY), &textBrush);
        currentY += 60; // Khoảng cách xuống dòng tiếp theo

        // E. MỤC 3: LƯU Ý (Đẩy xuống dưới)
        graphics.DrawString(L"\uE110 THAO TÁC CHUỘT:", -1, &headerFont, PointF(contentLeft, currentY), &headerBrush);
        currentY += 30;

        wstring notes = L"• Lăn chuột để Zoom nhanh hơn.\n"
            L"• Click chuột trái vào hình để chọn (Select).";
        graphics.DrawString(notes.c_str(), -1, &bodyFont, PointF(contentLeft + 20, currentY), &textBrush);

        // F. FOOTER
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

void DrawGrid(Graphics& graphics, float w, float h) {
    
    // 1. Cấu hình bút vẽ lưới (Màu xám nhạt)
    Pen gridPen(Color(80, 180, 180, 180), 1);
    gridPen.SetDashStyle(DashStyleDot); // Nét chấm

    Pen axisPen(Color(150, 100, 100, 100), 2);

    float step = 50.0f; 
    float range = 10000.0f; //t giả lập cứng là 10000 cho nó hết cái khung, w với h không dùng

    for (float x = -range; x <= range; x += step) {
        if (x == 0) continue;
        graphics.DrawLine(&gridPen, x, -range, x, range);
    }
    for (float y = -range; y <= range; y += step) {
        if (y == 0) continue;
        graphics.DrawLine(&gridPen, -range, y, range, y);
    }

    graphics.DrawLine(&axisPen, 0.0f, -range, 0.0f, range); // Trục tung (Y)
    graphics.DrawLine(&axisPen, -range, 0.0f, range, 0.0f); // Trục hoành (X)
}

// --- HÀM HỖ TRỢ LẤY ENCODER CỦA GDI+ (Bắt buộc để lưu ảnh) ---
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
    UINT  num = 0;          // number of image encoders
    UINT  size = 0;         // size of the image encoder array in bytes

    GetImageEncodersSize(&num, &size);
    if (size == 0) return -1;

    ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL) return -1;

    GetImageEncoders(num, size, pImageCodecInfo);

    for (UINT j = 0; j < num; ++j) {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;
        }
    }
    free(pImageCodecInfo);
    return -1;
}

// --- HÀM THỰC HIỆN EXPORT RA PNG ---
void ExportToPNG(HWND hWnd) {
    if (!g_pImg || !g_vb) {
        MessageBox(hWnd, L"Chưa có ảnh nào được mở!", L"Lỗi", MB_OK | MB_ICONERROR);
        return;
    }

    string savePath = "";
    FileExplorer fileTool;

    // 1. Mở hộp thoại chọn nơi lưu
    if (fileTool.SaveFileDialog(savePath)) {

        // 2. Lấy kích thước chuẩn của file SVG (không phụ thuộc vào Zoom hiện tại)
        float width = g_vb->getViewWidth();
        float height = g_vb->getViewHeight();

        // Nếu file SVG không khai báo kích thước, dùng mặc định
        if (width <= 0) width = 800;
        if (height <= 0) height = 600;

        // 3. Tạo Bitmap ảo trong bộ nhớ
        Bitmap* bitmap = new Bitmap((INT)width, (INT)height, PixelFormat32bppARGB);
        Graphics* memGraphics = Graphics::FromImage(bitmap);

        // Cài đặt chế độ vẽ đẹp nhất
        memGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
        memGraphics->SetTextRenderingHint(TextRenderingHintClearTypeGridFit);

        // Xóa nền (để trong suốt hoặc trắng) -> Ở đây tôi để nền trắng cho dễ nhìn
        // Nếu muốn trong suốt thì dùng Color::Transparent
        memGraphics->Clear(Color::White);

        // 4. Vẽ SVG lên Bitmap này
        renderer renderTool;
        g_pImg->renderImage(renderTool, *memGraphics);

        // 5. Tìm bộ mã hóa PNG
        CLSID pngClsid;
        GetEncoderClsid(L"image/png", &pngClsid);

        // 6. Lưu file xuống ổ cứng
        wstring wPath(savePath.begin(), savePath.end());
        Status stat = bitmap->Save(wPath.c_str(), &pngClsid, NULL);

        if (stat == Ok) {
            MessageBox(hWnd, L"Xuất ảnh thành công!", L"Thông báo", MB_OK | MB_ICONINFORMATION);
        }
        else {
            MessageBox(hWnd, L"Lỗi khi lưu file!", L"Lỗi", MB_OK | MB_ICONERROR);
        }

        // Dọn dẹp bộ nhớ
        delete memGraphics;
        delete bitmap;
    }
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

// PHẦN 3: XỬ LÝ LOGIC CỬA SỔ CHÍNH

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Tính toán AutoScale dùng chung
    RECT rect; GetWindowRect(hWnd, &rect);
    float WinW = (float)(rect.right - rect.left - 16);
    float WinH = (float)(rect.bottom - rect.top - 39);
    float svgW = (g_vb) ? g_vb->getViewWidth() : 0;
    float svgH = (g_vb) ? g_vb->getViewHeight() : 0;

    float autoScale = 1.5f;
    if (svgW > 0 && svgH > 0 && WinW > 0 && WinH > 0) {
        float sx = WinW / svgW;
        float sy = WinH / svgH;
        autoScale = (sx < sy) ? sx : sy;
    }
    float totalScale = scale * autoScale;

    switch (message)
    {
    case WM_KEYDOWN:
        if (GetKeyState(VK_CONTROL) & 0x8000) {
            if (wParam == 'S' || wParam == 's') {
                ExportToPNG(hWnd);
                return 0;
            }
        }
        switch (wParam) {
        case VK_UP:    scroll_y -= 20; break;
        case VK_DOWN:  scroll_y += 20; break;
        case VK_LEFT:  scroll_x -= 20; break;
        case VK_RIGHT: scroll_x += 20; break;
        case 'I': case 'i': scale *= 1.1f; break;
        case 'O': case 'o': scale *= 0.9f; break;
        case 'R': case 'r': Rotate += 5.0f; break;
        case 'L': case 'l': Rotate -= 5.0f; break;
        case 'G': case 'g':
            g_ShowGrid = !g_ShowGrid; // Đảo trạng thái
            InvalidateRect(hWnd, NULL, TRUE); // Yêu cầu vẽ lại
            break;
        case 'D': case 'd': scale = 1.0f; Rotate = 0.0f; scroll_x = 0; scroll_y = 0; break;
        }
        InvalidateRect(hWnd, NULL, TRUE); // Vẽ lại màn hình
        break;

    case WM_MOUSEWHEEL:
        if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) scale *= 1.1f;
        else scale *= 0.92f;
        InvalidateRect(hWnd, NULL, TRUE);
        break;


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

            if (g_ShowGrid && g_vb) {
                // Lấy kích thước ViewBox để vẽ lưới vừa khít
                float gridW = g_vb->getViewWidth();
                float gridH = g_vb->getViewHeight();

                // Nếu viewbox chưa set (ví dụ file lỗi), dùng kích thước mặc định to to chút
                if (gridW == 0) gridW = 2000;
                if (gridH == 0) gridH = 2000;

                DrawGrid(graphics, gridW, gridH);
            }

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