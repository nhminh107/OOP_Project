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

	Gdiplus::PointF pStart(
		shapeBound.X + this->start.x * shapeBound.Width,
		shapeBound.Y + this->start.y * shapeBound.Height
	);

	Gdiplus::PointF pEnd(
		shapeBound.X + this->end.x * shapeBound.Width,
		shapeBound.Y + this->end.y * shapeBound.Height
	);

	if (abs(pStart.X - pEnd.X) < 0.001f && abs(pStart.Y - pEnd.Y) < 0.001f) {
		pEnd.X += 0.1f; // Dịch chuyển nhẹ để brush vẫn có thể khởi tạo
	}

	LinearGradientBrush* brush = new LinearGradientBrush(pStart, pEnd, Gdiplus::Color(255, 0, 0, 0), Gdiplus::Color(255, 255, 255, 255));

	int count = stopList.size();
	if (count >= 2) {
		vector<Color> colors(count);
		vector<float> offsets(count);
	

		for (int i = 0; i < count; i++) {
			int a = (int)(stopList[i].stopColor.opacity * opacity * 255);
			colors[i] = Gdiplus::Color(a, stopList[i].stopColor.r,
				stopList[i].stopColor.g,
				stopList[i].stopColor.b);
			offsets[i] = stopList[i].offset;
		}

		// Truyền con trỏ từ vector bằng hàm .data()
		brush->SetInterpolationColors(colors.data(), offsets.data(), count);
	}

	Matrix mat; 
	this->getTransformMatrix(&mat); 
	brush->SetTransform(&mat);  

	return brush; 
}

/*LƯU Ý : 

Thứ tự Offset: GDI+ yêu cầu mảng offsets phải tăng dần từ 0.0 đến 1.0. Thông thường các file SVG đã sắp xếp sẵn, nhưng nếu gặp file lỗi, Brush sẽ không hiển thị.

Giải phóng bộ nhớ: Như đã nhấn mạnh, mỗi khi bạn gọi fillBrush = gradient->createBrush(...), bạn đang tạo ra một vùng nhớ mới bằng từ khóa new. Bạn bắt buộc phải gọi delete fillBrush ngay sau khi lệnh graphics.FillPath(...) kết thúc trong hàm draw.

Tọa độ âm hoặc > 100%: Với ObjectBoundingBox, SVG cho phép tọa độ nằm ngoài phạm vi 0-1 (ví dụ x1="-10%"). Hàm parseUnit của bạn đã xử lý tốt việc chia cho 100, nên các tọa độ này sẽ được createBrush tính toán chính xác ra ngoài phạm vi Bound của hình.*/