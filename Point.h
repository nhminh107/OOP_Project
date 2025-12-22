#ifndef _POINT_H_
#define _POINT_H_

class point {
public:
	float x, y;
	bool intersect;

public:
	point();
	~point();

	float getX();
	float getY();
	bool getIntersect();

	void setIntersect(bool);
	void setX(float);
	void setY(float);

	friend bool operator == (const point&, const point&);
};

#endif