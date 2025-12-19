#ifndef _SHAPE_H_
#define _SHAPE_H_

class Shape {
protected:
    string shape, text_name, line_str;
    vector<pair<string, vector<float>>> transVct;
    bool isSelected;
    stroke strk;
    color fill;
    string fillGradientID; //Lưu tên ID, vd: "grad1". Khi Parser thì lưu vào theo ID luôn
    bool isUseGradient; //True nếu ID != "" để quá trình vẽ nhanh hơn
public:
    Shape();
    virtual ~Shape();
    virtual void updateProperty();
    string getName();
    string getTextName();
    stroke getStroke();
    color getColor();
    string getFillGradientID();
    bool getIsUseGradient();
    vector<pair<string, vector<float>>> getTransVct();
    void setName(string);
    void setFillGradientID(const string& id);
    void setTextName(string);
    void setLine(string);
    void setStroke(stroke);
    void setSelected(bool s) { isSelected = s;  }
    void setColor(color);
    void updateTransformVct(string);
    void getTransformMatrix(Gdiplus::Matrix* matrix);
    virtual RectF getBoundingBox() = 0; 
    virtual void draw(Graphics& graphics) = 0;
};

#endif