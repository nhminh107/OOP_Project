#include "Library.h"

SVGPath::SVGPath() : Shape() {
	strokeLineJoin = "miter";
	strokeLineCap = "butt";
	fillRule = "nonzero";
}

SVGPath::~SVGPath() {}

void SVGPath::updateProperty() {
	ofstream ofs("test.txt", ios::out);
	stringstream ss(line_str);
	string property, val, temp;

	while (ss >> property) {
		getline(ss, temp, '"');
		getline(ss, val, '"');
		if (property == "stroke-linejoin")
			this->strokeLineJoin = val;
		else if (property == "stroke-linecap")
			this->strokeLineCap = val;
		else if (property == "d") {
			if (val[0] != 'M' && val[0] != 'm')
				return;
			for (int i = 0; i < val.size(); i++) {
				if (isalpha(val[i]) && val[i] != 'e') {
					if (i + 1 < val.size() && val[i + 1] != ' ')
						val.insert(i + 1, " ");
					if (i - 1 > -1 && isdigit(val[i - 1]))
						val.insert(i, " ");
				}
				if (val[i] == ',')
					val[i] = ' ';
				if (val[i] == '-' && val[i - 1] != ' ' && val[i - 1] != 'e')
					val.insert(i, " ");
			}

			for (int i = 0; i < val.size(); i++) {
				if (val[i] == '.') {
					int j = i + 1;
					for (j; j < val.size(); j++)
						if (val[j] == '.')
							break;
					int t = i + 1;
					for (t; t < j; ++t)
						if (val[t] == ' ')
							break;
					if (t == j) {
						val.insert(j, " ");
						i = j + 1;
					}
				}
			}

			for (int i = 0; i < val.size(); i++) {
				if (isalpha(val[i]) && val[i] != 'e') {
					int j = i + 1;
					while ((!isalpha(val[j]) || val[j] == 'e') && j < val.size())
						j++;
					string pointStr = val.substr(i, j - i);
					pair<char, vector<float>> pr;
					pr.first = pointStr[0];
					pointStr.erase(0, 2);

					stringstream ss(pointStr);

					if (pr.first == 'm') {
						bool first = true;
						string x = "", y = "";
						while (ss >> x >> y) {
							if (first) {
								int n = vct.size();
								if (n > 0) {
									int m = vct[n - 1].second.size();
									if (m > 1) {
										pr.second.push_back(stof(x) + vct[n - 1].second[m - 2]);
										pr.second.push_back(stof(y) + vct[n - 1].second[m - 1]);
									}
								}
								else {
									pr.second.push_back(stof(x));
									pr.second.push_back(stof(y));
								}
								first = false;
							}
							else {
								int n = pr.second.size();
								if (n > 1) {
									pr.second.push_back(stof(x) + pr.second[n - 2]);
									pr.second.push_back(stof(y) + pr.second[n - 1]);
								}
							}
						}
						vct.push_back(pr);
					}

					else if (pr.first == 'l') {
						bool first = true;
						string x = "", y = "";
						while (ss >> x >> y) {
							if (first) {
								int n = vct.size();
								if (n > 0) {
									int m = vct[n - 1].second.size();
									if (m > 1) {
										pr.second.push_back(stof(x) + vct[n - 1].second[m - 2]);
										pr.second.push_back(stof(y) + vct[n - 1].second[m - 1]);
										first = false;
									}
								}
							}
							else {
								int n = pr.second.size();
								if (n > 1) {
									pr.second.push_back(stof(x) + pr.second[n - 2]);
									pr.second.push_back(stof(y) + pr.second[n - 1]);
								}
							}
						}
						vct.push_back(pr);
					}

					else if (pr.first == 'H') {
						string x = "";
						while (ss >> x) {
							pr.second.push_back(stof(x));
							int n = vct.size();
							if (n > 0) {
								int m = vct[n - 1].second.size();
								if (m > 1)
									pr.second.push_back(vct[n - 1].second[m - 1]);
							}
						}
						vct.push_back(pr);
					}

					else if (pr.first == 'h') {
						bool first = true;
						string x = "";
						while (ss >> x) {
							if (first) {
								int n = vct.size();
								if (n > 0) {
									int m = vct[n - 1].second.size();
									if (m > 1) {
										pr.second.push_back(stof(x) + vct[n - 1].second[m - 2]);
										pr.second.push_back(vct[n - 1].second[m - 1]);
										first = false;
									}
								}
							}
							else {
								int n = pr.second.size();
								if (n > 1) {
									pr.second.push_back(stof(x) + pr.second[n - 2]);
									pr.second.push_back(pr.second[n - 1]);
								}
							}
						}
						vct.push_back(pr);
					}

					else if (pr.first == 'V') {
						string y = "";
						while (ss >> y) {
							int n = vct.size();
							if (n > 0) {
								int m = vct[n - 1].second.size();
								if (m > 1)
									pr.second.push_back(vct[n - 1].second[m - 2]);
							}
							pr.second.push_back(stof(y));
						}
						vct.push_back(pr);
					}

					else if (pr.first == 'v') {
						bool first = true;
						string y = "";
						while (ss >> y) {
							if (first) {
								int n = vct.size();
								if (n > 0) {
									int m = vct[n - 1].second.size();
									if (m > 1) {
										pr.second.push_back(vct[n - 1].second[m - 2]);
										pr.second.push_back(stof(y) + vct[n - 1].second[m - 1]);
										first = false;
									}
								}
							}
							else {
								int n = pr.second.size();
								if (n > 1) {
									pr.second.push_back(pr.second[n - 2]);
									pr.second.push_back(stof(y) + pr.second[n - 1]);
								}
							}
						}
						vct.push_back(pr);
					}

					else if (pr.first == 'T' || pr.first == 't') {
						bool first = true;
						string x = "", y = "";
						while (ss >> x >> y) {
							if (first) {
								int n = vct.size();
								if (n > 0) {
									int m = vct[n - 1].second.size();
									if (vct[n - 1].first == 'Q' || vct[n - 1].first == 'q' || vct[n - 1].first == 'T' || vct[n - 1].first == 't') {
										if (m > 3) {
											float oldx2 = vct[n - 1].second[m - 4];
											float oldy2 = vct[n - 1].second[m - 3];
											float curx = vct[n - 1].second[m - 2];
											float cury = vct[n - 1].second[m - 1];
											pr.second.push_back(2.f * curx - oldx2);
											pr.second.push_back(2.f * cury - oldy2);
										}
									}
									else {
										if (m > 1) {
											pr.second.push_back(vct[n - 1].second[m - 2]);
											pr.second.push_back(vct[n - 1].second[m - 1]);
										}
									}

									if (pr.first == 'T') {
										pr.second.push_back(stof(x));
										pr.second.push_back(stof(y));
									}
									else if (m > 1) {
										pr.second.push_back(stof(x) + vct[n - 1].second[m - 2]);
										pr.second.push_back(stof(y) + vct[n - 1].second[m - 1]);
									}
									first = false;
								}
							}
							else {
								int n = pr.second.size();
								if (n > 3) {
									float oldx2 = pr.second[n - 4];
									float oldy2 = pr.second[n - 3];
									float curx = pr.second[n - 2];
									float cury = pr.second[n - 1];
									pr.second.push_back(2.f * curx - oldx2);
									pr.second.push_back(2.f * cury - oldy2);
								}

								if (pr.first == 'T') {
									pr.second.push_back(stof(x));
									pr.second.push_back(stof(y));
								}
								else {
									pr.second.push_back(stof(x) + pr.second[n - 2]);
									pr.second.push_back(stof(y) + pr.second[n - 1]);
								}
							}
						}
						vct.push_back(pr);
					}

					else if (pr.first == 'c') {
						bool first = true;
						string x1 = "", y1 = "", x2 = "", y2 = "", x3 = "", y3 = "";
						while (ss >> x1 >> y1 >> x2 >> y2 >> x3 >> y3) {
							if (first) {
								int n = vct.size();
								if (n > 0) {
									int m = vct[n - 1].second.size();
									if (m > 1) {
										pr.second.push_back(stof(x1) + vct[n - 1].second[m - 2]);
										pr.second.push_back(stof(y1) + vct[n - 1].second[m - 1]);
										pr.second.push_back(stof(x2) + vct[n - 1].second[m - 2]);
										pr.second.push_back(stof(y2) + vct[n - 1].second[m - 1]);
										pr.second.push_back(stof(x3) + vct[n - 1].second[m - 2]);
										pr.second.push_back(stof(y3) + vct[n - 1].second[m - 1]);
										first = false;
									}
								}
							}
							else {
								int n = pr.second.size();
								if (n > 1) {
									pr.second.push_back(stof(x1) + pr.second[n - 2]);
									pr.second.push_back(stof(y1) + pr.second[n - 1]);
									pr.second.push_back(stof(x2) + pr.second[n - 2]);
									pr.second.push_back(stof(y2) + pr.second[n - 1]);
									pr.second.push_back(stof(x3) + pr.second[n - 2]);
									pr.second.push_back(stof(y3) + pr.second[n - 1]);
								}
							}
						}
						vct.push_back(pr);
					}

					else if (pr.first == 's' || pr.first == 'q') {
						bool first = true;
						string x1 = "", y1 = "", x2 = "", y2 = "";
						while (ss >> x1 >> y1 >> x2 >> y2) {
							if (first) {
								int n = vct.size();
								if (n > 0) {
									int m = vct[n - 1].second.size();
									if (m > 1) {
										pr.second.push_back(stof(x1) + vct[n - 1].second[m - 2]);
										pr.second.push_back(stof(y1) + vct[n - 1].second[m - 1]);
										pr.second.push_back(stof(x2) + vct[n - 1].second[m - 2]);
										pr.second.push_back(stof(y2) + vct[n - 1].second[m - 1]);
										first = false;
									}
								}
							}
							else {
								int n = pr.second.size();
								if (n > 1) {
									pr.second.push_back(stof(x1) + pr.second[n - 2]);
									pr.second.push_back(stof(y1) + pr.second[n - 1]);
									pr.second.push_back(stof(x2) + pr.second[n - 2]);
									pr.second.push_back(stof(y2) + pr.second[n - 1]);
								}
							}
						}
						vct.push_back(pr);
					}

					else if (pr.first == 'a') {
						bool first = true;
						string rx = "", ry = "", xAR = "0", lAF = "", sF = "", x = "", y = "";
						// xAR <=> x_axis_rotation, lAF <=> large_arc_flag, sF <=> sweep_flag
						while (ss >> rx >> ry >> xAR >> lAF >> sF >> x >> y) {
							if (first) {
								int n = vct.size();
								if (n > 0) {
									int m = vct[n - 1].second.size();
									if (m > 1) {
										pr.second.push_back(stof(rx));
										pr.second.push_back(stof(ry));
										pr.second.push_back(stof(xAR));
										pr.second.push_back(stof(lAF));
										pr.second.push_back(stof(sF));
										pr.second.push_back(stof(x) + vct[n - 1].second[m - 2]);
										pr.second.push_back(stof(y) + vct[n - 1].second[m - 1]);
										first = false;
									}
								}
							}
							else {
								int n = pr.second.size();
								if (n > 1) {
									pr.second.push_back(stof(rx));
									pr.second.push_back(stof(ry));
									pr.second.push_back(stof(xAR));
									pr.second.push_back(stof(lAF));
									pr.second.push_back(stof(sF));
									pr.second.push_back(stof(x) + pr.second[n - 2]);
									pr.second.push_back(stof(y) + pr.second[n - 1]);
								}
							}
						}
						vct.push_back(pr);
					}

					else if (pr.first == 'Z' || pr.first == 'z') {
						int n = vct.size();
						if (n > 0) {
							int m = vct[n - 1].second.size();
							if (m > 1) {
								pr.second.push_back(vct[n - 1].second[m - 2]);
								pr.second.push_back(vct[n - 1].second[m - 1]);
							}
						}
						vct.push_back(pr);
					}

					else { // M, L, C, S, Q, A
						string token;
						while (ss >> token)
							pr.second.push_back(stof(token));
						vct.push_back(pr);
					}
				}
			}
		}
	}
}
RectF SVGPath::getBoundingBox() {
	// 1. Kiểm tra nếu đã có dữ liệu lưu đệm thì trả về luôn để tiết kiệm CPU

	if (this->vct.empty()) {
		return RectF(0, 0, 0, 0);
	}

	// 2. Sử dụng GraphicsPath để dựng lại hình y hệt như lúc vẽ
	FillMode mode = (this->fillRule == "evenodd") ? FillModeAlternate : FillModeWinding;
	Gdiplus::GraphicsPath graphicsPath(mode);
	PointF P0(0, 0);

	// Duyệt qua vector lệnh vẽ đã được Parser chuẩn hóa sang tọa độ tuyệt đối
	for (int i = 0; i < this->vct.size(); i++) {
		char command = this->vct[i].first;
		const vector<float>& params = this->vct[i].second;
		int numPoint = params.size();

		if (command == 'M' || command == 'm') {
			graphicsPath.StartFigure();
			if (numPoint >= 2) {
				P0 = PointF(params[numPoint - 2], params[numPoint - 1]);
				// Nếu có nhiều cặp điểm sau M, GDI+ coi đó là các đường thẳng nối tiếp
				for (int j = 0; j < numPoint; j += 2) {
					if (j + 3 <= numPoint) {
						graphicsPath.AddLine(params[j], params[j + 1], params[j + 2], params[j + 3]);
					}
				}
			}
		}
		else if (command == 'L' || command == 'l' || command == 'H' || command == 'h' || command == 'V' || command == 'v') {
			for (int j = 0; j < numPoint; j += 2) {
				PointF P1(params[j], params[j + 1]);
				graphicsPath.AddLine(P0, P1);
				P0 = P1;
			}
		}
		else if (command == 'C' || command == 'c') {
			for (int j = 0; j < numPoint; j += 6) {
				graphicsPath.AddBezier(P0, PointF(params[j], params[j + 1]),
					PointF(params[j + 2], params[j + 3]),
					PointF(params[j + 4], params[j + 5]));
				P0 = PointF(params[j + 4], params[j + 5]);
			}
		}
		else if (command == 'Q' || command == 'q' || command == 'T' || command == 't') {
			for (int j = 0; j < numPoint; j += 4) {
				// Chuyển Quadratic Bezier sang Cubic Bezier để GDI+ xử lý
				graphicsPath.AddBezier(P0, PointF(params[j], params[j + 1]),
					PointF(params[j], params[j + 1]),
					PointF(params[j + 2], params[j + 3]));
				P0 = PointF(params[j + 2], params[j + 3]);
			}
		}
		else if (command == 'Z' || command == 'z') {
			graphicsPath.CloseFigure();
		}
		// Lưu ý: Với lệnh 'A' (Arc), bạn có thể copy logic tính toán phức tạp của mình 
		// để AddArc vào path này.
	}

	// 3. Sử dụng hàm GetBounds của thư viện GDI+ để tính toán tự động
	// Đây là cách chính xác nhất để lấy khung bao của cả các đường cong
	RectF bounds;
	graphicsPath.GetBounds(&bounds);

	// 4. Xử lý trường hợp hình quá mỏng (Width/Height = 0) gây lỗi cho Gradient
	if (bounds.Width <= 0) bounds.Width = 0.1f;
	if (bounds.Height <= 0) bounds.Height = 0.1f;
	return bounds;
}
void SVGPath::draw(Graphics& graphics) {
	// 1. Lưu trạng thái Graphics
	GraphicsState save = graphics.Save();
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	FillMode fillMode = (this->fillRule == "evenodd") ? FillModeAlternate : FillModeWinding;
	GraphicsPath graphicsPath(fillMode);
	PointF P0(0, 0); // Điểm hiện tại của con trỏ vẽ

	// 2. Duyệt qua vector lệnh vẽ để dựng Path (Giữ nguyên logic dựng hình của bạn)
	for (int i = 0; i < this->vct.size(); i++) {
		char command = this->vct[i].first;
		const vector<float>& params = this->vct[i].second;
		int numPoint = params.size();

		if (command == 'M' || command == 'm') {
			graphicsPath.StartFigure();
			if (numPoint >= 2) {
				P0 = PointF(params[numPoint - 2], params[numPoint - 1]);
				// Nếu có nhiều cặp điểm sau M, GDI+ coi đó là các đường thẳng nối tiếp
				for (int j = 0; j < numPoint - 2; j += 2) {
					graphicsPath.AddLine(params[j], params[j + 1], params[j + 2], params[j + 3]);
				}
			}
		}
		else if (command == 'L' || command == 'l' || command == 'H' || command == 'h' || command == 'V' || command == 'v') {
			for (int j = 0; j < numPoint; j += 2) {
				PointF P1(params[j], params[j + 1]);
				graphicsPath.AddLine(P0, P1);
				P0 = P1;
			}
		}
		else if (command == 'C' || command == 'c') {
			for (int j = 0; j < numPoint; j += 6) {
				graphicsPath.AddBezier(P0, PointF(params[j], params[j + 1]),
					PointF(params[j + 2], params[j + 3]),
					PointF(params[j + 4], params[j + 5]));
				P0 = PointF(params[j + 4], params[j + 5]);
			}
		}
		else if (command == 'Q' || command == 'q' || command == 'T' || command == 't') {
			for (int j = 0; j < numPoint; j += 4) {
				graphicsPath.AddBezier(P0, PointF(params[j], params[j + 1]),
					PointF(params[j], params[j + 1]),
					PointF(params[j + 2], params[j + 3]));
				P0 = PointF(params[j + 2], params[j + 3]);
			}
		}
		else if (command == 'Z' || command == 'z') {
			graphicsPath.CloseFigure();
		}
	}

	// 3. Chuẩn bị Brush
	Brush* fillBrush = nullptr;
	Color paddingColor;

	if (this->hasGradient) {
		string id = this->getFillGradientID();
		if (gradientMap.count(id)) {
			Gradient* grad = gradientMap[id];
			RectF bound = this->getBoundingBox();

			// Tạo Brush từ Gradient
			fillBrush = grad->createBrush(bound, this->getColor().opacity);

			// Lấy màu của Stop cuối cùng để làm màu Padding (Tô tràn)
			vector<stop> stops = grad->getStopList();
			if (!stops.empty()) {
				color c = stops.back().stopColor;
				paddingColor = Color(c.opacity * this->getColor().opacity * 255, c.r, c.g, c.b);
			}

			// --- BƯỚC 4: XỬ LÝ PADDING ĐẶC BIỆT CHO RADIAL GRADIENT ---
			if (grad->getType() == RADIAL) {
				RadialGradient* radial = dynamic_cast<RadialGradient*>(grad);

				// Tính toán hình elip giới hạn của Radial Gradient
				float rx = radial->getRadius() * bound.Width;
				float ry = radial->getRadius() * bound.Height;
				float cx = bound.X + radial->getCenter().x * bound.Width;
				float cy = bound.Y + radial->getCenter().y * bound.Height;

				GraphicsPath pathE;
				pathE.AddEllipse(cx - rx, cy - ry, rx * 2, ry * 2);

				// Sử dụng Region để tô màu nền cho phần nằm ngoài elip nhưng nằm trong Path
				// (Logic này giải quyết triệt để lỗi hở góc ở hình trái tim)
				SolidBrush padBrush(paddingColor);
				Region region(&graphicsPath);
				region.Exclude(&pathE);

				graphics.FillRegion(&padBrush, &region);
			}
		}
	}

	// Nếu không có gradient hoặc lỗi, dùng SolidBrush mặc định
	if (fillBrush == nullptr) {
		fillBrush = new SolidBrush(Color(this->getColor().opacity * 255,
			this->getColor().r, this->getColor().g, this->getColor().b));
	}

	// 5. Chuẩn bị bút vẽ (Pen)
	Pen penPath(Color(this->getStroke().getStrokeColor().opacity * 255,
		this->getStroke().getStrokeColor().r,
		this->getStroke().getStrokeColor().g,
		this->getStroke().getStrokeColor().b),
		this->getStroke().getStrokeWidth());

	// 6. Xử lý Transform (Dịch chuyển, Xoay, Scale)
	vector<pair<string, vector<float>>> transforms = this->getTransVct();
	for (auto trans : transforms) {
		float x = trans.second.empty() ? 0.0f : trans.second[0];
		float y = (trans.second.size() >= 2) ? trans.second[1] : x;

		if (trans.first == "translate") graphics.TranslateTransform(x, y);
		else if (trans.first == "rotate") graphics.RotateTransform(x);
		else if (trans.first == "scale") graphics.ScaleTransform(x, y);
		else if (trans.first == "matrix") {
			Matrix matrix(trans.second[0], trans.second[1], trans.second[2],
				trans.second[3], trans.second[4], trans.second[5]);
			graphics.MultiplyTransform(&matrix);
		}
	}

	// 7. Vẽ và Tô màu
	// Lưu ý: Radial Gradient đã được xử lý Padding ở Bước 4. 
	// Linear Gradient mặc định GDI+ đã hỗ trợ Pad spreadMethod.
	graphics.FillPath(fillBrush, &graphicsPath);
	graphics.DrawPath(&penPath, &graphicsPath);

	// 8. Giải phóng bộ nhớ và khôi phục trạng thái
	delete fillBrush;
	graphics.Restore(save);
}

string SVGPath::getStrokeLineJoin() {
	return this->strokeLineJoin;
}

string SVGPath::getStrokeLineCap() {
	return this->strokeLineCap;
}

string SVGPath::getFillRule() {
	return this->fillRule;
}

void SVGPath::setFillRule(string fillRule) {
	this->fillRule = fillRule;
}

void SVGPath::setStrokeLineJoin(string linejoin) {
	this->strokeLineJoin = linejoin;
}

void SVGPath::setStrokeLineCap(string linecap) {
	this->strokeLineCap = linecap;
}

void SVGPath::setVct(vector<pair<char, vector<float>>> vct) {
	this->vct = vct;
}

vector<pair<char, vector<float>>> SVGPath::getProp() {
	return this->vct;
}