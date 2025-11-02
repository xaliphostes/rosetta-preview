#pragma once
#include <vector>

namespace MyAPI {

    class Point {
    public:
        Point() : x_(0), y_(0), z_(0) {}
        Point(double x, double y, double z) : x_(x), y_(y), z_(z) {}
        double x() const { return x_; }
        double y() const { return y_; }
        double z() const { return z_; }
        void   setX(double x) { x_ = x; }
        void   setY(double y) { y_ = y; }
        void   setZ(double z) { z_ = z; }
        double magnitude() const {
            return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
        }

    private:
        double x_, y_, z_;
    };

    class Mesh {
    public:
        void         addVertex(const Point &v) { vertices_.push_back(v); }
        size_t       getVertexCount() const { return vertices_.size(); }
        const std::vector<Point> &getVertices() const { return vertices_; }
        void setVertices(const std::vector<Point> &verts) { vertices_ = verts; }

    private:
        std::vector<Point> vertices_;
    };

} // namespace MyAPI