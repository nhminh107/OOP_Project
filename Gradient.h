#ifndef  GRADIENT_H
#define GRADIENT_H

struct stop {
	float offset; 
	color stopColor;  
};

enum GradientType {
	LINEAR, 
	RADIAL 
}; //Khi dùng chỉ cần gọi GradientType::

enum GradientUnits {
	OBJECT_BOUNDING_BOX, //default of SVG 
	USER_SPACE_ON_USE 
}; 
class Gradient {
protected: 
	vector<stop> stopList;
	string id; 
	Matrix transformMatrix; 
	GradientUnits units;
	WrapMode spreadMethod; 
public: 
	Gradient(); 
	virtual ~Gradient(); 
	void setID(string id);
	string getID() const;
	/*BỔ SUNG TẤT CẢ SETTER / GETTER */
	virtual GradientType getType() const = 0; 
	void addStop(float offset, color c); 
	virtual Gdiplus::Brush* createBrush(const Gdiplus::RectF& shapeBound, float opacity) = 0; 

	/*Hàm creatBrush: 
	Mỗi hình có thể có fill-opacity riêng, mỗi stop cũng có opacity riêng. Khi Brush thì phải lwaays Opacity từng stop nhân với tổng thể của Shape
	để ra màu sắc cuối cùng trên màn hình
	
	Trong Shape sẽ dùng getBoundingBox() xong truyền vào đây */
};
#endif // ! GRADIENT_H
