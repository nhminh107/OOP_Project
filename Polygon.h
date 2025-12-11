#ifndef _POLYGON_H_
#define _POLYGON_H_

class polygon : public Shape {
private:
	vector<point> Vers;

public:
	Polygon();
	~Polygon();

	void setVers(vector<point>);
	vector<point> getVers();

	void updateProperty();
};

#endif
