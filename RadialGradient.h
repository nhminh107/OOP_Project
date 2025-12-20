#ifndef _RADIALGRADIENT_H_
#define _RADIALGRADIENT_H_
class radialGradient : public gradient{
private:
	float cx, cy, r, fx, fy;
	bool isLink;
public:
	radialGradient() : cx(0.5), cy(0.5), r(0.5), fx(0.5), fy(0.5), isLink(false) {}

	// Setter helper
	void setCX(float v) { cx = v; }
	void setCY(float v) { cy = v; }
	void setR(float v) { r = v; }
	void setFX(float v) { fx = v; }
	void setFY(float v) { fy = v; }
};

#endif
