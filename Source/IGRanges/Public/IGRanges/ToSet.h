// Copyright Ian Good

#pragma once

#include "Containers/Set.h"

#include "IGRanges/Impl/Prologue.inl"

namespace IG::Ranges
{
namespace Private
{
struct ToSet_fn
{
	template <typename RangeType>
	[[nodiscard]] constexpr auto operator()(RangeType&& Range) const
	{
		using T = ranges::range_value_t<RangeType>;
		TSet<T> Set;

		if constexpr (ranges::sized_range<RangeType>)
		{
			Set.Reserve(ranges::distance(Range));
		}

		for (auto&& X : Range)
		{
			Set.Emplace(X);
		}

		return Set;
	}
};

} // namespace Private

/**
 * Creates a `TSet` from a range.
 *
 * @usage
 * TSet<int32> UniqueNumbers = SomeNumbers | ToSet();
 */
[[nodiscard]] inline constexpr auto ToSet()
{
	return ranges::make_view_closure(_IGRP ToSet_fn{});
}

/**
 * Same as `ToSet` (no parameters) but first applies a projection to elements.
 * Equivalent to `Select(proj) | ToSet()`.
 *
 * @usage
 * TSet<USkeletalMesh*> UniqueMeshes = MySkMeshComponents | ToSet(&USkeletalMeshComponent::GetSkeletalMeshAsset);
 */
template <typename TransformT>
[[nodiscard]] constexpr auto ToSet(TransformT&& Trans)
{
	return ranges::views::transform(std::forward<TransformT>(Trans))
		 | IG::Ranges::ToSet();
}

} // namespace IG::Ranges

#include "IGRanges/Impl/Epilogue.inl"
