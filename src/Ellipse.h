#ifndef _ELLIPSE_H_
#define _ELLIPSE_H_

class SVGEllipse : public Shape {
private:
	float rx, ry;
	point center;

public:
	SVGEllipse();
	~SVGEllipse();

	point getCenter();
	float getRx();
	float getRy();

	void setCenter(float, float);
	void setRx(float);
	void setRy(float);

	void updateProperty();
	void draw(Graphics&) override;
};

#endif 