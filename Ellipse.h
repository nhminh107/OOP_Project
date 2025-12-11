#ifndef _ELLIPSE_H_
#define _ELLIPSE_H_

class ellipse : public Shape {
private:
	float rx, ry;
	point center;

public:
	ellipse();
	~ellipse();

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