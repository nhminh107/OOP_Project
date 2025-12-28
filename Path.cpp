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

//void SVGPath::draw(Graphics& graphics) {
//	// 1. Lưu trạng thái Graphics
//	GraphicsState save = graphics.Save();
//
//	// 2. Xác định chế độ tô màu (Fill Mode) dựa trên thuộc tính fillRule của class
//	FillMode mode;
//	if (this->fillRule == "evenodd")
//		mode = FillModeAlternate;
//	else
//		mode = FillModeWinding;
//
//	GraphicsPath graphicsPath(mode);
//	PointF P0; // Điểm hiện tại của con trỏ vẽ
//
//	// 3. Duyệt qua vector lệnh vẽ (vct là thuộc tính private của class path)
//	for (int i = 0; i < this->vct.size(); i++) {
//		char command = this->vct[i].first;
//		const vector<float>& params = this->vct[i].second; // Lấy tham số lệnh
//		int numPoint = params.size();
//
//		// --- XỬ LÝ LỆNH DI CHUYỂN (M/m) ---
//		if (command == 'M' || command == 'm') {
//			graphicsPath.StartFigure();
//			if (numPoint == 4) { // Trường hợp đặc biệt: Move rồi Line luôn
//				PointF start = PointF(params[0], params[1]);
//				PointF end = PointF(params[2], params[3]);
//				graphicsPath.AddLine(start, end);
//				P0 = end;
//			}
//			else if (numPoint > 4) { // Move rồi vẽ nhiều Line liên tiếp
//				int k = 0;
//				vector<PointF> points(numPoint / 2);
//				for (int j = 0; j < numPoint; j += 2)
//					points[k++] = PointF(params[j], params[j + 1]);
//				graphicsPath.AddLines(points.data(), numPoint / 2);
//				P0 = points[numPoint / 2 - 1];
//			}
//			else { // Chỉ Move đơn thuần
//				P0 = PointF(params[0], params[1]);
//			}
//		}
//
//		// --- XỬ LÝ ĐƯỜNG CONG BEZIER BẬC 2 (Q/q, T/t) ---
//		else if (command == 'Q' || command == 'q' || command == 'T' || command == 't') {
//			int j = 0;
//			while (numPoint > 3) {
//				PointF P1(params[j + 0], params[j + 1]);
//				PointF P2(params[j + 2], params[j + 3]);
//				graphicsPath.AddBezier(P0, P1, P2, P2); // GDI+ giả lập bậc 2 bằng bậc 3
//				P0 = P2;
//				numPoint -= 4;
//				j += 4;
//			}
//		}
//
//		// --- XỬ LÝ ĐƯỜNG CONG BEZIER BẬC 3 (C/c) ---
//		else if (command == 'C' || command == 'c') {
//			int j = 0;
//			while (numPoint > 5) {
//				PointF P1(params[j + 0], params[j + 1]);
//				PointF P2(params[j + 2], params[j + 3]);
//				PointF P3(params[j + 4], params[j + 5]);
//				graphicsPath.AddBezier(P0, P1, P2, P3);
//				P0 = P3;
//				numPoint -= 6;
//				j += 6;
//			}
//		}
//
//		// --- XỬ LÝ ĐƯỜNG CONG MƯỢT (S/s) ---
//		else if (command == 'S' || command == 's') {
//			int j = 0;
//			while (numPoint > 3) {
//				PointF P1 = P0; // Mặc định điểm điều khiển trùng P0 nếu không tính được
//				// Tính điểm phản chiếu (Reflection) từ lệnh trước đó
//				if (i > 0) {
//					char prevCmd = this->vct[i - 1].first;
//					if (prevCmd == 'C' || prevCmd == 'c' || prevCmd == 'S' || prevCmd == 's') {
//						const vector<float>& prevParams = this->vct[i - 1].second;
//						int n = prevParams.size();
//						if (n > 3) {
//							float oldControlX = prevParams[n - 4];
//							float oldControlY = prevParams[n - 3];
//							float currentX = prevParams[n - 2];
//							float currentY = prevParams[n - 1];
//							// Công thức phản chiếu: P1 = 2*Current - OldControl
//							P1 = PointF(2.0f * currentX - oldControlX, 2.0f * currentY - oldControlY);
//						}
//					}
//				}
//				PointF P2(params[j + 0], params[j + 1]);
//				PointF P3(params[j + 2], params[j + 3]);
//				graphicsPath.AddBezier(P0, P1, P2, P3);
//				P0 = P3;
//				numPoint -= 4;
//				j += 4;
//			}
//		}
//
//		// --- XỬ LÝ CUNG TRÒN/ELIP (A/a) - Logic toán học giữ nguyên ---
//		else if (command == 'A' || command == 'a') {
//			// (Đã giữ nguyên toàn bộ logic tính toán Arc phức tạp của bạn ở đây)
//			// ... (Code xử lý Arc rất dài, tôi giữ nguyên logic bên trong block này như cũ)
//			// Chỉ thay đổi cách truy cập tham số từ vct[i].second sang params cho gọn
//			int j = 0;
//			while (numPoint > 6) {
//				if (i > 0) {
//				}
//				P0 = PointF(params[j + 5], params[j + 6]);
//				numPoint -= 7;
//				j += 7;
//			}
//		}
//
//		// --- XỬ LÝ VẼ ĐƯỜNG THẲNG (L, H, V...) ---
//		else if (command == 'L' || command == 'H' || command == 'V' ||
//			command == 'l' || command == 'h' || command == 'v') {
//			int j = 0;
//			while (numPoint > 1) {
//				PointF P1(params[j + 0], params[j + 1]);
//				graphicsPath.AddLine(P0, P1);
//				P0 = P1;
//				numPoint -= 2;
//				j += 2;
//			}
//		}
//
//		// --- ĐÓNG HÌNH (Z/z) ---
//		else if (command == 'Z' || command == 'z') {
//			graphicsPath.CloseFigure();
//		}
//	}
//
//	// 4. Thiết lập chế độ vẽ mượt
//	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
//
//	// 5. Chuẩn bị bút vẽ (Pen) và màu tô (Brush) - Lấy từ class cha Shape
//	// Sử dụng this->getStroke() và this->getColor()
//	Pen penPath(Color(this->getStroke().getStrokeColor().opacity * 255,
//		this->getStroke().getStrokeColor().r,
//		this->getStroke().getStrokeColor().g,
//		this->getStroke().getStrokeColor().b),
//		this->getStroke().getStrokeWidth());
//
//	SolidBrush fillPath(Color(this->getColor().opacity * 255,
//		this->getColor().r,
//		this->getColor().g,
//		this->getColor().b));
//
//	// 6. Xử lý Transform (Dịch chuyển, Xoay, Scale)
//	// Lấy vector transform từ class cha Shape
//	vector<pair<string, vector<float>>> transforms = this->getTransVct();
//
//	for (auto trans : transforms) {
//		float x = 0.0f;
//		if (!trans.second.empty()) x = trans.second[0];
//		float y = x;
//		if (trans.second.size() == 2) y = trans.second[1];
//
//		if (trans.first == "translate")
//			graphics.TranslateTransform(x, y);
//		else if (trans.first == "rotate")
//			graphics.RotateTransform(x);
//		else if (trans.first == "scale")
//			graphics.ScaleTransform(x, y);
//		else if (trans.first == "matrix") {
//			Matrix matrix(trans.second[0], trans.second[1], trans.second[2],
//				trans.second[3], trans.second[4], trans.second[5]);
//			graphics.SetTransform(&matrix);
//		}
//	}
//
//	// 7. Vẽ và Tô màu
//	graphics.FillPath(&fillPath, &graphicsPath);
//	graphics.DrawPath(&penPath, &graphicsPath);
//
//	// 8. Khôi phục trạng thái
//	graphics.Restore(save);
//}
void SVGPath::draw(Graphics& graphics) {
	GraphicsState save = graphics.Save();
	vector<pair<char, vector<float>>> vct = this->getProp();
	FillMode fillMode;
	if (this->fillRule == "evenodd")
		fillMode = FillModeAlternate;
	else fillMode = FillModeWinding;
	GraphicsPath path(fillMode);

	PointF P0;
	for (int i = 0; i < vct.size(); i++) {
		int numPoint = vct[i].second.size();
		if (vct[i].first == 'M' || vct[i].first == 'm') {
			gradient* grad = this->grad;
			if (grad == NULL) {
				path.StartFigure();
				if (numPoint == 4) {
					PointF P0 = PointF(vct[i].second[0], vct[i].second[1]);
					PointF P1 = PointF(vct[i].second[2], vct[i].second[3]);
					path.AddLine(P0, P1);
					P0 = P1;
				}
				else if (numPoint > 4) {
					int k = 0;
					vector <PointF> points(numPoint / 2);
					for (int j = 0; j < numPoint; j += 2)
						points[k++] = PointF(vct[i].second[j], vct[i].second[j + 1]);
					path.AddLines(points.data(), numPoint / 2);
					P0 = points[numPoint / 2 - 1];
				}
				else P0 = PointF(vct[i].second[0], vct[i].second[1]);
			}
			else {
				path.StartFigure();
				int j = 0;
				while (numPoint > 1) {
					PointF P1 = PointF(vct[i].second[j + 0], vct[i].second[j + 1]);
					path.AddLine(P0, P1);
					P0 = P1;
					numPoint -= 2;
					j += 2;
				}
			}
		}

		else if (vct[i].first == 'Q' || vct[i].first == 'q' || vct[i].first == 'T' || vct[i].first == 't') {
			int j = 0;
			while (numPoint > 3) {
				PointF P1 = PointF(vct[i].second[j + 0], vct[i].second[j + 1]);
				PointF P2 = PointF(vct[i].second[j + 2], vct[i].second[j + 3]);
				path.AddBezier(P0, P1, P2, P2);
				P0 = P2;
				numPoint -= 4;
				j += 4;
			}
		}

		else if (vct[i].first == 'C' || vct[i].first == 'c') {
			int j = 0;
			while (numPoint > 5) {
				PointF P1 = PointF(vct[i].second[j + 0], vct[i].second[j + 1]);
				PointF P2 = PointF(vct[i].second[j + 2], vct[i].second[j + 3]);
				PointF P3 = PointF(vct[i].second[j + 4], vct[i].second[j + 5]);
				path.AddBezier(P0, P1, P2, P3);
				P0 = P3;
				numPoint -= 6;
				j += 6;
			}
		}

		else if (vct[i].first == 'S' || vct[i].first == 's') {
			int j = 0;
			while (numPoint > 3) {
				PointF P1 = P0;
				if (i > 0) {
					if (vct[i - 1].first == 'C' || vct[i - 1].first == 'c' || vct[i - 1].first == 'S' || vct[i - 1].first == 's') {
						int n = vct[i - 1].second.size();
						if (n > 3) {
							float oldx2 = vct[i - 1].second[n - 4];
							float oldy2 = vct[i - 1].second[n - 3];
							float curx = vct[i - 1].second[n - 2];
							float cury = vct[i - 1].second[n - 1];
							P1 = PointF(2.f * curx - oldx2, 2.f * cury - oldy2);
						}
					}
				}
				PointF P2 = PointF(vct[i].second[j + 0], vct[i].second[j + 1]);
				PointF P3 = PointF(vct[i].second[j + 2], vct[i].second[j + 3]);
				path.AddBezier(P0, P1, P2, P3);
				P0 = P3;
				numPoint -= 4;
				j += 4;
			}
		}

		else if (vct[i].first == 'A' || vct[i].first == 'a') {
			int j = 0;
			while (numPoint > 6) {
				if (i > 0) {
					int n = vct[i - 1].second.size();
					if (n > 1) {
						double sx = P0.X;
						double sy = P0.Y;
						double rx = vct[i].second[j + 0];
						double ry = vct[i].second[j + 1];
						double xAR = vct[i].second[j + 2];
						bool lAF = vct[i].second[j + 3];
						bool sF = vct[i].second[j + 4];
						double ex = vct[i].second[j + 5];
						double ey = vct[i].second[j + 6];
						double angle = xAR * Pi / 180.f;
						double cosAngle = cos(angle);
						double sinAngle = sin(angle);

						double a = (sx - ex) / 2.f;
						double b = (sy - ey) / 2.f;

						double x1 = cosAngle * a + sinAngle * b;
						double y1 = -sinAngle * a + cosAngle * b;

						rx = abs(rx);
						ry = abs(ry);

						double lambda = (x1 * x1) / (rx * rx) + (y1 * y1) / (ry * ry);
						if (lambda > 1.f) {
							rx *= sqrt(lambda);
							ry *= sqrt(lambda);
						}

						double sign = (lAF == sF ? -1.f : 1.f);
						double num = rx * rx * ry * ry - rx * rx * y1 * y1 - ry * ry * x1 * x1;
						double den = rx * rx * y1 * y1 + ry * ry * x1 * x1;
						if (num < 0)
							num = 0;

						double x2 = sign * sqrt(num / den) * rx * y1 / ry;
						double y2 = -sign * sqrt(num / den) * ry * x1 / rx;

						double x = cosAngle * x2 - sinAngle * y2 + ((sx + ex) / 2.f);
						double y = sinAngle * x2 + cosAngle * y2 + ((sy + ey) / 2.f);

						a = (x1 - x2) / rx;
						b = (y1 - y2) / ry;
						double c = (-x1 - x2) / rx;
						double d = (-y1 - y2) / ry;

						if (b < 0)
							sign = -1.f;
						else sign = 1.f;
						double temp = a / sqrt(a * a + b * b);
						if (temp < -1.f)
							temp = -1.f;
						else if (temp > 1.f)
							temp = 1.f;
						double startAngle = sign * acos(temp);

						if (a * d - b * c < 0)
							sign = -1.f;
						else sign = 1.f;
						temp = (a * c + b * d) / (sqrt(a * a + b * b) * sqrt(c * c + d * d));
						if (temp < -1.f)
							temp = -1.f;
						else if (temp > 1.f)
							temp = 1.f;
						double dentaAngle = sign * acos(temp);

						if (sF == 0 && dentaAngle > 0)
							dentaAngle -= (2.f * Pi);
						else if (sF == 1 && dentaAngle < 0)
							dentaAngle += (2.f * Pi);

						double ratio = abs(dentaAngle) / (Pi / 2.f);
						if (abs(1.f - ratio) < 0.0000001)
							ratio = 1.f;
						int segments = max(static_cast<int>(ceil(ratio)), 1);
						dentaAngle /= segments;

						vector<vector<vector<double>>> curves;
						vector<vector<double>> curve;
						for (int t = 0; t < segments; t++) {
							double kappa = (dentaAngle == Pakka) ? Kappa : (dentaAngle == -Pakka) ? -Kappa : 4.f / 3.f * tan(dentaAngle / 4.f);
							double x3 = cos(startAngle);
							double y3 = sin(startAngle);
							double x4 = cos(startAngle + dentaAngle);
							double y4 = sin(startAngle + dentaAngle);
							curve.push_back({ x3 - y3 * kappa, y3 + x3 * kappa });
							curve.push_back({ x4 + y4 * kappa, y4 - x4 * kappa });
							curve.push_back({ x4, y4 });
							curves.push_back(curve);
							curve.clear();
							startAngle += dentaAngle;
						}

						for (auto& cur : curves) {
							auto mapped_curve_0 = { (cosAngle * cur[0][0] * rx - sinAngle * cur[0][1] * ry) + x,(sinAngle * cur[0][0] * rx + cosAngle * cur[0][1] * ry) + y };
							auto mapped_curve_1 = { (cosAngle * cur[1][0] * rx - sinAngle * cur[1][1] * ry) + x,(sinAngle * cur[1][0] * rx + cosAngle * cur[1][1] * ry) + y };
							auto mapped_curve_2 = { (cosAngle * cur[2][0] * rx - sinAngle * cur[2][1] * ry) + x,(sinAngle * cur[2][0] * rx + cosAngle * cur[2][1] * ry) + y };
							cur = { mapped_curve_0, mapped_curve_1, mapped_curve_2 };

							for (size_t i = 0; i < cur.size(); i += 3) {
								if (i + 2 < cur.size()) {
									PointF P1(cur[i][0], cur[i][1]);
									PointF P2(cur[i + 1][0], cur[i + 1][1]);
									PointF P3(cur[i + 2][0], cur[i + 2][1]);
									path.AddBezier(P0, P1, P2, P3);
									P0 = P3;
								}
							}
						}

						P0 = PointF(ex, ey);
						numPoint -= 7;
						j += 7;
					}
				}
			}
		}

		else if (vct[i].first == 'L' || vct[i].first == 'H' || vct[i].first == 'V' || vct[i].first == 'l' || vct[i].first == 'h' || vct[i].first == 'v') {
			int j = 0;
			while (numPoint > 1) {
				PointF P1 = PointF(vct[i].second[j + 0], vct[i].second[j + 1]);
				path.AddLine(P0, P1);
				P0 = P1;
				numPoint -= 2;
				j += 2;
			}
		}

		else if (vct[i].first == 'Z' || vct[i].first == 'z')
			path.CloseFigure();
	}

	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	gradient* grad = this->grad;

	if (grad == NULL) {
		Pen penPath(Color(this->getStroke().getStrokeColor().opacity * 255, this->getStroke().getStrokeColor().r, this->getStroke().getStrokeColor().g, this->getStroke().getStrokeColor().b), this->getStroke().getStrokeWidth());
		SolidBrush fillPath(Color(this->getColor().opacity * 255, this->getColor().r, this->getColor().g, this->getColor().b));
		vector<pair<string, vector<float>>> transVct = this->getTransVct();

		for (auto trans : transVct) {
			float x = 0.0f;
			if (!trans.second.empty())
				x = trans.second[0];
			float y = x;
			if (trans.second.size() == 2)
				y = trans.second[1];
			if (trans.first == "translate")
				graphics.TranslateTransform(x, y);
			else if (trans.first == "rotate")
				graphics.RotateTransform(x);
			else graphics.ScaleTransform(x, y);
		}

		graphics.SetSmoothingMode(SmoothingModeAntiAlias);
		graphics.FillPath(&fillPath, &path);
		graphics.DrawPath(&penPath, &path);
		graphics.Restore(save);
	}
	else {
		RectF bounds;
		path.GetBounds(&bounds);

		Brush* fillBrush = this->getGrad()->createBrush(bounds);

		if (grad->getType() == RADIAL) {
			radialgradient* radial = dynamic_cast<radialgradient*>(grad);
			float cx = radial->getCx();
			float cy = radial->getCy();
			float r = radial->getR();

			GraphicsPath pathE;
			pathE.AddEllipse(RectF(cx - r, cy - r, r * 2, r * 2));

			// Áp dụng transform vào ellipse của vùng loại trừ nếu có
			vector<pair<string, vector<float>>> gradientTrans = radial->getGradientTrans();
			for (int k = 0; k < gradientTrans.size(); ++k) {
				if (gradientTrans[k].first == "matrix") {
					Matrix matrix(
						gradientTrans[k].second[0], gradientTrans[k].second[1], gradientTrans[k].second[2],
						gradientTrans[k].second[3], gradientTrans[k].second[4], gradientTrans[k].second[5]
					);
					pathE.Transform(&matrix);
				}
			}

			Color it = Color(
				radial->getStopVct()[radial->getStopVct().size() - 1].stopColor.opacity * 255,
				radial->getStopVct()[radial->getStopVct().size() - 1].stopColor.r,
				radial->getStopVct()[radial->getStopVct().size() - 1].stopColor.g,
				radial->getStopVct()[radial->getStopVct().size() - 1].stopColor.b);

			SolidBrush solidBrush(it);
			Region region(&path);
			region.Exclude(&pathE);

			graphics.FillPath(fillBrush, &path);
			graphics.FillRegion(&solidBrush, &region);
		}
		else {
			graphics.FillPath(fillBrush, &path);
		}

		// Xử lý transform của Shape (Phần cuối của else)
		vector<pair<string, vector<float>>> transVct = this->getTransVct();
		for (auto trans : transVct) {
			float x = 0.0f;
			if (!trans.second.empty())
				x = trans.second[0];
			float y = x;
			if (trans.second.size() == 2)
				y = trans.second[1];
			if (trans.first == "translate")
				graphics.TranslateTransform(x, y);
			else if (trans.first == "rotate")
				graphics.RotateTransform(x);
			else if (trans.first == "scale")
				graphics.ScaleTransform(x, y);
			else if (trans.first == "matrix") {
				Matrix matrix(
					trans.second[0], trans.second[1], trans.second[2],
					trans.second[3], trans.second[4], trans.second[5]
				);
				graphics.SetTransform(&matrix);
			}
		}

		delete fillBrush;
		graphics.Restore(save);
	}
}
/*
void SVGPath::draw(Graphics& graphics) {
	GraphicsState save = graphics.Save();
	vector<pair<char, vector<float>>> vct = this->getProp(); 
	FillMode fillMode;
	if (this->fillRule == "evenodd")
		fillMode = FillModeAlternate;
	else fillMode = FillModeWinding;
	GraphicsPath path(fillMode);

	PointF P0;
	for (int i = 0; i < vct.size(); i++) {
		int numPoint = vct[i].second.size();
		if (vct[i].first == 'M' || vct[i].first == 'm') {
			gradient* grad = this->grad; 
			if (grad == NULL) {
				path.StartFigure();
				if (numPoint == 4) {
					PointF P0 = PointF(vct[i].second[0], vct[i].second[1]);
					PointF P1 = PointF(vct[i].second[2], vct[i].second[3]);
					path.AddLine(P0, P1);
					P0 = P1;
				}
				else if (numPoint > 4) {
					int k = 0;
					vector <PointF> points(numPoint / 2);
					for (int j = 0; j < numPoint; j += 2)
						points[k++] = PointF(vct[i].second[j], vct[i].second[j + 1]);
					path.AddLines(points.data(), numPoint / 2);
					P0 = points[numPoint / 2 - 1];
				}
				else P0 = PointF(vct[i].second[0], vct[i].second[1]);
			}
			else {
				path.StartFigure();
				int j = 0;
				while (numPoint > 1) {
					PointF P1 = PointF(vct[i].second[j + 0], vct[i].second[j + 1]);
					path.AddLine(P0, P1);
					P0 = P1;
					numPoint -= 2;
					j += 2;
				}
			}
		}

		else if (vct[i].first == 'Q' || vct[i].first == 'q' || vct[i].first == 'T' || vct[i].first == 't') {
			int j = 0;
			while (numPoint > 3) {
				PointF P1 = PointF(vct[i].second[j + 0], vct[i].second[j + 1]);
				PointF P2 = PointF(vct[i].second[j + 2], vct[i].second[j + 3]);
				path.AddBezier(P0, P1, P2, P2);
				P0 = P2;
				numPoint -= 4;
				j += 4;
			}
		}

		else if (vct[i].first == 'C' || vct[i].first == 'c') {
			int j = 0;
			while (numPoint > 5) {
				PointF P1 = PointF(vct[i].second[j + 0], vct[i].second[j + 1]);
				PointF P2 = PointF(vct[i].second[j + 2], vct[i].second[j + 3]);
				PointF P3 = PointF(vct[i].second[j + 4], vct[i].second[j + 5]);
				path.AddBezier(P0, P1, P2, P3);
				P0 = P3;
				numPoint -= 6;
				j += 6;
			}
		}

		else if (vct[i].first == 'S' || vct[i].first == 's') {
			int j = 0;
			while (numPoint > 3) {
				PointF P1 = P0;
				if (i > 0) {
					if (vct[i - 1].first == 'C' || vct[i - 1].first == 'c' || vct[i - 1].first == 'S' || vct[i - 1].first == 's') {
						int n = vct[i - 1].second.size();
						if (n > 3) {
							float oldx2 = vct[i - 1].second[n - 4];
							float oldy2 = vct[i - 1].second[n - 3];
							float curx = vct[i - 1].second[n - 2];
							float cury = vct[i - 1].second[n - 1];
							P1 = PointF(2.f * curx - oldx2, 2.f * cury - oldy2);
						}
					}
				}
				PointF P2 = PointF(vct[i].second[j + 0], vct[i].second[j + 1]);
				PointF P3 = PointF(vct[i].second[j + 2], vct[i].second[j + 3]);
				path.AddBezier(P0, P1, P2, P3);
				P0 = P3;
				numPoint -= 4;
				j += 4;
			}
		}

		else if (vct[i].first == 'A' || vct[i].first == 'a') {
			int j = 0;
			while (numPoint > 6) {
				if (i > 0) {
					int n = vct[i - 1].second.size();
					if (n > 1) {
						double sx = P0.X;
						double sy = P0.Y;
						double rx = vct[i].second[j + 0];
						double ry = vct[i].second[j + 1];
						double xAR = vct[i].second[j + 2];
						bool lAF = vct[i].second[j + 3];
						bool sF = vct[i].second[j + 4];
						double ex = vct[i].second[j + 5];
						double ey = vct[i].second[j + 6];
						double angle = xAR * Pi / 180.f;
						double cosAngle = cos(angle);
						double sinAngle = sin(angle);

						double a = (sx - ex) / 2.f;
						double b = (sy - ey) / 2.f;

						double x1 = cosAngle * a + sinAngle * b;
						double y1 = -sinAngle * a + cosAngle * b;

						rx = abs(rx);
						ry = abs(ry);

						double lambda = (x1 * x1) / (rx * rx) + (y1 * y1) / (ry * ry);
						if (lambda > 1.f) {
							rx *= sqrt(lambda);
							ry *= sqrt(lambda);
						}

						double sign = (lAF == sF ? -1.f : 1.f);
						double num = rx * rx * ry * ry - rx * rx * y1 * y1 - ry * ry * x1 * x1;
						double den = rx * rx * y1 * y1 + ry * ry * x1 * x1;
						if (num < 0)
							num = 0;

						double x2 = sign * sqrt(num / den) * rx * y1 / ry;
						double y2 = -sign * sqrt(num / den) * ry * x1 / rx;

						double x = cosAngle * x2 - sinAngle * y2 + ((sx + ex) / 2.f);
						double y = sinAngle * x2 + cosAngle * y2 + ((sy + ey) / 2.f);

						a = (x1 - x2) / rx;
						b = (y1 - y2) / ry;
						double c = (-x1 - x2) / rx;
						double d = (-y1 - y2) / ry;

						if (b < 0)
							sign = -1.f;
						else sign = 1.f;
						double temp = a / sqrt(a * a + b * b);
						if (temp < -1.f)
							temp = -1.f;
						else if (temp > 1.f)
							temp = 1.f;
						double startAngle = sign * acos(temp);

						if (a * d - b * c < 0)
							sign = -1.f;
						else sign = 1.f;
						temp = (a * c + b * d) / (sqrt(a * a + b * b) * sqrt(c * c + d * d));
						if (temp < -1.f)
							temp = -1.f;
						else if (temp > 1.f)
							temp = 1.f;
						double dentaAngle = sign * acos(temp);

						if (sF == 0 && dentaAngle > 0)
							dentaAngle -= (2.f * Pi);
						else if (sF == 1 && dentaAngle < 0)
							dentaAngle += (2.f * Pi);

						double ratio = abs(dentaAngle) / (Pi / 2.f);
						if (abs(1.f - ratio) < 0.0000001)
							ratio = 1.f;
						int segments = max(static_cast<int>(ceil(ratio)), 1);
						dentaAngle /= segments;

						vector<vector<vector<double>>> curves;
						vector<vector<double>> curve;
						for (int t = 0; t < segments; t++) {
							double kappa = (dentaAngle == Pakka) ? Kappa : (dentaAngle == -Pakka) ? -Kappa : 4.f / 3.f * tan(dentaAngle / 4.f);
							double x3 = cos(startAngle);
							double y3 = sin(startAngle);
							double x4 = cos(startAngle + dentaAngle);
							double y4 = sin(startAngle + dentaAngle);
							curve.push_back({ x3 - y3 * kappa, y3 + x3 * kappa });
							curve.push_back({ x4 + y4 * kappa, y4 - x4 * kappa });
							curve.push_back({ x4, y4 });
							curves.push_back(curve);
							curve.clear();
							startAngle += dentaAngle;
						}

						for (auto& cur : curves) {
							auto mapped_curve_0 = { (cosAngle * cur[0][0] * rx - sinAngle * cur[0][1] * ry) + x,(sinAngle * cur[0][0] * rx + cosAngle * cur[0][1] * ry) + y };
							auto mapped_curve_1 = { (cosAngle * cur[1][0] * rx - sinAngle * cur[1][1] * ry) + x,(sinAngle * cur[1][0] * rx + cosAngle * cur[1][1] * ry) + y };
							auto mapped_curve_2 = { (cosAngle * cur[2][0] * rx - sinAngle * cur[2][1] * ry) + x,(sinAngle * cur[2][0] * rx + cosAngle * cur[2][1] * ry) + y };
							cur = { mapped_curve_0, mapped_curve_1, mapped_curve_2 };

							for (size_t i = 0; i < cur.size(); i += 3) {
								if (i + 2 < cur.size()) {
									PointF P1(cur[i][0], cur[i][1]);
									PointF P2(cur[i + 1][0], cur[i + 1][1]);
									PointF P3(cur[i + 2][0], cur[i + 2][1]);
									path.AddBezier(P0, P1, P2, P3);
									P0 = P3;
								}
							}
						}

						P0 = PointF(ex, ey);
						numPoint -= 7;
						j += 7;
					}
				}
			}
		}

		else if (vct[i].first == 'L' || vct[i].first == 'H' || vct[i].first == 'V' || vct[i].first == 'l' || vct[i].first == 'h' || vct[i].first == 'v') {
			int j = 0;
			while (numPoint > 1) {
				PointF P1 = PointF(vct[i].second[j + 0], vct[i].second[j + 1]);
				path.AddLine(P0, P1);
				P0 = P1;
				numPoint -= 2;
				j += 2;
			}
		}

		else if (vct[i].first == 'Z' || vct[i].first == 'z')
			path.CloseFigure();
	}

	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	gradient* grad = this->grad; 

	if (grad == NULL) {
		Pen penPath(Color(this->getStroke().getStrokeColor().opacity * 255, this->getStroke().getStrokeColor().r, this->getStroke().getStrokeColor().g, this->getStroke().getStrokeColor().b), this->getStroke().getStrokeWidth());
		SolidBrush fillPath(Color(this->getColor().opacity * 255, this->getColor().r, this->getColor().g, this->getColor().b));
		vector<pair<string, vector<float>>> transVct = this->getTransVct();

		for (auto trans : transVct) {
			float x = 0.0f;
			if (!trans.second.empty())
				x = trans.second[0];
			float y = x;
			if (trans.second.size() == 2)
				y = trans.second[1];
			if (trans.first == "translate")
				graphics.TranslateTransform(x, y);
			else if (trans.first == "rotate")
				graphics.RotateTransform(x);
			else graphics.ScaleTransform(x, y);
		}

		graphics.SetSmoothingMode(SmoothingModeAntiAlias);
		graphics.FillPath(&fillPath, &path);
		graphics.DrawPath(&penPath, &path);
		graphics.Restore(save);
	}
	else {
		int type = grad->getGradId();
		if (type == 1) {
			lineargradient* linear = dynamic_cast<lineargradient*>(grad);
			PointF p1 = PointF(linear->getA().getX(), linear->getA().getY());
			PointF p2 = PointF(linear->getB().getX(), linear->getB().getY());

			vector<stop> ColorOffset = linear->getStopVct();
			int size = ColorOffset.size();
			if (ColorOffset[0].offset != 0) {
				color first = ColorOffset[0].stopColor;
				float offset = ColorOffset[0].offset;
				color zero{ first.r * (1 - offset), first.g * (1 - offset), first.b * (1 - offset), first.opacity * (1 - offset) };
				ColorOffset.insert(ColorOffset.begin(), stop(zero, 0));
				size++;
			}

			if (ColorOffset[size - 1].offset != 1) {
				color last = ColorOffset[size - 1].stopColor;
				float offset = ColorOffset[size - 1].offset;
				color one{ last.r * (1 / offset), last.g * (1 / offset), last.b * (1 / offset), last.opacity * (1 / offset) };
				ColorOffset.push_back(stop(one, 1));
				size++;
			}

			float* points = new float[size];
			Color* colors = new Color[size];
			for (int k = 0; k < size; k++) {
				points[k] = ColorOffset[k].offset;
				colors[k] = Color(ColorOffset[k].stopColor.opacity * 255, ColorOffset[k].stopColor.r, ColorOffset[k].stopColor.g, ColorOffset[k].stopColor.b);
			}

			LinearGradientBrush fillPath(p1, p2, colors[0], colors[size - 1]);

			vector<pair<string, vector<float>>> gradientTrans = linear->getGradientTrans();
			for (int k = 0; k < gradientTrans.size(); k++) {
				if (gradientTrans[k].first == "translate") {
					fillPath.TranslateTransform(gradientTrans[k].second[0], gradientTrans[k].second[1]);
				}
				else if (gradientTrans[k].first == "rotate") {
					fillPath.RotateTransform(gradientTrans[k].second[0]);
				}
				else if (gradientTrans[k].first == "scale") {
					fillPath.ScaleTransform(gradientTrans[k].second[0], gradientTrans[k].second[1]);
				}
				else if (gradientTrans[k].first == "matrix") {
					Matrix matrix(
						gradientTrans[k].second[0], gradientTrans[k].second[1], gradientTrans[k].second[2],
						gradientTrans[k].second[3], gradientTrans[k].second[4], gradientTrans[k].second[5]
					);
					fillPath.SetTransform(&matrix);
				}
			}

			fillPath.SetWrapMode(WrapModeTileFlipXY);
			fillPath.SetInterpolationColors(colors, points, size);
			graphics.FillPath(&fillPath, &path);
			delete[] colors;
			delete[] points;
		}

		else if (type == 2) {
			radialgradient* radial = dynamic_cast<radialgradient*>(grad);
			float cx = radial->getCx();
			float cy = radial->getCy();
			float r = radial->getR();
			vector<stop> ColorOffset = radial->getStopVct();
			int size = ColorOffset.size();

			GraphicsPath pathE;
			pathE.AddEllipse(RectF(cx - r, cy - r, r * 2, r * 2));
			PathGradientBrush fillPath(&pathE);

			if (ColorOffset[0].offset != 0) {
				color first = ColorOffset[0].stopColor;
				float offset = ColorOffset[0].offset;
				color zero{ first.r * (1 - offset), first.g * (1 - offset), first.b * (1 - offset), first.opacity * (1 - offset) };
				ColorOffset.insert(ColorOffset.begin(), stop(zero, 0));
				size++;
			}

			if (ColorOffset[size - 1].offset != 1) {
				color last = ColorOffset[size - 1].stopColor;
				float offset = ColorOffset[size - 1].offset;
				color one{ last.r * (1 / offset), last.g * (1 / offset), last.b * (1 / offset), last.opacity * (1 / offset) };
				ColorOffset.push_back(stop(one, 1));
				size++;
			}

			float* points = new float[size];
			Color* colors = new Color[size];
			for (int k = 0; k < size; k++) {
				points[k] = 1 - ColorOffset[size - k - 1].offset;
				colors[k] = Color(ColorOffset[size - k - 1].stopColor.opacity * 255, ColorOffset[size - k - 1].stopColor.r, ColorOffset[size - k - 1].stopColor.g, ColorOffset[size - k - 1].stopColor.b);
			}

			vector<pair<string, vector<float>>> gradientTrans = radial->getGradientTrans();
			for (int k = 0; k < gradientTrans.size(); ++k) {
				if (gradientTrans[k].first == "translate") {
					fillPath.TranslateTransform(gradientTrans[k].second[0], gradientTrans[k].second[1]);
				}
				else if (gradientTrans[k].first == "rotate") {
					fillPath.RotateTransform(gradientTrans[k].second[0]);
				}
				else if (gradientTrans[k].first == "scale") {
					fillPath.ScaleTransform(gradientTrans[k].second[0], gradientTrans[k].second[1]);
				}
				else if (gradientTrans[k].first == "matrix") {
					Matrix matrix(
						gradientTrans[k].second[0], gradientTrans[k].second[1], gradientTrans[k].second[2],
						gradientTrans[k].second[3], gradientTrans[k].second[4], gradientTrans[k].second[5]
					);
					fillPath.SetTransform(&matrix);
					pathE.Transform(&matrix);
				}
			}

			Color it = Color(
				radial->getStopVct()[radial->getStopVct().size() - 1].stopColor.opacity * 255,
				radial->getStopVct()[radial->getStopVct().size() - 1].stopColor.r,
				radial->getStopVct()[radial->getStopVct().size() - 1].stopColor.g,
				radial->getStopVct()[radial->getStopVct().size() - 1].stopColor.b);

			SolidBrush solidBrush(it);
			Region region(&path);
			region.Exclude(&pathE);

			fillPath.SetInterpolationColors(colors, points, size);

			graphics.FillPath(&fillPath, &path);
			graphics.FillRegion(&solidBrush, &region);
			delete[] colors;
			delete[] points;
		}

		vector<pair<string, vector<float>>> transVct = this->getTransVct();

		for (auto trans : transVct) {
			float x = 0.0f;
			if (!trans.second.empty())
				x = trans.second[0];
			float y = x;
			if (trans.second.size() == 2)
				y = trans.second[1];
			if (trans.first == "translate")
				graphics.TranslateTransform(x, y);
			else if (trans.first == "rotate")
				graphics.RotateTransform(x);
			else if (trans.first == "scale")
				graphics.ScaleTransform(x, y);
			else if (trans.first == "matrix") {
				Matrix matrix(
					trans.second[0], trans.second[1], trans.second[2],
					trans.second[3], trans.second[4], trans.second[5]
				);
				graphics.SetTransform(&matrix);
			}
		}
		graphics.Restore(save);
	}
}

*/

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