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

// Hàm bổ trợ để lấy giá trị giữa cặp dấu ngoặc kép ""
string getVal(string attrName, string property) {
	size_t pos = property.find(attrName + "=");
	if (pos == string::npos) return "";
	size_t start = property.find_first_of("\"'", pos);
	size_t end = property.find_first_of("\"'", start + 1);
	return property.substr(start + 1, end - start - 1);

	//Sai thì phải kiểm tra hàm này, vì đây là hàm cốt lõi để tách chuỗi 
}

float parser::parseUnit(string s) {
	if (s.empty()) return 0.0f;

	// Loại bỏ khoảng trắng thừa nếu có
	size_t first = s.find_first_not_of(" ");
	size_t last = s.find_last_not_of(" ");
	s = s.substr(first, (last - first + 1));

	bool isPercent = (s.back() == '%');
	if (isPercent) {
		s.pop_back(); // Bỏ ký tự % để stof hoạt động
	}

	try {
		float val = stof(s);
		return isPercent ? (val / 100.0f) : val;
	}
	catch (...) {
		return 0.0f; // Trả về 0 nếu chuỗi không phải là số hợp lệ
	}
}

void parser::parseGradient(string name, string property) {
	Gradient* gradient = nullptr;

	if (name.find("linearGradient") != string::npos) {
		LinearGradient* linear = new LinearGradient();
		// Giả sử bạn xử lý tọa độ %, x1="20%" -> 0.2f
		string x1 = getVal("x1", property);
		string y1 = getVal("y1", property);
		string x2 = getVal("x2", property); 
		string y2 = getVal("y2", property); 

		linear->setStart(parseUnit(x1), parseUnit(y1)); 
		linear->setEnd(parseUnit(x2), parseUnit(y2));
		gradient = linear;
	}
	else if (name.find("radialGradient") != string::npos) {
		RadialGradient* radial = new RadialGradient();
		radial->setCenter(parseUnit(getVal("cx", property)), parseUnit(getVal("cy", property)));
		radial->setRadius(parseUnit(getVal("r", property)));
		gradient = radial;
	}

	if (gradient) {
		gradient->setID(getVal("id", property));
		string transStr = getVal("gradientTransform", property);
		if (!transStr.empty()) {
			gradient->updateTransform(transStr); // Gọi hàm vừa tạo
		}

		this->currentProcessingGradient = gradient;
		gradientMap[gradient->getID()] = gradient;
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

	// --- LOGIC MỚI: CHỈ XỬ LÝ MÀU ĐƠN SẮC ---
	// 1. Xử lý màu Fill
	if (fill.find("url(#") != string::npos) {
		// TRƯỜNG HỢP: GRADIENT
		size_t start = fill.find("#") + 1;
		size_t end = fill.find(")");
		string id = fill.substr(start, end - start);

		shape->setGradientID(id);
		shape->setHasGradient(true);

		// set một màu mặc định để an toàn
		color transparent = { 0, 0, 0, 0 };
		shape->setColor(transparent);
	}
	else {
		// TRƯỜNG HỢP: MÀU ĐƠN SẮC (SOLID COLOR)
		shape->setHasGradient(false);
		shape->setGradientID(""); // Reset ID

		color clrFill = { 0, 0, 0, 1 };
		if (fill == "none" || fill == "transparent" || fill == "") {
			processColor("none", "0", clrFill);
		}
		else {
			processColor(fill, fillOpa, clrFill);
		}
		shape->setColor(clrFill);
	}

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

		// Xử lý thẻ <svg> (Header)
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
		if (name == "<linearGradient" || name == "<radialGradient") {
			// 1. Tạo đối tượng và parse các thuộc tính (id, x1, y1, cx, cy, r...)
			parseGradient(name, property);
		}
		else if (name == "<stop") {
			// 2. Nếu đang nằm trong một cụm Gradient, thì thêm stop vào
			if (this->currentProcessingGradient != nullptr) {
				float offset = parseUnit(getVal("offset", property));
				string stopColorStr = getVal("stop-color", property);
				string stopOpacity = getVal("stop-opacity", property);
				if (stopOpacity.empty()) stopOpacity = "1";

				color c;
				processColor(stopColorStr, stopOpacity, c);
				this->currentProcessingGradient->addStop(offset, c);
			}
		}
		else if (name == "</linearGradient>" || name == "</radialGradient>") {
			// 3. Kết thúc cụm Gradient, reset biến tạm
			this->currentProcessingGradient = nullptr;
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