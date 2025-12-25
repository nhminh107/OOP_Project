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

GradientType LinearGradient::getType() const {
	return LINEAR; 
}

Brush* LinearGradient::createBrush(const Gdiplus::RectF& shapeBound, float opacity) {
    Matrix mat;
    this->getTransformMatrix(&mat);

    // 1. Map tọa độ Start/End (Giữ logic Mapping của bạn vì nó chuẩn hơn cho objectBoundingBox)
    PointF p1(shapeBound.X + this->start.x * shapeBound.Width, shapeBound.Y + this->start.y * shapeBound.Height);
    PointF p2(shapeBound.X + this->end.x * shapeBound.Width, shapeBound.Y + this->end.y * shapeBound.Height);

    // 2. Xử lý danh sách Stops (Bắt chước logic chèn biên của bạn bạn)
    vector<stop> ColorOffset = this->stopList;
    if (ColorOffset.empty()) return new SolidBrush(Color(255, 0, 0, 0));

    // Chèn stop 0 nếu thiếu
    if (ColorOffset[0].offset != 0) {
        ColorOffset.insert(ColorOffset.begin(), stop(ColorOffset[0].stopColor, 0));
    }
    // Chèn stop 1 nếu thiếu
    if (ColorOffset.back().offset != 1) {
        ColorOffset.push_back(stop(ColorOffset.back().stopColor, 1));
    }

    int size = ColorOffset.size();
    float* points = new float[size];
    Color* colors = new Color[size];

    for (int k = 0; k < size; k++) {
        points[k] = ColorOffset[k].offset;
        // Logic nhân opacity chuẩn của bạn bạn
        colors[k] = Color(ColorOffset[k].stopColor.opacity * 255,
            ColorOffset[k].stopColor.r,
            ColorOffset[k].stopColor.g,
            ColorOffset[k].stopColor.b);
    }

    LinearGradientBrush* brush = new LinearGradientBrush(p1, p2, colors[0], colors[size - 1]);
    brush->SetInterpolationColors(colors, points, size);
    brush->SetTransform(&mat); // Áp transform sau cùng

    delete[] points; delete[] colors;
    return brush;
}

/*LƯU Ý : 

Thứ tự Offset: GDI+ yêu cầu mảng offsets phải tăng dần từ 0.0 đến 1.0. Thông thường các file SVG đã sắp xếp sẵn, nhưng nếu gặp file lỗi, Brush sẽ không hiển thị.

Giải phóng bộ nhớ: Như đã nhấn mạnh, mỗi khi bạn gọi fillBrush = gradient->createBrush(...), bạn đang tạo ra một vùng nhớ mới bằng từ khóa new. Bạn bắt buộc phải gọi delete fillBrush ngay sau khi lệnh graphics.FillPath(...) kết thúc trong hàm draw.

Tọa độ âm hoặc > 100%: Với ObjectBoundingBox, SVG cho phép tọa độ nằm ngoài phạm vi 0-1 (ví dụ x1="-10%"). Hàm parseUnit của bạn đã xử lý tốt việc chia cho 100, nên các tọa độ này sẽ được createBrush tính toán chính xác ra ngoài phạm vi Bound của hình.*/