#include <rosetta/generators/js.h>

using Stress = std::array<double, 6>;

// -----------------------------------------------------

class A : public rosetta::Introspectable {
    INTROSPECTABLE(A)
public:
    const Stress stress() const { return stress_; }
    void         setStress(const Stress &stress) { stress_ = stress; }

private:
    Stress stress_;
};

void A::registerIntrospection(rosetta::TypeRegistrar<A> reg) {
    reg.method("stress", &A::stress).method("setStress", &A::setStress);
}

// -----------------------------------------------------

BEGIN_JS(generator) {
    registerAllForClasses<A>(generator);
}
END_JS();
