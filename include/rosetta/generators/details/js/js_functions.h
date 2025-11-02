#pragma once
#include <rosetta/generators/details/js/js_generator.h>
#include <rosetta/function_registry.h>

namespace rosetta {

    /**
     * @brief Register a single function by name
     */
    void registerFunction(JsGenerator &generator, const std::string &func_name);

    /**
     * @brief Register multiple specific functions by name
     */
    inline void registerFunctions(JsGenerator &generator, const std::vector<std::string> &func_names);

    /**
     * @brief Register ALL registered functions
     */
    inline void registerAllFunctions(JsGenerator &generator);

} // namespace rosetta

#include "inline/js_functions.hxx"
