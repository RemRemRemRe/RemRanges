// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>

#include "transranger_view.hpp"

namespace Rem::Ranges
{

	template<typename T>
	auto MakeRefChecked(T* Pointer)
	{
		return std::ref(*Pointer);
	}

	template<typename Range>
	void ForEach(Range Ranger)
	{
		for (auto Iter : transrangers::input_view(Ranger))
		{
		}
	}

	template<typename Range, typename TFunctor>
	void ForEach(Range Ranger, TFunctor Functor)
	{
		for (auto Iter : transrangers::input_view(Ranger))
		{
			Functor(Iter);
		}
	}
}
