#ifndef _PARSER_H_
#define _PARSER_H_

class parser {
private:
	unordered_map<string, color> colorMap;
	unordered_map<string, gradient*> idMap;
	void processColor(string, string, color&);
	void loadColorMap();
	void processProperty(string, string, string, Shape*&);

public:
	~parser();
	void parseItem(SVGGroup*, string, viewbox&);
};

#endif