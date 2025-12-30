#include "Library.h"

point lineargradient::getA() {
    return A;
}

point lineargradient::getB() {
    return B;
}

void lineargradient::setA(point p) {
    this->A = p;
}

void lineargradient::setB(point p) {
    this->B = p;
}
//Hàm phụ trợ, mực đích chuẩn hóa chuỗi 

float parseSVGValue1(string val) {
    if (val.empty()) return 0;
    // Nếu có ký tự '%', chia giá trị cho 100
    if (val.find('%') != string::npos) {
        return stof(val.substr(0, val.find('%'))) / 100.0f;
    }
    // Nếu không, trả về giá trị số thực bình thường
    return stof(val);
}
lineargradient::lineargradient() {
    A.setX(0);
    A.setY(0);
    B.setX(0);
    B.setY(0);
}

lineargradient::lineargradient(const lineargradient& linear) {
    this->A = linear.A;
    this->B = linear.B;

}

lineargradient& lineargradient::operator=(const lineargradient& linear) {
    if (this != &linear) {
        this->A = linear.A;
        this->B = linear.B;
    }
    return *this;
}
Gdiplus::Brush* lineargradient::createBrush(Gdiplus::RectF bounds) {
    using namespace Gdiplus;

    // --- PHẦN 1: XÁC ĐỊNH TỌA ĐỘ GỐC ---
    // Khác với code cũ, ta KHÔNG cộng bounds.X/Y ở đây.
    // Ta lấy nguyên giá trị raw từ file SVG.
    // Nếu là ObjectBoundingBox: A, B thường là (0,0) -> (1,0) (tỉ lệ).
    // Nếu là UserSpaceOnUse: A, B là tọa độ pixel thật.
    PointF p1(this->getA().getX(), this->getA().getY());
    PointF p2(this->getB().getX(), this->getB().getY());

    // --- PHẦN 2: LOGIC STOP MÀU (Giữ nguyên 100% từ code cũ của bạn) ---
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
        color one{ last.r * (1 / offset), last.g * (1 / offset), last.b * (1 / offset), last.opacity * (1 / offset) };
        ColorOffset.push_back(stop(one, 1));
        size++;
    }

    float* points = new float[size];
    Color* colors = new Color[size];
    for (int k = 0; k < size; k++) {
        points[k] = ColorOffset[k].offset;
        colors[k] = Color(ColorOffset[k].stopColor.opacity * 255, ColorOffset[k].stopColor.r, ColorOffset[k].stopColor.g, ColorOffset[k].stopColor.b);
    }

    // Tạo Brush cơ bản
    LinearGradientBrush* fillBrush = new LinearGradientBrush(p1, p2, colors[0], colors[size - 1]);
    fillBrush->SetWrapMode(WrapModeTileFlipXY);
    fillBrush->SetInterpolationColors(colors, points, size);

    // --- PHẦN 3: XỬ LÝ MA TRẬN (SỬA LẠI THEO LOGIC CHUẨN) ---
    Matrix finalMatrix; // Ma trận đơn vị

    // A. ÁP DỤNG GRADIENT TRANSFORM (TỪ SVG)
    // Phần này áp dụng cho CẢ 2 trường hợp (UserSpace và ObjectBoundingBox)
    vector<pair<string, vector<float>>> gradientTrans = this->getGradientTrans();
    for (int k = 0; k < gradientTrans.size(); k++) {
        string type = gradientTrans[k].first;
        vector<float> val = gradientTrans[k].second;

        // Lưu ý: Dùng MatrixOrderPrepend để biến đổi hệ tọa độ của Gradient trước
        if (type == "translate") {
            // translate(tx, ty)
            float tx = val[0];
            float ty = (val.size() > 1) ? val[1] : 0;
            finalMatrix.Translate(tx, ty, MatrixOrderPrepend);
        }
        else if (type == "rotate") {
            // rotate(angle, [cx, cy])
            float angle = val[0];
            if (val.size() >= 3) {
                finalMatrix.RotateAt(angle, PointF(val[1], val[2]), MatrixOrderPrepend);
            }
            else {
                finalMatrix.Rotate(angle, MatrixOrderPrepend);
            }
        }
        else if (type == "scale") {
            // scale(sx, [sy])
            float sx = val[0];
            float sy = (val.size() > 1) ? val[1] : sx;
            finalMatrix.Scale(sx, sy, MatrixOrderPrepend);
        }
        else if (type == "matrix") {
            // matrix(a, b, c, d, e, f)
            if (val.size() >= 6) {
                Matrix matrix(val[0], val[1], val[2], val[3], val[4], val[5]);
                finalMatrix.Multiply(&matrix, MatrixOrderPrepend);
            }
        }
    }

    // B. MAPPING VÀO BOUNDING BOX (CHỈ DÀNH CHO OBJECT_BOUNDING_BOX)
    if (this->getUnits() == OBJECT_BOUNDING_BOX) {
        // Logic: Biến đổi từ hệ toạ độ đơn vị (0..1) sang hệ toạ độ thực (X..X+W, Y..Y+H)

        Matrix boundsMatrix;
        // 1. Dịch chuyển đến góc trái trên của hình (bounds.X, bounds.Y)
        boundsMatrix.Translate(bounds.X, bounds.Y);
        // 2. Phóng to theo kích thước hình (Width, Height)
        boundsMatrix.Scale(bounds.Width, bounds.Height);

        // Ốp ma trận này vào SAU CÙNG (Append)
        // Tức là: Gradient gốc -> Biến đổi SVG -> Ốp vào hình
        finalMatrix.Multiply(&boundsMatrix, MatrixOrderAppend);
    }

    // --- PHẦN 4: ÁP DỤNG MA TRẬN VÀO BRUSH VÀ DỌN DẸP ---
    fillBrush->SetTransform(&finalMatrix);

    delete[] colors;
    delete[] points;
    return fillBrush;
}

//Gdiplus::Brush* lineargradient::createBrush(Gdiplus::RectF bounds) {
//    using namespace Gdiplus;
//    PointF p1, p2;
//
//    // --- LOGIC MAPPING TỌA ĐỘ (Giữ nguyên) ---
//    if (this->getUnits() == OBJECT_BOUNDING_BOX) {
//        p1 = PointF(bounds.X + this->getA().getX() * bounds.Width, 
//                    bounds.Y + this->getA().getY() * bounds.Height);
//        p2 = PointF(bounds.X + this->getB().getX() * bounds.Width, 
//                    bounds.Y + this->getB().getY() * bounds.Height);
//    }
//    else {
//        p1 = PointF(this->getA().getX(), this->getA().getY());
//        p2 = PointF(this->getB().getX(), this->getB().getY());
//    }
//
//    // --- LOGIC STOP MÀU (Giữ nguyên) ---
//    vector<stop> ColorOffset = this->getStopVct();
//    int size = ColorOffset.size();
//    if (size > 0 && ColorOffset[0].offset != 0) {
//        color first = ColorOffset[0].stopColor;
//        float offset = ColorOffset[0].offset;
//        color zero{ first.r * (1 - offset), first.g * (1 - offset), first.b * (1 - offset), first.opacity * (1 - offset) };
//        ColorOffset.insert(ColorOffset.begin(), stop(zero, 0));
//        size++;
//    }
//    if (size > 0 && ColorOffset[size - 1].offset != 1) {
//        color last = ColorOffset[size - 1].stopColor;
//        float offset = ColorOffset[size - 1].offset;
//        color one{ last.r * (1 / offset), last.g * (1 / offset), last.b * (1 / offset), last.opacity * (1 / offset) };
//        ColorOffset.push_back(stop(one, 1));
//        size++;
//    }
//
//    float* points = new float[size];
//    Color* colors = new Color[size];
//    for (int k = 0; k < size; k++) {
//        points[k] = ColorOffset[k].offset;
//        colors[k] = Color(ColorOffset[k].stopColor.opacity * 255, ColorOffset[k].stopColor.r, ColorOffset[k].stopColor.g, ColorOffset[k].stopColor.b);
//    }
//
//    LinearGradientBrush* fillBrush = new LinearGradientBrush(p1, p2, colors[0], colors[size - 1]);
//    fillBrush->SetWrapMode(WrapModeTileFlipXY);
//    fillBrush->SetInterpolationColors(colors, points, size);
//
//    // --- SỬA LỖI PROBLEM 4: XỬ LÝ TRANSFORM THEO UNITS ---
//    vector<pair<string, vector<float>>> gradientTrans = this->getGradientTrans();
//    for (int k = 0; k < gradientTrans.size(); k++) {
//        string type = gradientTrans[k].first;
//        vector<float> val = gradientTrans[k].second;
//
//        if (this->getUnits() == OBJECT_BOUNDING_BOX) {
//            // NẾU LÀ OBJECT BOUNDING BOX: PHẢI NHÂN TRANSLATE VỚI BOUNDS
//            if (type == "translate") {
//                fillBrush->TranslateTransform(val[0] * bounds.Width, val[1] * bounds.Height);
//            }
//            else if (type == "rotate") {
//                fillBrush->RotateTransform(val[0]);
//            }
//            else if (type == "scale") {
//                fillBrush->ScaleTransform(val[0], val[1]);
//            }
//            else if (type == "matrix") {
//                // Matrix(a, b, c, d, dx, dy) -> dx, dy là thành phần thứ 4 và 5
//                // Cần nhân dx, dy với Width, Height
//                Matrix matrix(val[0], val[1], val[2], val[3], 
//                              val[4] * bounds.Width, 
//                              val[5] * bounds.Height);
//                fillBrush->MultiplyTransform(&matrix);
//            }
//        }
//        else {
//            // NẾU LÀ USER SPACE: GIỮ NGUYÊN (Logic cũ)
//            if (type == "translate") {
//                fillBrush->TranslateTransform(val[0], val[1]);
//            }
//            else if (type == "rotate") {
//                fillBrush->RotateTransform(val[0]);
//            }
//            else if (type == "scale") {
//                fillBrush->ScaleTransform(val[0], val[1]);
//            }
//            else if (type == "matrix") {
//                Matrix matrix(val[0], val[1], val[2], val[3], val[4], val[5]);
//                fillBrush->MultiplyTransform(&matrix); // Nên dùng Multiply thay vì SetTransform để cộng dồn
//            }
//        }
//    }
//
//    delete[] colors;
//    delete[] points;
//    return fillBrush;
//}
GradientType lineargradient::getType() {
    return LINEAR;
}
void lineargradient::updateElement() {
    stringstream sss(strLine);
    string temp = "", attribute = "", value = "", transformGradient = "";

    while (sss >> attribute) {
        getline(sss, temp, '"');
        getline(sss, value, '"');

        if (attribute == "x1") {
            A.setX(parseSVGValue1(value)); // Sử dụng hàm parse mới
        }
        if (attribute == "y1") {
            A.setY(parseSVGValue1(value));
        }
        if (attribute == "x2") {
            B.setX(parseSVGValue1(value));
        }
        if (attribute == "y2") {
            B.setY(parseSVGValue1(value));
        }
        if (attribute == "gradientUnits") {
            if (value == "userSpaceOnUse")
                this->setUnits(USER_SPACE_ON_USE); // Giả sử 0 là USER_SPACE
            else
                this->setUnits(OBJECT_BOUNDING_BOX); // Giả sử 1 là OBJECT_BOUNDING_BOX
        }
        if (attribute == "gradientTransform") {
            transformGradient = value;
        }
    }
    this->updateGradientTransform(transformGradient);
}