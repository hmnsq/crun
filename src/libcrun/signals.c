/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --lookup-function-name libcrun_signal_in_word_set -m 100 --null-strings --pic -tCEG -S1 src/libcrun/signals.perf  */
/* Computed positions: -k'2,4,$' */

#যদি !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 19 "src/libcrun/signals.perf"

#define _GNU_SOURCE

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stddef.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব "utils.h"
#line 27 "src/libcrun/signals.perf"
কাঠামো signal_s
{
  পূর্ণসংখ্যা name;
  পূর্ণসংখ্যা মান;
};
গণনা
  {
    যোগ_KEYWORDS = 62,
    MIN_WORD_দৈর্ঘ্য = 2,
    MAX_WORD_দৈর্ঘ্য = 8,
    MIN_HASH_মান = 13,
    MAX_HASH_মান = 86
  };

/* maximum key range = 74, duplicates = 0 */

#ifdef __GNUC__
__inline
#অন্যথা
#ifdef __cplusplus
অন্তঃরেখ
#endif
#endif
স্থির unsigned পূর্ণসংখ্যা
hash (নিবন্ধ ধ্রুবক অক্ষর *str, নিবন্ধ size_t len)
{
  স্থির ধ্রুবক unsigned অক্ষর asso_মানসমূহ[] =
    {
      87, 87, 87, 87, 87, 87, 87, 87, 87, 87,
      87, 87, 87, 87, 87, 87, 87, 87, 87, 87,
      87, 87, 87, 87, 87, 87, 87, 87, 87, 87,
      87, 87, 87, 87, 87, 87, 87, 87, 87, 87,
      87, 87, 87, 87, 87, 87, 87, 87, 43,  9,
       5, 17, 13, 31, 43, 40, 39, 34, 87, 87,
      87, 87, 87, 87, 87,  7, 60, 56, 32, 33,
      26, 39,  5,  5, 87, 87,  5, 23, 32, 27,
       6, 87, 30, 31,  6,  5, 16, 35, 49, 31,
      18, 87, 87, 87, 87, 87, 87, 87, 87, 87,
      87, 87, 87, 87, 87, 87, 87, 87, 87, 87,
      87, 87, 87, 87, 87, 87, 87, 87, 87, 87,
      87, 87, 87, 87, 87, 87, 87, 87, 87, 87,
      87, 87, 87, 87, 87, 87, 87, 87, 87, 87,
      87, 87, 87, 87, 87, 87, 87, 87, 87, 87,
      87, 87, 87, 87, 87, 87, 87, 87, 87, 87,
      87, 87, 87, 87, 87, 87, 87, 87, 87, 87,
      87, 87, 87, 87, 87, 87, 87, 87, 87, 87,
      87, 87, 87, 87, 87, 87, 87, 87, 87, 87,
      87, 87, 87, 87, 87, 87, 87, 87, 87, 87,
      87, 87, 87, 87, 87, 87, 87, 87, 87, 87,
      87, 87, 87, 87, 87, 87, 87, 87, 87, 87,
      87, 87, 87, 87, 87, 87, 87, 87, 87, 87,
      87, 87, 87, 87, 87, 87, 87, 87, 87, 87,
      87, 87, 87, 87, 87, 87, 87, 87, 87, 87,
      87, 87, 87, 87, 87, 87
    };
  নিবন্ধ unsigned পূর্ণসংখ্যা hval = len;

  নির্বাচন (hval)
    {
      পূর্বনির্ধারিত:
        hval += asso_মানসমূহ[(unsigned অক্ষর)str[3]];
      /*FALLTHROUGH*/
      ক্ষেত্র 3:
      ক্ষেত্র 2:
        hval += asso_মানসমূহ[(unsigned অক্ষর)str[1]];
        বিরতি;
    }
  ফেরত hval + asso_মানসমূহ[(unsigned অক্ষর)str[len - 1]];
}

কাঠামো stringpool_t
  {
    অক্ষর stringpool_str0[আকার("ILL")];
    অক্ষর stringpool_str1[আকার("HUP")];
    অক্ষর stringpool_str2[আকার("KILL")];
    অক্ষর stringpool_str3[আকার("TTOU")];
    অক্ষর stringpool_str4[আকার("QUIT")];
    অক্ষর stringpool_str5[আকার("STOP")];
    অক্ষর stringpool_str6[আকার("RTMIN+2")];
    অক্ষর stringpool_str7[আকার("RTMIN+12")];
    অক্ষর stringpool_str8[আকার("RTMAX-2")];
    অক্ষর stringpool_str9[আকার("RTMAX-12")];
    অক্ষর stringpool_str10[আকার("RTMIN+1")];
    অক্ষর stringpool_str11[আকার("RTMIN+11")];
    অক্ষর stringpool_str12[আকার("RTMAX-1")];
    অক্ষর stringpool_str13[আকার("RTMAX-11")];
    অক্ষর stringpool_str14[আকার("RTMIN+4")];
    অক্ষর stringpool_str15[আকার("RTMIN+14")];
    অক্ষর stringpool_str16[আকার("RTMAX-4")];
    অক্ষর stringpool_str17[আকার("RTMAX-14")];
    অক্ষর stringpool_str18[আকার("RTMIN+3")];
    অক্ষর stringpool_str19[আকার("RTMIN+13")];
    অক্ষর stringpool_str20[আকার("RTMAX-3")];
    অক্ষর stringpool_str21[আকার("RTMAX-13")];
    অক্ষর stringpool_str22[আকার("BUS")];
    অক্ষর stringpool_str23[আকার("VTALRM")];
    অক্ষর stringpool_str24[আকার("INT")];
    অক্ষর stringpool_str25[আকার("FPE")];
    অক্ষর stringpool_str26[আকার("CONT")];
    অক্ষর stringpool_str27[আকার("STKFLT")];
    অক্ষর stringpool_str28[আকার("USR2")];
    অক্ষর stringpool_str29[আকার("TRAP")];
    অক্ষর stringpool_str30[আকার("TSTP")];
    অক্ষর stringpool_str31[আকার("RTMIN")];
    অক্ষর stringpool_str32[আকার("RTMIN+5")];
    অক্ষর stringpool_str33[আকার("RTMIN+15")];
    অক্ষর stringpool_str34[আকার("RTMAX-5")];
    অক্ষর stringpool_str35[আকার("RTMIN+9")];
    অক্ষর stringpool_str36[আকার("USR1")];
    অক্ষর stringpool_str37[আকার("RTMAX-9")];
    অক্ষর stringpool_str38[আকার("ALRM")];
    অক্ষর stringpool_str39[আকার("IO")];
    অক্ষর stringpool_str40[আকার("RTMIN+8")];
    অক্ষর stringpool_str41[আকার("RTMIN+7")];
    অক্ষর stringpool_str42[আকার("RTMAX-8")];
    অক্ষর stringpool_str43[আকার("RTMAX-7")];
    অক্ষর stringpool_str44[আকার("RTMIN+6")];
    অক্ষর stringpool_str45[আকার("RTMIN+10")];
    অক্ষর stringpool_str46[আকার("RTMAX-6")];
    অক্ষর stringpool_str47[আকার("RTMAX-10")];
    অক্ষর stringpool_str48[আকার("SYS")];
    অক্ষর stringpool_str49[আকার("XFSZ")];
    অক্ষর stringpool_str50[আকার("RTMAX")];
    অক্ষর stringpool_str51[আকার("PWR")];
    অক্ষর stringpool_str52[আকার("SEGV")];
    অক্ষর stringpool_str53[আকার("XCPU")];
    অক্ষর stringpool_str54[আকার("WINCH")];
    অক্ষর stringpool_str55[আকার("URG")];
    অক্ষর stringpool_str56[আকার("CHLD")];
    অক্ষর stringpool_str57[আকার("TTIN")];
    অক্ষর stringpool_str58[আকার("PIPE")];
    অক্ষর stringpool_str59[আকার("ABRT")];
    অক্ষর stringpool_str60[আকার("TERM")];
    অক্ষর stringpool_str61[আকার("PROF")];
  };
স্থির ধ্রুবক কাঠামো stringpool_t stringpool_contents =
  {
    "ILL",
    "HUP",
    "KILL",
    "TTOU",
    "QUIT",
    "STOP",
    "RTMIN+2",
    "RTMIN+12",
    "RTMAX-2",
    "RTMAX-12",
    "RTMIN+1",
    "RTMIN+11",
    "RTMAX-1",
    "RTMAX-11",
    "RTMIN+4",
    "RTMIN+14",
    "RTMAX-4",
    "RTMAX-14",
    "RTMIN+3",
    "RTMIN+13",
    "RTMAX-3",
    "RTMAX-13",
    "BUS",
    "VTALRM",
    "INT",
    "FPE",
    "CONT",
    "STKFLT",
    "USR2",
    "TRAP",
    "TSTP",
    "RTMIN",
    "RTMIN+5",
    "RTMIN+15",
    "RTMAX-5",
    "RTMIN+9",
    "USR1",
    "RTMAX-9",
    "ALRM",
    "IO",
    "RTMIN+8",
    "RTMIN+7",
    "RTMAX-8",
    "RTMAX-7",
    "RTMIN+6",
    "RTMIN+10",
    "RTMAX-6",
    "RTMAX-10",
    "SYS",
    "XFSZ",
    "RTMAX",
    "PWR",
    "SEGV",
    "XCPU",
    "WINCH",
    "URG",
    "CHLD",
    "TTIN",
    "PIPE",
    "ABRT",
    "TERM",
    "PROF"
  };
#define stringpool ((ধ্রুবক অক্ষর *) &stringpool_contents)

স্থির ধ্রুবক কাঠামো signal_s wordlist[] =
  {
#line 36 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str0, 4},
#line 33 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str1, 1},
#line 41 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str2, 9},
#line 54 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str3, 22},
#line 35 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str4, 3},
#line 51 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str5, 19},
#line 66 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str6, 36},
#line 76 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str7, 46},
#line 92 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str8, 62},
#line 82 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str9, 52},
#line 65 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str10, 35},
#line 75 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str11, 45},
#line 93 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str12, 63},
#line 83 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str13, 53},
#line 68 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str14, 38},
#line 78 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str15, 48},
#line 90 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str16, 60},
#line 80 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str17, 50},
#line 67 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str18, 37},
#line 77 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str19, 47},
#line 91 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str20, 61},
#line 81 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str21, 51},
#line 39 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str22, 7},
#line 58 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str23, 26},
#line 34 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str24, 2},
#line 40 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str25, 8},
#line 50 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str26, 18},
#line 48 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str27, 16},
#line 44 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str28, 12},
#line 37 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str29, 5},
#line 52 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str30, 20},
#line 64 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str31, 34},
#line 69 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str32, 39},
#line 79 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str33, 49},
#line 89 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str34, 59},
#line 73 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str35, 43},
#line 42 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str36, 10},
#line 85 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str37, 55},
#line 46 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str38, 14},
#line 61 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str39, 29},
#line 72 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str40, 42},
#line 71 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str41, 41},
#line 86 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str42, 56},
#line 87 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str43, 57},
#line 70 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str44, 40},
#line 74 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str45, 44},
#line 88 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str46, 58},
#line 84 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str47, 54},
#line 63 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str48, 31},
#line 57 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str49, 25},
#line 94 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str50, 64},
#line 62 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str51, 30},
#line 43 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str52, 11},
#line 56 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str53, 24},
#line 60 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str54, 28},
#line 55 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str55, 23},
#line 49 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str56, 17},
#line 53 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str57, 21},
#line 45 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str58, 13},
#line 38 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str59, 6},
#line 47 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str60, 15},
#line 59 "src/libcrun/signals.perf"
    {(পূর্ণসংখ্যা)(size_t)&((কাঠামো stringpool_t *)0)->stringpool_str61, 27}
  };

ধ্রুবক কাঠামো signal_s *
libcrun_signal_in_word_set (নিবন্ধ ধ্রুবক অক্ষর *str, নিবন্ধ size_t len)
{
  যদি (len <= MAX_WORD_দৈর্ঘ্য && len >= MIN_WORD_দৈর্ঘ্য)
    {
      নিবন্ধ unsigned পূর্ণসংখ্যা চাবি = hash (str, len);

      যদি (চাবি <= MAX_HASH_মান && চাবি >= MIN_HASH_মান)
        {
          নিবন্ধ ধ্রুবক কাঠামো signal_s *resword;

          নির্বাচন (চাবি - 13)
            {
              ক্ষেত্র 0:
                resword = &wordlist[0];
                যাও compare;
              ক্ষেত্র 1:
                resword = &wordlist[1];
                যাও compare;
              ক্ষেত্র 6:
                resword = &wordlist[2];
                যাও compare;
              ক্ষেত্র 7:
                resword = &wordlist[3];
                যাও compare;
              ক্ষেত্র 8:
                resword = &wordlist[4];
                যাও compare;
              ক্ষেত্র 9:
                resword = &wordlist[5];
                যাও compare;
              ক্ষেত্র 10:
                resword = &wordlist[6];
                যাও compare;
              ক্ষেত্র 11:
                resword = &wordlist[7];
                যাও compare;
              ক্ষেত্র 12:
                resword = &wordlist[8];
                যাও compare;
              ক্ষেত্র 13:
                resword = &wordlist[9];
                যাও compare;
              ক্ষেত্র 14:
                resword = &wordlist[10];
                যাও compare;
              ক্ষেত্র 15:
                resword = &wordlist[11];
                যাও compare;
              ক্ষেত্র 16:
                resword = &wordlist[12];
                যাও compare;
              ক্ষেত্র 17:
                resword = &wordlist[13];
                যাও compare;
              ক্ষেত্র 18:
                resword = &wordlist[14];
                যাও compare;
              ক্ষেত্র 19:
                resword = &wordlist[15];
                যাও compare;
              ক্ষেত্র 20:
                resword = &wordlist[16];
                যাও compare;
              ক্ষেত্র 21:
                resword = &wordlist[17];
                যাও compare;
              ক্ষেত্র 22:
                resword = &wordlist[18];
                যাও compare;
              ক্ষেত্র 23:
                resword = &wordlist[19];
                যাও compare;
              ক্ষেত্র 24:
                resword = &wordlist[20];
                যাও compare;
              ক্ষেত্র 25:
                resword = &wordlist[21];
                যাও compare;
              ক্ষেত্র 26:
                resword = &wordlist[22];
                যাও compare;
              ক্ষেত্র 27:
                resword = &wordlist[23];
                যাও compare;
              ক্ষেত্র 28:
                resword = &wordlist[24];
                যাও compare;
              ক্ষেত্র 29:
                resword = &wordlist[25];
                যাও compare;
              ক্ষেত্র 30:
                resword = &wordlist[26];
                যাও compare;
              ক্ষেত্র 31:
                resword = &wordlist[27];
                যাও compare;
              ক্ষেত্র 32:
                resword = &wordlist[28];
                যাও compare;
              ক্ষেত্র 33:
                resword = &wordlist[29];
                যাও compare;
              ক্ষেত্র 34:
                resword = &wordlist[30];
                যাও compare;
              ক্ষেত্র 35:
                resword = &wordlist[31];
                যাও compare;
              ক্ষেত্র 36:
                resword = &wordlist[32];
                যাও compare;
              ক্ষেত্র 37:
                resword = &wordlist[33];
                যাও compare;
              ক্ষেত্র 38:
                resword = &wordlist[34];
                যাও compare;
              ক্ষেত্র 39:
                resword = &wordlist[35];
                যাও compare;
              ক্ষেত্র 40:
                resword = &wordlist[36];
                যাও compare;
              ক্ষেত্র 41:
                resword = &wordlist[37];
                যাও compare;
              ক্ষেত্র 42:
                resword = &wordlist[38];
                যাও compare;
              ক্ষেত্র 43:
                resword = &wordlist[39];
                যাও compare;
              ক্ষেত্র 44:
                resword = &wordlist[40];
                যাও compare;
              ক্ষেত্র 45:
                resword = &wordlist[41];
                যাও compare;
              ক্ষেত্র 46:
                resword = &wordlist[42];
                যাও compare;
              ক্ষেত্র 47:
                resword = &wordlist[43];
                যাও compare;
              ক্ষেত্র 48:
                resword = &wordlist[44];
                যাও compare;
              ক্ষেত্র 49:
                resword = &wordlist[45];
                যাও compare;
              ক্ষেত্র 50:
                resword = &wordlist[46];
                যাও compare;
              ক্ষেত্র 51:
                resword = &wordlist[47];
                যাও compare;
              ক্ষেত্র 52:
                resword = &wordlist[48];
                যাও compare;
              ক্ষেত্র 53:
                resword = &wordlist[49];
                যাও compare;
              ক্ষেত্র 54:
                resword = &wordlist[50];
                যাও compare;
              ক্ষেত্র 55:
                resword = &wordlist[51];
                যাও compare;
              ক্ষেত্র 56:
                resword = &wordlist[52];
                যাও compare;
              ক্ষেত্র 57:
                resword = &wordlist[53];
                যাও compare;
              ক্ষেত্র 58:
                resword = &wordlist[54];
                যাও compare;
              ক্ষেত্র 59:
                resword = &wordlist[55];
                যাও compare;
              ক্ষেত্র 60:
                resword = &wordlist[56];
                যাও compare;
              ক্ষেত্র 61:
                resword = &wordlist[57];
                যাও compare;
              ক্ষেত্র 62:
                resword = &wordlist[58];
                যাও compare;
              ক্ষেত্র 63:
                resword = &wordlist[59];
                যাও compare;
              ক্ষেত্র 70:
                resword = &wordlist[60];
                যাও compare;
              ক্ষেত্র 73:
                resword = &wordlist[61];
                যাও compare;
            }
          ফেরত 0;
        compare:
          {
            নিবন্ধ ধ্রুবক অক্ষর *s = resword->name + stringpool;

            যদি (*str == *s && !strcmp (str + 1, s + 1))
              ফেরত resword;
          }
        }
    }
  ফেরত 0;
}
#line 95 "src/libcrun/signals.perf"

পূর্ণসংখ্যা
str2sig (ধ্রুবক অক্ষর *name)
{
  ধ্রুবক কাঠামো signal_s *s;

  যদি (has_prefix (name, "SIG"))
    name += 3;

  s = libcrun_signal_in_word_set (name, strlen (name));
  যদি (s == ফাঁকা)
    {
      long পূর্ণসংখ্যা মান;

      যদি (!isdigit(name[0]))
        {
          errno = EINVAL;
          ফেরত -1;
        }

      errno = 0;
      মান = strtol (name, ফাঁকা, 10);
      যদি (errno != 0)
        ফেরত -1;

      ফেরত মান;
    }

  ফেরত s->মান;
}
