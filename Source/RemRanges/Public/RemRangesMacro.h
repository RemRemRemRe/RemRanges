// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once


/**
 * somehow, it's a workaround for "Error C1060 : compiler is out of heap space"
 * 
 * it serves for two purpose:
 * 1. markup places where Rem::Ranges::ForEach can't be used because of the compiler error
 * 2. reduce boilerplate
 */
#define REM_RANGES_FOREACH(Ranger) \
    Ranger([&](const typename decltype(Ranger)::cursor& Cursor) \
    { \
        auto _ = *Cursor; \
        return true; \
    });
