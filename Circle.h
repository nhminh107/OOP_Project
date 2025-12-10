#ifndef _CIRCLE_H_
#define _CIRCLE_H_

class Circle : public Shape {
private:
	float rx, ry;
	point center;

public:
	Circle();
	~Circle();

	point getCenter();
	float getRx();
	float getRy();

	void setCenter(float, float);
	void setRx(float);
	void setRy(float);

	void updateProperty();
};

#endif

