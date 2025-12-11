#ifndef _LIBRARY_H_
#define _LIBRARY_H_

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <unordered_map>
#include <stack>
#include <locale>
#include <codecvt>
#include "rapidxml.hpp"
#include <objidl.h>
#include <gdiplus.h>
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN 
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")


#include "Color.h"      // Màu sắc
#include "Point.h"      // Tọa độ điểm
#include "viewBox.h"    // Khung nhìn

#include "Shape.h"


#include "Rectangle.h"
#include "Line.h"
#include "Circle.h"
#include "Ellipse.h"
#include "Polyline.h"
#include "Polygon.h" 
#include "Path.h"
#include "Text.h"
#include "Group.h" 

#include "ShapeFactory.h"
#include "Render.h" 
#include "Parser.h"
#include "Image.h"

#define Pi 3.14159265358979323846
#define Kappa 0.551915024494
#define Pakka 1.5707963267948966

#endif 