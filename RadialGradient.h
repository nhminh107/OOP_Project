#ifndef RADIALGRADIENT_H
#define RADIALGRADIENT_H

class RadialGradient : public Gradient {
private: 
	point center; 
	float radius; 
	point focalPoint; //Điểm hội tụ 
public: 
	RadialGradient(); 
	point getCenter() { return center; }
	point getFocal() { return focalPoint; }
	float getRadius() { return radius; }
	void setFocal(float x1, float x2); 
	void setCenter(float x, float y); 
	void setRadius(float r); 
	GradientType getType() const;
	Gdiplus::Brush* createBrush(const Gdiplus::RectF& shapeBound, float opacity) override;
};
#endif // !RADIALGRADIENT_H
