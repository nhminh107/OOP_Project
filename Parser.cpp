#include "Library.h"

// --- HÀM LOAD BẢNG MÀU (GIỮ NGUYÊN) ---
void parser::loadColorMap() {
	ifstream color_file("Color.txt", ios::in);

	if (!color_file.is_open()) {
		cout << "Error Loading Color File\n";
		return;
	}

	string color_line = "";
	while (getline(color_file, color_line)) {
		stringstream ss(color_line);
		string token = "";
		vector<string> vct;

		while (ss >> token)
			vct.push_back(token);

		string color_name, hexa_code;
		int n = vct.size();

		for (int i = 0; i < n - 1; i++)
			color_name = color_name + vct[i];

		hexa_code = vct[n - 1];
		color color;
		color.r = stoi(hexa_code.substr(1, 2), NULL, 16);
		color.g = stoi(hexa_code.substr(3, 2), NULL, 16);
		color.b = stoi(hexa_code.substr(5, 2), NULL, 16);

		colorMap[color_name] = color;
	}
	colorMap["none"] = { 0, 0, 0, 0 };
}

// --- HÀM XỬ LÝ MÀU ĐƠN SẮC (GIỮ NGUYÊN) ---
void parser::processColor(string strokecolor, string strokeopa, color& clr) {
	// Xử lý trường hợp "url(#...)" còn sót lại (nếu có) -> coi như trong suốt
	if (strokecolor.find("url") != string::npos) {
		clr = { 0, 0, 0, 0 }; // Transparent
		return;
	}

	if (strokecolor.find("rgb") != string::npos) {
		clr.opacity = stof(strokeopa);

		for (int i = 0; i < strokecolor.size(); i++) {
			if (!isdigit(strokecolor[i]))	//If the character is not number then change to ' '
				strokecolor[i] = ' ';
		}

		stringstream ss(strokecolor);
		string r, g, b;
		ss >> r >> g >> b;
		clr.r = stof(r); clr.g = stof(g); clr.b = stof(b);
		if (clr.r > 255)
			clr.r = 255.0;
		if (clr.g > 255)
			clr.g = 255.0;
		if (clr.b > 255)
			clr.b = 255.0;
		ss.ignore();
	}
	else if (strokecolor[0] == '#') {
		if (strokecolor.size() == 4) {
			string tmp = "#";
			for (int i = 1; i < 4; i++)
				tmp = tmp + strokecolor[i] + strokecolor[i];
			strokecolor = tmp;
		}
		clr.r = stoi(strokecolor.substr(1, 2), NULL, 16);
		clr.g = stoi(strokecolor.substr(3, 2), NULL, 16);
		clr.b = stoi(strokecolor.substr(5, 2), NULL, 16);
		clr.opacity = stof(strokeopa);
	}
	else {
		for (int i = 0; i < strokecolor.size(); i++)
			if (isupper(strokecolor[i]))
				strokecolor[i] = tolower(strokecolor[i]);
		clr = colorMap[strokecolor];
		clr.opacity = stof(strokeopa);
	}
}

// --- HÀM XỬ LÝ THUỘC TÍNH (ĐÃ BỎ LOGIC GRADIENT) ---
void parser::processProperty(string name, string property, string textName, Shape*& shape) {
	shape->setName(name);
	shape->setTextName(textName);
	shape->setLine(property);

	stringstream ss(property);
	string attribute, value;
	string strokeWidth = "1", sStroke = "", strokeOpa = "1", fill = "", fillOpa = "1";
	string strTransform = "";
	string temp = "";

	while (ss >> attribute) {
		getline(ss, temp, '"');
		getline(ss, value, '"');

		if (attribute == "style") {
			// Xử lý chuỗi style="..."
			for (int i = 0; i < value.size(); i++) {
				if (value[i] == ':') {
					value.insert(value.begin() + i + 1, '"');
					i++;
				}
				else if (value[i] == ';') {
					value.insert(value.begin() + i, '"');
					i++;
				}
			}
			value.push_back('"');

			for (int i = 0; i < value.size(); i++) {
				if (value[i] == ':' || value[i] == ';')
					value[i] = ' ';
			}
			stringstream valStream(value);
			string attr, subVal, subTemp;

			while (valStream >> attr) {
				getline(valStream, subTemp, '"');
				getline(valStream, subVal, '"');

				if (attr == "fill") {
					fill = subVal;
				}
				if (attr == "fill-opacity") {
					fillOpa = subVal;
				}
				if (attr == "stroke-width") {
					strokeWidth = subVal;
				}
				if (attr == "stroke-opacity") {
					strokeOpa = subVal;
				}
				if (attr == "stroke") {
					sStroke = subVal; // Đã sửa value thành subVal cho đúng logic style
				}
			}
		}

		// Xử lý các attribute rời rạc
		if (attribute == "stroke-width")
			strokeWidth = value;
		if (attribute == "fill-opacity")
			fillOpa = value;
		if (attribute == "fill") {
			fill = value;
		}
		if (attribute == "stroke")
			sStroke = value;
		if (attribute == "stroke-opacity")
			strokeOpa = value;
		if (attribute == "transform")
			strTransform += (" " + value + " ");
	}
	// Kiểm tra nếu fill có dạng "url(#...)"
	if (fill.find("url") != string::npos) {
		size_t start = fill.find("#");
		size_t end = fill.find(")");
		if (start != string::npos) {
			string idStr = (end != string::npos) ? fill.substr(start + 1, end - start - 1) : fill.substr(start + 1);

			// Lưu ID vào Shape (theo cấu trúc Shape.h của bạn)
			shape->setFillGradientID(idStr);

			// Kiểm tra ID có tồn tại trong map không để đánh dấu
			if (gradientMap.find(idStr) != gradientMap.end()) {
				
			}
		}
		shape->setColor({ 0, 0, 0, 0 }); // Màu base trong suốt
	}
	else {
		// 1. Xử lý màu Fill
		color clr = { 0, 0, 0, 1 };
		if (fill == "none" || fill == "transparent" || fill == "") {
			// Nếu không có fill hoặc fill=none thì coi như trong suốt
			
			processColor(fill, "0", clr);
		}
		else {
			processColor(fill, fillOpa, clr);
		}
		shape->setColor(clr);
	}
	// --- LOGIC MỚI: CHỈ XỬ LÝ MÀU ĐƠN SẮC ---
	

	// 2. Xử lý Stroke (Viền)
	stroke strk;
	strk.setStrokeWidth(stof(strokeWidth));
	color strokeColor = { 0, 0, 0, 1 };

	if (sStroke == "none" || sStroke == "transparent" || sStroke == "")
		processColor(sStroke, "0", strokeColor);
	else
		processColor(sStroke, strokeOpa, strokeColor);

	strk.setStrokeColor(strokeColor);
	shape->setStroke(strk);

	// 3. Xử lý Transform & Update Property riêng của hình
	shape->updateProperty();
	if (!strTransform.empty()) {
		shape->updateTransformVct(strTransform);
	}
}

// --- HÀM PARSE ITEM (ĐÃ BỎ ĐỌC DEFS/GRADIENT) ---
void parser::parseItem(SVGGroup* root, string fileName, viewbox& vb) {
	ifstream fin(fileName, ios::in);
	if (!fin.is_open()) {
		cout << "Error Opening SVG File\n";
		return;
	}
	loadColorMap();

	string line_str = "";
	ShapeFactory factory;

	stack<string> groupStack;
	groupStack.push(" ");

	SVGGroup* curGroup = root;

	gradient* curGradient = NULL;
	bool inDefs = false;

	// Reset ViewBox
	vb.setPortWidth(0); vb.setPortHeight(0);
	float viewX = 0, viewY = 0, viewWidth = 0, viewHeight = 0, portWidth = 0, portHeight = 0;
	string preservedForm = "", preservedMode = "";

	while (getline(fin, line_str, '>')) {
		line_str += ">";
		string name = "", property = "", textContent = "";
		stringstream stream(line_str);
		stream >> name;
		getline(stream, property, '>');

		// Chuẩn hóa chuỗi property
		for (int i = 0; i < property.size(); i++) {
			if (property[i] == '/' || property[i] == '=') {
				property[i] = ' ';
			}
			if (property[i] == '\'') {
				property[i] = '"';
			}
		}

		if (!name.empty() && name[0] == '<') name.erase(0, 1);

		// 1. Quản lý thẻ <defs>
		if (name == "defs") { inDefs = true; continue; }
		if (name == "/defs") { inDefs = false; continue; }

		// 2. Xử lý thẻ mở Gradient
		if (name == "linearGradient" || name == "radialGradient") {
			if (name == "linearGradient") curGradient = new linearGradient();
			else curGradient = new radialGradient();

			stringstream ss(property);
			string attr, val, temp;
			while (ss >> attr) {
				getline(ss, temp, '"');
				getline(ss, val, '"');

				if (attr == "id") curGradient->setGradID(val);

				if (name == "linearGradient") {
					linearGradient* lg = (linearGradient*)curGradient;
					// Gán vào biến A, B của struct LinearGradient
					if (attr == "x1") lg->setX1(stof(val)); 
					if (attr == "y1") lg->setY1(stof(val));
					if (attr == "x2") lg->setX2(stof(val)); 
					if (attr == "y2") lg->setY2(stof(val)); 
				}
				else {
					radialGradient* rg = (radialGradient*)curGradient;
					// Gán vào biến cx, cy, r... của struct RadialGradient
					if (attr == "cx") rg->setCX(stof(val));
					if (attr == "cy") rg->setCY(stof(val));
					if (attr == "r")  rg->setR(stof(val));
					if (attr == "fx") rg->setFX(stof(val));
					if (attr == "fy") rg->setFY(stof(val));
				}
			}
		}

		// 3. Xử lý thẻ <stop>
		else if (name == "stop" && curGradient != NULL) {
			stringstream ss(property);
			string attr, val, temp;
			float offset = 0;
			string stopColorStr = "black";
			string stopOpacityStr = "1"; // Mặc định opacity là 1

			while (ss >> attr) {
				getline(ss, temp, '"');
				getline(ss, val, '"');

				if (attr == "offset") {
					if (!val.empty() && val.back() == '%') {
						val.pop_back();
						offset = stof(val) / 100.0f;
					}
					else {
						offset = stof(val);
					}
				}
				if (attr == "stop-color") stopColorStr = val;
				if (attr == "stop-opacity") stopOpacityStr = val;

				// Xử lý trường hợp style="stop-color:..."
				if (attr == "style") {
					if (val.find("stop-color") != string::npos) {
						size_t pos = val.find("stop-color");
						size_t start = val.find(":", pos) + 1;
						size_t end = val.find(";", pos);
						if (end == string::npos) end = val.length();
						stopColorStr = val.substr(start, end - start);
					}
					// Kiểm tra thêm stop-opacity trong style nếu cần
					if (val.find("stop-opacity") != string::npos) {
						size_t pos = val.find("stop-opacity");
						size_t start = val.find(":", pos) + 1;
						size_t end = val.find(";", pos);
						if (end == string::npos) end = val.length();
						stopOpacityStr = val.substr(start, end - start);
					}
				}
			}

			color c;
			
			processColor(stopColorStr, "1", c);
			c.opacity = stof(stopOpacityStr);

			curGradient->addStop(stop(offset, c));
		}

		else if ((name == "/linearGradient" || name == "/radialGradient") && curGradient) {
			if (!curGradient->getGradID().empty()) {
				gradientMap[curGradient->getGradID()] = curGradient;
			}
			curGradient = NULL;
		}
		if (name == "<svg") {
			stringstream sss(property);
			string attribute, temp, val;

			while (sss >> attribute) {
				getline(sss, temp, '"');
				getline(sss, val, '"');
				if (attribute == "viewBox") {
					stringstream ssss(val);
					ssss >> viewX >> viewY >> viewWidth >> viewHeight;
					vb.setViewX(viewX);
					vb.setViewY(viewY);
					vb.setViewWidth(viewWidth);
					vb.setViewHeight(viewHeight);
				}
				if (attribute == "preserveAspectRatio") {
					stringstream ssss(val);
					ssss >> preservedForm >> preservedMode;
					vb.setPreservedForm(preservedForm);
					vb.setPreservedMode(preservedMode);
				}
				if (attribute == "width") {
					portWidth = stof(val);
					if (val.find("pt") != string::npos) portWidth *= 96.f / 72.f;
					else if (val.find("cm") != string::npos) portWidth *= 96.f / 2.54f;
					vb.setPortWidth(portWidth);
				}
				if (attribute == "height") {
					portHeight = stof(val);
					if (val.find("pt") != string::npos) portHeight *= 96.f / 72.f;
					else if (val.find("cm") != string::npos) portHeight *= 96.f / 2.54f;
					vb.setPortHeight(portHeight);
				}
			}
		}

		// Xử lý thẻ Group <g>
		if (name.find("<g") != string::npos) {
			property = " " + groupStack.top() + " " + property + " ";
			groupStack.push(property);

			SVGGroup* newGroup = new SVGGroup();
			newGroup->setName("g");
			newGroup->setParent(curGroup);
			curGroup->addShape(newGroup);
			curGroup = newGroup;
		}
		// Xử lý thẻ đóng Group </g>
		else if (name.find("</g") != string::npos) {
			if (!groupStack.empty())
				groupStack.pop();
			if (curGroup->getParent() != NULL) // Kiểm tra an toàn
				curGroup = curGroup->getParent();
		}
		// Xử lý các hình vẽ (Rect, Circle, Path...)
		else {
			// Bỏ dấu < ở đầu tên thẻ
			if (!name.empty() && name[0] == '<') name.erase(0, 1);

			// Nếu là thẻ text thì đọc nội dung text
			if (name == "text") {
				string temp = "";
				getline(fin, textContent, '<');
				getline(fin, temp, '>');
			}

			// Tạo hình từ Factory
			Shape* shape = factory.getShape(name);

			// Nếu Factory trả về NULL (vd: gặp thẻ <defs>, <linearGradient>...) -> Bỏ qua luôn
			if (shape) {
				if (!groupStack.empty()) {
					property = " " + groupStack.top() + " " + property + " ";
				}
				processProperty(name, property, textContent, shape);
				curGroup->addShape(shape);
			}
		}
	}
	fin.close();
}

parser::~parser() {}