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
	    Ranger([&](auto)
        {
            return true;
        });
	}

	template<typename TRanger, typename TFunctor>
	void ForEach(TRanger Ranger, TFunctor Functor)
	{
        Ranger([&](auto Iter)
        {
            Functor(std::forward<decltype(*Iter)>(std::move(*Iter)));
            return true;
        });
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

    template<typename ArrayType>
	requires Rem::is_instance_v<ArrayType, TArray>
    auto ArrayView(ArrayType&& Array)
    {
        return transrangers::all(MakeArrayView(Array));
    }

    template<typename ArrayType>
    requires Rem::is_instance_v<ArrayType, TArray>
    auto ConstArrayView(ArrayType&& Array)
    {
        return transrangers::all(MakeConstArrayView(Array));
    }
}
