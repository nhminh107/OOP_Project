#include "Library.h"

SVGPolyline::SVGPolyline() : Shape() {
	fill.r = fill.g = fill.b = 0;
	fill.opacity = 1;
}

SVGPolyline::~SVGPolyline() {
	Vers = {};
}

void SVGPolyline::updateProperty() {
	stringstream ss(line_str);
	string property, val, temp;

	while (ss >> property) {
		getline(ss, temp, '"');
		getline(ss, val, '"');

		if (property == "points" || property == "point") {
			for (int i = 0; i < val.size(); i++)
				if (val[i] == ',' || val[i] == '/')
					val[i] = ' ';

			stringstream ss(val);
			string x = "", y = "";
			while (ss >> x >> y) {
				point p;
				p.setX(stof(x));
				p.setY(stof(y));
				Vers.push_back(p);
			}
		}
	}
}

void SVGPolyline::draw(Graphics& graphics) {
    // 1. Lưu trạng thái
    GraphicsState save = graphics.Save();

    // 2. Tạo bút và cọ từ dữ liệu của object (dùng this->)
    Pen penPolyline(Color(this->getStroke().getStrokeColor().opacity * 255,
        this->getStroke().getStrokeColor().r,
        this->getStroke().getStrokeColor().g,
        this->getStroke().getStrokeColor().b),
        this->getStroke().getStrokeWidth());

    SolidBrush fillPolyline(Color(this->getColor().opacity * 255,
        this->getColor().r,
        this->getColor().g,
        this->getColor().b));

    // 3. Xử lý Transform
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
        else if (trans.first == "matrix") {
            if (trans.second.size() >= 6) {
                Matrix matrix(trans.second[0], trans.second[1], trans.second[2],
                    trans.second[3], trans.second[4], trans.second[5]);
                graphics.MultiplyTransform(&matrix);
            }
        }
    }

    vector<point> vertices = this->getVers();
    int numPoint = vertices.size();

    if (numPoint > 0) {
        PointF* p = new PointF[numPoint];
        for (int i = 0; i < numPoint; i++) {
            // Lấy từ biến cục bộ 'vertices' thay vì gọi hàm this->getVers()
            p[i] = PointF(vertices[i].getX(), vertices[i].getY());
        }

        graphics.SetSmoothingMode(SmoothingModeAntiAlias);

        graphics.FillPolygon(&fillPolyline, p, numPoint, FillModeWinding);

        graphics.DrawLines(&penPolyline, p, numPoint);

        delete[] p; 
    }

    graphics.Restore(save);
}

vector<point> SVGPolyline::getVers() {
	return this->Vers;
}

void SVGPolyline::setVers(vector<point> Vers) {
	this->Vers = Vers;
}