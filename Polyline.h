#ifndef _POLYLINE_H_
#define _POLYLINE_H_

class polyline : public Shape {
private:
	vector<point> Vers;

public:
	polyline();
	~polyline();

	void draw(Graphics&) override;
	void updateProperty();

	vector<point> getVers();
	void setVers(vector<point>);
};

#endif