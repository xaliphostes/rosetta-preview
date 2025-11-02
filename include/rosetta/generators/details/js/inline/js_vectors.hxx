/*
 * Copyright (c) 2025-now fmaerten@gmail.com
 * LGPL v3 license
 *
 */
#include "../js_common.h"
#include "../js_generator.h"
#include <rosetta/type_registry.h>
#include <type_traits>
#include <typeinfo>

namespace rosetta {

    namespace detail {
        /**
         * @brief Generic vector to JavaScript converter (factored implementation)
         * @tparam VectorType The vector type (std::vector<T> or alias)
         * @tparam ElementType The element type
         */
        template <typename VectorType, typename ElementType>
        inline Napi::Value vectorToJs(Napi::Env env, const std::any &value) {
            try {
                const auto &vec = std::any_cast<const VectorType &>(value);
                auto        arr = Napi::Array::New(env, vec.size());
                for (size_t i = 0; i < vec.size(); ++i) {
                    arr.Set(i, toNapiValue<ElementType>(env, vec[i]));
                }
                return arr;
            } catch (const std::bad_any_cast &) {
                // Fallback: try value copy instead of reference
                auto vec = std::any_cast<VectorType>(value);
                auto arr = Napi::Array::New(env, vec.size());
                for (size_t i = 0; i < vec.size(); ++i) {
                    arr.Set(i, toNapiValue<ElementType>(env, vec[i]));
                }
                return arr;
            }
        }

        /**
         * @brief Generic JavaScript to vector converter (factored implementation)
         * @tparam VectorType The vector type (std::vector<T> or alias)
         * @tparam ElementType The element type
         */
        template <typename VectorType, typename ElementType>
        inline std::any jsToVector(const Napi::Value &js_val) {
            if (!js_val.IsArray()) {
                throw Napi::TypeError::New(js_val.Env(), "Expected array");
            }

            auto       arr = js_val.As<Napi::Array>();
            VectorType vec;
            vec.reserve(arr.Length());

            for (uint32_t i = 0; i < arr.Length(); ++i) {
                vec.push_back(fromNapiValue<ElementType>(arr.Get(i)));
            }

            return vec;
        }
    } // namespace detail

    // ============================================================================
    // Auto vector registration - Public API
    // ============================================================================

    template <typename T> inline void registerVectorType(JsGenerator &generator) {
        using VectorType     = std::vector<T>;
        std::string typeName = typeid(VectorType).name();

        generator.register_type_converter(typeName, detail::vectorToJs<VectorType, T>,
                                          detail::jsToVector<VectorType, T>);
    }

    template <typename AliasType, typename ElementType>
    inline void registerTypeAlias(JsGenerator &generator) {
        std::string aliasName = typeid(AliasType).name();

        generator.register_type_converter(aliasName, detail::vectorToJs<AliasType, ElementType>,
                                          detail::jsToVector<AliasType, ElementType>);
    }

    inline void registerCommonVectorTypes(JsGenerator &generator) {
        // Integer types
        registerVectorType<int>(generator);
        registerVectorType<unsigned int>(generator);
        registerVectorType<int32_t>(generator);
        registerVectorType<uint32_t>(generator);
        registerVectorType<int64_t>(generator);
        registerVectorType<uint64_t>(generator);
        registerVectorType<size_t>(generator);

        // Floating point types
        registerVectorType<float>(generator);
        registerVectorType<double>(generator);

        // Other basic types
        registerVectorType<bool>(generator);
        registerVectorType<char>(generator);
        registerVectorType<std::string>(generator);
    }

    template <typename T> inline void registerIntrospectableVectorType(rosetta::JsGenerator &gen) {
        static_assert(std::is_base_of_v<rosetta::Introspectable, T>,
                      "Type must inherit from Introspectable");

        // Use getTypeName to be consistent with how types are looked up
        std::string typeName = rosetta::getTypeName<std::vector<T>>();

        gen.register_type_converter(
            typeName,
            // C++ vector<T> -> JS array
            [](Napi::Env env, const std::any &value) -> Napi::Value {
                try {
                    const auto &vec = std::any_cast<const std::vector<T> &>(value);
                    auto        arr = Napi::Array::New(env, vec.size());
                    for (size_t i = 0; i < vec.size(); ++i) {
                        auto constructor = rosetta::ObjectWrapper<T>::constructor.Value();
                        auto instance    = constructor.New({});
                        auto wrapper =
                            Napi::ObjectWrap<rosetta::ObjectWrapper<T>>::Unwrap(instance);
                        *wrapper->GetCppObject() = vec[i];
                        arr.Set(i, instance);
                    }
                    return arr;
                } catch (const std::bad_any_cast &) {
                    return env.Undefined();
                }
            },
            // JS array -> C++ vector<T>
            [](const Napi::Value &js_val) -> std::any {
                if (!js_val.IsArray()) {
                    throw std::runtime_error("Expected array");
                }
                auto           arr = js_val.As<Napi::Array>();
                std::vector<T> vec;
                vec.reserve(arr.Length());
                for (uint32_t i = 0; i < arr.Length(); ++i) {
                    auto obj     = arr.Get(i).As<Napi::Object>();
                    auto wrapper = Napi::ObjectWrap<rosetta::ObjectWrapper<T>>::Unwrap(obj);
                    vec.push_back(*wrapper->GetCppObject());
                }
                return vec;
            });
    }

    // Variadic version to register multiple vector types at once
    template <typename... Types> inline void registerIntrospectableVectorTypes(rosetta::JsGenerator &gen) {
        (registerIntrospectableVectorType<Types>(gen), ...);
    }

} // namespace rosetta
