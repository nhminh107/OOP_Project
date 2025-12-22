#ifndef RADIALGRADIENT_H
#define RADIALGRADIENT_H

class RadialGradient : public Gradient {
private: 
	point center; 
	float radius; 
	point focalPoint; //Điểm hội tụ 
public: 
	RadialGradient(); 
	point getCenter() const ;
	point getFocal() const;
	float getRadius() const;
	void setFocal(float x1, float x2); 
	void setCenter(float x, float y); 
	void setRadius(float r); 
	virtual GradientType getType() const = 0;
	Gdiplus::Brush* createBrush(const Gdiplus::RectF& shapeBound, float opacity) override;
};
#endif // !RADIALGRADIENT_H
