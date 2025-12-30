#ifndef _POINT_H_
#define _POINT_H_

#include <cmath> // Thêm thư viện toán học phòng khi cần

class point {
public:
    float x, y;
    bool intersect; // Giữ lại thuộc tính cũ của bạn

public:
    // --- CONSTRUCTORS ---

    // Constructor mặc định (Khởi tạo giá trị ban đầu)
    point() : x(0.0f), y(0.0f), intersect(false) {}

    // Constructor có tham số (Rất quan trọng cho các phép tính vector mới)
    point(float x, float y) : x(x), y(y), intersect(false) {}

    // Destructor
    ~point() {}

    // --- GETTERS / SETTERS (Giữ nguyên để tương thích code cũ) ---
    // Thêm 'const' vào getter để đảm bảo an toàn dữ liệu
    float getX() const { return x; }
    float getY() const { return y; }
    bool getIntersect() const { return intersect; }

    void setIntersect(bool val) { intersect = val; }
    void setX(float val) { x = val; }
    void setY(float val) { y = val; }

    // --- CÁC PHÉP TOÁN NÂNG CẤP (QUAN TRỌNG CHO GRADIENT) ---

    // 1. Cộng hai điểm (Vector addition): p1 + p2
    // Dùng để cộng toạ độ gốc (Origin) với độ dời
    point operator+(const point& other) const {
        return point(x + other.x, y + other.y);
    }

    // 2. Trừ hai điểm (Vector subtraction): p1 - p2
    // Dùng để tính vector hướng hoặc kích thước
    point operator-(const point& other) const {
        return point(x - other.x, y - other.y);
    }

    // 3. Nhân với một số thực (Scale): p * 2.0
    // Dùng khi zoom hoặc scale hình
    point operator*(float scalar) const {
        return point(x * scalar, y * scalar);
    }

    // 4. Nhân hai điểm với nhau (Component-wise multiplication): p1 * p2
    // CỰC KỲ QUAN TRỌNG: Dùng để tính (Tỉ lệ Gradient) * (Kích thước BoundingBox)
    // Ví dụ: (0.5, 0.5) * (100, 200) = (50, 100)
    point operator*(const point& other) const {
        return point(x * other.x, y * other.y);
    }

    // 5. Chia cho một số thực: p / 2.0
    point operator/(float scalar) const {
        if (scalar == 0.0f) return point(0, 0); // Tránh chia cho 0
        return point(x / scalar, y / scalar);
    }

    // --- CÁC PHÉP SO SÁNH ---

    // So sánh bằng (Friend function như code cũ của bạn)
    friend bool operator == (const point& p1, const point& p2) {
        // So sánh xấp xỉ float để tránh lỗi làm tròn (epsilon)
        return (std::abs(p1.x - p2.x) < 1e-5) && (std::abs(p1.y - p2.y) < 1e-5);
    }

    // So sánh khác (Bổ sung thêm cho tiện)
    friend bool operator != (const point& p1, const point& p2) {
        return !(p1 == p2);
    }
};

#endif // _POINT_H_