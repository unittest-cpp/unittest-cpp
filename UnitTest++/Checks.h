#ifndef UNITTEST_CHECKS_H
#define UNITTEST_CHECKS_H

#include "Config.h"
#include "TestResults.h"
#include "MemoryOutStream.h"

#ifdef UNITTEST_CPP11
#include <type_traits>
#endif

namespace UnitTest {


template< typename Value >
bool Check(Value const value)
{
    return !!value; // doing double negative to avoid silly VS warnings
}


#ifdef UNITTEST_CPP11

// C++11 strongly typed enum overload
template< typename Expected, typename Actual >
typename std::enable_if<std::is_enum<Expected>::value && std::is_enum<Actual>::value>::type
CheckEqual(TestResults& results, Expected const& expected, Actual const& actual, TestDetails const& details)
{
    if (!(expected == actual))
    {
        UnitTest::MemoryOutStream stream;
        stream << "Expected " << static_cast<int>(expected)
               << " but was " << static_cast<int>(actual);

        results.OnTestFailure(details, stream.GetText());
    }
}


// Non-strongly type enum overload
template< typename Expected, typename Actual >
typename std::enable_if<!std::is_enum<Expected>::value || !std::is_enum<Actual>::value>::type
CheckEqual(TestResults& results, Expected const& expected, Actual const& actual, TestDetails const& details)
{
    if (!(expected == actual))
    {
        UnitTest::MemoryOutStream stream;
        stream << "Expected " << expected << " but was " << actual;

        results.OnTestFailure(details, stream.GetText());
    }
}

#else

template< typename Expected, typename Actual >
void CheckEqual(TestResults& results, Expected const& expected, Actual const& actual, TestDetails const& details)
{
    if (!(expected == actual))
    {
        UnitTest::MemoryOutStream stream;
        stream << "Expected " << expected << " but was " << actual;

        results.OnTestFailure(details, stream.GetText());
    }
}

#endif


UNITTEST_LINKAGE void CheckEqual(TestResults& results, char const* expected, char const* actual, TestDetails const& details);

UNITTEST_LINKAGE void CheckEqual(TestResults& results, char* expected, char* actual, TestDetails const& details);

UNITTEST_LINKAGE void CheckEqual(TestResults& results, char* expected, char const* actual, TestDetails const& details);

UNITTEST_LINKAGE void CheckEqual(TestResults& results, char const* expected, char* actual, TestDetails const& details);

template< typename Expected, typename Actual, typename Tolerance >
bool AreClose(Expected const& expected, Actual const& actual, Tolerance const& tolerance)
{
    return (actual >= (expected - tolerance)) && (actual <= (expected + tolerance));
}

template< typename Expected, typename Actual, typename Tolerance >
void CheckClose(TestResults& results, Expected const& expected, Actual const& actual, Tolerance const& tolerance,
                TestDetails const& details)
{
    if (!AreClose(expected, actual, tolerance))
    { 
        UnitTest::MemoryOutStream stream;
        stream << "Expected " << expected << " +/- " << tolerance << " but was " << actual;

        results.OnTestFailure(details, stream.GetText());
    }
}

#ifdef UNITTEST_CPP11

// C++11 strongly typed enum overload
template< typename Expected, typename Actual >
typename std::enable_if<std::is_enum<typename std::remove_all_extents<Expected>::type>::value &&
                        std::is_enum<typename std::remove_all_extents<Actual>::type>::value>::type
CheckArrayEqual(TestResults& results, Expected const& expected, Actual const& actual,
                int const count, TestDetails const& details)
{
    bool equal = true;
    for (int i = 0; i < count; ++i)
        equal &= (expected[i] == actual[i]);

    if (!equal)
    {
        UnitTest::MemoryOutStream stream;

        stream << "Expected [ ";

        for (int expectedIndex = 0; expectedIndex < count; ++expectedIndex)
            stream << static_cast<int>(expected[expectedIndex]) << " ";

        stream << "] but was [ ";

        for (int actualIndex = 0; actualIndex < count; ++actualIndex)
            stream << static_cast<int>(actual[actualIndex]) << " ";

        stream << "]";

        results.OnTestFailure(details, stream.GetText());
    }
}


// Non-strongly type enum overload
template< typename Expected, typename Actual >
typename std::enable_if<!std::is_enum<typename std::remove_all_extents<Expected>::type>::value ||
                        !std::is_enum<typename std::remove_all_extents<Actual>::type>::value>::type
CheckArrayEqual(TestResults& results, Expected const& expected, Actual const& actual,
                int const count, TestDetails const& details)
{
    bool equal = true;
    for (int i = 0; i < count; ++i)
        equal &= (expected[i] == actual[i]);

    if (!equal)
    {
        UnitTest::MemoryOutStream stream;

        stream << "Expected [ ";

        for (int expectedIndex = 0; expectedIndex < count; ++expectedIndex)
            stream << expected[expectedIndex] << " ";

        stream << "] but was [ ";

        for (int actualIndex = 0; actualIndex < count; ++actualIndex)
            stream << actual[actualIndex] << " ";

        stream << "]";

        results.OnTestFailure(details, stream.GetText());
    }
}



#else

template< typename Expected, typename Actual >
void CheckArrayEqual(TestResults& results, Expected const& expected, Actual const& actual,
                int const count, TestDetails const& details)
{
    bool equal = true;
    for (int i = 0; i < count; ++i)
        equal &= (expected[i] == actual[i]);

    if (!equal)
    {
        UnitTest::MemoryOutStream stream;

		stream << "Expected [ ";

		for (int expectedIndex = 0; expectedIndex < count; ++expectedIndex)
            stream << expected[expectedIndex] << " ";

		stream << "] but was [ ";

		for (int actualIndex = 0; actualIndex < count; ++actualIndex)
            stream << actual[actualIndex] << " ";

		stream << "]";

        results.OnTestFailure(details, stream.GetText());
    }
}

#endif

template< typename Expected, typename Actual, typename Tolerance >
bool ArrayAreClose(Expected const& expected, Actual const& actual, int const count, Tolerance const& tolerance)
{
    bool equal = true;
    for (int i = 0; i < count; ++i)
        equal &= AreClose(expected[i], actual[i], tolerance);
    return equal;
}

template< typename Expected, typename Actual, typename Tolerance >
void CheckArrayClose(TestResults& results, Expected const& expected, Actual const& actual,
                   int const count, Tolerance const& tolerance, TestDetails const& details)
{
    bool equal = ArrayAreClose(expected, actual, count, tolerance);

    if (!equal)
    {
        UnitTest::MemoryOutStream stream;

        stream << "Expected [ ";
        for (int expectedIndex = 0; expectedIndex < count; ++expectedIndex)
            stream << expected[expectedIndex] << " ";
        stream << "] +/- " << tolerance << " but was [ ";

		for (int actualIndex = 0; actualIndex < count; ++actualIndex)
            stream << actual[actualIndex] << " ";
        stream << "]";

        results.OnTestFailure(details, stream.GetText());
    }
}

template< typename Expected, typename Actual, typename Tolerance >
void CheckArray2DClose(TestResults& results, Expected const& expected, Actual const& actual,
                   int const rows, int const columns, Tolerance const& tolerance, TestDetails const& details)
{
    bool equal = true;
    for (int i = 0; i < rows; ++i)
        equal &= ArrayAreClose(expected[i], actual[i], columns, tolerance);

    if (!equal)
    {
        UnitTest::MemoryOutStream stream;

        stream << "Expected [ ";    

		for (int expectedRow = 0; expectedRow < rows; ++expectedRow)
        {
            stream << "[ ";
            for (int expectedColumn = 0; expectedColumn < columns; ++expectedColumn)
                stream << expected[expectedRow][expectedColumn] << " ";
            stream << "] ";
        }

		stream << "] +/- " << tolerance << " but was [ ";

		for (int actualRow = 0; actualRow < rows; ++actualRow)
        {
            stream << "[ ";
            for (int actualColumn = 0; actualColumn < columns; ++actualColumn)
                stream << actual[actualRow][actualColumn] << " ";
            stream << "] ";
        }

		stream << "]";

        results.OnTestFailure(details, stream.GetText());
    }
}

}

#endif
