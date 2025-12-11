#ifndef _POLYGON_H_
#define _POLYGON_H_

class polygon : public Shape {
private:
	vector<point> Vers;

public:
	polygon();
	~polygon();

	void setVers(vector<point>);
	vector<point> getVers();

	void updateProperty();
	void draw(Graphics&) override;
};

#endif