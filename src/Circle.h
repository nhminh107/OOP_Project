#ifndef _CIRCLE_H_
#define _CIRCLE_H_

class SVGCircle : public Shape {
private:
	float radius;
	point center;

public:
	SVGCircle();
	~SVGCircle();

	point getCenter();
	float getRadius();

	void setCenter(float, float);
	void setRadius(float);

	void updateProperty();
	void draw(Graphics&) override;
};

#endif
