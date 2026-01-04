#include "Library.h"

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

void parser::processColor(string strokecolor, string strokeopa, color& clr) {
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

void parser::processProperty(string name, string property, string textName, Shape*& fig) {
	// 1. Cài đặt thông tin cơ bản
	fig->setName(name);
	fig->setTextName(textName);
	fig->setLine(property);

	// 2. Khởi tạo giá trị mặc định cho các thuộc tính đồ họa
	string strokeWidth = "1", sStroke = "", strokeOpa = "1";
	string fill = "", fillOpa = "1";
	string strTransform = "";
	bool isGradient = false;

	// 3. Duyệt chuỗi thuộc tính
	stringstream ss(property);
	string attribute, value, temp;

	while (ss >> attribute) {
		getline(ss, temp, '"');
		getline(ss, value, '"');

		// --- TRƯỜNG HỢP 1: THUỘC TÍNH 'STYLE' (Cần xử lý chuỗi đặc biệt) ---
		if (attribute == "style") {
			// A. Format lại chuỗi style: chèn dấu " bao quanh giá trị
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
			if (!value.empty() && value.back() != '"') {
				value.push_back('"');
			}

			// B. Thay thế ký tự phân cách bằng khoảng trắng để parse
			for (int i = 0; i < value.size(); i++) {
				if (value[i] == ':' || value[i] == ';')
					value[i] = ' ';
			}

			// C. Parse các thuộc tính con trong style
			stringstream valStream(value);
			string attr, subVal, subTemp;

			while (valStream >> attr) {
				getline(valStream, subTemp, '"');
				getline(valStream, subVal, '"');

				if (attr == "fill") {
					// Logic xử lý Gradient URL trong Style
					if (subVal.find("url") != string::npos) {
						isGradient = true;
						stringstream sss(subVal);
						string idPart;
						getline(sss, temp, '#');
						getline(sss, idPart, ')');
						getline(sss, temp, '"'); // Clean phần thừa

						// Xóa các ký tự rác cuối chuỗi ID
						while (!idPart.empty() && (idPart.back() == ' ' || idPart.back() == '"')) {
							idPart.pop_back();
						}
						fill = idPart;
					}
					else {
						fill = subVal;
					}
				}
				else if (attr == "fill-opacity") {
					fillOpa = subVal;
				}
				else if (attr == "stroke-width") {
					strokeWidth = subVal;
				}
				else if (attr == "stroke-opacity") {
					strokeOpa = subVal;
				}
				else if (attr == "stroke") {
					sStroke = subVal; // Đã sửa từ 'value' thành 'subVal' cho đúng logic
				}
			}
		}

		// --- TRƯỜNG HỢP 2: CÁC THUỘC TÍNH RIÊNG LẺ ---
		else if (attribute == "stroke-width") {
			strokeWidth = value;
		}
		else if (attribute == "fill-opacity") {
			fillOpa = value;
		}
		else if (attribute == "stroke-opacity") {
			strokeOpa = value;
		}
		else if (attribute == "stroke") {
			sStroke = value;
		}
		else if (attribute == "transform") {
			strTransform += (" " + value + " ");
		}
		else if (attribute == "fill") {
			// Logic xử lý Gradient URL trong thuộc tính Fill
			if (value.find("url") != string::npos) {
				isGradient = true;
				stringstream sss(value);
				string idPart;
				getline(sss, temp, '#');
				getline(sss, idPart, ')');
				getline(sss, temp, '"');

				while (!idPart.empty() && (idPart.back() == ' ' || idPart.back() == '"')) {
					idPart.pop_back();
				}
				fill = idPart;
			}
			else {
				fill = value;
			}
		}
	}

	// 4. Áp dụng thuộc tính vào Shape (Gradient hoặc Màu đơn sắc)
	if (isGradient) {
		// Logic tìm ID Gradient: Thử type 1 (Linear), nếu không được thử type 2 (Radial)
		string originalId = fill;
		string tryLinear = originalId + " 1";

		if (idMap.find(tryLinear) != idMap.end()) {
			idMap[tryLinear]->setGradId(1);
			fig->convertGradient(idMap[tryLinear]);
		}
		else {
			string tryRadial = originalId + " 2";
			if (idMap.find(tryRadial) != idMap.end()) {
				idMap[tryRadial]->setGradId(2);
				fig->convertGradient(idMap[tryRadial]);
			}
		}
	}
	else {
		// Xử lý Fill Color
		color clr = { 0, 0, 0, 1 };
		if (fill == "none" || fill == "transparent") {
			processColor(fill, "0", clr);
		}
		else {
			processColor(fill, fillOpa, clr);
		}
		fig->setColor(clr);

		// Xử lý Stroke
		stroke strk;
		strk.setStrokeWidth(stof(strokeWidth));

		color strokeColor = { 0, 0, 0, 1 };
		if (sStroke == "none" || sStroke == "transparent" || sStroke.empty()) {
			processColor(sStroke, "0", strokeColor);
		}
		else {
			processColor(sStroke, strokeOpa, strokeColor);
		}

		strk.setStrokeColor(strokeColor);
		fig->setStroke(strk);
	}

	// 5. Cập nhật Transform
	fig->updateProperty(); // Gọi cập nhật chung
	if (!strTransform.empty()) {
		fig->updateTransformVct(strTransform);
	}
}
void parser::parseItem(SVGGroup* root, string fileName, viewbox& vb) {
	ifstream fin(fileName, ios::in);
	if (!fin.is_open()) {
		cout << "Error Opening SVG File\n";
		return;
	}
	loadColorMap();

	string line_str = "";
	ShapeFactory factory;
	SVGGroup* curGroup = root;

	// Stack quản lý Group
	stack<string> groupStack;
	groupStack.push(" ");

	// Các cờ quản lý trạng thái Defs/Gradient
	bool openDef = false;
	bool openLinear = false;
	bool openRadial = false;
	bool closeRadial = false;

	string idStr = "";
	vector<string> gradVct;

	// Reset ViewBox
	vb.setPortWidth(0);
	vb.setPortHeight(0);

	while (getline(fin, line_str, '>')) {
		line_str += ">";
		string name = "", property = "";
		stringstream stream(line_str);

		stream >> name;
		getline(stream, property, '>');

		// Chuẩn hóa chuỗi property: thay thế ký tự đặc biệt bằng khoảng trắng hoặc dấu nháy kép
		for (char& c : property) {
			if (c == '/' || c == '=') c = ' ';
			if (c == '\'') c = '"';
		}

		// --- XỬ LÝ TAG <svg> ---
		if (name == "<svg") {
			stringstream sss(property);
			string attribute, temp, val;

			while (sss >> attribute) {
				getline(sss, temp, '"');
				getline(sss, val, '"');

				if (attribute == "viewBox") {
					float vx, vy, vw, vh;
					stringstream ssss(val);
					ssss >> vx >> vy >> vw >> vh;
					vb.setViewX(vx);
					vb.setViewY(vy);
					vb.setViewWidth(vw);
					vb.setViewHeight(vh);
				}
				else if (attribute == "preserveAspectRatio") {
					string pForm, pMode;
					stringstream ssss(val);
					ssss >> pForm >> pMode;
					vb.setPreservedForm(pForm);
					vb.setPreservedMode(pMode);
				}
				else if (attribute == "width") {
					float w = stof(val);
					if (val.find("pt") != string::npos) w *= 96.f / 72.f;
					else if (val.find("cm") != string::npos) w *= 96.f / 2.54f;
					vb.setPortWidth(w);
				}
				else if (attribute == "height") {
					float h = stof(val);
					if (val.find("pt") != string::npos) h *= 96.f / 72.f;
					else if (val.find("cm") != string::npos) h *= 96.f / 2.54f;
					vb.setPortHeight(h);
				}
			}
		}

		// --- XỬ LÝ TAG <defs> ---
		if (name == "<defs>") {
			openDef = true;
		}

		if (openDef) {
			// Xác định loại Gradient đang mở
			if (name == "<linearGradient") openLinear = true;
			else if (name == "<radialGradient") openRadial = true;

			// Logic tích lũy dòng lệnh cho Gradient
			if (openLinear) {
				if (property.find("id") != string::npos) {
					stringstream sss(property);
					string temp, remainLine;
					getline(sss, temp, '"');
					getline(sss, idStr, '"'); // Lấy ID
					getline(sss, remainLine);
					gradVct.push_back(remainLine);
				}
				else {
					gradVct.push_back(property);
				}
			}
			else if (openRadial) {
				if (property.find("id") != string::npos) {
					stringstream sss(property);
					string temp, remainLine;
					getline(sss, temp, '"');
					getline(sss, idStr, '"'); // Lấy ID
					getline(sss, remainLine);
					gradVct.push_back(remainLine);

					// Kiểm tra xlink:href ngay dòng đầu
					if (remainLine.find("xlink:href") != string::npos) {
						closeRadial = true;
					}
				}
				else {
					gradVct.push_back(property);
				}
			}

			// Đóng Linear Gradient
			if (name.find("/linearGradient") != string::npos) {
				openLinear = false;
				lineargradient* linear = new lineargradient;

				if (!gradVct.empty()) {
					linear->setStrLine(gradVct[0]);
					linear->updateElement();
				}

				// Duyệt các thẻ <stop>
				for (size_t i = 1; i < gradVct.size() - 1; i++) {
					stop stp;
					stringstream sss(gradVct[i]);
					string temp, attribute, value;
					string stopColorStr = "", stopOpaStr = "1";

					while (sss >> attribute) {
						getline(sss, temp, '"');
						getline(sss, value, '"');

						if (attribute == "stop-color") stopColorStr = value;
						if (attribute == "stop-opacity") stopOpaStr = value;
						if (attribute == "offset") stp.offset = stof(value);
					}

					color clr = { 0, 0, 0, 1 };
					if (stopColorStr == "none" || stopColorStr == "transparent")
						processColor(stopColorStr, "0", clr);
					else
						processColor(stopColorStr, stopOpaStr, clr);

					stp.stopColor = clr;
					linear->addStop(stp);
				}

				// Lưu vào map (Type 1)
				idStr += " 1";
				if (idMap.find(idStr) == idMap.end()) {
					linear->setGradId(1);
					idMap[idStr] = linear;
				}
				idStr = "";
				gradVct.clear();
			}

			// Đóng Radial Gradient
			if (name.find("/radialGradient") != string::npos || closeRadial) {
				openRadial = false;
				radialgradient* radial = new radialgradient;

				if (!gradVct.empty()) {
					radial->setStrLine(gradVct[0]);
					radial->updateElement();

					// Xử lý liên kết xlink:href (kế thừa stops)
					if (closeRadial) {
						stringstream stream(gradVct[0]);
						string temp, val, attr;
						while (stream >> attr) {
							getline(stream, temp, '"');
							getline(stream, val, '"');
							if (attr == "xlink:href") {
								val.erase(0, 1); // Xóa dấu #
								string strLink = val + " 1"; // Tìm bên Linear trước
								if (idMap.find(strLink) != idMap.end()) {
									vector<stop> linkedStops = idMap[strLink]->getStopVct();
									for (const auto& s : linkedStops) {
										radial->addStop(s);
									}
								}
							}
						}
					}
				}

				// Duyệt các thẻ <stop> (nếu có)
				for (size_t i = 1; i < gradVct.size() - 1; i++) {
					stop stp;
					stringstream sss(gradVct[i]);
					string temp, attribute, value;
					string stopColorStr = "", stopOpaStr = "1";

					while (sss >> attribute) {
						getline(sss, temp, '"');
						getline(sss, value, '"');

						if (attribute == "stop-color") stopColorStr = value;
						if (attribute == "stop-opacity") stopOpaStr = value;
						if (attribute == "offset") stp.offset = stof(value);
					}

					color clr = { 0, 0, 0, 1 };
					if (stopColorStr == "none" || stopColorStr == "transparent")
						processColor(stopColorStr, "0", clr);
					else
						processColor(stopColorStr, stopOpaStr, clr);

					stp.stopColor = clr;
					radial->addStop(stp);
				}

				// Lưu vào map (Type 2)
				idStr += " 2";
				if (idMap.find(idStr) == idMap.end()) {
					radial->setGradId(2);
					idMap[idStr] = radial;
				}
				idStr = "";
				gradVct.clear();
				closeRadial = false;
			}
		}

		if (name.find("/def") != string::npos) {
			openDef = false;
		}

		// --- XỬ LÝ NHÓM <g> ---
		if (name.find("<g") != string::npos) {
			// Kế thừa thuộc tính từ nhóm cha
			property = " " + groupStack.top() + " " + property + " ";
			groupStack.push(property);

			SVGGroup* newGroup = new SVGGroup();
			newGroup->setName("g");
			newGroup->setParent(curGroup);
			curGroup->addShape(newGroup);
			curGroup = newGroup;
		}
		else if (name.find("</g") != string::npos) {
			if (!groupStack.empty())
				groupStack.pop();
			curGroup = curGroup->getParent();
		}
		// --- XỬ LÝ CÁC HÌNH VẼ (SHAPES) ---
		else {
			name.erase(0, 1); // Xóa dấu <

			string textContent = "";
			if (name == "text") {
				string temp;
				getline(fin, textContent, '<');
				getline(fin, temp, '>');
			}

			Shape* fig = factory.getShape(name);
			if (fig) {
				// Áp dụng thuộc tính nhóm nếu có
				if (!groupStack.empty()) {
					property = " " + groupStack.top() + " " + property + " ";
				}
				processProperty(name, property, textContent, fig);
				curGroup->addShape(fig);
			}
		}
	}
	fin.close();
}

parser::~parser() {}