#include <iostream>
#include <rosetta/generators/js.h>

struct Point : public rosetta::Introspectable {
    INTROSPECTABLE(Point)
    double x, y, z;
    Point() : x(0), y(0), z(0) {}
    Point(double x, double y, double z) : x(x), y(y), z(z) {}
};
void Point::registerIntrospection(rosetta::TypeRegistrar<Point> reg) {
    reg.constructor<>()
        .constructor<double, double, double>()
        .member("x", &Point::x)
        .member("y", &Point::y)
        .member("z", &Point::z);
}

// -----------------------------------------------------

struct Triangle : public rosetta::Introspectable {
    INTROSPECTABLE(Triangle)
    int a, b, c;
    Triangle() : a(0), b(0), c(0) {}
    Triangle(int a, int b, int c) : a(a), b(b), c(c) {}
};
void Triangle::registerIntrospection(rosetta::TypeRegistrar<Triangle> reg) {
    reg.constructor<>()
        .constructor<int, int, int>()
        .member("a", &Triangle::a)
        .member("b", &Triangle::b)
        .member("c", &Triangle::c);
}

// -----------------------------------------------------

class Surface : public rosetta::Introspectable {
    INTROSPECTABLE(Surface)
public:
    std::vector<Point>    points;
    std::vector<Triangle> triangles;

    Surface() = default;
    Surface(const std::vector<double> &positions, const std::vector<int> &indices) {
        size_t num_points = positions.size() / 3;
        for (size_t i = 0; i < num_points; ++i) {
            size_t idx = 3 * i;
            points.push_back(Point{positions[idx], positions[idx + 1], positions[idx + 2]});
        }

        size_t num_triangles = indices.size() / 3;
        for (size_t i = 0; i < num_triangles; ++i) {
            size_t idx = 3 * i;
            triangles.push_back(Triangle{indices[idx], indices[idx + 1], indices[idx + 2]});
        }
    }

    // --- converters (get/set) exposed to Rosetta ---

    const std::vector<Point>    &getPoints() const { return points; }
    void                         setPoints(const std::vector<Point> &pts) { points = pts; }
    const std::vector<Triangle> &getTriangles() const { return triangles; }
    void setTriangles(const std::vector<Triangle> &tris) { triangles = tris; }
};
void Surface::registerIntrospection(rosetta::TypeRegistrar<Surface> reg) {
    reg.constructor<>()
        .constructor<const std::vector<double> &, const std::vector<int> &>()
        .member("points", &Surface::points)
        .member("triangles", &Surface::triangles)
        .method("setPoints", &Surface::setPoints)
        .method("setTriangles", &Surface::setTriangles)
        .method("getPoints", &Surface::getPoints)
        .method("getTriangles", &Surface::getTriangles);
}

// -----------------------------------------------------
// Use a wrapper IModel class to expose Model
// -----------------------------------------------------
class Model {
    std::vector<Surface> surfaces;
public:
    Model() = default;
    void                        addSurface(const Surface &surface) { surfaces.push_back(surface); }
    const std::vector<Surface> &getSurfaces() const { return surfaces; }
    void                        setSurfaces(const std::vector<Surface> &s) { surfaces = s; }
};
class IModel : public rosetta::Introspectable {
    INTROSPECTABLE(IModel)
    Model model;

public:
    IModel() = default;
    void                        addSurface(const Surface &surface) { model.addSurface(surface); }
    const std::vector<Surface> &getSurfaces() const { return model.getSurfaces(); }
    void                        setSurfaces(const std::vector<Surface> &s) { model.setSurfaces(s); }
};
void IModel::registerIntrospection(rosetta::TypeRegistrar<IModel> reg) {
    reg.method("getSurfaces", &IModel::getSurfaces)  //- a getter for surfaces (will gen property surfaces)
        .method("setSurfaces", &IModel::setSurfaces) //- a setter for surfaces (will gen property surfaces)
        .method("addSurface", &IModel::addSurface);
}



// --------------------
// ROSETTA REGISTRATION
// --------------------
BEGIN_JS(generator) {
    // WARNING: define the classes in order of dependency, so Model after Surface
    registerAllForClasses<Point, Triangle, Surface>(generator);
    registerAllForClass<IModel>(generator, "Model");
}
END_JS();
