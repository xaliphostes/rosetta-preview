#include <rosetta/generators/js.h>

// -----------------------------------------------------

/**
 * @brief Simple algorithm class to be wrapped. This class is totally independent
 * of Rosetta and has no introspection capabilities.
 */
class Algo {
public:
    double run(double tol) {
        // Long computation
        double       a = 0;
        const double N = 1000000;
        for (size_t i = 0; i < N; ++i) {
            a += std::sin(i / N * 3.1415926);
        }
        return a / N;
    }
};

// -----------------------------------------------------

/**
 * @brief Wrapper around class IAlgo to have proper introspection.
 * This class uses Rosetta's introspection capabilities.
 * It simply wraps an instance of Algo and exposes its methods.
 * This class is the one that will be bound to JavaScript.
 * It is a common pattern to separate the introspectable wrapper
 * from the actual implementation.
 */
class IAlgo : public rosetta::Introspectable {
    INTROSPECTABLE(IAlgo)
public:
    double run(double tol) { return algo_.run(tol); }

private:
    Algo algo_;
};

void IAlgo::registerIntrospection(rosetta::TypeRegistrar<IAlgo> reg) {
    reg.constructor<>().method("run", &IAlgo::run);
}
// -----------------------------------------------------

BEGIN_JS(generator) {
    registerAllForClass<IAlgo>(generator, "Algo");
}
END_JS()
