// Copyright Ian Good

#pragma once

#include <numeric>

#include "IGRanges/Impl/Prologue.inl"
#include "functional/bind_back.hpp"
#include "view/view.hpp"

namespace IG::Ranges
{
namespace Private
{
struct Accumulate_fn
{
	template <typename RangeType, typename SeedType, typename FoldType>
	[[nodiscard]] auto operator()(RangeType&& Range, SeedType&& Seed, FoldType&& Fold) const
	{
		return std::accumulate(std::begin(Range), std::end(Range), std::forward<SeedType>(Seed), std::forward<FoldType>(Fold));
	}
};

} // namespace Private

/**
 * Applies an accumulator function over a range.
 * The specified seed value is used as the initial accumulator value.
 *
 * Alias for `std::accumulate`:
 * Initializes an accumulator value with the seed value and then modifies it with `Fold(acc, *i)` for every element in
 * the range.
 *
 * @usage
 * FString Results =
 *     SomeObjects
 *     | Accumulate(FString(TEXT("Objects:")), [](FString Acc, const UObject* Obj) {
 *           return Acc + GetNameSafe(Obj) + TEXT(',');
 *       });
 * // example Results = "Objects:Foo,Bar,None,Blah,"
 */
template <typename T, typename FoldType>
[[nodiscard]] constexpr auto Accumulate(T&& Seed, FoldType&& Fold)
{
	return ranges::make_view_closure(ranges::bind_back(
		_IGRP Accumulate_fn{}, std::forward<T>(Seed), std::forward<FoldType>(Fold)));
}

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
