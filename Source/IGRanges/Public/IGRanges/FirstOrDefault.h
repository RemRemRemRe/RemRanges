// Copyright Ian Good

#pragma once

#include "IGRanges/Impl/Common.h"
#include "Templates/SharedPointer.h"

#include "IGRanges/Impl/Prologue.inl"
#include "view/view.hpp"
#include "functional/bind_back.hpp"

namespace IG::Ranges
{
namespace Private
{
struct FirstOrDefault_fn
{
	template <typename RangeType, class _Pr>
	[[nodiscard]] constexpr auto operator()(RangeType&& Range, _Pr _Pred) const
	{
		using T = ranges::range_value_t<RangeType>;

		static_assert(!TIsTSharedRef_V<T>, "`FirstOrDefault` cannot operate on ranges of `TSharedRef`.");

		for (auto&& X : Range)
		{
			if (std::invoke(_Pred, X))
			{
				return X;
			}
		}

		return _IGRP Construct<T>();
	}
};

} // namespace Private

/**
 * Returns the first element of a sequence, or a default-initialized value if the sequence contains no elements.
 *
 * If a predicate is specified, then returns the first element of the sequence that satisfies the predicate.
 * Equivalent to `Where(pred) | FirstOrDefault()`.
 *
 * @usage
 * AActor* Rosie = SomeActors | Where([](const AActor* A) { return GetNameSafe(A) == TEXT("Rosie"); }) | FirstOrDefault();
 * AActor* Rosie = SomeActors | FirstOrDefault([](const AActor* A) { return GetNameSafe(A) == TEXT("Rosie"); });
 */
template <class _Pr = _IGRP AlwaysTrue>
[[nodiscard]] constexpr auto FirstOrDefault(_Pr&& _Pred = {})
{
	return ranges::make_view_closure(ranges::bind_back(_IGRP FirstOrDefault_fn{}, std::forward<_Pr>(_Pred)));
}

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
