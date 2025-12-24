#include "Library.h"
using namespace std;

Brush* RadialGradient::createBrush(const Gdiplus::RectF& shapeBound, float opacity) {
	
	float realCx = shapeBound.X + this->center.x * shapeBound.Width; 
	float realCy = shapeBound.Y + this->center.y * shapeBound.Height; 
	float realRadius = this->radius * (shapeBound.Width + shapeBound.Height) / 2.0f;

	//Radial mà méo méo lệch lệch thì check ở đâyy

	GraphicsPath path; 

	path.AddEllipse(realCx - realRadius, realCy - realRadius, realRadius * 2, realRadius * 2); 
	Gdiplus::PathGradientBrush* brush = new Gdiplus::PathGradientBrush(&path);

	float realFx = shapeBound.X + this->focalPoint.x * shapeBound.Width;
	float realFy = shapeBound.Y + this->focalPoint.y * shapeBound.Height;
	brush->SetCenterPoint(Gdiplus::PointF(realFx, realFy));

    int count = stopList.size(); 
    if (count >= 2) {
        vector<Color> colors(count); 
        vector<float> offsets(count); 
        // Lưu ý: GDI+ PathGradientBrush tính offset từ viền (0.0) vào tâm (1.0)
        for (int i = 0; i < count; i++) {
            int idx = count - 1 - i; // Lấy màu từ cuối StopList (SVG viền) đưa lên đầu (GDI+ viền)
            int a = (int)(stopList[idx].stopColor.opacity * opacity * 255);
            colors[i] = Gdiplus::Color(a, stopList[idx].stopColor.r,
                stopList[idx].stopColor.g,
                stopList[idx].stopColor.b);
            offsets[i] = 1.0f - stopList[idx].offset; // Chuyển 1.0 (SVG viền) thành 0.0 (GDI+ viền)
        }
        brush->SetInterpolationColors(colors.data(), offsets.data(), count);
    }

    Gdiplus::Matrix mat;
    this->getTransformMatrix(&mat);
    brush->SetTransform(&mat);
    
    return brush;
}

GradientType RadialGradient::getType() const {
    return RADIAL;
}

RadialGradient::RadialGradient() {
	center.x = 0.5f; 
	center.y = 0.5f; // Mặc định ở giữa
    radius = 0.5f;
    focalPoint.x = 0.5f; focalPoint.y = 0.5f;
}
void RadialGradient::setCenter(float x, float y) { center.x = x; center.y = y; }
void RadialGradient::setRadius(float r) { radius = r; }
void RadialGradient::setFocal(float x, float y) { focalPoint.x = x; focalPoint.y = y; }



//Hoàn thiện các hàm Setter Getter còn thiếu 