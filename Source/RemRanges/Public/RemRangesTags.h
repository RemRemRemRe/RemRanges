// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include "RemMetaTags.h"
#include "RemRangesTags.generated.h"

#define REM_API REMRANGES_API

UCLASS(Config = RemRangesTags)
class REM_API URemRangesTags : public URemMetaTags
{
	GENERATED_BODY()
};

#undef REM_API
