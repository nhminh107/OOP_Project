#ifndef _PARSER_H_
#define _PARSER_H_
#include "Library.h"
class gradient;
class parser {
private:
	unordered_map<string, color> colorMap;
	map< string, gradient* > gradientMap;
	void processColor(string, string, color&);
	void loadColorMap();
	void processProperty(string, string, string, Shape*&);

public:
	~parser();
	void parseItem(SVGGroup*, string, viewbox&);
};

#endif