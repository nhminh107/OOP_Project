#include "Library.h"
using namespace std; 

LinearGradient::LinearGradient() {
	start.setX(0); 
	start.setY(0); 
	end.setX(1); 
	end.setY(0);
}

void LinearGradient::setStart(float x, float y) {
	start.x = x; 
	start.y = y; 
}

void LinearGradient::setEnd(float x, float y) { 
	end.x = x; 
	end.y = y;
}

Brush* LinearGradient::createBrush(const Gdiplus::RectF& shapeBound, float opacity) {
	Brush* temp; 
	return temp; 

	//Để đại return để chạy ko lỗi 
}