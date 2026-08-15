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
    // rangers over const views yield reference elements: the container must
    // store plain values, while the transform lambda keeps the original
    // forwarding semantics of ValueType
    using ValueType      = transrangers::ranger_element_t<TRanger>;
    using ArrayValueType = std::remove_cvref_t<ValueType>;

    ForEach(transrangers::transform(
        [&](ValueType&& Value)
        {
            OutArray.Add(ArrayValueType{Value});
            return true;
        },
        std::move(Ranger)));
}

template <transrangers::is_ranger TRanger>
[[nodiscard]] constexpr decltype(auto) ToArray(TRanger Ranger)
{
    using ValueType      = transrangers::ranger_element_t<TRanger>;
    using ArrayValueType = std::remove_cvref_t<ValueType>;

    TArray<ArrayValueType> OutArray{};
    ToArray(OutArray, Ranger);

    return OutArray;
}

template <uint32 Count, transrangers::is_ranger TRanger>
[[nodiscard]] constexpr decltype(auto) TakeN(TRanger Ranger)
{
    using ValueType      = transrangers::ranger_element_t<TRanger>;
    using ArrayValueType = std::remove_cvref_t<ValueType>;

    static_assert(Count > 0, "Count must be greater than 0");

    TArray<ArrayValueType> OutArray{};
    ToArray(OutArray, transrangers::take<Count>(Ranger));

    return OutArray;
}

template <uint32 Ordinal, transrangers::is_ranger TRanger>
[[nodiscard]] constexpr decltype(auto) NthElement(TRanger Ranger)
{
    using ValueType      = transrangers::ranger_element_t<TRanger>;
    using ArrayValueType = std::remove_cvref_t<ValueType>;

    auto Output = ArrayValueType{};

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

namespace Detail
{
/** Cursor yielding the run's sub-view of the source. */
template <typename T>
struct TRunCursor
{
    TArrayView<T> Run;

    TRunCursor() = default;

    explicit TRunCursor(const TArrayView<T>& InRun)
        : Run(InRun)
    {
    }

    [[nodiscard]] TArrayView<T> operator*() const
    {
        return Run;
    }
};
}

/**
 * Groups consecutive elements sharing the same key into runs, emitting one
 * TArrayView<element> per run: a zero-copy sub-view of the source elements.
 *
 * Precondition: elements with equal keys must be contiguous in the input
 * (e.g. after sorting by the key), like transrangers::unique -- only adjacent
 * elements are compared. The source must yield reference elements into one
 * contiguous array (ArrayView/ConstArrayView; do not place a filter in front)
 * so each run can be described by a pointer range. Emitted views stay valid
 * as long as the source array does.
 */
template <typename FKey, transrangers::is_ranger TRanger>
[[nodiscard]] auto GroupAdjacent(FKey&& Key, TRanger Ranger)
{
    using Element = transrangers::ranger_element_t<TRanger>;
    static_assert(std::is_lvalue_reference_v<Element>,
        "GroupAdjacent requires reference elements: the source must be a contiguous "
        "view over an array (ArrayView/ConstArrayView) so runs can be emitted as "
        "sub-views without copying.");

    using RunElement = std::remove_reference_t<Element>;
    using RunView    = TArrayView<RunElement>;
    using RunCursor  = Detail::TRunCursor<RunElement>;

    return transrangers::ranger<RunCursor>(
        [ranger = std::move(Ranger),
            key = std::forward<FKey>(Key),
            bStarted = false,
            bExhausted = false,
            p = typename TRanger::cursor{}]
    (auto dst) mutable
        {
            while (!bExhausted)
            {
                // Pull the first element of the run into p. The source is
                // stopped right after it: ranger returns true only when the
                // source had nothing left to yield, i.e. it is exhausted.
                if (!bStarted)
                {
                    if (ranger([&](const auto& q)
                    {
                        p = q;
                        return false;
                    }))
                    {
                        bExhausted = true;
                        break;
                    }
                    bStarted = true;
                }

                const RunElement* RunBegin = &*p;
                auto pLast                 = p;
                const auto RunKey          = key(*p);

                // Consume the remainder of the run. The first element whose
                // key differs is held in p for the next call: the source
                // consumes the element that stops it, so it must be remembered
                // here (same pattern as transrangers::unique).
                auto bHasBoundary = false;
                if (ranger([&](const auto& q)
                {
                    if (key(*q) == RunKey)
                    {
                        pLast = q;
                        return true;
                    }
                    p            = q;
                    bHasBoundary = true;
                    return false;
                }))
                {
                    bExhausted = true;
                }

                // Run end: the boundary element's address, or one past the last
                // collected element when the run reaches the source's end.
                const RunElement* RunEnd = bHasBoundary
                                               ? &*p
                                               : &*pLast + 1;

                if (!dst(RunCursor(RunView(RunBegin, static_cast<int32>(RunEnd - RunBegin)))))
                {
                    return false;
                }
            }

            return true;
        });
}
}
