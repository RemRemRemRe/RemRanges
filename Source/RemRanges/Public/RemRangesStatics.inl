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
		for (auto&& Iter : transrangers::input_view(std::move(Ranger)))
		{
		}
	}

	template<typename TRanger, typename TFunctor>
	void ForEach(TRanger Ranger, TFunctor Functor)
	{
		for (auto&& Iter : transrangers::input_view(std::move(Ranger)))
		{
			Functor(std::forward<decltype(Iter)>(std::move(Iter)));
		}
	}

	template<typename TRanger, typename ArrayType>
	requires Rem::is_instance_v<ArrayType, TArray>
	void ToArray(ArrayType& OutArray, TRanger Ranger)
	{
		ForEach(std::move(Ranger), [&](auto&& Iter)
		{
			OutArray.Add(std::forward<ArrayType::ElementType>(Iter));
		});
	}
}
