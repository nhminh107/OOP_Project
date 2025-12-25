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

void LogLinearBrushStep(string id, Gdiplus::PointF p1, Gdiplus::PointF p2,
    float* points, Gdiplus::Color* colors, int size, Gdiplus::Matrix* mat) {

    // ios::app để lưu lại lịch sử tất cả các lần hình được vẽ
    std::ofstream log("LINEAR_DEBUG_TRACE.txt", std::ios::app);
    if (!log.is_open()) return;

    log << "================================================================" << endl;
    log << ">>> [LINEAR BRUSH RUN] ID: " << id << endl;

    // 1. Kiểm tra tọa độ Pixel thực tế sau khi đã nhân Bound
    log << "    - GDI+ P1 (Pixel): (" << p1.X << ", " << p1.Y << ")" << endl;
    log << "    - GDI+ P2 (Pixel): (" << p2.X << ", " << p2.Y << ")" << endl;

    // 2. Kiểm tra danh sách màu sau khi xử lý chèn biên (0.0 và 1.0)
    log << "    - Interpolation Stops (Total: " << size << "):" << endl;
    for (int i = 0; i < size; i++) {
        log << "      [" << i << "] Offset: " << std::fixed << std::setprecision(2) << points[i]
            << " | ARGB: (" << (int)colors[i].GetA() << ","
            << (int)colors[i].GetR() << ","
            << (int)colors[i].GetG() << ","
            << (int)colors[i].GetB() << ")" << endl;
    }

    // 3. Kiểm tra Ma trận biến đổi cuối cùng
    float e[6];
    mat->GetElements(e);
    log << "    - Final Matrix: [" << e[0] << "," << e[1] << "," << e[2] << ","
        << e[3] << "," << e[4] << "," << e[5] << "]" << endl;

    log << "================================================================" << endl;

    log.flush();
    log.close();
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

    // GỌI HÀM TEST TẠI ĐÂY
    LogLinearBrushStep(this->id, p1, p2, points, colors, size, &mat);

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