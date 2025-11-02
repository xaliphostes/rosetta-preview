/*
 * Copyright (c) 2025-now fmaerten@gmail.com
 * LGPL v3 license
 */
#pragma once
#include <rosetta/rosetta.h>

/**
 * @file adapter.h
 * @brief Adapter class template to wrap original types for introspection.
 * This file defines the Adapter class template that allows existing types
 * to be adapted for use with the Rosetta introspection system. It provides
 * mechanisms to register member variables, methods, and constructors of the
 * original type.
 */

namespace rosetta {

    /**
     * @brief Trait to provide type name for Adapter
     */
    template <typename T> struct TypeNameTrait;

    /**
     * @brief Adapter class to wrap original types for introspection.
     * This class template adapts an existing type to be compatible with the
     * Rosetta introspection system. It inherits from Introspectable and provides
     * the necessary type information and registration mechanisms.
     * @tparam OriginalType The original type to be adapted
     * @example
     * ```cpp
     * # include "mesh.h"
     * # include <rosetta/adapter.h>
     * 
     * template <> struct TypeNameTrait<MyAPI::Mesh> {
     *     static constexpr const char* name = "Mesh";
     * };
     * 
     * template <>
     * void Adapter<MyAPI::Mesh>::registerIntrospection(
     *     rosetta::TypeRegistrar<Adapter<MyAPI::Mesh>> reg) 
     * {
     *     reg.constructor<>()
     *         .method("addVertex", [](Adapter<MyAPI::Mesh>* self, const Adapter<MyAPI::Point>& p) {
     *             self->getOriginal().addVertex(p.getOriginal());
     *         })
     *         .method("getVertexCount", [](Adapter<MyAPI::Mesh>* self) {
     *             return self->getOriginal().getVertexCount();
     *         });
     * }
     * 
     * // Register for Javascript
     * BEGIN_JS(generator) {
     *     registerAllForClass<Adapter<MyAPI::Mesh>>(generator, "Mesh");
     * }
     * END_JS();
     * ```
     */
    template <typename OriginalType> class Adapter : public rosetta::Introspectable {
    private:
        OriginalType original;

        // Manual implementation instead of INTROSPECTABLE macro
        static rosetta::TypeInfo &getStaticTypeInfoImpl() {
            static rosetta::TypeInfo info(TypeNameTrait<OriginalType>::name);
            static bool              initialized = false;
            if (!initialized) {
                registerIntrospection(rosetta::TypeRegistrar<Adapter<OriginalType>>(info));
                initialized = true;
            }
            return info;
        }

    public:
        Adapter() = default;

        template <typename... Args>
        Adapter(Args &&...args) : original(std::forward<Args>(args)...) {}

        // Implement Introspectable interface
        static rosetta::TypeInfo &getStaticTypeInfo() { return getStaticTypeInfoImpl(); }

        const rosetta::TypeInfo &getTypeInfo() const override { return getStaticTypeInfo(); }

        OriginalType       &getOriginal() { return original; }
        const OriginalType &getOriginal() const { return original; }

    private:
        static void registerIntrospection(rosetta::TypeRegistrar<Adapter<OriginalType>> reg);
    };

} // namespace rosetta