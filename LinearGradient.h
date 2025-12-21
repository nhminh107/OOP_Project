#ifndef _LINEARGRADIENT_H_
#define _LINEARGRADIENT_H_
#include"Library.h"

class linearGradient : public gradient {
private:
	point A, B;
public:
	void setX1(float v);
	void setY1(float v);
	void setX2(float v);
	void setY2(float v);

	point getA() const;
	point getB() const;

	linearGradient();
	linearGradient(const linearGradient&);
	linearGradient& operator = (const linearGradient&);

	void updateElement() override;
};


#endif
