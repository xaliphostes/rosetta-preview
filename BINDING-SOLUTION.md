# Template Adapter Pattern

## Solution 1: Manual Specialization (Recommended)

Create named wrappers:

```cpp
#include <rosetta/rosetta.h>
#include <MyAPI/API.h>

// ============================================
// Create specific wrapper classes
// ============================================

class PointAdapter : public rosetta::Introspectable {
    INTROSPECTABLE(PointAdapter)

public:
    MyAPI::Point original;

    PointAdapter() = default;
    PointAdapter(double x, double y, double z) : original(x, y, z) {}
    
    double getX() const { return original.x; }
    void setX(double v) { original.x = v; }
    double getY() const { return original.y; }
    void setY(double v) { original.y = v; }
    double getZ() const { return original.z; }
    void setZ(double v) { original.z = v; }
    double magnitude() const { return original.magnitude(); }


};

void PointAdapter::registerIntrospection(rosetta::TypeRegistrar<PointAdapter> reg) {
    reg.constructor<>()
        .constructor<double, double, double>()
        .method("getX", &PointAdapter::getX)
        .method("setX", &PointAdapter::setX)
        .method("getY", &PointAdapter::getY)
        .method("setY", &PointAdapter::setY)
        .method("getZ", &PointAdapter::getZ)
        .method("setZ", &PointAdapter::setZ)
        .method("magnitude", &PointAdapter::magnitude);
}

class MeshAdapter : public rosetta::Introspectable {
    INTROSPECTABLE(MeshAdapter)
private:
    MyAPI::Mesh original;

public:
    MeshAdapter() = default;
    
    void addVertex(const PointAdapter& p) {
        // Need to extract original Point from PointAdapter
        // This van be a conversion challenge (here, simple)!
        original.addVertex(p.original);
    }
    
    // ... other methods
};

void MeshAdapter::registerIntrospection(rosetta::TypeRegistrar<MeshAdapter> reg) {
    reg.constructor<>()
        .method("addVertex", &MeshAdapter::addVertex);
}

// Binding
BEGIN_JS(generator) {
    registerAllForClass<PointAdapter>(generator, "Point");
    registerAllForClass<MeshAdapter>(generator, "Mesh");
}
END_JS();
```

**Pros**:
- ✅ Clean, no template issues
- ✅ Each class has proper name
- ✅ Straightforward to understand

**Cons**:
- ❌ Repetitive code for similar classes
- ❌ Can't share logic easily

---

## Solution 2: Template Base + Explicit Derived Classes

Use a template for common functionality, but derive named classes:

```cpp
#include <rosetta/rosetta.h>
#include <MyAPI/API.h>

// ============================================
// Template base (NOT introspectable)
// ============================================

template <typename OriginalType>
class AdapterBase {
protected:
    OriginalType original;

public:
    AdapterBase() = default;
    
    template <typename... Args>
    AdapterBase(Args&&... args) : original(std::forward<Args>(args)...) {}
    
    OriginalType& getOriginal() { return original; }
    const OriginalType& getOriginal() const { return original; }
};

// ============================================
// Explicit named wrappers (introspectable)
// ============================================

class PointAdapter : public AdapterBase<MyAPI::Point>, 
                     public rosetta::Introspectable {
    INTROSPECTABLE(PointAdapter)

public:
    using AdapterBase<MyAPI::Point>::AdapterBase;  // Inherit constructors
    
    // Expose methods
    double getX() const { return original.x; }
    void setX(double v) { original.x = v; }
    double getY() const { return original.y; }
    void setY(double v) { original.y = v; }
    double getZ() const { return original.z; }
    void setZ(double v) { original.z = v; }
    double magnitude() const { return original.magnitude(); }
};

void PointAdapter::registerIntrospection(rosetta::TypeRegistrar<PointAdapter> reg) {
    reg.constructor<>()
        .constructor<double, double, double>()
        .method("getX", &PointAdapter::getX)
        .method("setX", &PointAdapter::setX)
        .method("getY", &PointAdapter::getY)
        .method("setY", &PointAdapter::setY)
        .method("getZ", &PointAdapter::getZ)
        .method("setZ", &PointAdapter::setZ)
        .method("magnitude", &PointAdapter::magnitude);
}

class MeshAdapter : public AdapterBase<MyAPI::Mesh>,
                    public rosetta::Introspectable {
    INTROSPECTABLE(MeshAdapter)

public:
    using AdapterBase<MyAPI::Mesh>::AdapterBase;
    
    void addVertex(const PointAdapter& p) {
        original.addVertex(p.getOriginal());
    }
    
    int getVertexCount() const { 
        return original.getVertexCount(); 
    }
};

void MeshAdapter::registerIntrospection(rosetta::TypeRegistrar<MeshAdapter> reg) {
    reg.constructor<>()
        .method("addVertex", &MeshAdapter::addVertex)
        .method("getVertexCount", &MeshAdapter::getVertexCount);
}

// Binding
BEGIN_JS(generator) {
    registerAllForClass<PointAdapter>(generator, "Point");
    registerAllForClass<MeshAdapter>(generator, "Mesh");
}
END_JS();
```

**Pros**:
- ✅ Shared functionality in template base
- ✅ Explicit, named introspectable classes
- ✅ No template/INTROSPECTABLE conflicts

**Cons**:
- ❌ Still need to write derived classes
- ❌ Slightly more complex hierarchy

---

## Recommendations

For most cases, **don't use templates** for the introspectable class itself. Use:

### Solution 1: Simple Named Wrappers
```cpp
class PointWrapper { ... };  // Explicit class
class MeshWrapper { ... };   // Explicit class
```

### Solution 2: Template Base + Named Derived
```cpp
template <typename T>
class AdapterBase { ... };   // Template for shared code

class PointAdapter : public AdapterBase<Point>, 
                     public Introspectable { ... };  // Named introspectable
class MeshAdapter : public AdapterBase<Mesh>,
                    public Introspectable { ... };   // Named introspectable
```
