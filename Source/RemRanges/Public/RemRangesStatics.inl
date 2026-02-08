// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>
#include "Templates/RemIsInstance.h"
#include "Macro/RemFunctorMacro.h"
#include "RemMisc.h"

#include "transrangers.hpp"

namespace Rem::Ranges
{
    template <typename T>
    [[nodiscard]] constexpr auto MakeRefChecked(T* Pointer)
    {
        return std::ref(*Pointer);
    }

    REM_FUNCTION_TO_FUNCTOR_SIMPLE(MakeRefChecked)

    template <transrangers::is_ranger TRanger>
    [[nodiscard]] constexpr auto GetValidRef(TRanger Ranger)
    {
        return transrangers::transform(Fn::MakeRefChecked,
            transrangers::filter(Rem::Fn::IsValid, Ranger));
    }

    template <transrangers::is_ranger TRanger>
    void ForEach(TRanger Ranger)
    {
        Ranger([&](const typename TRanger::cursor& Cursor)
            {
                // required to run the Ranger
                return *Cursor;
            });
    }

    template <transrangers::is_ranger TRanger, typename ArrayType>
        requires Rem::is_instance_v<ArrayType, TArray>
    constexpr void ToArray(ArrayType& OutArray, TRanger Ranger)
    {
        using ValueType = transrangers::ranger_element_t<TRanger>;

        ForEach(transrangers::transform(
            [&](ValueType&& Value)
                {
                    OutArray.Add(Value);
                    return true;
                },
            std::move(Ranger)));
    }

    template <transrangers::is_ranger TRanger>
    [[nodiscard]] constexpr auto ToArray(TRanger Ranger) -> decltype(auto)
    {
        using ValueType = transrangers::ranger_element_t<TRanger>;

        TArray<ValueType> OutArray{};
        ToArray(OutArray, Ranger);

        return OutArray;
    }

    template <uint32 Count, transrangers::is_ranger TRanger>
    [[nodiscard]] constexpr auto TakeN(TRanger Ranger) -> decltype(auto)
    {
        using ValueType = transrangers::ranger_element_t<TRanger>;

        static_assert(Count > 0, "Count must be greater than 0");

        TArray<ValueType> OutArray{};
        ToArray(OutArray, transrangers::take<Count>(Ranger));

        return OutArray;
    }

    template <uint32 Ordinal, transrangers::is_ranger TRanger>
    [[nodiscard]] constexpr auto NthElement(TRanger Ranger) -> decltype(auto)
    {
        using ValueType = transrangers::ranger_element_t<TRanger>;

        auto Output = std::remove_reference_t<ValueType>{};

        auto NthRanger = transrangers::take<Ordinal + 1>(Ranger);

        ForEach(transrangers::transform(
            [&](ValueType&& Value)
                {
                    Output = Value;
                    return true;
                },
            std::move(NthRanger)));

        return Output;
    }

    template <transrangers::is_ranger TRanger>
    [[nodiscard]] constexpr auto FirstElement(TRanger Ranger) -> decltype(auto)
    {
        return NthElement<0, TRanger>(Ranger);
    }

    template <typename ArrayType>
        requires Rem::is_instance_v<ArrayType, TArray>
    [[nodiscard]] constexpr auto ArrayView(ArrayType&& Array)
    {
        return transrangers::all(MakeArrayView(Array));
    }

    template <typename ArrayType>
        requires Rem::is_instance_v<ArrayType, TArray>
    [[nodiscard]] constexpr auto ConstArrayView(ArrayType&& Array)
    {
        return transrangers::all(MakeConstArrayView(Array));
    }
}
