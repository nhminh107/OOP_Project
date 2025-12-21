#ifndef _RADIALGRADIENT_H_
#define _RADIALGRADIENT_H_

#include"Library.h"
class radialGradient : public gradient {
private:
	float cx, cy, r, fx, fy;
	bool isLink;
public:
	radialGradient();
	radialGradient(const radialGradient&);
	radialGradient& operator=(const radialGradient&);

	float getCx() const;
	float getCy() const;
	float getR() const;
	float getFx() const;
	float getFy() const;
	bool getIsLink() const;

	void setCX(float);
	void setCY(float);
	void setR(float);
	void setFX(float);
	void setFY(float);
	void setIsLink(bool);

	void updateElement() override;
};

#endif
