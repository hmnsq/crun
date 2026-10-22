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
সংরচনা signal_s
{
  পূর্ণাঙ্ক name;
  পূর্ণাঙ্ক মান;
};
গণনা
  {
    TOTAL_KEYWORDS = 62,
    MIN_WORD_LENGTH = 2,
    MAX_WORD_LENGTH = 8,
    MIN_HASH_মান = 13,
    MAX_HASH_মান = 86
  };

/* maximum key range = 74, duplicates = 0 */

#ifdef __GNUC__
__inline
#অন্যথা
#ifdef __cplusplus
অন্তঃস্থ
#endif
#endif
স্থির unsigned পূর্ণাঙ্ক
hash (পঞ্জিকা ধ্রুব বর্ণ *str, পঞ্জিকা size_t len)
{
  স্থির ধ্রুব unsigned বর্ণ asso_মানসমূহ[] =
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
  পঞ্জিকা unsigned পূর্ণাঙ্ক hval = len;

  বিকল্প (hval)
    {
      স্বভাব:
        hval += asso_মানসমূহ[(unsigned বর্ণ)str[3]];
      /*FALLTHROUGH*/
      পক্ষ 3:
      পক্ষ 2:
        hval += asso_মানসমূহ[(unsigned বর্ণ)str[1]];
        ভঙ্গ;
    }
  প্রত্যাগম hval + asso_মানসমূহ[(unsigned বর্ণ)str[len - 1]];
}

সংরচনা stringpool_t
  {
    বর্ণ stringpool_str0[পরিমাণ("ILL")];
    বর্ণ stringpool_str1[পরিমাণ("HUP")];
    বর্ণ stringpool_str2[পরিমাণ("KILL")];
    বর্ণ stringpool_str3[পরিমাণ("TTOU")];
    বর্ণ stringpool_str4[পরিমাণ("QUIT")];
    বর্ণ stringpool_str5[পরিমাণ("STOP")];
    বর্ণ stringpool_str6[পরিমাণ("RTMIN+2")];
    বর্ণ stringpool_str7[পরিমাণ("RTMIN+12")];
    বর্ণ stringpool_str8[পরিমাণ("RTMAX-2")];
    বর্ণ stringpool_str9[পরিমাণ("RTMAX-12")];
    বর্ণ stringpool_str10[পরিমাণ("RTMIN+1")];
    বর্ণ stringpool_str11[পরিমাণ("RTMIN+11")];
    বর্ণ stringpool_str12[পরিমাণ("RTMAX-1")];
    বর্ণ stringpool_str13[পরিমাণ("RTMAX-11")];
    বর্ণ stringpool_str14[পরিমাণ("RTMIN+4")];
    বর্ণ stringpool_str15[পরিমাণ("RTMIN+14")];
    বর্ণ stringpool_str16[পরিমাণ("RTMAX-4")];
    বর্ণ stringpool_str17[পরিমাণ("RTMAX-14")];
    বর্ণ stringpool_str18[পরিমাণ("RTMIN+3")];
    বর্ণ stringpool_str19[পরিমাণ("RTMIN+13")];
    বর্ণ stringpool_str20[পরিমাণ("RTMAX-3")];
    বর্ণ stringpool_str21[পরিমাণ("RTMAX-13")];
    বর্ণ stringpool_str22[পরিমাণ("BUS")];
    বর্ণ stringpool_str23[পরিমাণ("VTALRM")];
    বর্ণ stringpool_str24[পরিমাণ("INT")];
    বর্ণ stringpool_str25[পরিমাণ("FPE")];
    বর্ণ stringpool_str26[পরিমাণ("CONT")];
    বর্ণ stringpool_str27[পরিমাণ("STKFLT")];
    বর্ণ stringpool_str28[পরিমাণ("USR2")];
    বর্ণ stringpool_str29[পরিমাণ("TRAP")];
    বর্ণ stringpool_str30[পরিমাণ("TSTP")];
    বর্ণ stringpool_str31[পরিমাণ("RTMIN")];
    বর্ণ stringpool_str32[পরিমাণ("RTMIN+5")];
    বর্ণ stringpool_str33[পরিমাণ("RTMIN+15")];
    বর্ণ stringpool_str34[পরিমাণ("RTMAX-5")];
    বর্ণ stringpool_str35[পরিমাণ("RTMIN+9")];
    বর্ণ stringpool_str36[পরিমাণ("USR1")];
    বর্ণ stringpool_str37[পরিমাণ("RTMAX-9")];
    বর্ণ stringpool_str38[পরিমাণ("ALRM")];
    বর্ণ stringpool_str39[পরিমাণ("IO")];
    বর্ণ stringpool_str40[পরিমাণ("RTMIN+8")];
    বর্ণ stringpool_str41[পরিমাণ("RTMIN+7")];
    বর্ণ stringpool_str42[পরিমাণ("RTMAX-8")];
    বর্ণ stringpool_str43[পরিমাণ("RTMAX-7")];
    বর্ণ stringpool_str44[পরিমাণ("RTMIN+6")];
    বর্ণ stringpool_str45[পরিমাণ("RTMIN+10")];
    বর্ণ stringpool_str46[পরিমাণ("RTMAX-6")];
    বর্ণ stringpool_str47[পরিমাণ("RTMAX-10")];
    বর্ণ stringpool_str48[পরিমাণ("SYS")];
    বর্ণ stringpool_str49[পরিমাণ("XFSZ")];
    বর্ণ stringpool_str50[পরিমাণ("RTMAX")];
    বর্ণ stringpool_str51[পরিমাণ("PWR")];
    বর্ণ stringpool_str52[পরিমাণ("SEGV")];
    বর্ণ stringpool_str53[পরিমাণ("XCPU")];
    বর্ণ stringpool_str54[পরিমাণ("WINCH")];
    বর্ণ stringpool_str55[পরিমাণ("URG")];
    বর্ণ stringpool_str56[পরিমাণ("CHLD")];
    বর্ণ stringpool_str57[পরিমাণ("TTIN")];
    বর্ণ stringpool_str58[পরিমাণ("PIPE")];
    বর্ণ stringpool_str59[পরিমাণ("ABRT")];
    বর্ণ stringpool_str60[পরিমাণ("TERM")];
    বর্ণ stringpool_str61[পরিমাণ("PROF")];
  };
স্থির ধ্রুব সংরচনা stringpool_t stringpool_contents =
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
#define stringpool ((ধ্রুব বর্ণ *) &stringpool_contents)

স্থির ধ্রুব সংরচনা signal_s wordlist[] =
  {
#line 36 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str0, 4},
#line 33 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str1, 1},
#line 41 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str2, 9},
#line 54 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str3, 22},
#line 35 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str4, 3},
#line 51 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str5, 19},
#line 66 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str6, 36},
#line 76 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str7, 46},
#line 92 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str8, 62},
#line 82 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str9, 52},
#line 65 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str10, 35},
#line 75 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str11, 45},
#line 93 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str12, 63},
#line 83 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str13, 53},
#line 68 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str14, 38},
#line 78 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str15, 48},
#line 90 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str16, 60},
#line 80 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str17, 50},
#line 67 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str18, 37},
#line 77 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str19, 47},
#line 91 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str20, 61},
#line 81 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str21, 51},
#line 39 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str22, 7},
#line 58 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str23, 26},
#line 34 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str24, 2},
#line 40 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str25, 8},
#line 50 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str26, 18},
#line 48 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str27, 16},
#line 44 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str28, 12},
#line 37 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str29, 5},
#line 52 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str30, 20},
#line 64 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str31, 34},
#line 69 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str32, 39},
#line 79 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str33, 49},
#line 89 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str34, 59},
#line 73 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str35, 43},
#line 42 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str36, 10},
#line 85 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str37, 55},
#line 46 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str38, 14},
#line 61 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str39, 29},
#line 72 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str40, 42},
#line 71 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str41, 41},
#line 86 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str42, 56},
#line 87 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str43, 57},
#line 70 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str44, 40},
#line 74 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str45, 44},
#line 88 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str46, 58},
#line 84 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str47, 54},
#line 63 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str48, 31},
#line 57 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str49, 25},
#line 94 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str50, 64},
#line 62 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str51, 30},
#line 43 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str52, 11},
#line 56 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str53, 24},
#line 60 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str54, 28},
#line 55 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str55, 23},
#line 49 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str56, 17},
#line 53 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str57, 21},
#line 45 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str58, 13},
#line 38 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str59, 6},
#line 47 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str60, 15},
#line 59 "src/libcrun/signals.perf"
    {(পূর্ণাঙ্ক)(size_t)&((সংরচনা stringpool_t *)0)->stringpool_str61, 27}
  };

ধ্রুব সংরচনা signal_s *
libcrun_signal_in_word_set (পঞ্জিকা ধ্রুব বর্ণ *str, পঞ্জিকা size_t len)
{
  যদি (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      পঞ্জিকা unsigned পূর্ণাঙ্ক চাবি = hash (str, len);

      যদি (চাবি <= MAX_HASH_মান && চাবি >= MIN_HASH_মান)
        {
          পঞ্জিকা ধ্রুব সংরচনা signal_s *resword;

          বিকল্প (চাবি - 13)
            {
              পক্ষ 0:
                resword = &wordlist[0];
                গচ্ছ compare;
              পক্ষ 1:
                resword = &wordlist[1];
                গচ্ছ compare;
              পক্ষ 6:
                resword = &wordlist[2];
                গচ্ছ compare;
              পক্ষ 7:
                resword = &wordlist[3];
                গচ্ছ compare;
              পক্ষ 8:
                resword = &wordlist[4];
                গচ্ছ compare;
              পক্ষ 9:
                resword = &wordlist[5];
                গচ্ছ compare;
              পক্ষ 10:
                resword = &wordlist[6];
                গচ্ছ compare;
              পক্ষ 11:
                resword = &wordlist[7];
                গচ্ছ compare;
              পক্ষ 12:
                resword = &wordlist[8];
                গচ্ছ compare;
              পক্ষ 13:
                resword = &wordlist[9];
                গচ্ছ compare;
              পক্ষ 14:
                resword = &wordlist[10];
                গচ্ছ compare;
              পক্ষ 15:
                resword = &wordlist[11];
                গচ্ছ compare;
              পক্ষ 16:
                resword = &wordlist[12];
                গচ্ছ compare;
              পক্ষ 17:
                resword = &wordlist[13];
                গচ্ছ compare;
              পক্ষ 18:
                resword = &wordlist[14];
                গচ্ছ compare;
              পক্ষ 19:
                resword = &wordlist[15];
                গচ্ছ compare;
              পক্ষ 20:
                resword = &wordlist[16];
                গচ্ছ compare;
              পক্ষ 21:
                resword = &wordlist[17];
                গচ্ছ compare;
              পক্ষ 22:
                resword = &wordlist[18];
                গচ্ছ compare;
              পক্ষ 23:
                resword = &wordlist[19];
                গচ্ছ compare;
              পক্ষ 24:
                resword = &wordlist[20];
                গচ্ছ compare;
              পক্ষ 25:
                resword = &wordlist[21];
                গচ্ছ compare;
              পক্ষ 26:
                resword = &wordlist[22];
                গচ্ছ compare;
              পক্ষ 27:
                resword = &wordlist[23];
                গচ্ছ compare;
              পক্ষ 28:
                resword = &wordlist[24];
                গচ্ছ compare;
              পক্ষ 29:
                resword = &wordlist[25];
                গচ্ছ compare;
              পক্ষ 30:
                resword = &wordlist[26];
                গচ্ছ compare;
              পক্ষ 31:
                resword = &wordlist[27];
                গচ্ছ compare;
              পক্ষ 32:
                resword = &wordlist[28];
                গচ্ছ compare;
              পক্ষ 33:
                resword = &wordlist[29];
                গচ্ছ compare;
              পক্ষ 34:
                resword = &wordlist[30];
                গচ্ছ compare;
              পক্ষ 35:
                resword = &wordlist[31];
                গচ্ছ compare;
              পক্ষ 36:
                resword = &wordlist[32];
                গচ্ছ compare;
              পক্ষ 37:
                resword = &wordlist[33];
                গচ্ছ compare;
              পক্ষ 38:
                resword = &wordlist[34];
                গচ্ছ compare;
              পক্ষ 39:
                resword = &wordlist[35];
                গচ্ছ compare;
              পক্ষ 40:
                resword = &wordlist[36];
                গচ্ছ compare;
              পক্ষ 41:
                resword = &wordlist[37];
                গচ্ছ compare;
              পক্ষ 42:
                resword = &wordlist[38];
                গচ্ছ compare;
              পক্ষ 43:
                resword = &wordlist[39];
                গচ্ছ compare;
              পক্ষ 44:
                resword = &wordlist[40];
                গচ্ছ compare;
              পক্ষ 45:
                resword = &wordlist[41];
                গচ্ছ compare;
              পক্ষ 46:
                resword = &wordlist[42];
                গচ্ছ compare;
              পক্ষ 47:
                resword = &wordlist[43];
                গচ্ছ compare;
              পক্ষ 48:
                resword = &wordlist[44];
                গচ্ছ compare;
              পক্ষ 49:
                resword = &wordlist[45];
                গচ্ছ compare;
              পক্ষ 50:
                resword = &wordlist[46];
                গচ্ছ compare;
              পক্ষ 51:
                resword = &wordlist[47];
                গচ্ছ compare;
              পক্ষ 52:
                resword = &wordlist[48];
                গচ্ছ compare;
              পক্ষ 53:
                resword = &wordlist[49];
                গচ্ছ compare;
              পক্ষ 54:
                resword = &wordlist[50];
                গচ্ছ compare;
              পক্ষ 55:
                resword = &wordlist[51];
                গচ্ছ compare;
              পক্ষ 56:
                resword = &wordlist[52];
                গচ্ছ compare;
              পক্ষ 57:
                resword = &wordlist[53];
                গচ্ছ compare;
              পক্ষ 58:
                resword = &wordlist[54];
                গচ্ছ compare;
              পক্ষ 59:
                resword = &wordlist[55];
                গচ্ছ compare;
              পক্ষ 60:
                resword = &wordlist[56];
                গচ্ছ compare;
              পক্ষ 61:
                resword = &wordlist[57];
                গচ্ছ compare;
              পক্ষ 62:
                resword = &wordlist[58];
                গচ্ছ compare;
              পক্ষ 63:
                resword = &wordlist[59];
                গচ্ছ compare;
              পক্ষ 70:
                resword = &wordlist[60];
                গচ্ছ compare;
              পক্ষ 73:
                resword = &wordlist[61];
                গচ্ছ compare;
            }
          প্রত্যাগম 0;
        compare:
          {
            পঞ্জিকা ধ্রুব বর্ণ *s = resword->name + stringpool;

            যদি (*str == *s && !strcmp (str + 1, s + 1))
              প্রত্যাগম resword;
          }
        }
    }
  প্রত্যাগম 0;
}
#line 95 "src/libcrun/signals.perf"

পূর্ণাঙ্ক
str2sig (ধ্রুব বর্ণ *name)
{
  ধ্রুব সংরচনা signal_s *s;

  যদি (has_prefix (name, "SIG"))
    name += 3;

  s = libcrun_signal_in_word_set (name, strlen (name));
  যদি (s == রিক্ত)
    {
      long পূর্ণাঙ্ক মান;

      যদি (!isdigit(name[0]))
        {
          errno = EINVAL;
          প্রত্যাগম -1;
        }

      errno = 0;
      মান = strtol (name, রিক্ত, 10);
      যদি (errno != 0)
        প্রত্যাগম -1;

      প্রত্যাগম মান;
    }

  প্রত্যাগম s->মান;
}
