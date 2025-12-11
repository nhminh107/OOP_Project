#ifndef _CIRCLE_H_
#define _CIRCLE_H_

class Circle : public Shape {
private:
	float radius;
	point center;

public:
	Circle();
	~Circle();

	point getCenter();
	float getRadius();

	void setCenter(float, float);
	void setRadius(float);

	void updateProperty();
	void drawCircle(Graphics&, circle*);
};

#endif

