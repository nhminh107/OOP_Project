#ifndef _POLYLINE_H_
#define _POLYLINE_H_

class SVGPolyline : public Shape {
private:
	vector<point> Vers;

public:
	SVGPolyline();
	~SVGPolyline();

	void draw(Graphics&) override;
	void updateProperty();

	vector<point> getVers();
	void setVers(vector<point>);
};

#endif