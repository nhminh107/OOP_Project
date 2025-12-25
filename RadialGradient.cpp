#include "Library.h"
using namespace std;

Brush* RadialGradient::createBrush(const Gdiplus::RectF& shapeBound, float opacity) {
    float realCx = shapeBound.X + this->center.x * shapeBound.Width;
    float realCy = shapeBound.Y + this->center.y * shapeBound.Height;
    float realRadius = this->radius * (shapeBound.Width + shapeBound.Height) / 2.0f;

    GraphicsPath pathE;
    pathE.AddEllipse(realCx - realRadius, realCy - realRadius, realRadius * 2, realRadius * 2);
    PathGradientBrush* brush = new PathGradientBrush(&pathE);

    // Xử lý Stops (Đảo ngược theo logic của bạn bạn)
    vector<stop> ColorOffset = this->stopList;
    if (ColorOffset.empty()) return new SolidBrush(Color(255, 0, 0, 0));

    // Chèn biên 0 và 1
    if (ColorOffset[0].offset != 0) ColorOffset.insert(ColorOffset.begin(), stop(ColorOffset[0].stopColor, 0));
    if (ColorOffset.back().offset != 1) ColorOffset.push_back(stop(ColorOffset.back().stopColor, 1));

    int size = ColorOffset.size();
    float* points = new float[size];
    Color* colors = new Color[size];

    for (int k = 0; k < size; k++) {
        // Đảo ngược logic: GDI+ tính từ Viền (0.0) vào Tâm (1.0)
        points[k] = 1.0f - ColorOffset[size - k - 1].offset;
        colors[k] = Color(ColorOffset[size - k - 1].stopColor.opacity * 255,
            ColorOffset[size - k - 1].stopColor.r,
            ColorOffset[size - k - 1].stopColor.g,
            ColorOffset[size - k - 1].stopColor.b);
    }

    brush->SetInterpolationColors(colors, points, size);

    // Thiết lập tâm điểm (Focal Point)
    float realFx = shapeBound.X + this->focalPoint.x * shapeBound.Width;
    float realFy = shapeBound.Y + this->focalPoint.y * shapeBound.Height;
    brush->SetCenterPoint(PointF(realFx, realFy));

    Matrix mat;
    this->getTransformMatrix(&mat);
    brush->SetTransform(&mat);

    delete[] points; delete[] colors;
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