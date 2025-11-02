#include <rosetta/generators/js.h>
#include "../../classes_demo.h"

BEGIN_JS(generator) {
    registerAllForClasses<Person, Vehicle>(generator);
}
END_JS();
