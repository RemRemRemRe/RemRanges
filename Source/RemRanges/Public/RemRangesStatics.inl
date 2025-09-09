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
	    Ranger([&](auto)
        {
            return true;
        });
	}

	template<transrangers::is_ranger TRanger, typename TFunctor>
	void ForEach(TRanger Ranger, TFunctor Functor)
	{
        Ranger([&](auto Iter)
        {
            Functor(std::forward<decltype(*Iter)>(std::move(*Iter)));
            return true;
        });
	}
    
    // TODO walkaround for
    // transrangers::take(1) SOMETIMES causing "Error C1060 : compiler is out of heap space" on MSVC 14.44.35207 Windows 10.0.26100.0 SDK
    template<uint32 Count, transrangers::is_ranger TRanger>
	void Take(TRanger Ranger)
	{
	    static_assert(Count > 0, "Count must be greater than 0");
	    
	    uint32 Counter = Count;
	    Ranger([&](auto)
        {
	        --Counter;
            if (Counter > 0)
            {
                return true;
            }
	        
            return false;
        });
	}

	template<transrangers::is_ranger TRanger, typename ArrayType>
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
