#include "Library.h"

SVGPolygon::SVGPolygon() : Shape() {
    Vers = {};
}

SVGPolygon::~SVGPolygon() {
    Vers = {};
}

RectF SVGPolygon::getBoundingBox() {

    if (this->Vers.empty()) {
        return RectF(0, 0, 0, 0);
    }

    float min_X = FLT_MAX;
    float min_Y = FLT_MAX;
    float max_X = -FLT_MAX;
    float max_Y = -FLT_MAX;

    for (auto& p : this->Vers) {
        float x_cur = p.getX();
        float y_cur = p.getY();

        if (x_cur > max_X) max_X = x_cur;
        if (x_cur < min_X) min_X = x_cur;
        if (y_cur > max_Y) max_Y = y_cur;
        if (y_cur < min_Y) min_Y = y_cur;
    }

    RectF boundingBox;
    boundingBox.X = min_X;
    boundingBox.Y = min_Y;

    boundingBox.Width = max_X - min_X;
    boundingBox.Height = max_Y - min_Y;

    return boundingBox;
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
    GraphicsState save = graphics.Save();

    Pen penPolygon(
        Color(this->getStroke().getStrokeColor().opacity * 255,
            this->getStroke().getStrokeColor().r,
            this->getStroke().getStrokeColor().g,
            this->getStroke().getStrokeColor().b),
        this->getStroke().getStrokeWidth()
    );

    SolidBrush fillPolygon(
        Color(this->getColor().opacity * 255,
            this->getColor().r,
            this->getColor().g,
            this->getColor().b)
    );

    vector<pair<string, vector<float>>> transVct = this->getTransVct();
    for (auto trans : transVct) {
        float x = 0.f;
        if (!trans.second.empty())
            x = trans.second[0];

        float y = x;
        if (trans.second.size() == 2)
            y = trans.second[1];

        if (trans.first == "translate")
            graphics.TranslateTransform(x, y);
        else if (trans.first == "rotate")
            graphics.RotateTransform(x);
        else
            graphics.ScaleTransform(x, y);
    }

    int numPoint = this->getVers().size();
    PointF* p = new PointF[numPoint];
    for (int i = 0; i < numPoint; i++)
        p[i] = PointF(this->getVers()[i].getX(),
            this->getVers()[i].getY());

    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.FillPolygon(&fillPolygon, p, numPoint, FillModeWinding);
    graphics.DrawPolygon(&penPolygon, p, numPoint);
    graphics.Restore(save);
    delete[] p;
}
