

//#include <xalanc/Include/PlatformDefinitions.hpp>

#include "XalanDateTime.hpp"


void dateTimeFromStringTest()
{
	char * invalidRepresentations[] =
	{
		// Empty string
		"",

		// Strings completely out of format.
		"GSoC 2012",
		"This function is well tested.",

		// Representations where the fields are not numbers
		"yyyy",
		"yyyy-mm",
		"yyyy-mm-dd",
		"yyyy-mm-ddThh:mm:ss.sss",

		// Fields with wrong sizes
		"-1",
		"0001-2",
		"-0001-002",
		"-0001-02-1",
		"0001-02-001",
		"-0001-02-01T1:1:1",
		"-0001-02-01T001:001:001",
		"0001-02-01T10:1:1",
		"-0001-02-01T10:001:001",
		"-0001-02-01T10:10:1",
		"0001-02-01T10:10:001",

		// Values exceeding the upper bounds
		"2012-13",
		"-2012-12-32",
		"2012-12-32T24:00:00",
		"2012-12-32T23:99:00",
		"-2012-12-32T23:59:60",

		// Wrong Separators
		"-2012*12*12",
		"2012-12-12P12:12:12",
		"-2012-12-12T12-12-12",

		// Wrong floating-point representation of seconds
		"0001-12-01T12:32:2.123",
		"0001-12-01T12:32:2.",

		// Days that do not exist
		"1970-02-29",
		"1970-04-31",
		"1970-06-31",
		"1970-09-31",
		"1970-11-31",
		"-0002-02-29",
		"-0002-04-31",
		"-0002-06-31",
		"-0002-09-31",
		"-0002-11-31",

		// Year, month and day fields with zero value
		"0000",
		"0001-00",
		"0001-01-00",

		// Incomplete time representation
		"-1234-12-12T14",
		"-1234-12-12T14:10",

		// Floating-point in integers fields
		"0001.5",
		"2000-11.2",
		"2000-11-10.7",
		"2000-11-10T12.2:12:12",
		"2000-11-10T12:1.2:12",

		// Representation does not ends with '\0'
		"2000'GSoC - Google Summer of Code'",
		"2000-01-01Z'GSoC - Google Summer of Code'",

		// Date/time separator missing
		"2000-11-1012:12:12",

		// Timezone: Strings completely out of format.
		"-1234-12-12T14:10:10GSoC 2012",

		// Timezone: Representations where the fields are not numbers
		"2012-09-08T12:00:00+hh:ss",

		// Timezone: Fields with wrong sizes
		"2012-09-08T12:00:00+1:20",
		"2012-09-08T12:00:00-10:1",

		// Timezone: Values exceeding the upper bounds
		"-0001-01-01T01:01:01+15:00",
		"-0001-01-01T01:01:01-13:60",

		// Timezone: Wrong Separator
		"0001-01-01T01:01:01+10-00",

		// Timezone: UTC wrong representation
		"0001-01-01T01:01:01z",

		// When timezone has hours 14, minutes must be 0
		"0001-01-01T01:01:01+14:01",
		"0001-01-01T01:01:01-14:01",

		// Timezone: incomplete representation
		"2000+14",
		"2000-12-12T00:00:00-14",

		// Timezone: floating-point representing hours and minutes
		"-1234+12.2:32",
		"-0003+12:32.5"
	};

	for (int iErr = 0; iErr < sizeof(invalidRepresentations)/sizeof(*invalidRepresentations); iErr++)
	{
		try
		{
			XalanDateTime::fromString(invalidRepresentations[iErr]);

			CPPUNIT_FAIL("An Exception was expected!");
		}
		catch (InvalidFormat& iFormat)
		{
		}
	}

	char * validRepresentations[] =
	{
		// xs:gYear representation
		"0001",
		"0001Z",
		"0001+14:00",
		"0001-14:00",
		"-0001",
		"-0001Z",
		"-0001+14:00",
		"-0001-14:00",

		// xs:gYearMonth representation
		"0001-12",
		"0001-12Z",
		"0001-12+14:00",
		"0001-12-14:00",
		"-0001-12",
		"-0001-12Z",
		"-0001-12+14:00",
		"-0001-12-14:00",

		// xs:date representation
		"0001-12-31",
		"0001-12-31Z",
		"0001-12-31+14:00",
		"0001-12-31-14:00",
		"-0001-12-31",
		"-0001-12-31Z",
		"-0001-12-31+14:00",
		"-0001-12-31-14:00",

		// xs:dateTime representation
		"0001-12-31T00:00:00.1",
		"0001-12-31T00:00:00.1Z",
		"0001-12-31T00:00:00.1+14:00",
		"0001-12-31T00:00:00.1-14:00",
		"-0001-12-31T00:00:00.1",
		"-0001-12-31T00:00:00.1Z",
		"-0001-12-31T00:00:00.1+14:00",
		"-0001-12-31T00:00:00.1-14:00"
	};

	for (int iErr = 0; iErr < sizeof(validRepresentations)/sizeof(*validRepresentations); iErr++)
	{
		try
		{
			XalanDateTime::fromString(validRepresentations[iErr]);
		}
		catch (InvalidFormat& iFormat)
		{
			CPPUNIT_FAIL("An Exception was unexpected!");
		}
	}
}

void durationFromStringTest()
{
	char * invalidRepresentations[] =
	{
		// Empty string
		"",

		// Strings completely out of format.
		"GSoC 2012",
		"This function is well tested.",

		// Representations where the fields are not numbers
		"PnY",
		"PnM",
		"-PnD",
		"PTnH",
		"PTnM",
		"-PTnS",
		"PnYnMnDTnHnMnS",

		// Wrong Date/time separator or inexistent
		"PnYnMnDnHnMnS",
		"-PnYnMnDtnHnMnS",

		// Wrong floating-point representation of seconds
		"-P2YT2.S",
		"PT673M.212S",

		// Seconds' field identifier not present
		"PT1M3",
		"-P312Y93MT33M13.3",

		// All fields with zero value
		"P",
		"PT",

		// Floating-point in integers fields
		"P1.2Y",
		"P3.5M",
		"P17.9D",
		"PT23.1H",
		"PT12.31M",

		// Representation does not ends with '\0'
		"P2000Y'GSoC - Google Summer of Code'",
		"P1Y1M1DT1H1M1.1S'GSoC - Google Summer of Code'",

		// Date/time separator missing
		"P1D3H",
		"P124Y187M541D781H31M13S",

		"-P10Y112M13DT12H13M11",
	};

	for (int iErr = 0; iErr < sizeof(invalidRepresentations)/sizeof(*invalidRepresentations); iErr++)
	{
		try
		{
			XalanDuration::fromString(invalidRepresentations[iErr]);

			CPPUNIT_FAIL("An Exception was expected!");
		}
		catch (InvalidFormat& iFormat)
		{
		}
	}

	char * validRepresentations[] =
	{
		// Duration of Year 
		"P1Y",
		"-P1Y",
		"P2012Y",
		"-P2012Y",

		// Duration of Month
		"P1M",
		"-P1M",
		"P13M",
		"-P13M",

		// Duration of Day
		"P31D",
		"-P31D",
		"P365D",
		"-P365D",

		// Duration of Hours
		"PT24H",
		"-PT24H",
		"PT480H",
		"-PT480H",

		// Duration of Minutes
		"PT59M",
		"-PT59M",
		"PT1440M",
		"-PT1440M",

		// Duration of Seconds
		"PT1S",
		"-PT1S",
		"PT86400S",
		"-PT86400S",

		// Mixed Durations
		"P1Y9M",
		"P1Y90D",
		"P1YT4H",
		"P1YT40M",
		"P1YT80S",
		"P83M43D",
		"P83MT41H",
		"P83MT12M",
		"P83MT12.346S",
		"P46DT32H",
		"P46DT64M",
		"P46DT32.765S",
		"PT534H123M",
		"PT534H1.3S",
		"PT512M0.321S",
		"P5MT5M0.5S"
	};

	for (int iErr = 0; iErr < sizeof(validRepresentations)/sizeof(*validRepresentations); iErr++)
	{
		try
		{
			XalanDuration::fromString(validRepresentations[iErr]);
		}
		catch (InvalidFormat& iFormat)
		{
			CPPUNIT_FAIL("An Exception was unexpected!");
		}
	}
}
