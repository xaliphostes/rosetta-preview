/*

- Copyright (c) 2025-now fmaerten@gmail.com
- LGPL v3 license
-

*/
#include <array>
#include "../js_generator.h"
#include "../js_common.h"
#include <rosetta/type_registry.h>
#include <type_traits>
#include <typeinfo>

namespace rosetta {

    namespace detail {
        /**
         * @brief Generic array to JavaScript converter
         * @tparam ArrayType The array type (std::array<T,N> or alias)
         * @tparam ElementType The element type
         * @tparam N Size of the array
         */
        template <typename ArrayType, typename ElementType, size_t N>
        inline Napi::Value arrayToJs(Napi::Env env, const std::any& value)
        {
            try {
                const auto& arr = std::any_cast<const ArrayType&>(value);
                auto js_arr = Napi::Array::New(env, N);
                for (size_t i = 0; i < N; ++i) {
                    js_arr.Set(i, toNapiValue<ElementType>(env, arr[i]));
                }
                return js_arr;
            } catch (const std::bad_any_cast&) {
                // Fallback: try value copy instead of reference
                auto arr = std::any_cast<ArrayType>(value);
                auto js_arr = Napi::Array::New(env, N);
                for (size_t i = 0; i < N; ++i) {
                    js_arr.Set(i, toNapiValue<ElementType>(env, arr[i]));
                }
                return js_arr;
            }
        }

        /**
         * @brief Generic JavaScript to array converter (factored implementation)
         * @tparam ArrayType The array type (std::array<T,N> or alias)
         * @tparam ElementType The element type
         * @tparam N Size of the array
         */
        template <typename ArrayType, typename ElementType, size_t N>
        inline std::any jsToArray(const Napi::Value& js_val)
        {
            if (!js_val.IsArray()) {
                throw Napi::TypeError::New(js_val.Env(), "Expected array");
            }

            auto js_arr = js_val.As<Napi::Array>();
            if (js_arr.Length() != N) {
                throw Napi::TypeError::New(js_val.Env(),
                    "Expected array of length " + std::to_string(N) + ", got "
                        + std::to_string(js_arr.Length()));
            }

            ArrayType arr;
            for (uint32_t i = 0; i < N; ++i) {
                arr[i] = fromNapiValue<ElementType>(js_arr.Get(i));
            }

            return arr;
        }
    } // namespace detail

    // ============================================================================
    // Auto array registration - Public API
    // ============================================================================

    /**
     * @brief Register std::array type converter using automatic type name from typeid
     * @tparam T Element type of the array
     * @tparam N Size of the array
     * @param generator The JavaScript generator to register with
     */
    template <typename T, size_t N> inline void registerArrayType(JsGenerator& generator)
    {
        using ArrayType = std::array<T, N>;
        std::string typeName = typeid(ArrayType).name();

        generator.register_type_converter(
            typeName, detail::arrayToJs<ArrayType, T, N>, detail::jsToArray<ArrayType, T, N>);
    }

    /**
     * @brief Register type alias converter for std::array
     * @tparam AliasType The type alias (e.g., Vec3 = std::array<double, 3>)
     * @tparam ElementType The element type of the underlying array
     * @tparam N Size of the array
     * @param generator The JavaScript generator to register with
     */
    template <typename AliasType, typename ElementType, size_t N>
    inline void registerArrayAlias(JsGenerator& generator)
    {
        std::string aliasName = typeid(AliasType).name();

        generator.register_type_converter(aliasName, detail::arrayToJs<AliasType, ElementType, N>,
            detail::jsToArray<AliasType, ElementType, N>);
    }

    /**
     * @brief Register common fixed-size array types
     * @param generator The JavaScript generator to register with
     */
    inline void registerCommonArrayTypes(JsGenerator& generator)
    {
        // 2D vectors/points
        registerArrayType<double, 2>(generator);
        registerArrayType<float, 2>(generator);
        registerArrayType<int, 2>(generator);

        // 3D vectors/points / 2x2 sym matrices
        registerArrayType<double, 3>(generator);
        registerArrayType<float, 3>(generator);
        registerArrayType<int, 3>(generator);

        // 4D vectors (homogeneous coordinates, quaternions)
        registerArrayType<double, 4>(generator);
        registerArrayType<float, 4>(generator);

        // 3x3 sym matrices (flattened)
        registerArrayType<double, 6>(generator);
        registerArrayType<float, 6>(generator);

        // 3x3 matrices (flattened)
        registerArrayType<double, 9>(generator);
        registerArrayType<float, 9>(generator);

        // 4x4 matrices (flattened)
        registerArrayType<double, 16>(generator);
        registerArrayType<float, 16>(generator);

        // RGB colors
        registerArrayType<unsigned char, 3>(generator);
        registerArrayType<float, 3>(generator);

        // RGBA colors
        registerArrayType<unsigned char, 4>(generator);
        registerArrayType<float, 4>(generator);
    }

} // namespace rosetta
