#ifndef _LINEARGRADIENT_H_
#define _LINEARGRADIENT_H_
#include"Library.h"

class linearGradient : public gradient {
private:
	point A, B;
public:
	void setX1(float v) { A.setX(v); }
	void setY1(float v) { A.setY(v); }
	void setX2(float v) { B.setX(v); }
	void setY2(float v) { B.setY(v); }
};


#endif
