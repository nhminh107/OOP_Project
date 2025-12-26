#include "Library.h"

float radialgradient::getCx() {
	return cx;
}
float radialgradient::getCy() {
	return cy;
}
float radialgradient::getR() {
	return r;
}
float radialgradient::getFx() {
	return fx;
}
float radialgradient::getFy() {
	return fy;
}

radialgradient::radialgradient() {
	isLink = false;
	cx = cy = r = fx = fy = 0;
}

radialgradient::radialgradient(const radialgradient& radial) {
	cx = radial.cx;
	cy = radial.cy;
	r = radial.r;
	fx = radial.fx;
	fy = radial.fy;
}
radialgradient& radialgradient::operator = (const radialgradient& radial) {
	if (this != &radial) {
		cx = radial.cx;
		cy = radial.cy;
		r = radial.r;
		fx = radial.fx;
		fy = radial.fy;
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
	float realCx, realCy, realRx, realRy;

	// --- LOGIC MAPPING TỌA ĐỘ (Giữ nguyên) ---
	if (this->getUnits() == OBJECT_BOUNDING_BOX) {
		realCx = bounds.X + this->getCx() * bounds.Width;
		realCy = bounds.Y + this->getCy() * bounds.Height;
		realRx = this->getR() * bounds.Width;
		realRy = this->getR() * bounds.Height;
	}
	else {
		realCx = this->getCx();
		realCy = this->getCy();
		realRx = realRy = this->getR();
	}

	GraphicsPath pathE;
	pathE.AddEllipse(RectF(realCx - realRx, realCy - realRy, realRx * 2, realRy * 2));
	PathGradientBrush* fillBrush = new PathGradientBrush(&pathE);

	// --- LOGIC STOP MÀU (Giữ nguyên) ---
	vector<stop> ColorOffset = this->getStopVct();
	int size = ColorOffset.size();

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
		float safeOffset = (offset == 0) ? 1.0f : offset;
		color one{ last.r * (1 / safeOffset), last.g * (1 / safeOffset), last.b * (1 / safeOffset), last.opacity * (1 / safeOffset) };
		ColorOffset.push_back(stop(one, 1));
		size++;
	}

	float* points = new float[size];
	Color* colors = new Color[size];
	for (int k = 0; k < size; k++) {
		points[k] = 1.0f - ColorOffset[size - k - 1].offset;
		colors[k] = Color(ColorOffset[size - k - 1].stopColor.opacity * 255,
			ColorOffset[size - k - 1].stopColor.r,
			ColorOffset[size - k - 1].stopColor.g,
			ColorOffset[size - k - 1].stopColor.b);
	}

	// --- SỬA LỖI PROBLEM 4: XỬ LÝ TRANSFORM THEO UNITS ---
	vector<pair<string, vector<float>>> gradientTrans = this->getGradientTrans();
	for (int k = 0; k < gradientTrans.size(); ++k) {
		string type = gradientTrans[k].first;
		vector<float> val = gradientTrans[k].second;

		if (this->getUnits() == OBJECT_BOUNDING_BOX) {
			// NẾU LÀ OBJECT BOUNDING BOX: NHÂN VỚI WIDTH/HEIGHT
			if (type == "translate") {
				fillBrush->TranslateTransform(val[0] * bounds.Width, val[1] * bounds.Height);
			}
			else if (type == "rotate") {
				fillBrush->RotateTransform(val[0]);
			}
			else if (type == "scale") {
				fillBrush->ScaleTransform(val[0], val[1]);
			}
			else if (type == "matrix") {
				// Nhân thành phần dịch chuyển (dx, dy) với kích thước bounding box
				Matrix matrix(val[0], val[1], val[2], val[3],
					val[4] * bounds.Width,
					val[5] * bounds.Height);
				fillBrush->MultiplyTransform(&matrix);
			}
		}
		else {
			// NẾU LÀ USER SPACE: GIỮ NGUYÊN
			if (type == "translate") {
				fillBrush->TranslateTransform(val[0], val[1]);
			}
			else if (type == "rotate") {
				fillBrush->RotateTransform(val[0]);
			}
			else if (type == "scale") {
				fillBrush->ScaleTransform(val[0], val[1]);
			}
			else if (type == "matrix") {
				Matrix matrix(val[0], val[1], val[2], val[3], val[4], val[5]);
				fillBrush->MultiplyTransform(&matrix);
			}
		}
	}

	fillBrush->SetInterpolationColors(colors, points, size);

	delete[] colors;
	delete[] points;
	return fillBrush;
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
			this->cx = parseSVGValue2(value);
		}
		if (attribute == "cy") {
			this->cy = parseSVGValue2(value);
		}
		if (attribute == "r") {
			this->r = parseSVGValue2(value);
		}
		if (attribute == "fx") {
			this->fx = parseSVGValue2(value);
		}
		if (attribute == "fy") {
			this->fy = parseSVGValue2(value);
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