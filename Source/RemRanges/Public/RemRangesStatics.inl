// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>
#include "Templates/RemIsInstance.h"

#include "transranger_view.hpp"
#include "transrangers.hpp"

namespace Rem::Ranges
{
	template<typename T>
	auto MakeRefChecked(T* Pointer)
	{
		return std::ref(*Pointer);
	}
	
	template<typename TRanger>
	void ForEach(TRanger Ranger)
	{
		auto View = transrangers::input_view(std::move(Ranger));
		for (auto Iter = View.begin(); Iter != View.end(); ++Iter)
		{
		}
	}

	template<typename TRanger, typename TFunctor>
	void ForEach(TRanger Ranger, TFunctor Functor)
	{
		auto View = transrangers::input_view(std::move(Ranger));
		for (auto Iter = View.begin(); Iter != View.end(); ++Iter)
		{
			Functor(std::forward<decltype(*Iter)>(std::move(*Iter)));
		}
	}

	template<typename TRanger, typename ArrayType>
	requires Rem::is_instance_v<ArrayType, TArray>
	void ToArray(ArrayType& OutArray, TRanger Ranger)
	{
		ForEach(std::move(Ranger), [&](auto&& Value)
		{
			OutArray.Add(std::forward<ArrayType::ElementType>(std::move(Value)));
		});
	}
}
