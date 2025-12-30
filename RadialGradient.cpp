#include "Library.h"



radialgradient::radialgradient() {
	isLink = false;
	center = point(0.5f, 0.5f);
	focal = point(0.5f, 0.5f);
	r = 0.5f;
}

radialgradient::radialgradient(const radialgradient& radial) {
	center = radial.center; 
	r = radial.r;
	focal = radial.focal;
}
radialgradient& radialgradient::operator = (const radialgradient& radial) {
	if (this != &radial) {
		center = radial.center;
		r = radial.r;
		focal = radial.focal;
	}
	return *this;
}

GradientType radialgradient::getType() {
	return GradientType::RADIAL;
}

/*
Gdiplus::Brush* radialgradient::createBrush(RectF bounds) {
	float realCx, realCy, realR;

	if (this->getUnits() == OBJECT_BOUNDING_BOX) {
		// Tọa độ tương đối [0,1] -> tuyệt đối
		realCx = bounds.X + this->getCx() * bounds.Width;
		realCy = bounds.Y + this->getCy() * bounds.Height;
		// Bán kính: trung bình cả 2 chiều
		realR = this->getR() * (bounds.Width + bounds.Height) / 2.0f;
	} else {
		realCx = this->getCx();
		realCy = this->getCy();
		realR = this->getR();
	}

	GraphicsPath pathE;
	pathE.AddEllipse(RectF(realCx - realR, realCy - realR, realR * 2, realR * 2));
	PathGradientBrush* fillBrush = new PathGradientBrush(&pathE);

	// Xử lý mốc màu
	if (size > 0 && ColorOffset[0].offset != 0) {
		color first = ColorOffset[0].stopColor;
		float offset = ColorOffset[0].offset;
		color zero{ first.r * (1 - offset), first.g * (1 - offset), first.b * (1 - offset), first.opacity * (1 - offset) };
		ColorOffset.insert(ColorOffset.begin(), stop(zero, 0));
		size++;
	}

	if (size > 0 && ColorOffset[size - 1].offset != 1) {
		color last = ColorOffset[size - 1].stopColor;
		float offset = ColorOffset[size - 1].offset;
		color one{ last.r * (1 / offset), last.g * (1 / offset), last.b * (1 / offset), last.opacity * (1 / offset) };
		ColorOffset.push_back(stop(one, 1));
		size++;
	}

	float* points = new float[size];
	Color* colors = new Color[size];
	for (int k = 0; k < size; k++) {
		points[k] = 1 - ColorOffset[size - k - 1].offset;
		colors[k] = Color(ColorOffset[size - k - 1].stopColor.opacity * 255, ColorOffset[size - k - 1].stopColor.r, ColorOffset[size - k - 1].stopColor.g, ColorOffset[size - k - 1].stopColor.b);
	}

	// Gradient Transform
	vector<pair<string, vector<float>>> gradientTrans = this->getGradientTrans();
	for (int k = 0; k < gradientTrans.size(); ++k) {
		if (gradientTrans[k].first == "translate") {
			fillBrush->TranslateTransform(gradientTrans[k].second[0], gradientTrans[k].second[1]);
		}
		else if (gradientTrans[k].first == "rotate") {
			fillBrush->RotateTransform(gradientTrans[k].second[0]);
		}
		else if (gradientTrans[k].first == "scale") {
			fillBrush->ScaleTransform(gradientTrans[k].second[0], gradientTrans[k].second[1]);
		}
		else if (gradientTrans[k].first == "matrix") {
			Matrix matrix(
				gradientTrans[k].second[0], gradientTrans[k].second[1], gradientTrans[k].second[2],
				gradientTrans[k].second[3], gradientTrans[k].second[4], gradientTrans[k].second[5]
			);
			fillBrush->SetTransform(&matrix);
		}
	}

	fillBrush->SetInterpolationColors(colors, points, size);

	delete[] colors;
	delete[] points;
	return fillBrush;
}
*/

Gdiplus::Brush* radialgradient::createBrush(Gdiplus::RectF bounds) {
	using namespace Gdiplus;

	// --- PHẦN 1: LẤY DỮ LIỆU THÔ (RAW DATA) ---
	// Không nhân chia gì cả, lấy đúng giá trị parse được
	float rawCx = this->getCenter().getX();
	float rawCy = this->getCenter().getY();
	float rawR = this->getR();
	float rawFx = this->getFocal().getX();
	float rawFy = this->getFocal().getY();

	// --- PHẦN 2: TẠO PATH CƠ BẢN ---
	GraphicsPath path;
	// Tạo một hình tròn dựa trên tâm và bán kính thô
	path.AddEllipse(rawCx - rawR, rawCy - rawR, rawR * 2, rawR * 2);

	// Tạo Brush từ Path này
	PathGradientBrush* brush = new PathGradientBrush(&path);

	// Đặt tiêu điểm (Focal Point) - Nơi màu bắt đầu tỏa ra
	brush->SetCenterPoint(PointF(rawFx, rawFy));

	// --- PHẦN 3: XỬ LÝ MÀU (LOGIC ĐẢO NGƯỢC CHO GDI+) ---
	// SVG: 0% (Tâm) -> 100% (Biên)
	// GDI+: 0.0 (Biên) -> 1.0 (Tâm)
	// => Cần đảo ngược thứ tự stops và công thức tính offset = 1.0 - svgoffset

	vector<stop> ColorOffset = this->getStopVct();
	int size = ColorOffset.size();

	// (Giữ nguyên logic thêm stop 0 và 1 nếu thiếu của bạn - rất tốt)
	if (size > 0 && ColorOffset[0].offset != 0) {
		color first = ColorOffset[0].stopColor;
		float offset = ColorOffset[0].offset;
		color zero{ first.r * (1 - offset), first.g * (1 - offset), first.b * (1 - offset), first.opacity * (1 - offset) };
		ColorOffset.insert(ColorOffset.begin(), stop(zero, 0));
		size++;
	}
	if (size > 0 && ColorOffset[size - 1].offset != 1) {
		color last = ColorOffset[size - 1].stopColor;
		float offset = ColorOffset[size - 1].offset;
		float safeOffset = (offset == 0) ? 1.0f : offset; // Tránh chia cho 0
		color one{ last.r * (1 / safeOffset), last.g * (1 / safeOffset), last.b * (1 / safeOffset), last.opacity * (1 / safeOffset) };
		ColorOffset.push_back(stop(one, 1));
		size++;
	}

	// Convert sang mảng GDI+
	float* points = new float[size];
	Color* colors = new Color[size];

	for (int k = 0; k < size; k++) {
		// Đảo ngược index: SVG[0] là tâm -> GDI+ phải ở cuối (hoặc position = 1.0)
		// Cách mapping chuẩn nhất cho Radial GDI+:
		// SVG Offset: x (0->1)  ===> GDI Position: 1.0 - x
		// Màu cũng phải lấy tương ứng.

		int svgIndex = k;

		// GDI+ yêu cầu mảng points tăng dần từ 0 -> 1 (tức là từ biên vào tâm)
		// Nên ta gán ngược:
		// points[0] (Biên) ứng với SVG Offset 1.0 -> GDI pos = 1.0 - 1.0 = 0.0
		// points[MAX] (Tâm) ứng với SVG Offset 0.0 -> GDI pos = 1.0 - 0.0 = 1.0

		// Để dễ hiểu, ta duyệt ngược từ stop cuối của SVG về đầu
		int indexFromBack = size - 1 - k;

		points[k] = 1.0f - ColorOffset[indexFromBack].offset;

		colors[k] = Color(
			ColorOffset[indexFromBack].stopColor.opacity * 255,
			ColorOffset[indexFromBack].stopColor.r,
			ColorOffset[indexFromBack].stopColor.g,
			ColorOffset[indexFromBack].stopColor.b
		);
	}

	brush->SetInterpolationColors(colors, points, size);

	// --- PHẦN 4: XỬ LÝ MATRIX (Logic chuẩn giống LinearGradient) ---
	Matrix finalMatrix;

	// A. Apply SVG Transform (rotate, scale, skew...)
	vector<pair<string, vector<float>>> gradientTrans = this->getGradientTrans();
	for (int k = 0; k < gradientTrans.size(); ++k) {
		string type = gradientTrans[k].first;
		vector<float> val = gradientTrans[k].second;

		// Dùng MatrixOrderPrepend để tác động lên hệ toạ độ Gradient trước
		if (type == "translate") {
			float tx = val[0];
			float ty = (val.size() > 1) ? val[1] : 0;
			finalMatrix.Translate(tx, ty, MatrixOrderPrepend);
		}
		else if (type == "rotate") {
			float angle = val[0];
			if (val.size() >= 3) {
				finalMatrix.RotateAt(angle, PointF(val[1], val[2]), MatrixOrderPrepend);
			}
			else {
				finalMatrix.Rotate(angle, MatrixOrderPrepend);
			}
		}
		else if (type == "scale") {
			float sx = val[0];
			float sy = (val.size() > 1) ? val[1] : sx;
			finalMatrix.Scale(sx, sy, MatrixOrderPrepend);
		}
		else if (type == "matrix") {
			if (val.size() >= 6) {
				Matrix matrix(val[0], val[1], val[2], val[3], val[4], val[5]);
				finalMatrix.Multiply(&matrix, MatrixOrderPrepend);
			}
		}
	}

	// B. Apply Bounding Box Mapping (Chỉ cho ObjectBoundingBox)
	if (this->getUnits() == OBJECT_BOUNDING_BOX) {
		// Ma trận này sẽ:
		// 1. Scale hình tròn đơn vị (hoặc ellipse đơn vị) thành kích thước bounds
		// 2. Dịch chuyển nó đến vị trí bounds
		Matrix boundsMatrix;
		boundsMatrix.Translate(bounds.X, bounds.Y);
		boundsMatrix.Scale(bounds.Width, bounds.Height);

		// Append: Áp dụng sau khi đã transform SVG xong
		finalMatrix.Multiply(&boundsMatrix, MatrixOrderAppend);
	}

	// --- PHẦN 5: ÁP DỤNG VÀO BRUSH ---
	brush->SetTransform(&finalMatrix);

	delete[] colors;
	delete[] points;
	return brush;
}
/*
Gdiplus::Brush* radialgradient::createBrush(RectF bounds) {
	using namespace Gdiplus;
	float cx = this->getCx();
	float cy = this->getCy();
	float r = this->getR();
	vector<stop> ColorOffset = this->getStopVct();
	int size = ColorOffset.size();

	GraphicsPath pathE;
	pathE.AddEllipse(RectF(cx - r, cy - r, r * 2, r * 2));
	PathGradientBrush* fillBrush = new PathGradientBrush(&pathE);

	// Xử lý mốc màu
	if (size > 0 && ColorOffset[0].offset != 0) {
		color first = ColorOffset[0].stopColor;
		float offset = ColorOffset[0].offset;
		color zero{ first.r * (1 - offset), first.g * (1 - offset), first.b * (1 - offset), first.opacity * (1 - offset) };
		ColorOffset.insert(ColorOffset.begin(), stop(zero, 0));
		size++;
	}

	if (size > 0 && ColorOffset[size - 1].offset != 1) {
		color last = ColorOffset[size - 1].stopColor;
		float offset = ColorOffset[size - 1].offset;
		color one{ last.r * (1 / offset), last.g * (1 / offset), last.b * (1 / offset), last.opacity * (1 / offset) };
		ColorOffset.push_back(stop(one, 1));
		size++;
	}

	float* points = new float[size];
	Color* colors = new Color[size];
	for (int k = 0; k < size; k++) {
		points[k] = 1 - ColorOffset[size - k - 1].offset;
		colors[k] = Color(ColorOffset[size - k - 1].stopColor.opacity * 255, ColorOffset[size - k - 1].stopColor.r, ColorOffset[size - k - 1].stopColor.g, ColorOffset[size - k - 1].stopColor.b);
	}

	// Gradient Transform
	vector<pair<string, vector<float>>> gradientTrans = this->getGradientTrans();
	for (int k = 0; k < gradientTrans.size(); ++k) {
		if (gradientTrans[k].first == "translate") {
			fillBrush->TranslateTransform(gradientTrans[k].second[0], gradientTrans[k].second[1]);
		}
		else if (gradientTrans[k].first == "rotate") {
			fillBrush->RotateTransform(gradientTrans[k].second[0]);
		}
		else if (gradientTrans[k].first == "scale") {
			fillBrush->ScaleTransform(gradientTrans[k].second[0], gradientTrans[k].second[1]);
		}
		else if (gradientTrans[k].first == "matrix") {
			Matrix matrix(
				gradientTrans[k].second[0], gradientTrans[k].second[1], gradientTrans[k].second[2],
				gradientTrans[k].second[3], gradientTrans[k].second[4], gradientTrans[k].second[5]
			);
			fillBrush->SetTransform(&matrix);
		}
	}

	fillBrush->SetInterpolationColors(colors, points, size);

	delete[] colors;
	delete[] points;
	return fillBrush;
}
*/
//Gdiplus::Brush* radialgradient::createBrush(RectF bounds) {
//	using namespace Gdiplus;
//
//	// 1. Tính toán tọa độ (thêm hỗ trợ OBJECT_BOUNDING_BOX)
//	float cx, cy, r;
//
//	if (this->getUnits() == OBJECT_BOUNDING_BOX) {
//		// Map từ tọa độ tương đối [0,1] sang tuyệt đối
//		cx = bounds.X + this->getCx() * bounds.Width;
//		cy = bounds.Y + this->getCy() * bounds.Height;
//		// Dùng trung bình Width/Height cho bán kính
//		r = this->getR() * (bounds.Width + bounds.Height) / 2.0f;
//	}
//	else {
//		// userSpaceOnUse - dùng tọa độ tuyệt đối như cũ
//		cx = this->getCx();
//		cy = this->getCy();
//		r = this->getR();
//	}
//
//	// 2-7. GIỮ NGUYÊN HOÀN TOÀN phần còn lại của code gốc
//	vector<stop> ColorOffset = this->getStopVct();
//	int size = ColorOffset.size();
//
//	GraphicsPath pathE;
//	pathE.AddEllipse(RectF(cx - r, cy - r, r * 2, r * 2));
//	PathGradientBrush* fillBrush = new PathGradientBrush(&pathE);
//
//	// Xử lý mốc màu - GIỮ NGUYÊN LOGIC CỦA BẠN
//	if (size > 0 && ColorOffset[0].offset != 0) {
//		color first = ColorOffset[0].stopColor;
//		float offset = ColorOffset[0].offset;
//		color zero{ first.r * (1 - offset), first.g * (1 - offset), first.b * (1 - offset), first.opacity * (1 - offset) };
//		ColorOffset.insert(ColorOffset.begin(), stop(zero, 0));
//		size++;
//	}
//
//	if (size > 0 && ColorOffset[size - 1].offset != 1) {
//		color last = ColorOffset[size - 1].stopColor;
//		float offset = ColorOffset[size - 1].offset;
//		color one{ last.r * (1 / offset), last.g * (1 / offset), last.b * (1 / offset), last.opacity * (1 / offset) };
//		ColorOffset.push_back(stop(one, 1));
//		size++;
//	}
//
//	float* points = new float[size];
//	Color* colors = new Color[size];
//	for (int k = 0; k < size; k++) {
//		points[k] = 1 - ColorOffset[size - k - 1].offset;
//		colors[k] = Color(ColorOffset[size - k - 1].stopColor.opacity * 255,
//			ColorOffset[size - k - 1].stopColor.r,
//			ColorOffset[size - k - 1].stopColor.g,
//			ColorOffset[size - k - 1].stopColor.b);
//	}
//
//	// Gradient Transform - GIỮ NGUYÊN
//	vector<pair<string, vector<float>>> gradientTrans = this->getGradientTrans();
//	for (int k = 0; k < gradientTrans.size(); ++k) {
//		if (gradientTrans[k].first == "translate") {
//			fillBrush->TranslateTransform(gradientTrans[k].second[0], gradientTrans[k].second[1]);
//		}
//		else if (gradientTrans[k].first == "rotate") {
//			fillBrush->RotateTransform(gradientTrans[k].second[0]);
//		}
//		else if (gradientTrans[k].first == "scale") {
//			fillBrush->ScaleTransform(gradientTrans[k].second[0], gradientTrans[k].second[1]);
//		}
//		else if (gradientTrans[k].first == "matrix") {
//			Matrix matrix(
//				gradientTrans[k].second[0], gradientTrans[k].second[1], gradientTrans[k].second[2],
//				gradientTrans[k].second[3], gradientTrans[k].second[4], gradientTrans[k].second[5]
//			);
//			fillBrush->SetTransform(&matrix);
//		}
//	}
//
//	fillBrush->SetInterpolationColors(colors, points, size);
//
//	delete[] colors;
//	delete[] points;
//
//	return fillBrush;
//}

float parseSVGValue2(string val) {
	if (val.empty()) return 0;
	// Nếu có ký tự '%', chia giá trị cho 100
	if (val.find('%') != string::npos) {
		return stof(val.substr(0, val.find('%'))) / 100.0f;
	}
	// Nếu không, trả về giá trị số thực bình thường
	return stof(val);
}
void radialgradient::updateElement() {
	stringstream sss(strLine);
	string temp = "", attribute = "", value = "", transformGradient = "";

	while (sss >> attribute) {
		getline(sss, temp, '"');
		getline(sss, value, '"');

		if (attribute == "cx") {
			this->center.x = parseSVGValue2(value);
		}
		if (attribute == "cy") {
			this->center.y = parseSVGValue2(value);
		}
		if (attribute == "r") {
			this->r = parseSVGValue2(value);
		}
		if (attribute == "fx") {
			this->focal.x = parseSVGValue2(value);
		}
		if (attribute == "fy") {
			this->focal.y = parseSVGValue2(value);
		}
		if (attribute == "gradientUnits") {
			if (value == "userSpaceOnUse")
				this->setUnits(USER_SPACE_ON_USE);
			else
				this->setUnits(GradientUnits::OBJECT_BOUNDING_BOX);
		}
		if (attribute == "gradientTransform") {
			transformGradient = value;
		}
		if (attribute == "xlink:href") {
			this->setIsLink(true);
		}
	}
	this->updateGradientTransform(transformGradient);
}

bool radialgradient::getIsLink() {
	return this->isLink;
}

void radialgradient::setIsLink(bool link) {
	this->isLink = link;
}