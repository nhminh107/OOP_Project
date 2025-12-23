#ifndef _PARSER_H_
#define _PARSER_H_

class parser {
private:
	unordered_map<string, color> colorMap;

	void processColor(string, string, color&);
	void loadColorMap();
	void processProperty(string, string, string, Shape*&);
	Gradient* currentProcessingGradient; 

public:
	~parser(); //Viết hàm Hủy cho Parser 
	void parseGradient(string name, string property);
	float parseUnit(string s); 
	void parseItem(SVGGroup*, string, viewbox&);
};

#endif