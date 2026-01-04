#include "Library.h"

SVGPolygon::SVGPolygon() : Shape() {
    Vers = {};
}

SVGPolygon::~SVGPolygon() {
    Vers = {};
}

void SVGPolygon::updateProperty() {

    stringstream ss(line_str);
    string property, val, temp;

    while (ss >> property) {
        getline(ss, temp, '"');
        getline(ss, val, '"');
        if (property == "points" || property == "point") {
            for (int i = 0; i < val.size(); i++)
                if (val[i] == ',')
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

vector<point> SVGPolygon::getVers() {
    return this->Vers;
}

void SVGPolygon::setVers(vector<point> Vers) {
    this->Vers = Vers;
}

void SVGPolygon::draw(Graphics& graphics) {
    //Luu trang thai
    GraphicsState save = graphics.Save();

    //Ap dung transform
    Matrix mat;
    mat.SetElements(1, 0, 0, 1, 0, 0); 
    this->getTransformMatrix(&mat);
    graphics.MultiplyTransform(&mat);

    //Pen
    Pen penPolygon(
        Color(
            this->getStroke().getStrokeColor().opacity * 255,
            this->getStroke().getStrokeColor().r,
            this->getStroke().getStrokeColor().g,
            this->getStroke().getStrokeColor().b
        ),
        this->getStroke().getStrokeWidth()
    );

    //Chuan bi brush
    Brush* fillBrush = nullptr;


    //Tinh bounding box
    float minX = FLT_MAX, minY = FLT_MAX;
    float maxX = -FLT_MAX, maxY = -FLT_MAX;

    for (auto& p : Vers) {
        minX = min(minX, p.getX());
        minY = min(minY, p.getY());
        maxX = max(maxX, p.getX());
        maxY = max(maxY, p.getY());
    }

    RectF bound(minX, minY, maxX - minX, maxY - minY);

    //Neu co gradient
    if (this->getGrad()) {
        fillBrush = this->getGrad()->createBrush(bound);
    }

   //Neu khong co gradient
    if (!fillBrush) {
        fillBrush = new SolidBrush(
            Color(this->getColor().opacity * 255,this->getColor().r,this->getColor().g,this->getColor().b)
        );
    }

    //Tao mang cac point
    int numPoint = (int)Vers.size();
    vector<PointF> points;
    points.reserve(numPoint);

    for (auto& pt : Vers) {
        points.emplace_back(pt.getX(), pt.getY());
    }

    //draw
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.FillPolygon(fillBrush, points.data(), numPoint, FillModeWinding);
    graphics.DrawPolygon(&penPolygon, points.data(), numPoint);

    //delete 
    delete fillBrush;
    graphics.Restore(save);
}
