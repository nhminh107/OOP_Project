#ifndef _POLYLINE_H_
#define _POLYLINE_H_

class Polyline : public Shape {
private:
	vector<point> Vers;

public:
	Polyline();
	~Polyline();

	void updateProperty();

	vector<point> getVers();
	void setVers(vector<point>);
};

#endif
