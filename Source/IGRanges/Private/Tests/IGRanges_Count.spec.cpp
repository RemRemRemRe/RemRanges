// Copyright Ian Good

#include "IGRanges/Count.h"
#include "IGRangesInternal.h"
#include "Misc/AutomationTest.h"

#include "view/empty.hpp"
#include "view/transform.hpp"

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FIGRangesCountSpec, "IG.Ranges.Count", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FIGRangesCountSpec::Define()
{
	using namespace IG::Ranges;

	static constexpr int32 SomeValues[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	static constexpr int32 NumSomeValues = UE_ARRAY_COUNT(SomeValues);

	It("empty", [this]() {
		const int32 ActualCount = ranges::empty_view<int32>() | Count();
		TestEqual("count", ActualCount, 0);
	});

	It("many", [this]() {
		const int32 ActualCount = SomeValues | Count();
		TestEqual("count", ActualCount, NumSomeValues);
	});

	It("many_transformed", [this]() {
		const auto Square = [](auto&& x) {
			return x * x;
		};

		const int32 ActualCount = SomeValues | ranges::views::transform(Square) | Count();
		TestEqual("count", ActualCount, NumSomeValues);
	});

	It("many_filtered", [this]() {
		const auto IsEven = [](auto&& x) {
			return x % 2 == 0;
		};

		int32 ExpectedCount = 0;
		for (auto&& X : SomeValues)
		{
			if (IsEven(X))
			{
				++ExpectedCount;
			}
		}

		int32 ActualCount = SomeValues | ranges::views::filter(IsEven) | Count();
		TestEqual("count", ActualCount, ExpectedCount);

		ActualCount = SomeValues | Count(IsEven);
		TestEqual("count", ActualCount, ExpectedCount);
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
