#include "XalanDateTimeTestSuite.hpp"

XALAN_CPP_NAMESPACE_USE

XalanDateTimeTestSuite::XalanDateTimeTestSuite()
{
}

void XalanDateTimeTestSuite::notifyFailure(const char * errMsg)
{
    printf("\n    ERROR: %s", errMsg);

    printf("\n\n");
    system("PAUSE");
    printf("\n");
}

void XalanDateTimeTestSuite::assertNumberEquals(int one, int other)
{
    if (one != other)
    {
        char * errMsg = new char[100];
        sprintf(errMsg, "Number '%d' should be equals to '%d'", one, other);
        
        notifyFailure(errMsg);
    }
}

void XalanDateTimeTestSuite::assertStringEquals(const char * one, const char * other)
{
    if (strcmp(one, other))
    {
        char * errMsg = new char[100];
        sprintf(errMsg, "String '%s' should be equals to '%s'", one, other);
        
        notifyFailure(errMsg);
    }
}

void XalanDateTimeTestSuite::assertInvalidDateTimes(
    char * invalidRepresentations[], int nRepresentations,
    XalanDateTimeFormat format)
{
    for (int iElement = 0; iElement < nRepresentations; iElement++)
    {
        try
        {
            XalanDateTime::fromString(invalidRepresentations[iElement++], format);
            
            notifyFailure("An Exception was expected!");
        }
        catch (XalanInvalidCalendar& iFormat)
        {
        }
    }

    printf("\n    %d invalid representations tested", nRepresentations);
}

void XalanDateTimeTestSuite::assertValidDateTimes(char * validRepresentations[],
    int nRepresentations, XalanDateTimeFormat format)
{
    for (int iElement = 0; iElement < nRepresentations; iElement++)
    {
        try
        {
            XalanDateTime * dateTime = XalanDateTime::fromString(
                validRepresentations[iElement], format);

            if (format != dateTime->getFormatType())
            {
                throw XalanInvalidCalendar("");
            }

            assertStringEquals(validRepresentations[iElement],
                dateTime->toString());
        }
        catch (XalanInvalidCalendar& iFormat)
        {
            notifyFailure("An Exception was unexpected!");
        }
    }

    printf("\n    %d valid representations tested", nRepresentations);
}

void XalanDateTimeTestSuite::assertInvalidDurations(
    char * invalidRepresentations[], int nRepresentations)
{
    for (int iElement = 0; iElement < nRepresentations; iElement++)
    {
        try
        {
            XalanDuration::fromString(invalidRepresentations[iElement++]);
            
            notifyFailure("An Exception was expected!");
        }
        catch (XalanInvalidCalendar& iFormat)
        {
        }
    }

    printf("\n    %d invalid representations tested", nRepresentations);
}

void XalanDateTimeTestSuite::assertValidDurations(char * validRepresentations[],
    int nRepresentations)
{
    for (int iElement = 0; iElement < nRepresentations; iElement++)
    {
        try
        {
            assertStringEquals(validRepresentations[iElement],
                 XalanDuration::fromString(validRepresentations[iElement])->toString());
        }
        catch (XalanInvalidCalendar& iFormat)
        {
            notifyFailure("An Exception was unexpected!");
        }
    }

    printf("\n    %d valid representations tested", nRepresentations);
}

void XalanDateTimeTestSuite::dtTimeFormatFromToStringTest()
{
    printf("\n---------------------------------------------------------------");
    printf("\ndtTimeFormatFromStringTest()");

    const int nInvalidTimes = 48;
    char * invalidTimes[nInvalidTimes] = 
    {
        /* The lexical representation for time is the left truncated lexical 
           representation for dateTime: hh:mm:ss.sss with optional following 
           time zone indicator. For example, to indicate 1:20 pm for Eastern 
           Standard Time which is 5 hours behind Coordinated Universal Time 
           (UTC), one would write: 13:20:00-05:00.                            */
    
        // Empty string
        "",						" ",					"  ",

        // Starting or ending with invalid characters
        "?12:00:19",			"12:00:19?",			" 19:00:43",
        "19:00:43 ",

        // Any field not represented
        ":59:59.999",			"23::59.999",			"23:59:",

        // Representations where the fields are not numbers
        "HH:00:00",				"00:MM:00",				"00:00:SS",

        // Fields with wrong sizes
        "001:00:59",			"20:023:00",			"13:34:002",

        // Values exceding the upper bounds
        "23:60:00.000",			"23:59:60.000",

        // Wrong Separators
        "01^00^59",				"20?23?00",

        // Wrong floating-point representation of seconds
        "02:28:30.",			"12:43:.218",

        // Incomplete time representation
        "22:",					"14:10:",				"02:12",

        // Floating-point in integers fields
        "02.5:52:20",			"12:46.5:25",

        // Representation does not end with '\0'
        "10:10:42GSoC",			"24:00:00 ",

        // Timezone: Representations where the fields are not numbers
        "02:01:53+HH:MM",		"02:01:53-HH:MM",

        // Timezone: Fields with wrong sizes
        "17:12:43.213-001:10",	"23:53:21.000+01:010",

        // Timezone: Values exceeding the upper bounds
        "16:21:53+24:24",		"16:21:53-24:24",		"00:00:00+10:60",
        "00:00:00-10:60",

        // Timezone: Wrong Separator
        "22:50:21+01^00",		"22:50:21-01^10",

        // Timezone: UTC wrong representation
        "22:02:52z",

        // Timezone: When hours are 14, minutes must be 0
        "12:52:36.360+14:01",	"12:52:36.360-14:01",

        // Timezone: Incomplete representation
        "21:32:39+12",			"21:32:39-12",

        // Timezone: Floating-point representing hours and minutes
        "24:00:00+12.4:10",		"13:02:14-14:00.0",

        // Timezone: Representation does not end with '\0'
        "09:09:09:009Zzz"
    };

    const int nValidTimes = 9;
    char * validTimes[nValidTimes] =
    {
        "00:00:00",				"24:00:00",				"01:01:01.1",
        "02:02:02.22",			"03:03:03.333",			"23:23:23",
        "23:00:00",			    "23:59:00",	    		"23:59:59.999"
    };

    assertValidDateTimes(validTimes, nValidTimes, XalanDateTimeFormat::TIME);
    assertInvalidDateTimes(invalidTimes, nInvalidTimes, XalanDateTimeFormat::TIME);
}

void XalanDateTimeTestSuite::dtDateFormatFromToStringTest()
{
    printf("\n---------------------------------------------------------------");
    printf("\ndtDateFormatFromStringTest()");

    const int nInvalidDates = 90;
    char * invalidDates[nInvalidDates] =
    {
        /* For the following discussion, let the 'date portion' of a dateTime or
           date object be an object similar to a dateTime or date object, with
           similar year, month, and day properties, but no others, having the
           same value for these properties as the original dateTime or date
           object. The lexical space of date consists of finite-length sequences
           of characters of the form: '-'? yyyy '-' mm '-' dd zzzzzz? where the
           date and optional timezone are represented exactly the same way as
           they are for dateTime. The first moment of the interval is that
           represented by: '-' yyyy '-' mm '-' dd 'T00:00:00' zzzzzz? and the
           least upper bound of the interval is the timeline point represented
           (noncanonically) by: '-' yyyy '-' mm '-' dd 'T24:00:00' zzzzzz?.   */

        // Empty string
        "",						" ",					"  ",

        // Starting or ending with invalid characters
        "?2012-09",				"2012-09?",				" 9999-04",
        "9999-04 ",

        // Any field not represented
        "-05-30",				"2012--24",				"-2012--24",
        "2012-10-",				"-2012-10-",

        // Representations where the fields are not numbers
        "CCYY-02-29",			"-CCYY-02-29",			"2012-MM-29",
        "-2013-MM-29",			"7280-12-DD",			"-5240-12-DD",

        // Fields with wrong sizes
        "333-12-01",			"-333-12-01",			"22-12-21",
        "-22-12-21",			"1-12-11",				"-1-12-11",
        "9999-1-15",			"-9999-1-15",			"0007-001-26",
        "-0007-001-26",			"3287-10-5",			"-4212-11-1",
        "4933-01-006",			"-9632-01-005",

        // Values exceding the upper bounds
        "54893-13-01",			"-1342-13-15",			"2013-12-32",
        "0001-03-32",

        // 5-digit or more representations do not accept zeros at left
        "00001-10-23",			"-00001-10-12",			"09999-08-17",
        "-09999-08-28",			"099999-11-29",			"-099999-11-16",

        // Wrong Separators
        "3287/10/5",			"-0007:001:26",

        // Days that do not exist
        "1970-02-29",					"1970-04-31",
        "1970-06-31",					"1970-09-31",
        "1970-11-31",					"-0002-02-29",
        "-0002-04-31",					"-0002-06-31",
        "-0002-09-31",					"-0002-11-31",

        // Year, month and day fields with zero value
        "0000-07-12",			"-0000-07-13",			"250000-00-02",
        "-250000-00-07",		"6842-09-00",			"-5421-07-00",

        // Floating-point in integers fields
        "0002.34-09-28",		"-0002.34-09-28",		"250000-09.2-28",
        "-250000-09.2-28",		"8123-12-30.5",			"8123-12-30.5",

        // Representation does not end with '\0'
        "250000-01-07GSoC",		"250000-01-07 ",	
        
        // Timezone: Representations where the fields are not numbers
        "2213-10-12-HH:MM",		"-2213-10-10+HH:MM",

        // Timezone: Fields with wrong sizes
        "4331-05-04+001:10",	"-4331-05-05-001:10",	"4331-08-08+01:010",
        "-4331-08-09-01:010",

        // Timezone: Values exceeding the upper bounds
        "6537-11-29+24:24",		"-6537-11-18+24:24",	"6537-01-07+10:60",
        "-6537-01-28+10:60",

        // Timezone: Wrong Separator
        "1111-09-09+01^00",		"-1111-09-07-01^10",

        // Timezone: UTC wrong representation
        "9999-06-30z",			"-9999-06-30z",

        // Timezone: When hours are 14, minutes must be 0
        "2013-04-18-14:01",		"-2013-04-29-14:01",

        // Timezone: Incomplete representation
        "3323-12-05+12",		"-3323-12-18+12",

        // Timezone: Floating-point representing hours and minutes

        // Timezone: Representation does not end with '\0'
        "2013-04-01Zzz",		"-2013-04-04Z ",		"3149-10-30+10:00 "
    };

    const int nValidDates = 13;
    char * validDates[nValidDates] =
    {
        "0001-01-01",			"-0001-02-29",			"9999-12-31",
        "-9999-12-07",			"2012-12-12Z",			"-2012-12-04Z",
        "250250-02-28Z",		"-250250-02-28Z",		"2013-07-07+14:00",
        "-2013-07-28-14:00",	"8775-08-02+00:00",		"-8775-08-17-00:00",
        "201305-01-01+01:10"
    };

    assertValidDateTimes(validDates, nValidDates, XalanDateTimeFormat::DATE);
    assertInvalidDateTimes(invalidDates, nInvalidDates, XalanDateTimeFormat::DATE);
}

void XalanDateTimeTestSuite::dtGDayFormatFromToStringTest()
{
    printf("\n---------------------------------------------------------------");
    printf("\ndtGDayFormatFromStringTest()");

    const int nInvalidGDays = 44;
    char * invalidGDays[nInvalidGDays] =
    {
        /* The lexical representation for gDay is the left truncated lexical
           representation for date: ---DD . An optional following time zone
           qualifier is allowed as for date. No preceding sign is allowed.
           No other formats are allowed.									  */
        
        // Empty string
        "",						" ",					"  ",

        // Starting or ending with invalid characters
        "?---29",				"---21?",				" ---23",
        "---11 ",

        // Any field not represented
        "---",					"---Z",

        // Representations where the fields are not numbers
        "---DD",

        // Fields with wrong sizes
        "---1",					"---031",

        // Values exceding the upper bounds
        "---32",

        // Wrong Separators
        "///01",				"+++30",

        // Year, month and day fields with zero value
        "---00",				"---00Z",			"---00+14:00",

        // Floating-point in integers fields
        "---12.5",				"---02.3Z",			"---31.3-14:00",

        // Representation does not end with '\0'
        "---13?",				"---13 ",

        // Timezone: Representations where the fields are not numbers
        "---24+HH:MM",			"---13-HH:MM",

        // Timezone: Fields with wrong sizes
        "---27+001:10",			"---17-001:10",			"---09+01:010",
        "---19-01:010",

        // Timezone: Values exceeding the upper bounds
        "---16+24:24",			"---06-24:24",			"---05+10:60",
        "---31-10:60",

        // Timezone: Wrong Separator
        "---22+01^00",			"---17-01^10",

        // Timezone: UTC wrong representation
        "---12z",

        // Timezone: When hours are 14, minutes must be 0
        "---13+14:01",			"---08-14:20",

        // Timezone: Incomplete representation
        "---26+12",				"---03-12",

        // Timezone: Floating-point representing hours and minutes
        "---30-10.1:05",		"---16+13:02.1",

        // Timezone: Representation does not end with '\0'
        "---15?",				"---09Zzz"
    };

    const int nValidGDays = 31;
    char * validGDays[nValidGDays] =
    {
        "---01",				"---02",				"---03",
        "---04",				"---05",				"---06",
        "---07",				"---08",				"---09",
        "---10",				"---11",				"---12",
        "---13",				"---14",				"---15",
        "---16",				"---17",				"---18",
        "---19",				"---20",				"---21",
        "---22",				"---23",				"---24",
        "---25",				"---26",				"---27",
        "---28",				"---29",				"---30",
        "---31"
    };

    assertValidDateTimes(validGDays, nValidGDays, XalanDateTimeFormat::G_DAY);
    assertInvalidDateTimes(invalidGDays, nInvalidGDays, XalanDateTimeFormat::G_DAY);
}

void XalanDateTimeTestSuite::dtGYearFormatFromToStringTest()
{
    printf("\n---------------------------------------------------------------");
    printf("\ndtGYearFormatFromStringTest()");

    const int nInvalidGYears = 50;
    char * invalidGYears[nInvalidGYears] =
    {
        /* The lexical representation for gYear is the reduced (right truncated)
           lexical representation for dateTime: CCYY. No left truncation is
           allowed. An optional following time zone qualifier is allowed as for
           dateTime. To accommodate year values outside the range from 0001 to
           9999, additional   digits can be added to the left of this
           representation and a preceding - sign is allowed.				  */

        // Empty string
        " ",

        // Starting or ending with invalid characters
        "?2012",				"2012?",				" 9999",
        "9999 ",

        // Any field not represented
        "",						"-",

        // Representations where the fields are not numbers
        "CCYY",					"-CCYY",

        // Fields with wrong sizes
        "333",					"-333",					"22",
        "-22",					"1",					"-1",

        // 5-digit or more representations do not accept zeros at left
        "00001",				"-00001",				"09999",
        "-09999",				"010000",				"-010000",

        // Year field with zero value
        "0000",					"-0000",

        // Floating-point in integers fields
        "2012.3Z",				"-2012.2Z",				"2013.86+14:00",
        
        // Representation does not end with '\0'
        "31283 ",				"-094534GSoC",

        // Timezone: Representations where the fields are not numbers
        "2213-HH:MM",			"-2213-HH:MM",

        // Timezone: Fields with wrong sizes
        "4331+001:10",			"-4331-001:10",			"4331+01:010",
        "-4331-01:010",

        // Timezone: Values exceeding the upper bounds
        "6537+24:24",			"-6537+24:24",			"6537+10:60",
        "-6537+10:60",

        // Timezone: Wrong Separator
        "1111+01^00",			"-1111-01^10",
        
        // Timezone: UTC wrong representation
        "9999z",				"-9999z",

        // Timezone: When hours are 14, minutes must be 0
        "2013-14:06",			"-2013-14:01",

        // Timezone: Incomplete representation
        "3323+12",				"-3323-12",

        // Timezone: Floating-point representing hours and minutes
        "891341+10.4:00",		"-48582-12:13.2",

        // Timezone: Representation does not end with '\0'
        "2013Zzz",				"-2013Zzz"
    };

    const int nValidGYears = 12;
    char * validGYears[nValidGYears] =
    {
        "0001",					"-0001",				"9999",
        "-9999",				"20120505",				"-20120505",
        "2012Z",				"-2012Z",				"2013+14:00",
        "-2013-14:00",			"8775+00:00",			"-8775-00:00"
    };

    assertValidDateTimes(validGYears, nValidGYears, XalanDateTimeFormat::G_YEAR);
    assertInvalidDateTimes(invalidGYears, nInvalidGYears, XalanDateTimeFormat::G_YEAR);
}

void XalanDateTimeTestSuite::dtGMonthFormatFromToStringTest()
{
    printf("\n---------------------------------------------------------------");
    printf("\ndtGMonthFormatFromStringTest()");

    const int nInvalidGMonths = 40;
    char * invalidGMonths[nInvalidGMonths] =
    {
        /* The lexical representation for gMonth is the left and right truncated
           lexical representation for date: --MM. An optional following time
           zone qualifier is   allowed as for date. No preceding sign is
           allowed. No other formats are allowed.							  */

        // Empty string
        "",						" ",					"  ",

        // Starting or ending with invalid characters
        "?--12",				"--12?",				" --03",
        "--11 ",

        // Any field not represented
        "--",

        // Representations where the fields are not numbers
        "--MM",

        // Fields with wrong sizes
        "--1",					"--001",

        // Values exceding the upper bounds
        "--13",					"--31",

        // Wrong Separators
        "//12",

        // Month and day fields with zero value
        "--00",

        // Floating-point in integers fields
        "--02.4",

        // Representation does not end with '\0'
        "--02GSoC",				"--12 ",

        // Timezone: Representations where the fields are not numbers
        "--04+HH:MM",			"--04-HH:MM",

        // Timezone: Fields with wrong sizes
        "--07+001:10",			"--07-001:10",			"--09+01:010",
        "--09-01:010",

        // Timezone: Values exceeding the upper bounds
        "--06+24:24",			"--06-24:24",			"--05+10:60",
        "--05-10:60",

        // Timezone: Wrong Separator
        "--02+01^00",			"--02-01^10",

        // Timezone: UTC wrong representation
        "--10z",

        // Timezone: When hours are 14, minutes must be 0
        "--08+14:42",			"--08-14:12",

        // Timezone: Incomplete representation
        "--06+12",				"--06-12:",

        // Timezone: Floating-point representing hours and minutes
        "--11-11.2:32",			"--03+12:23.5",

        // Timezone: Representation does not end with '\0'
        "--05?",				"--09Zzz"
    };

    const int nValidGMonths = 12;
    char * validGMonths[nValidGMonths] =
    {
        "--01",					"--02",					"--03",
        "--04",					"--05",					"--06",
        "--07",					"--08",					"--09",
        "--10",					"--11",					"--12"
    };

    assertValidDateTimes(validGMonths, nValidGMonths, XalanDateTimeFormat::G_MONTH);
    assertInvalidDateTimes(invalidGMonths, nInvalidGMonths, XalanDateTimeFormat::G_MONTH);
}

void XalanDateTimeTestSuite::dtDateTimeFormatFromToStringTest()
{
    printf("\n---------------------------------------------------------------");
    printf("\ndtDateTimeFormatFromStringTest()");

    const int nInvalidDateTimes = 72;
    char * invalidDateTimes[nInvalidDateTimes] =
    {
        /* The lexical space of dateTime consists of finite-length sequences of
        characters of the form: '-'? yyyy '-' mm '-' dd 'T' hh ':' mm ':' ss
        ('.' s+)? (zzzzzz)?, where '-'? yyyy is a four-or-more digit optionally
        negative-signed numeral that represents the year; if more than four
        digits, leading zeros are prohibited, and '0000' is prohibited; also
        note that a plus sign is not permitted; the remaining '-'s are
        separators between parts of the date portion; the first mm is a
        two-digit numeral that represents the month; dd is a two-digit numeral
        that represents the day; 'T' is a separator indicating that time-of-day
        follows; hh is a two-digit numeral that represents the hour; '24' is
        permitted if the minutes and seconds represented are zero, and the
        dateTime value so represented is the first instant of the following day
        (the hour property of a dateTime object in the value space cannot have a
        value greater than 23); ':' is a separator between parts of the
        time-of-day portion; the second mm is a two-digit numeral that
        represents the minute; ss is a two-integer-digit numeral that represents
        the whole seconds; '.' s+ (if present) represents the fractional
        seconds; zzzzzz (if present) represents the timezone.				  */

        // Empty string
        "",									" ",

        // Starting or ending with invalid characters

        // Any field not represented
        "-12-10T00:00:00-14:00",			"20120505--10T00:00:00-14:00",
        "20120505-02-T00:00:00-14:00",		"-20120505-01-10T:02:12-14:00",
        "-20120505-01-10T01::59.999-14:00",	"20120505-01-10T01:12:-14:00",

        // Representation where the fields are not numbers
        "yyyy-mm-ddThh:mm:ss.sss",

        // Fields with wrong sizes
        "2012-07-17T01:01:1",				"-2012-07-17T01:01:001",
        "-2012-07-17T01:1:01",				"2012-07-17T01:001:01",
        "2012-07-17T1:01:01",				"-2012-07-17T001:01:01",
        "-2012-07-7T01:01:01",				"2012-07-007T01:01:01",
        "2012-7-17T01:01:01",				"-2012-007-17T01:01:01",
        "-012-07-17T01:01:01",				"02012-07-17T01:01:01",

        // Values exceding the upper bounds
        "45678-13-01T00:00:00",			"45678-12-32T01:20:33.127",
        "3513-12-01T24:00:00",			"7854-12-01T23:99:00",
        "-16789-12-01T23:59:60",

        // Wrong Separators
        "-4201*12*04T12:23:01",			"2012-12-12P12:12:12",
        "-2243-10-11T10-12-12",

        // Wrong floating-point representation of seconds
        "0001-12-01T12:32:2.123",		"0001-12-01T12:32:2.",
        "6543-03-24T13:17:.742",

        // Days that do not exist
        "-1970-02-29T01:01:01",			"1970-04-31T24:00:00",
        "1970-06-31T12:42:12",			"-1970-09-31T23:13:34",
        "-1970-11-31T21:42:52",			"0002-02-29T21:22:45",
        "0002-04-31T22:52:18",			"-0002-06-31T04:34:06",
        "-0002-09-31T17:26:34",			"0002-11-31T06:40:25",

        // Year, month and day fields with zero value
        "0000-01-01T12:43:35",			"0001-00-01T01:01:01",
        "0001-01-00T12:12:12",

        // Incomplete time representation
        "-1234-12-12T14",				"-1234-12-12T14:10",

        // Floating-point in integers fields
        "5678.5-12-12T01:34:52",		"-7213-11.2-10T19:29:10",
        "7284-11-10.7T00:20:53",		"-5421-11-10T12.2:12:12",
        "3741-11-10T12:1.2:12",

        // Representation does not end with '\0'
        "7348-01-20T01:43:31.132GSoC",	"2000-01-01T10:02:43ZGSoC",

        // Date/time separator missing
        "2000-11-1012:12:12",

        // Timezone: Representations where the fields are not numbers
        "2012-09-08T12:00:00+hh:ss",

        // Timezone: Fields with wrong sizes
        "2012-09-08T12:00:00+1:20",		"2012-09-08T12:00:00+001:20",
        "2012-09-08T12:00:00-10:1",		"2012-09-08T12:00:00-10:001",

        // Timezone: Values exceeding the upper bounds
        "-0001-01-01T01:01:01+15:00",	"-0001-01-01T01:01:01-13:60",

        // Timezone: Wrong Separator
        "0001-01-01T01:01:01+10-00",

        // Timezone: UTC wrong representation
        "0001-01-01T01:01:01z",

        // Timezone: When hours are 14, minutes must be 0
        "0001-01-01T01:01:01+14:34",	"0001-01-01T01:01:01-14:01",

        // Timezone: Incomplete representation
        "4136-05-19T01:05:19+14",		"2000-12-12T00:00:00-14",

        // Timezone: Floating-point representing hours and minutes
        "-4136-05-19T01:05:19+12.2:32",	"-2013-04-24T13:12:59.999+12:32.5",

        // Timezone: Representation does not end with '\0'
        "349239-03-05T12:50:30ZGSoC",	"-82354-02-11T02:02:37Z "
    };

    const int nValidDateTimes = 10;
    char * validDateTimes[nValidDateTimes] =
    {
        "847832-12-21T20:41:54.1",		"7814-01-29T04:12:43.1Z",
        "4136-05-19T01:05:19+11:42",	"281045-12-12T15:38:21-14:00",
        "0001-12-31T00:00:00.1+14:00",	"-437401-12-31T00:12:00.1-14:00",
        "2012-09-08T22:22:53.124-10:18","-2012-09-08T24:00:00-10:01",
        "-4136-05-19T01:05:19+12:32",	"-2013-04-24T13:12:59.999Z",
    };

    assertValidDateTimes(validDateTimes, nValidDateTimes, XalanDateTimeFormat::DATE_TIME);
    assertInvalidDateTimes(invalidDateTimes, nInvalidDateTimes, XalanDateTimeFormat::DATE_TIME);
}

void XalanDateTimeTestSuite::dtGMonthDayFormatFromToStringTest()
{
    printf("\n---------------------------------------------------------------");
    printf("\ndtGMonthDayFormatFromStringTest()");

    const int nInvalidGMonthDays = 48;
    char * invalidGMonthDays[nInvalidGMonthDays] =
    {
        // Empty string
        "",						" ",					"  ",

        // Starting or ending with invalid characters
        "?--09-21",				"--09-21?",				" --10-21",
        "--10-21 ",

        // Any field not represented
        "--05-",

        // Representations where the fields are not numbers
        "--MM-DD",				"--MM-21",				"--01-DD",

        // Fields with wrong sizes
        "--1-12",				"--012-12",				"--09-1",
        "--09-030",

        // Values exceding the upper bounds
        "--13-10",				"--31-05",				"--12-32",
        "--10-43",

        // Wrong Separators
        "//12/12",

        // Month and day fields with zero value
        "--00-21",				"--10-00",

        // Floating-point in integers fields
        "--01.3-10",			"--12-30.5",

        // Representation does not end with '\0'
        "--12-31GSoC",			"--12-31 ",				"--10-20Z ",

        // Timezone: Representations where the fields are not numbers
        "--10-21+HH:MM",		"--10-27-HH:MM",

        // Timezone: Fields with wrong sizes
        "--05-12+001:10",		"--05-27-001:10",		"--08-16+01:010",
        "--06-04-01:010",

        // Timezone: Values exceeding the upper bounds
        "--11-11+24:24",		"--11-30-24:24",		"--01-27+10:60",
        "--02-29+10:60",

        // Timezone: Wrong Separator
        "--09-21+01^00",		"--03-30-01-10",

        // Timezone: UTC wrong representation
        "--12-31z",

        // Timezone: When hours are 14, minutes must be 0
        "--04-13-14:01",		"--11-18+14:01",

        // Timezone: Incomplete representation
        "--09-30+12",			"--10-31+14",

        // Timezone: Floating-point representing hours and minutes
        "--11-11.2:32",			"--03+12:23.5",

        // Timezone: Representation does not end with '\0'
        "--02-28-02?",			"--04-15Zzz"
    };

    // All GMonthDays possible will be tested

    const int nValidGMonthDays = 366;
    char * validGMonthDays[nValidGMonthDays];
    
    // Some possible valid timezone representations
    char * timeZones[] =
        {
            "Z",				"-14:00",				"+14:00",
            "+00:00",			"-10:43",				"+12:14"
        };

    // Number of days for each month
    int nDays[] =
        {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 30, 31};

    // Generating the GMonthDays
    char * cValue;
    int dayYear = 0;
    for (int iMonth = 0; iMonth < 12; iMonth++)
    {
        for (int iDay = 0; iDay < nDays[iMonth]; iDay ++)
        {
            cValue = new char[13];
            sprintf(cValue, "--%02d-%02d", iMonth + 1, iDay + 1);

            validGMonthDays[dayYear++] = strcat(cValue,
                timeZones[rand()%(sizeof(timeZones)/sizeof(*timeZones))]);
        }
    }
    
    assertValidDateTimes(validGMonthDays, nValidGMonthDays, XalanDateTimeFormat::G_MONTH_DAY);
    assertInvalidDateTimes(invalidGMonthDays, nInvalidGMonthDays, XalanDateTimeFormat::G_MONTH_DAY);
}

void XalanDateTimeTestSuite::dtGYearMonthFormatFromToStringTest()
{
    printf("\n---------------------------------------------------------------");
    printf("\ndtGYearMonthFormatFromStringTest()");

    const int nInvalidGYearMonths = 66;
    char * invalidGYearMonths[nInvalidGYearMonths] =
    {
        /* The lexical representation for gYearMonth is the reduced (right
           truncated) lexical representation for dateTime: CCYY-MM. No left
           truncation is allowed. An optional following time zone qualifier is
           allowed. To accommodate year values outside the range from 0001 to
           9999, additional digits can be added to the left of this 
           representation and a preceding - sign is allowed.				  */

        // Empty string
        "",						" ",					"  ",

        // Starting or ending with invalid characters
        "?2012-09",				"2012-09?",				" 9999-04",
        "9999-04 ",

        // Any field not represented
        "-05",					"2012-",				"-2012-",

        // Representations where the fields are not numbers
        "CCYY-MM",				"-CCYY-MM",				"CCYY-12",
        "-CCYY-12",				"82108954-MM",			"-8021242-MM",

        // Fields with wrong sizes
        "333-12",				"-333-12",				"22-12",
        "-22-12",				"1-12",					"-1-12",
        "9999-1",				"-9999-1",				"0007-001",
        "-0007-001",

        // Values exceding the upper bounds
        "2041-13",				"-2139-36",

        // 5-digit or more representations do not accept zeros at left
        "00001-10",				"-00001-10",			"09999-08",
        "-09999-08",			"099999-11",			"-099999-11",

        // Wrong Separators
        "8349/10",				"2013/12",

        // Year and month fields with zero value
        "0000-07",				"-0000-07",				"250000-00",
        "-250000-00",

        // Floating-point in integers fields
        "2012.5-12",			"-2012-10.5",

        // Representation does not end with '\0'
        "250000-02?",			"49834328-10GSoC",		"3918741278-03 ",

        // Timezone: Representations where the fields are not numbers
        "2213-10-HH:MM",		"-2213-10-HH:MM",

        // Timezone: Fields with wrong sizes
        "2013-07+001:10",		"-2012-05-001:10",		"0001-08+01:010",
        "-0001-08-01:010",

        // Timezone: Values exceeding the upper bounds
        "27940-11+24:47",		"-3721-11+24:24",		"0001-01+10:60",
        "-5283-01+10:67",

        // Timezone: Wrong Separator
        "1111-09+01^00",		"-1111-09-01-10",

        // Timezone: UTC wrong representation
        "9999-06z",				"-9999-06z",

        // Timezone: When hours are 14, minutes must be 0
        "2013-06-14:42",		"-2015-07-14:01",

        // Timezone: Incomplete representation
        "4809035-12+12",			"-9841-12+12",

        // Timezone: Floating-point representing hours and minutes

        // Timezone: Representation does not end with '\0'
        "2013-04Zzz",				"-2013-04Z "
    };

    const int nValidGYearMonths = 12;
    char * validGYearMonths[nValidGYearMonths] =
    {
        "0001-09",				"-0001-09",				"9999-12",
        "-9999-12",				"2012-12Z",				"-2012-12Z",
        "250250-02Z",			"-250250-02Z",			"2013-07+14:00",
        "-2013-07-14:00",		"8775-08+00:00",		"-8775-08-00:00"
    };

    assertValidDateTimes(validGYearMonths, nValidGYearMonths, XalanDateTimeFormat::G_YEAR_MONTH);
    assertInvalidDateTimes(invalidGYearMonths, nInvalidGYearMonths, XalanDateTimeFormat::G_YEAR_MONTH);
}

void XalanDateTimeTestSuite::durationFromToStringTest()
{
    printf("\n---------------------------------------------------------------");
    printf("\ndurationFromStringTest()");

    const int nInvalidDurations = 28;
    char * invalidDurations[nInvalidDurations] =
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

    const int nValidDurations = 40;
    char * validDurations[nValidDurations] =
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

    assertValidDurations(validDurations, nValidDurations);
    assertInvalidDurations(invalidDurations, nInvalidDurations);
}

void XalanDateTimeTestSuite::dtDayOfWeekTest()
{
    printf("\n---------------------------------------------------------------");
    printf("\ndtDayOfWeekTest()");

    const int N_DATES = 45;
    const char * DAYS_ABRVS[7] =
    {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };

    const char * DAYS_NMS[7] =
    {
        "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
    };

    const char * DATES[N_DATES] = 
    {
        "2096-12-31",           "2096-06-15",           "2096-01-01",
        "2080-12-31",           "2080-06-15",           "2080-01-01",
        "2064-12-31",           "2064-06-15",           "2064-01-01",
        "2048-12-31",           "2048-06-15",           "2048-01-01",
        "2032-12-31",           "2032-06-15",           "2032-01-01",
        "2016-12-31",           "2016-06-15",           "2016-01-01",

        "2000-12-31",           "2000-06-15",           "2000-01-01",

        "1984-12-31",           "1984-06-15",           "1984-01-01",
        "1968-12-31",           "1968-06-15",           "1968-01-01",
        "1952-12-31",           "1952-06-15",           "1952-01-01",
        "1936-12-31",           "1936-06-15",           "1936-01-01",
        "1920-12-31",           "1920-06-15",           "1920-01-01",
        "1904-12-31",           "1904-06-15",           "1904-01-01",

        "0001-12-31",           "0001-06-15",           "0001-01-01",
        "-0001-12-31",          "-0001-06-15",          "-0001-01-01"
    };

    const int DAYS_OF_WEEK[N_DATES] =
    {
        2,                      6,                      1,
        3,                      7,                      2,
        4,                      1,                      3,
        5,                      2,                      4,
        6,                      3,                      5,
        7,                      4,                      6,

        1,                      5,                      7,

        2,                      6,                      1,
        3,                      7,                      2,
        4,                      1,                      3,
        5,                      2,                      4,
        6,                      3,                      5,
        7,                      4,                      6,

        2,                      6,                      2,
        1,                      5,                      7
    };

    const char * DAYS_NAMES[N_DATES];
    const char * DAYS_ABBREVIATIONS[N_DATES];
    
    for (int iDate = 0; iDate < N_DATES; iDate++)
    {
        DAYS_NAMES[iDate] = DAYS_NMS[DAYS_OF_WEEK[iDate] - 1];
        DAYS_ABBREVIATIONS[iDate] = DAYS_ABRVS[DAYS_OF_WEEK[iDate] - 1];
    }

    XalanDateTime * dateTime;
    
    for (int iDate = 0; iDate < N_DATES; iDate++)
    {
        dateTime = XalanDateTime::fromString(DATES[iDate], XalanDateTimeFormat::DATE);

        assertNumberEquals(DAYS_OF_WEEK[iDate], dateTime->getDayInWeek());
        assertStringEquals(DAYS_ABBREVIATIONS[iDate], dateTime->getDayAbbreviation());
        assertStringEquals(DAYS_NAMES[iDate], dateTime->getDayName());

        delete dateTime;
    }

    printf("\n    %d days of week tested", N_DATES);
}

int main(int argc, char * argv[])
{
    XalanDateTimeTestSuite * testSuite = new XalanDateTimeTestSuite();
    
    testSuite->dtTimeFormatFromToStringTest();
    testSuite->dtDateFormatFromToStringTest();
    testSuite->dtGDayFormatFromToStringTest();
    testSuite->dtGYearFormatFromToStringTest();
    testSuite->dtGMonthFormatFromToStringTest();
    testSuite->dtDateTimeFormatFromToStringTest();
    testSuite->dtGMonthDayFormatFromToStringTest();
    testSuite->dtGYearMonthFormatFromToStringTest();

    testSuite->durationFromToStringTest();

    testSuite->dtDayOfWeekTest();

    delete testSuite;

    printf("\n\n");
    system("PAUSE");

    return 0;
}