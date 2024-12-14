// Copyright Ian Good

#include "IGRanges/CustomizationPoints.h"
#include "IGRangesInternal.h"
#include "Misc/AutomationTest.h"
#include "view/filter.hpp"
#include "view/transform.hpp"

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FIGRangesCPOSpec, "IG.Ranges.CPO", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

/**
 * This function pretty much just ensures that things compile when using UE containers with C++ Ranges.
 * If this fails to compile, that means there's probably some missing customization points for the container (see
 * `CustomizationPoints.h`).
 */
static void CheckCompat(auto&& Container)
{
	UE_LOG(LogIGRangesTests, Verbose, TEXT("%hs"), __FUNCSIG__);

	const bool bIsEmpty = ranges::empty(Container);
	UE_LOG(LogIGRangesTests, Verbose, TEXT("IsEmpty=%s"), bIsEmpty ? TEXT("true") : TEXT("false"));

	const int32 Count = ranges::distance(Container);
	UE_LOG(LogIGRangesTests, Verbose, TEXT("Count=%d"), Count);

	auto EvenElements = Container | ranges::views::filter([](auto&& x) { return x % 2 == 0; });

	auto SquaredElements = Container | ranges::views::transform([](auto&& x) { return x * x; });

	auto Sqevens =
		Container
		| ranges::views::filter([](auto&& x) { return x % 2 == 0; })
		| ranges::views::transform([](auto&& x) { return x * x; });
	for (const int32 X : Sqevens)
	{
		UE_LOG(LogIGRangesTests, Verbose, TEXT("X=%d"), X);
	}
}

#define IGR_CHECK_COMPAT(_ContainerType)    \
	{                                       \
		_ContainerType C;                   \
		CheckCompat(C);                     \
		CheckCompat(MoveTempIfPossible(C)); \
	}

void FIGRangesCPOSpec::Define()
{
	It("check_compat", [this]() {
		IGR_CHECK_COMPAT(TArray<int32>);
		IGR_CHECK_COMPAT(TArray<const int32>);
		IGR_CHECK_COMPAT(const TArray<int32>);
		IGR_CHECK_COMPAT(const TArray<const int32>);

		IGR_CHECK_COMPAT(TArrayView<int32>);
		IGR_CHECK_COMPAT(TArrayView<const int32>);
		IGR_CHECK_COMPAT(const TArrayView<int32>);
		IGR_CHECK_COMPAT(const TArrayView<const int32>);
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
