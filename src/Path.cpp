#include "Library.h"

SVGPath::SVGPath() : Shape() {
	strokeLineJoin = "miter";
	strokeLineCap = "butt";
	fillRule = "nonzero";
}

SVGPath::~SVGPath() {}

void SVGPath::updateProperty() {
    stringstream ss(line_str);
    string property, val, temp;

    while (ss >> property) {
        getline(ss, temp, '"');
        getline(ss, val, '"');

        if (property == "stroke-linejoin") {
            this->strokeLineJoin = val;
        }
        else if (property == "stroke-linecap") {
            this->strokeLineCap = val;
        }
        else if (property == "d") {
            // Validate cơ bản
            if (val.empty() || (val[0] != 'M' && val[0] != 'm'))
                return;

            for (int i = 0; i < val.size(); i++) {
                if (isalpha(val[i]) && val[i] != 'e') {
                    // Thêm space sau lệnh nếu chưa có
                    if (i + 1 < val.size() && val[i + 1] != ' ')
                        val.insert(i + 1, " ");
                    // Thêm space trước lệnh nếu liền trước là số
                    if (i - 1 > -1 && isdigit(val[i - 1]))
                        val.insert(i, " ");
                }
                // Thay dấu phẩy bằng space
                if (val[i] == ',')
                    val[i] = ' ';
                // Tách dấu trừ nếu nó dính liền số trước đó (ví dụ: 10-10 -> 10 -10)
                if (val[i] == '-' && i > 0 && val[i - 1] != ' ' && val[i - 1] != 'e')
                    val.insert(i, " ");
            }

            // 1.2. Xử lý lỗi dính dấu chấm (ví dụ: .5.5 -> .5 .5)
            for (int i = 0; i < val.size(); i++) {
                if (val[i] == '.') {
                    int j = i + 1;
                    while (j < val.size() && val[j] != '.') j++; // Tìm dấu chấm tiếp theo

                    int t = i + 1;
                    while (t < j && val[t] != ' ') t++; // Kiểm tra khoảng giữa có space không

                    if (t == j) { // Nếu không có space, chèn vào
                        val.insert(j, " ");
                        i = j + 1;
                    }
                }
            }
            for (int i = 0; i < val.size(); i++) {
                if (isalpha(val[i]) && val[i] != 'e') {
                    // Cắt chuỗi con chứa lệnh và tham số của nó
                    int j = i + 1;
                    while (j < val.size() && (!isalpha(val[j]) || val[j] == 'e'))
                        j++;

                    string pointStr = val.substr(i, j - i);

                    pair<char, vector<float>> pr;
                    pr.first = pointStr[0];
                    if (pointStr.size() >= 2) pointStr.erase(0, 2);
                    else pointStr.erase(0, 1); // Fallback an toàn

                    stringstream paramStream(pointStr);

                    // --- XỬ LÝ TỪNG LOẠI LỆNH ---

                    if (pr.first == 'm') {
                        bool first = true;
                        string x, y;
                        while (paramStream >> x >> y) {
                            float refX = 0, refY = 0;
                            if (first) {
                                if (!vct.empty()) {
                                    size_t m = vct.back().second.size();
                                    if (m > 1) {
                                        refX = vct.back().second[m - 2];
                                        refY = vct.back().second[m - 1];
                                    }
                                }
                                first = false;
                            }
                            else {
                                size_t n = pr.second.size();
                                if (n > 1) {
                                    refX = pr.second[n - 2];
                                    refY = pr.second[n - 1];
                                }
                            }
                            pr.second.push_back(stof(x) + refX);
                            pr.second.push_back(stof(y) + refY);
                        }
                        vct.push_back(pr);
                    }

                    else if (pr.first == 'l') {
                        bool first = true;
                        string x, y;
                        while (paramStream >> x >> y) {
                            float refX = 0, refY = 0;
                            if (first) {
                                if (!vct.empty()) {
                                    size_t m = vct.back().second.size();
                                    if (m > 1) {
                                        refX = vct.back().second[m - 2];
                                        refY = vct.back().second[m - 1];
                                        first = false;
                                    }
                                }
                            }
                            else {
                                size_t n = pr.second.size();
                                if (n > 1) {
                                    refX = pr.second[n - 2];
                                    refY = pr.second[n - 1];
                                }
                            }
                            pr.second.push_back(stof(x) + refX);
                            pr.second.push_back(stof(y) + refY);
                        }
                        vct.push_back(pr);
                    }

                    else if (pr.first == 'H') {
                        string x;
                        while (paramStream >> x) {
                            pr.second.push_back(stof(x));
                            if (!vct.empty()) {
                                size_t m = vct.back().second.size();
                                if (m > 1)
                                    pr.second.push_back(vct.back().second[m - 1]);
                            }
                        }
                        vct.push_back(pr);
                    }

                    else if (pr.first == 'h') {
                        bool first = true;
                        string x;
                        while (paramStream >> x) {
                            float refX = 0, currentY = 0;
                            if (first) {
                                if (!vct.empty()) {
                                    size_t m = vct.back().second.size();
                                    if (m > 1) {
                                        refX = vct.back().second[m - 2];
                                        currentY = vct.back().second[m - 1];
                                        first = false;
                                    }
                                }
                            }
                            else {
                                size_t n = pr.second.size();
                                if (n > 1) {
                                    refX = pr.second[n - 2];
                                    currentY = pr.second[n - 1];
                                }
                            }
                            pr.second.push_back(stof(x) + refX);
                            pr.second.push_back(currentY);
                        }
                        vct.push_back(pr);
                    }

                    else if (pr.first == 'V') {
                        string y;
                        while (paramStream >> y) {
                            // V chỉ có y, nên lấy x của điểm cũ
                            if (!vct.empty()) {
                                size_t m = vct.back().second.size();
                                if (m > 1)
                                    pr.second.push_back(vct.back().second[m - 2]);
                            }
                            pr.second.push_back(stof(y));
                        }
                        vct.push_back(pr);
                    }

                    else if (pr.first == 'v') {
                        bool first = true;
                        string y;
                        while (paramStream >> y) {
                            float currentX = 0, refY = 0;
                            if (first) {
                                if (!vct.empty()) {
                                    size_t m = vct.back().second.size();
                                    if (m > 1) {
                                        currentX = vct.back().second[m - 2];
                                        refY = vct.back().second[m - 1];
                                        first = false;
                                    }
                                }
                            }
                            else {
                                size_t n = pr.second.size();
                                if (n > 1) {
                                    currentX = pr.second[n - 2];
                                    refY = pr.second[n - 1];
                                }
                            }
                            pr.second.push_back(currentX);
                            pr.second.push_back(stof(y) + refY);
                        }
                        vct.push_back(pr);
                    }

                    else if (pr.first == 'T' || pr.first == 't') {
                        bool first = true;
                        string x, y;
                        while (paramStream >> x >> y) {
                            if (first) {
                                if (!vct.empty()) {
                                    size_t m = vct.back().second.size();
                                    char prevCmd = vct.back().first;

                                    // Tính điểm reflection
                                    if (prevCmd == 'Q' || prevCmd == 'q' || prevCmd == 'T' || prevCmd == 't') {
                                        if (m > 3) {
                                            float oldx2 = vct.back().second[m - 4];
                                            float oldy2 = vct.back().second[m - 3];
                                            float curx = vct.back().second[m - 2];
                                            float cury = vct.back().second[m - 1];
                                            pr.second.push_back(2.f * curx - oldx2);
                                            pr.second.push_back(2.f * cury - oldy2);
                                        }
                                    }
                                    else {
                                        if (m > 1) {
                                            pr.second.push_back(vct.back().second[m - 2]);
                                            pr.second.push_back(vct.back().second[m - 1]);
                                        }
                                    }

                                    if (pr.first == 'T') {
                                        pr.second.push_back(stof(x));
                                        pr.second.push_back(stof(y));
                                    }
                                    else if (m > 1) {
                                        pr.second.push_back(stof(x) + vct.back().second[m - 2]);
                                        pr.second.push_back(stof(y) + vct.back().second[m - 1]);
                                    }
                                    first = false;
                                }
                            }
                            else {
                                size_t n = pr.second.size();
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
                        string x1, y1, x2, y2, x3, y3;
                        while (paramStream >> x1 >> y1 >> x2 >> y2 >> x3 >> y3) {
                            float refX = 0, refY = 0;
                            if (first) {
                                if (!vct.empty()) {
                                    size_t m = vct.back().second.size();
                                    if (m > 1) {
                                        refX = vct.back().second[m - 2];
                                        refY = vct.back().second[m - 1];
                                        first = false;
                                    }
                                }
                            }
                            else {
                                size_t n = pr.second.size();
                                if (n > 1) {
                                    refX = pr.second[n - 2];
                                    refY = pr.second[n - 1];
                                }
                            }
                            pr.second.push_back(stof(x1) + refX);
                            pr.second.push_back(stof(y1) + refY);
                            pr.second.push_back(stof(x2) + refX);
                            pr.second.push_back(stof(y2) + refY);
                            pr.second.push_back(stof(x3) + refX);
                            pr.second.push_back(stof(y3) + refY);
                        }
                        vct.push_back(pr);
                    }

                    else if (pr.first == 's' || pr.first == 'q') {
                        bool first = true;
                        string x1, y1, x2, y2;
                        while (paramStream >> x1 >> y1 >> x2 >> y2) {
                            float refX = 0, refY = 0;
                            if (first) {
                                if (!vct.empty()) {
                                    size_t m = vct.back().second.size();
                                    if (m > 1) {
                                        refX = vct.back().second[m - 2];
                                        refY = vct.back().second[m - 1];
                                        first = false;
                                    }
                                }
                            }
                            else {
                                size_t n = pr.second.size();
                                if (n > 1) {
                                    refX = pr.second[n - 2];
                                    refY = pr.second[n - 1];
                                }
                            }
                            pr.second.push_back(stof(x1) + refX);
                            pr.second.push_back(stof(y1) + refY);
                            pr.second.push_back(stof(x2) + refX);
                            pr.second.push_back(stof(y2) + refY);
                        }
                        vct.push_back(pr);
                    }

                    else if (pr.first == 'a') {
                        bool first = true;
                        string rx, ry, xAR, lAF, sF, x, y;
                        while (paramStream >> rx >> ry >> xAR >> lAF >> sF >> x >> y) {
                            float refX = 0, refY = 0;
                            if (first) {
                                if (!vct.empty()) {
                                    size_t m = vct.back().second.size();
                                    if (m > 1) {
                                        refX = vct.back().second[m - 2];
                                        refY = vct.back().second[m - 1];
                                        first = false;
                                    }
                                }
                            }
                            else {
                                size_t n = pr.second.size();
                                if (n > 1) {
                                    refX = pr.second[n - 2];
                                    refY = pr.second[n - 1];
                                }
                            }
                            pr.second.push_back(stof(rx));
                            pr.second.push_back(stof(ry));
                            pr.second.push_back(stof(xAR));
                            pr.second.push_back(stof(lAF));
                            pr.second.push_back(stof(sF));
                            pr.second.push_back(stof(x) + refX);
                            pr.second.push_back(stof(y) + refY);
                        }
                        vct.push_back(pr);
                    }

                    else if (pr.first == 'Z' || pr.first == 'z') {
                        if (!vct.empty()) {
                            size_t m = vct.back().second.size();
                            if (m > 1) {
                                pr.second.push_back(vct.back().second[m - 2]);
                                pr.second.push_back(vct.back().second[m - 1]);
                            }
                        }
                        vct.push_back(pr);
                    }

                    else { // Trường hợp M, L, C, S, Q, A (Chữ hoa - Tọa độ tuyệt đối)
                        string token;
                        while (paramStream >> token)
                            pr.second.push_back(stof(token));
                        vct.push_back(pr);
                    }
                }
            }
        }
    }
}

void SVGPath::draw(Graphics& graphics) {
    GraphicsState save = graphics.Save();

    vector<pair<char, vector<float>>> vct = this->getProp();
    FillMode fillMode = (this->fillRule == "evenodd") ? FillModeAlternate : FillModeWinding;
    GraphicsPath path(fillMode);
    PointF P0;

    for (int i = 0; i < vct.size(); i++) {
        char cmd = vct[i].first;
        vector<float>& params = vct[i].second;
        int numPoint = params.size();

        switch (cmd) {
        case 'M': case 'm': {
            if (this->grad == NULL) {
                path.StartFigure();
                if (numPoint == 4) {
                    PointF start(params[0], params[1]);
                    PointF end(params[2], params[3]);
                    path.AddLine(start, end);
                    P0 = end;
                }
                else if (numPoint > 4) {
                    int k = 0;
                    vector<PointF> points(numPoint / 2);
                    for (int j = 0; j < numPoint; j += 2)
                        points[k++] = PointF(params[j], params[j + 1]);
                    path.AddLines(points.data(), numPoint / 2);
                    P0 = points[numPoint / 2 - 1];
                }
                else {
                    P0 = PointF(params[0], params[1]);
                }
            }
            else {
                path.StartFigure();
                int j = 0;
                while (numPoint > 1) {
                    PointF P1(params[j], params[j + 1]);
                    path.AddLine(P0, P1);
                    P0 = P1;
                    numPoint -= 2;
                    j += 2;
                }
            }
            break;
        }

        case 'Q': case 'q': case 'T': case 't': {
            int j = 0;
            while (numPoint > 3) {
                PointF P1(params[j], params[j + 1]);
                PointF P2(params[j + 2], params[j + 3]);
                path.AddBezier(P0, P1, P2, P2);
                P0 = P2;
                numPoint -= 4;
                j += 4;
            }
            break;
        }

        case 'C': case 'c': {
            int j = 0;
            while (numPoint > 5) {
                PointF P1(params[j], params[j + 1]);
                PointF P2(params[j + 2], params[j + 3]);
                PointF P3(params[j + 4], params[j + 5]);
                path.AddBezier(P0, P1, P2, P3);
                P0 = P3;
                numPoint -= 6;
                j += 6;
            }
            break;
        }

        case 'S': case 's': {
            int j = 0;
            while (numPoint > 3) {
                PointF P1 = P0;
                if (i > 0) {
                    char prevCmd = vct[i - 1].first;
                    if (prevCmd == 'C' || prevCmd == 'c' || prevCmd == 'S' || prevCmd == 's') {
                        vector<float>& prevParams = vct[i - 1].second;
                        int n = prevParams.size();
                        if (n > 3) {
                            float oldx2 = prevParams[n - 4];
                            float oldy2 = prevParams[n - 3];
                            float curx = prevParams[n - 2];
                            float cury = prevParams[n - 1];
                            P1 = PointF(2.f * curx - oldx2, 2.f * cury - oldy2);
                        }
                    }
                }
                PointF P2(params[j], params[j + 1]);
                PointF P3(params[j + 2], params[j + 3]);
                path.AddBezier(P0, P1, P2, P3);
                P0 = P3;
                numPoint -= 4;
                j += 4;
            }
            break;
        }

        case 'A': case 'a': {
            int j = 0;
            while (numPoint > 6) {
                if (i > 0) {
                    if (vct[i - 1].second.size() > 1) {
                        double sx = P0.X;
                        double sy = P0.Y;
                        double rx = params[j + 0];
                        double ry = params[j + 1];
                        double xAR = params[j + 2];
                        bool lAF = params[j + 3];
                        bool sF = params[j + 4];
                        double ex = params[j + 5];
                        double ey = params[j + 6];

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
                        if (num < 0) num = 0;

                        double x2 = sign * sqrt(num / den) * rx * y1 / ry;
                        double y2 = -sign * sqrt(num / den) * ry * x1 / rx;

                        double x = cosAngle * x2 - sinAngle * y2 + ((sx + ex) / 2.f);
                        double y = sinAngle * x2 + cosAngle * y2 + ((sy + ey) / 2.f);

                        a = (x1 - x2) / rx;
                        b = (y1 - y2) / ry;
                        double c = (-x1 - x2) / rx;
                        double d = (-y1 - y2) / ry;

                        if (b < 0) sign = -1.f; else sign = 1.f;
                        double temp = a / sqrt(a * a + b * b);
                        if (temp < -1.f) temp = -1.f; else if (temp > 1.f) temp = 1.f;
                        double startAngle = sign * acos(temp);

                        if (a * d - b * c < 0) sign = -1.f; else sign = 1.f;
                        temp = (a * c + b * d) / (sqrt(a * a + b * b) * sqrt(c * c + d * d));
                        if (temp < -1.f) temp = -1.f; else if (temp > 1.f) temp = 1.f;
                        double dentaAngle = sign * acos(temp);

                        if (sF == 0 && dentaAngle > 0) dentaAngle -= (2.f * Pi);
                        else if (sF == 1 && dentaAngle < 0) dentaAngle += (2.f * Pi);

                        double ratio = abs(dentaAngle) / (Pi / 2.f);
                        if (abs(1.f - ratio) < 0.0000001) ratio = 1.f;
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

                            for (size_t k = 0; k < cur.size(); k += 3) {
                                if (k + 2 < cur.size()) {
                                    PointF P1(cur[k][0], cur[k][1]);
                                    PointF P2(cur[k + 1][0], cur[k + 1][1]);
                                    PointF P3(cur[k + 2][0], cur[k + 2][1]);
                                    path.AddBezier(P0, P1, P2, P3);
                                    P0 = P3;
                                }
                            }
                        }
                        P0 = PointF(ex, ey);
                    }
                }
                numPoint -= 7;
                j += 7;
            }
            break;
        }

        case 'L': case 'H': case 'V': case 'l': case 'h': case 'v': {
            int j = 0;
            while (numPoint > 1) {
                PointF P1(params[j], params[j + 1]);
                path.AddLine(P0, P1);
                P0 = P1;
                numPoint -= 2;
                j += 2;
            }
            break;
        }

        case 'Z': case 'z':
            path.CloseFigure();
            break;
        }
    }

    RectF originalBounds;
    path.GetBounds(&originalBounds);

    vector<pair<string, vector<float>>> transVct = this->getTransVct();
    for (const auto& trans : transVct) {
        string type = trans.first;
        const vector<float>& args = trans.second;
        float x = args.empty() ? 0.0f : args[0];
        float y = (args.size() == 2) ? args[1] : x;

        if (type == "translate") {
            graphics.TranslateTransform(x, y);
        }
        else if (type == "rotate") {
            graphics.RotateTransform(x);
        }
        else if (type == "scale") {
            graphics.ScaleTransform(x, y);
        }
        else if (type == "matrix") {
            if (args.size() >= 6) {
                Matrix matrix(args[0], args[1], args[2], args[3], args[4], args[5]);
                graphics.MultiplyTransform(&matrix);
            }
        }
    }

    graphics.SetSmoothingMode(SmoothingModeAntiAlias);

    if (this->grad == NULL) {
        color sColor = this->getStroke().getStrokeColor();
        color fColor = this->getColor();
        Pen penPath(Color(sColor.opacity * 255, sColor.r, sColor.g, sColor.b), this->getStroke().getStrokeWidth());
        SolidBrush fillPath(Color(fColor.opacity * 255, fColor.r, fColor.g, fColor.b));

        graphics.FillPath(&fillPath, &path);
        graphics.DrawPath(&penPath, &path);
    }
    else {
        Brush* fillBrush = this->getGrad()->createBrush(originalBounds);

        if (this->grad->getType() == RADIAL) {
            radialgradient* radial = dynamic_cast<radialgradient*>(this->grad);
            vector<stop> stops = radial->getStopVct();
            Color outerColor(255, 255, 255, 255);

            if (!stops.empty()) {
                stop lastStop = stops.back();
                outerColor = Color(
                    lastStop.stopColor.opacity * 255,
                    lastStop.stopColor.r,
                    lastStop.stopColor.g,
                    lastStop.stopColor.b
                );
            }

            SolidBrush solidBrush(outerColor);
            graphics.FillPath(&solidBrush, &path);
            graphics.FillPath(fillBrush, &path);
        }
        else {
            graphics.FillPath(fillBrush, &path);
        }
        delete fillBrush;
    }

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