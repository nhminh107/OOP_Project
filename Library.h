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


#include "Image.h"
#include "Parser.h"
#include "Render.h"
#include "Shape.h"
#include "ShapeFactory.h"
#include "Path.h"
#include "Group.h"
#include "Circle.h"
#include "Ellipse.h"
#include "Polyline.h"
#include "Point.h"
#include "Color.h"
#include "Polygon.h"
#include "Rectangle.h"
#include "Text.h"
#include "Line.h"
#include "Viewbox.h"
#endif 