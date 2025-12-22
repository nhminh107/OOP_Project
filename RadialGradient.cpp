#include "Library.h"
using namespace std;

Brush* RadialGradient::createBrush(const Gdiplus::RectF& shapeBound, float opacity) {
	Brush* temp;
	return temp;

	//Để đại 
}

RadialGradient::RadialGradient() {
	center.x = 0.5f; 
	center.y = 0.5f; // Mặc định ở giữa
    radius = 0.5f;
	focalPoint.x, focalPoint.y = 0.5f, 0.5f; // Mặc định trùng tâm
}
void RadialGradient::setCenter(float x, float y) { center.x = x; center.y = y; }
void RadialGradient::setRadius(float r) { radius = r; }
void RadialGradient::setFocal(float x, float y) { focalPoint.x = x; focalPoint.y = y; }



//Hoàn thiện các hàm Setter Getter còn thiếu 