/*
 * Copyright (c) 2025-now fmaerten@gmail.com
 * LGPL v3 license
 */
#pragma once
#include <any>
#include <napi.h>
#include <unordered_set>

namespace rosetta {

    using CppToJsConverter = std::function<Napi::Value(Napi::Env, const std::any &)>;
    using JsToCppConverter = std::function<std::any(const Napi::Value &)>;

    /**
     * @brief Automatic N-API/JavaScript binding generator for introspectable classes
     * @example
     * ```cpp
     * // Usage example:
     * BEGIN_JS(generator) {
     *     registerClasses<Person, Vehicle>(generator);
     * }
     * END_JS();
     * ```
     */
    class JsGenerator {
    public:
        JsGenerator(Napi::Env env, Napi::Object exports);

        template <typename T> JsGenerator &bind_class(const std::string &class_name = "");

        JsGenerator &add_utilities();
        JsGenerator &register_type_converter(const std::string &, CppToJsConverter,
                                             JsToCppConverter);

        // Public members for convenience (beurk!)
        Napi::Env    env;
        Napi::Object exports;

    private:
        std::unordered_set<std::string> bound_classes;
    };

    // ============================================================================

    /**
     * @brief Add utility functions to the JS generator
     */
    void registerUtilities(JsGenerator &gen);

    /**
     * @brief Register introspectable object type
     * @example
     * ```cpp
     * registerIntrospectableObjectTypes<Point, Triangle, Surface>(gen);
     * ```
     */
    template <typename T> void        registerIntrospectableObjectType(rosetta::JsGenerator &gen);
    /**
     * @brief Register multiple introspectable object types
     */
    template <typename... Types> void registerIntrospectableObjectTypes(rosetta::JsGenerator &gen);

    /**
     * @brief Register type in type name registry
     * @example
     * ```cpp
     * registerTypes<Point, Triangle, Surface, Model>(gen);
     * ```
     */
    template <typename T> void        registerType(rosetta::JsGenerator &gen);
    /**
     * @brief Register multiple types in type name registry
     */
    template <typename... Types> void registerTypes(rosetta::JsGenerator &gen);

    /**
     * @brief Register class (binds the class to JS)
     */
    template <typename T>
    void registerClass(rosetta::JsGenerator &gen, const std::string &class_name = "");
    /**
     * @brief Register multiple classes (binds the classes to JS)
     */
    template <typename... Types> void registerClasses(rosetta::JsGenerator &gen);

    /**
     * @brief Register all (introspectable object types and type names) for a class
     * (class, object type, vector type, functor, vectors, pointers, etc.)
     */
    template <typename T>
    void registerAllForclass(rosetta::JsGenerator &gen, const std::string &class_name = "");
    /**
     * @brief Register all (introspectable object types and type names) for multiple classes.
     * @see registerAllForclass
     */
    template <typename... Types> void registerAllForClasses(rosetta::JsGenerator &gen);

} // namespace rosetta

#define NAPI_AUTO_BIND_CLASS(generator, ClassName) generator.bind_class<ClassName>(#ClassName)

#include "inline/js_generator.hxx"