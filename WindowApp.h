#ifndef _WINDOW_APP_H_
#define _WINDOW_APP_H_
#include "Library.h"
//CÁC HÀM CON
bool InitGDI();
void ShutdownGDI();

void InitSVGData();

HWND CreateMainWindow(HINSTANCE hInstance, int nCmdShow);
void MessageLoop();

void Cleanup();


// ===== BIẾN TOÀN CỤC DÙNG CHUNG =====
extern float scale;
extern float Rotate;
extern float scroll_x;
extern float scroll_y;

extern string g_InputFileName;
extern image* g_pImg;
extern viewbox* g_vb;

// ===== KHAI BÁO HÀM =====
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK IntroWndProc(HWND, UINT, WPARAM, LPARAM);
void ShowWelcomeWindow(HINSTANCE hInstance);





#endif 
