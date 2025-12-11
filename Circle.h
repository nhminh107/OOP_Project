#ifndef _CIRCLE_H_
#define _CIRCLE_H_

class circle : public Shape {
private:
	float radius;
	point center;

public:
	circle();
	~circle();

	point getCenter();
	float getRadius();

	void setCenter(float, float);
	void setRadius(float);

	void updateProperty();
	void drawCircle(Graphics&, circle*);
};

#endif

