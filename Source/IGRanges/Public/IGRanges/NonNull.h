// Copyright Ian Good

#pragma once

#include "Templates/SharedPointer.h"

#include "IGRanges/Impl/Prologue.inl"
#include "algorithm/equal_range.hpp"
#include "view/view.hpp"
#include "view/filter.hpp"
#include "view/transform.hpp"

namespace IG::Ranges
{
namespace Private
{
template<bool bValidate>
struct NonNullRef_fn
{
	template <typename RangeType>
	[[nodiscard]] constexpr auto operator()(RangeType&& Range) const
	{
		using T = ranges::range_value_t<RangeType>;

		if constexpr (TIsTWeakPtr_V<T>) // Support for `TWeakPtr`
		{
			if constexpr (bValidate)
			{
				return std::forward<RangeType>(Range)
					 | ranges::views::filter([](auto&& x) { return x.IsValid(); })
					 | ranges::views::transform([](auto&& x) -> typename T::ElementType& { return *x.Pin().Get(); });
			}
			else
			{
				return std::forward<RangeType>(Range)
					 | ranges::views::transform([](auto&& x) -> typename T::ElementType& { return *x.Pin().Get(); });
			}
		}
		else
		{
			if constexpr (bValidate)
			{
				return std::forward<RangeType>(Range)
					 | ranges::views::filter([](auto&& x) { return x != nullptr; })
					 | ranges::views::transform([](auto&& x) -> decltype(*x)& { return *x; });
			}
			else
			{
				return std::forward<RangeType>(Range)
					 | ranges::views::transform([](auto&& x) -> decltype(*x)& { return *x; });
			}
		}
	}
};

} // namespace Private

/**
 * Filters a sequence of pointer-like values, removing null elements.
 * Works on raw pointers (e.g. `UFoo*`) and smart pointers (e.g. `TObjectPtr<UFoo>`, `TWeakPointer<FBar>`, etc.) and
 * anything that can be null-checked.
 */
[[nodiscard]] inline constexpr auto NonNull()
{
	return ranges::views::filter([]<typename T>(T&& x) {
		if constexpr (TIsTWeakPtr_V<T>) // Support for `TWeakPtr`
		{
			return x.IsValid();
		}
		else
		{
			return x != nullptr;
		}
	});
}

/**
 * Same as `NonNull` but yields references to values instead of pointers.
 */
[[nodiscard]] inline constexpr auto NonNullRef()
{
	return ranges::make_view_closure(_IGRP NonNullRef_fn<true>{});
}

/**
 * Same as `NonNull` but yields references to values instead of pointers and skipping validation
 */
[[nodiscard]] inline constexpr auto NonNullRefChecked()
{
	return ranges::make_view_closure(_IGRP NonNullRef_fn<false>{});
}

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
