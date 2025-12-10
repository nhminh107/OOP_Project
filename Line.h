#ifndef _LINE_H_
#define _LINE_H_

class Line :public Shape {
private:
	point p1, p2;

public:
	Line();
	~Line();

	void updateProperty();

	point getP1();
	point getP2();

	void setP1(point);
	void setP2(point);
};

#endif