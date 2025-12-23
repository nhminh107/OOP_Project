#ifndef _SHAPE_H_
#define _SHAPE_H_

class Shape {
protected:
    string shape, text_name, line_str;
    vector<pair<string, vector<float>>> transVct;
    bool isSelected;
    stroke strk;
    color fill;

    string fillGradientID; // Thêm cái này
    bool hasGradient;      // Để biết lúc vẽ thì dùng Brush hay dùng Color

public:
    Shape();
    virtual ~Shape();    
    virtual void updateProperty();

    string getName();
    string getTextName();
    stroke getStroke();
    color getColor();
    vector<pair<string, vector<float>>> getTransVct();
    void setName(string);
    void setTextName(string);
    void setLine(string);
    void setStroke(stroke);
    void setHasGradient(bool); 
    void setGradientID(const string&); 
    void setSelected(bool s) { isSelected = s;  }
    void setColor(color);
    void updateTransformVct(string);
    void getTransformMatrix(Gdiplus::Matrix* matrix);
    virtual RectF getBoundingBox() = 0; 
    virtual void draw(Graphics& graphics) = 0;
};

#endif