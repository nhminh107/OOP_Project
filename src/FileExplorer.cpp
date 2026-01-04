#include "Library.h"


// --- HÀM MỞ FILE ---
bool FileExplorer::OpenFileDialog(string& selectedPath) {
    OPENFILENAMEA ofn;
    char szFile[260] = { 0 };

    // 1. Lấy đường dẫn hiện tại nơi file .exe đang chạy
    char currentDir[260] = { 0 };
    GetCurrentDirectoryA(260, currentDir);

    string targetDir = string(currentDir) + "\\TestCases";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "SVG Files\0*.svg\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;

    ofn.lpstrInitialDir = targetDir.c_str();

    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&ofn) == TRUE) {
        selectedPath = string(szFile);
        return true;
    }
    return false;
}


bool FileExplorer::SaveFileDialog(string& selectedPath) {
    OPENFILENAMEA ofn;
    char szFile[260] = { 0 };
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "PNG Image\0*.png\0"; // Chỉ cho phép lưu PNG
    ofn.nFilterIndex = 1;
    ofn.lpstrDefExt = "png"; // Mặc định đuôi file
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

    if (GetSaveFileNameA(&ofn) == TRUE) {
        selectedPath = string(szFile);
        return true;
    }
    return false;
}