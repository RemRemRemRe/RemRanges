// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>
#include "Templates/RemInstanceOf.h"

#include "transrangers.hpp"

namespace Rem::Ranges
{
template <transrangers::is_ranger TRanger>
void ForEach(TRanger Ranger)
{
    Ranger([&](const typename TRanger::cursor& Cursor)
    {
        // required to run the Ranger
        return *Cursor;
    });
}

template <transrangers::is_ranger TRanger, CInstanceOf<TArray> ArrayType>
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
[[nodiscard]] constexpr decltype(auto) ToArray(TRanger Ranger)
{
    using ValueType = transrangers::ranger_element_t<TRanger>;

    TArray<ValueType> OutArray{};
    ToArray(OutArray, Ranger);

    return OutArray;
}

template <uint32 Count, transrangers::is_ranger TRanger>
[[nodiscard]] constexpr decltype(auto) TakeN(TRanger Ranger)
{
    using ValueType = transrangers::ranger_element_t<TRanger>;

    static_assert(Count > 0, "Count must be greater than 0");

    TArray<ValueType> OutArray{};
    ToArray(OutArray, transrangers::take<Count>(Ranger));

    return OutArray;
}

template <uint32 Ordinal, transrangers::is_ranger TRanger>
[[nodiscard]] constexpr decltype(auto) NthElement(TRanger Ranger)
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
[[nodiscard]] constexpr decltype(auto) FirstElement(TRanger Ranger)
{
    return NthElement<0, TRanger>(Ranger);
}

template <CInstanceOf<TArray> ArrayType>
[[nodiscard]] constexpr auto ArrayView(ArrayType&& Array)
{
    return transrangers::all(MakeArrayView(Array));
}

template <CInstanceOf<TArray> ArrayType>
[[nodiscard]] constexpr auto ConstArrayView(ArrayType&& Array)
{
    return transrangers::all(MakeConstArrayView(Array));
}
}
