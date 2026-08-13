// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Curves/CurveFloat.h"
#include "Engine/DataTable.h"
#include "Misc/AutomationTest.h"
#include "RemCastFn.h"
#include "RemMisc.h"
#include "RemRangesMacro.h"
#include "RemRangesStatics.inl"
#include "transrangers.hpp"

#if WITH_DEV_AUTOMATION_TESTS

namespace
{
TArray<int32> MakeNumbers(const int32 Count)
{
    TArray<int32> Numbers;
    Numbers.Reserve(Count);
    for (int32 Index = 0; Index < Count; ++Index)
    {
        Numbers.Add(Index);
    }
    return Numbers;
}

// ArrayView yields reference elements; the materializing wrappers (ToArray
// single-arg, TakeN, NthElement, FirstElement) require value elements, so
// identity-transform to values first.
auto AsValue = [](const int32 Value)
{
    return Value;
};
}

DEFINE_SPEC(FRemRangesTest, "Rem.Ranges",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemRangesTest::Define()
{
    Describe(TEXT("ForEach"), [this]
    {
        It(TEXT("should visit every element"), [this]
        {
            const auto Numbers = MakeNumbers(5);
            int32 Sum{};

            // ForEach always exhausts the range; side effects go through transform
            Rem::Ranges::ForEach(
                transrangers::transform(
                    [&Sum](const int32 Value) -> bool
                    {
                        Sum += Value;
                        return true;
                    },
                    Rem::Ranges::ConstArrayView(Numbers)));

            TestEqual(TEXT("sum"), Sum, 10);
        });

        It(TEXT("should support a filter-transform side-effect pipeline"), [this]
        {
            const auto Numbers = MakeNumbers(10);
            int32 SumOfEvens{};

            Rem::Ranges::ForEach(
                transrangers::transform(
                    [&SumOfEvens](const int32 Value) -> bool
                    {
                        SumOfEvens += Value;
                        return true;
                    },
                    transrangers::filter(
                        [](const int32 Value)
                        {
                            return Value % 2 == 0;
                        },
                        Rem::Ranges::ConstArrayView(Numbers))));

            TestEqual(TEXT("sum of evens"), SumOfEvens, 20);
        });
    });

    Describe(TEXT("ToArray"), [this]
    {
        It(TEXT("should materialize a filter-transform pipeline"), [this]
        {
            const auto Numbers = MakeNumbers(10);

            const auto Result = Rem::Ranges::ToArray(
                transrangers::transform(
                    [](const int32 Value)
                    {
                        return Value * 10;
                    },
                    transrangers::filter(
                        [](const int32 Value)
                        {
                            return Value % 2 == 0;
                        },
                        Rem::Ranges::ConstArrayView(Numbers))));

            TestEqual(TEXT("count"), Result.Num(), 5);
            TestEqual(TEXT("first"), Result[0], 0);
            TestEqual(TEXT("second"), Result[1], 20);
            TestEqual(TEXT("last"), Result[4], 80);
        });

        It(TEXT("the two-arg overload should append into an existing array"), [this]
        {
            const auto Numbers = MakeNumbers(5);
            TArray<int32> Result{99};

            Rem::Ranges::ToArray(Result, Rem::Ranges::ConstArrayView(Numbers));

            TestEqual(TEXT("count"), Result.Num(), 6);
            TestEqual(TEXT("existing value kept"), Result[0], 99);
            TestEqual(TEXT("appended first"), Result[1], 0);
        });

        It(TEXT("should flatten nested arrays with ranger_join and reference elements"), [this]
        {
            TArray<TArray<int32>> Outer;
            Outer.Add({1, 2});
            Outer.Add({3, 4, 5});

            // ranger_join yields reference elements: the two-arg overload is required
            TArray<int32> Flat;
            Rem::Ranges::ToArray(Flat, transrangers::ranger_join(Rem::Ranges::ArrayView(Outer)));

            TestEqual(TEXT("count"), Flat.Num(), 5);
            TestEqual(TEXT("first"), Flat[0], 1);
            TestEqual(TEXT("last"), Flat[4], 5);
        });
    });

    Describe(TEXT("TakeN"), [this]
    {
        It(TEXT("should take exactly N elements"), [this]
        {
            const auto Numbers = MakeNumbers(10);

            const auto Result = Rem::Ranges::TakeN<3>(transrangers::transform(AsValue,
                Rem::Ranges::ConstArrayView(Numbers)));

            TestEqual(TEXT("count"), Result.Num(), 3);
            TestEqual(TEXT("first"), Result[0], 0);
            TestEqual(TEXT("last"), Result[2], 2);
        });

        It(TEXT("should return fewer elements when the range is shorter"), [this]
        {
            const auto Numbers = MakeNumbers(3);

            const auto Result = Rem::Ranges::TakeN<5>(transrangers::transform(AsValue,
                Rem::Ranges::ConstArrayView(Numbers)));

            TestEqual(TEXT("count"), Result.Num(), 3);
        });
    });

    Describe(TEXT("NthElement and FirstElement"), [this]
    {
        It(TEXT("should return the Nth element"), [this]
        {
            const auto Numbers = MakeNumbers(10);

            TestEqual(TEXT("first"),
                Rem::Ranges::NthElement<0>(transrangers::transform(AsValue, Rem::Ranges::ConstArrayView(Numbers))),
                0);
            TestEqual(TEXT("third"),
                Rem::Ranges::NthElement<2>(transrangers::transform(AsValue, Rem::Ranges::ConstArrayView(Numbers))),
                2);
            TestEqual(TEXT("last"),
                Rem::Ranges::NthElement<9>(transrangers::transform(AsValue, Rem::Ranges::ConstArrayView(Numbers))),
                9);
        });

        It(TEXT("should return the last element when out of range"), [this]
        {
            const auto Numbers = MakeNumbers(3);

            // NthElement takes Ordinal + 1 (or fewer) elements and keeps the last
            // consumed value: an out-of-range ordinal yields the final element
            TestEqual(TEXT("last element"), Rem::Ranges::NthElement<99>(transrangers::transform(AsValue,
                Rem::Ranges::ConstArrayView(Numbers))), 2);
        });

        It(TEXT("FirstElement should stop iterating at the first match (early exit)"), [this]
        {
            const auto Numbers = MakeNumbers(100);
            int32 Visited{};

            const auto Result = Rem::Ranges::FirstElement(
                transrangers::filter(
                    [](const int32 Value)
                    {
                        return Value > 5;
                    },
                    transrangers::transform(
                        [&Visited](const int32 Value)
                        {
                            ++Visited;
                            return Value;
                        },
                        Rem::Ranges::ConstArrayView(Numbers))));

            TestEqual(TEXT("first match"), Result, 6);
            // the source yields one extra element before the take<1> stop propagates
            TestEqual(TEXT("visited up to the first match"), Visited, 8);
        });

        It(TEXT("FirstElement should return a default value when nothing matches"), [this]
        {
            const auto Numbers = MakeNumbers(5);

            const auto Result = Rem::Ranges::FirstElement(
                transrangers::filter(
                    [](const int32 Value)
                    {
                        return Value > 100;
                    },
                    transrangers::transform(AsValue, Rem::Ranges::ConstArrayView(Numbers))));

            TestEqual(TEXT("default"), Result, 0);
        });
    });

    Describe(TEXT("ArrayView"), [this]
    {
        It(TEXT("should expose mutable elements"), [this]
        {
            auto Numbers = MakeNumbers(3);

            Rem::Ranges::ForEach(
                transrangers::transform(
                    [](int32& Value) -> bool
                    {
                        Value *= 2;
                        return true;
                    },
                    Rem::Ranges::ArrayView(Numbers)));

            TestEqual(TEXT("doubled"), Numbers[0], 0);
            TestEqual(TEXT("doubled"), Numbers[1], 2);
            TestEqual(TEXT("doubled"), Numbers[2], 4);
        });
    });

    Describe(TEXT("ConstArrayView"), [this]
    {
        It(TEXT("should expose const elements without copies"), [this]
        {
            auto Numbers = MakeNumbers(3);

            int32 Sum = 0;
            Rem::Ranges::ForEach(
                transrangers::transform(
                    [&Sum](const int32& Value) -> bool
                    {
                        Sum += Value;
                        return true;
                    },
                    Rem::Ranges::ConstArrayView(Numbers)));

            TestEqual(TEXT("sum"), Sum, 3);
        });

        It(TEXT("should materialize a copy of the const view"), [this]
        {
            auto Numbers = MakeNumbers(4);

            const auto Materialized = Rem::Ranges::ToArray(
                Rem::Ranges::ConstArrayView(Numbers));

            TestEqual(TEXT("count"), Materialized.Num(), 4);
            TestEqual(TEXT("values preserved"), Materialized[3], 3);
        });
    });

    Describe(TEXT("Compositions"), [this]
    {
        It(TEXT("concat should join rangers end to end"), [this]
        {
            const auto NumbersA = MakeNumbers(2);
            const auto NumbersB = MakeNumbers(2);

            TArray<int32> Result;
            Rem::Ranges::ToArray(Result, transrangers::concat(
                Rem::Ranges::ConstArrayView(NumbersA), Rem::Ranges::ConstArrayView(NumbersB)));

            TestEqual(TEXT("count"), Result.Num(), 4);
            TestEqual(TEXT("second"), Result[1], 1);
            TestEqual(TEXT("third"), Result[2], 0);
        });

        It(TEXT("zip should pair elements from parallel arrays"), [this]
        {
            const TArray<int32> Left{1, 2, 3};
            const TArray<int32> Right{10, 20, 30};

            const auto Sums = Rem::Ranges::ToArray(
                transrangers::transform(
                    [](const std::tuple<const int32&, const int32&>& Pair)
                    {
                        return std::get<0>(Pair) + std::get<1>(Pair);
                    },
                    transrangers::zip(transrangers::all(Left), transrangers::all(Right))));

            TestEqual(TEXT("count"), Sums.Num(), 3);
            TestEqual(TEXT("first"), Sums[0], 11);
            TestEqual(TEXT("second"), Sums[1], 22);
            TestEqual(TEXT("third"), Sums[2], 33);
        });

        It(TEXT("CastTo should filter a mixed object array by type"), [this]
        {
            TArray<UObject*> Objects;
            Objects.Add(NewObject<UDataTable>());
            Objects.Add(NewObject<UCurveFloat>());
            Objects.Add(NewObject<UDataTable>());

            const auto Tables = Rem::Ranges::ToArray(
                transrangers::filter(Rem::Fn::IsValid,
                    transrangers::transform(Rem::Cast<UDataTable>,
                        Rem::Ranges::ConstArrayView(Objects))));

            TestEqual(TEXT("count"), Tables.Num(), 2);
            TestTrue(TEXT("first is a data table"), Tables[0]->IsA<UDataTable>());
            TestTrue(TEXT("second is a data table"), Tables[1]->IsA<UDataTable>());
        });
    });

    Describe(TEXT("REM_RANGES_FOREACH"), [this]
    {
        It(TEXT("should run the pipeline with the C1060 workaround macro"), [this]
        {
            const auto Numbers = MakeNumbers(5);
            int32 Visited{};

            // the macro takes a single argument: build the pipeline first
            auto Pipeline = transrangers::transform(
                [&Visited](const int32 Value)
                {
                    ++Visited;
                    return Value;
                },
                Rem::Ranges::ConstArrayView(Numbers));
            REM_RANGES_FOREACH(Pipeline);

            TestEqual(TEXT("visited"), Visited, 5);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
