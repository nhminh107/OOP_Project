#include "Library.h"

polygon::polygon() : Shape() {
	Vers = {};
}

polygon::~polygon() {
	Vers = {};
}

void polygon::updateProperty() {

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

vector<point> polygon::getVers() {
	return this->Vers;
}

void polygon::setVers(vector<point> Vers) {
	this->Vers = Vers;
}
void polygon::draw(Graphics& graphics){
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

