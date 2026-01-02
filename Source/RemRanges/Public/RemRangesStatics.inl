// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>
#include "Templates/RemIsInstance.h"

#include "transrangers.hpp"

namespace Rem::Ranges
{    
	template<typename T>
	auto MakeRefChecked(T* Pointer)
	{
		return std::ref(*Pointer);
	}

	template<transrangers::is_ranger TRanger>
	void ForEach(TRanger Ranger)
	{
	    Ranger([&](typename TRanger::cursor)
        {
            return true;
        });
	}

	template<transrangers::is_ranger TRanger, typename TFunctor>
	void ForEach(TRanger Ranger, TFunctor Functor)
	{
	    using ValueType = transrangers::ranger_element_t<TRanger>;
	    
        Ranger([&](typename TRanger::cursor Iter)
        {
            Functor(std::forward<ValueType>(std::move(*Iter)));
            return true;
        });
	}

	template<transrangers::is_ranger TRanger, typename ArrayType>
	requires Rem::is_instance_v<ArrayType, TArray>
	void ToArray(ArrayType& OutArray, TRanger Ranger)
	{
	    using ValueType = transrangers::ranger_element_t<TRanger>;
	    
		ForEach(std::move(Ranger), [&](ValueType&& Value)
		{
			OutArray.Add(std::forward<ValueType>(std::move(Value)));
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
