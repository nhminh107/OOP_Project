#ifndef _POLYGON_H_
#define _POLYGON_H_

class SVGPolygon : public Shape {
private:
	vector<point> Vers;

public:
	SVGPolygon();
	~SVGPolygon();

	void setVers(vector<point>);
	vector<point> getVers();
	RectF getBoundingBox() override;
	void updateProperty();
	void draw(Graphics&) override;
};

#endif