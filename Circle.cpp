#include "Library.h"

SVGCircle::SVGCircle() : Shape() {
    this->setCenter(0, 0);
    this->setRadius(0);
}

SVGCircle::~SVGCircle() {
    this->setCenter(0, 0);
    this->setRadius(0);
}

void SVGCircle::updateProperty() {
    stringstream ss(line_str);
    string attribute, val, temp;

    while (ss >> attribute) {
        getline(ss, temp, '"');
        getline(ss, val, '"');

        if (attribute == "r") {
            this->setRadius(stof(val));
        }
        if (attribute == "cx")
            this->center.setX((stof(val)));
        if (attribute == "cy")
            this->center.setY((stof(val)));
    }
}
point SVGCircle::getCenter() {
    return this->center;
}

float SVGCircle::getRadius() {
    return this->radius;
}

void SVGCircle::setCenter(float x, float y) {
    this->center.setX(x);
    this->center.setY(y);
}

void SVGCircle::setRadius(float r) {
    this->radius = r;
}

RectF SVGCircle::getBoundingBox() {
    RectF boundingBox;
    boundingBox.X = this->center.getX() - this->radius;
    boundingBox.Y = this->center.getY() - this->radius;

    boundingBox.Width = 2.0 * radius;
    boundingBox.Height = 2.0 * radius;

    return boundingBox;
}
void SVGCircle::draw(Graphics& graphics) {
    // 1. Lưu trạng thái Graphics
    GraphicsState save = graphics.Save();

    // 2. Tạo bút vẽ (Pen) - Lấy từ dữ liệu của chính mình (this)
    Pen penCircle(Color(this->getStroke().getStrokeColor().opacity * 255,
        this->getStroke().getStrokeColor().r,
        this->getStroke().getStrokeColor().g,
        this->getStroke().getStrokeColor().b),
        this->getStroke().getStrokeWidth());

    // 3. Tạo cọ tô màu (Brush)
    SolidBrush fillCircle(Color(this->getColor().opacity * 255,
        this->getColor().r,
        this->getColor().g,
        this->getColor().b));

    // 4. Xử lý Transform
    vector<pair<string, vector<float>>> transVct = this->getTransVct();
    for (auto trans : transVct) {
        float x = 0.0f;
        if (!trans.second.empty()) x = trans.second[0];
        float y = x;
        if (trans.second.size() == 2) y = trans.second[1];

        if (trans.first == "translate")
            graphics.TranslateTransform(x, y);
        else if (trans.first == "rotate")
            graphics.RotateTransform(x);
        else if (trans.first == "scale")
            graphics.ScaleTransform(x, y);
        else if (trans.first == "matrix") { // Bổ sung cho đủ bộ
            if (trans.second.size() >= 6) {
                Matrix matrix(trans.second[0], trans.second[1], trans.second[2],
                    trans.second[3], trans.second[4], trans.second[5]);
                graphics.MultiplyTransform(&matrix);
            }
        }
    }

    // 5. Tính toán tọa độ vẽ
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);

    float r = this->getRadius(); // Lấy bán kính
    float d = 2.0f * r;          // Đường kính

    // GDI+ vẽ Ellipse từ góc trên-trái, nên phải trừ đi bán kính từ tâm
    float x_pos = this->getCenter().getX() - r;
    float y_pos = this->getCenter().getY() - r;

    // 6. Vẽ và Tô màu
    graphics.FillEllipse(&fillCircle, x_pos, y_pos, d, d);
    graphics.DrawEllipse(&penCircle, x_pos, y_pos, d, d);

    // 7. Khôi phục trạng thái
    graphics.Restore(save);
}