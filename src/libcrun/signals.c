/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --lookup-function-name libcrun_signal_in_word_set -m 100 --null-strings --pic -tCEG -S1 src/libcrun/signals.perf  */
/* Computed positions: -k'2,4,$' */

#ଯଦି !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stddef.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ "utils.h"
#line 27 "src/libcrun/signals.perf"
ସଂରଚନା signal_s
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା name;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ;
};
ଗଣନା
  {
    TOTAL_KEYWORDS = 62,
    MIN_WORD_LENGTH = 2,
    MAX_WORD_LENGTH = 8,
    MIN_HASH_ମୂଲ୍ୟ = 13,
    MAX_HASH_ମୂଲ୍ୟ = 86
  };

/* maximum key range = 74, duplicates = 0 */

#ifdef __GNUC__
__inline
#ଅନ୍ୟଥା
#ifdef __cplusplus
ଧାଡ଼ିଭିତରେ
#endif
#endif
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hash (ପଞ୍ଜିକରଣ ଅଚଳ ଅକ୍ଷର *str, ପଞ୍ଜିକରଣ size_t len)
{
  ସ୍ଥିର ଅଚଳ unsigned ଅକ୍ଷର asso_ମୂଲ୍ୟଗୁଡ଼ିକ[] =
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
  ପଞ୍ଜିକରଣ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା hval = len;

  ବାଛ (hval)
    {
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        hval += asso_ମୂଲ୍ୟଗୁଡ଼ିକ[(unsigned ଅକ୍ଷର)str[3]];
      /*FALLTHROUGH*/
      ସ୍ଥିତି 3:
      ସ୍ଥିତି 2:
        hval += asso_ମୂଲ୍ୟଗୁଡ଼ିକ[(unsigned ଅକ୍ଷର)str[1]];
        ଭାଙ୍ଗ;
    }
  ଫେରନ୍ତୁ hval + asso_ମୂଲ୍ୟଗୁଡ଼ିକ[(unsigned ଅକ୍ଷର)str[len - 1]];
}

ସଂରଚନା stringpool_t
  {
    ଅକ୍ଷର stringpool_str0[ଆକାର("ILL")];
    ଅକ୍ଷର stringpool_str1[ଆକାର("HUP")];
    ଅକ୍ଷର stringpool_str2[ଆକାର("KILL")];
    ଅକ୍ଷର stringpool_str3[ଆକାର("TTOU")];
    ଅକ୍ଷର stringpool_str4[ଆକାର("QUIT")];
    ଅକ୍ଷର stringpool_str5[ଆକାର("STOP")];
    ଅକ୍ଷର stringpool_str6[ଆକାର("RTMIN+2")];
    ଅକ୍ଷର stringpool_str7[ଆକାର("RTMIN+12")];
    ଅକ୍ଷର stringpool_str8[ଆକାର("RTMAX-2")];
    ଅକ୍ଷର stringpool_str9[ଆକାର("RTMAX-12")];
    ଅକ୍ଷର stringpool_str10[ଆକାର("RTMIN+1")];
    ଅକ୍ଷର stringpool_str11[ଆକାର("RTMIN+11")];
    ଅକ୍ଷର stringpool_str12[ଆକାର("RTMAX-1")];
    ଅକ୍ଷର stringpool_str13[ଆକାର("RTMAX-11")];
    ଅକ୍ଷର stringpool_str14[ଆକାର("RTMIN+4")];
    ଅକ୍ଷର stringpool_str15[ଆକାର("RTMIN+14")];
    ଅକ୍ଷର stringpool_str16[ଆକାର("RTMAX-4")];
    ଅକ୍ଷର stringpool_str17[ଆକାର("RTMAX-14")];
    ଅକ୍ଷର stringpool_str18[ଆକାର("RTMIN+3")];
    ଅକ୍ଷର stringpool_str19[ଆକାର("RTMIN+13")];
    ଅକ୍ଷର stringpool_str20[ଆକାର("RTMAX-3")];
    ଅକ୍ଷର stringpool_str21[ଆକାର("RTMAX-13")];
    ଅକ୍ଷର stringpool_str22[ଆକାର("BUS")];
    ଅକ୍ଷର stringpool_str23[ଆକାର("VTALRM")];
    ଅକ୍ଷର stringpool_str24[ଆକାର("INT")];
    ଅକ୍ଷର stringpool_str25[ଆକାର("FPE")];
    ଅକ୍ଷର stringpool_str26[ଆକାର("CONT")];
    ଅକ୍ଷର stringpool_str27[ଆକାର("STKFLT")];
    ଅକ୍ଷର stringpool_str28[ଆକାର("USR2")];
    ଅକ୍ଷର stringpool_str29[ଆକାର("TRAP")];
    ଅକ୍ଷର stringpool_str30[ଆକାର("TSTP")];
    ଅକ୍ଷର stringpool_str31[ଆକାର("RTMIN")];
    ଅକ୍ଷର stringpool_str32[ଆକାର("RTMIN+5")];
    ଅକ୍ଷର stringpool_str33[ଆକାର("RTMIN+15")];
    ଅକ୍ଷର stringpool_str34[ଆକାର("RTMAX-5")];
    ଅକ୍ଷର stringpool_str35[ଆକାର("RTMIN+9")];
    ଅକ୍ଷର stringpool_str36[ଆକାର("USR1")];
    ଅକ୍ଷର stringpool_str37[ଆକାର("RTMAX-9")];
    ଅକ୍ଷର stringpool_str38[ଆକାର("ALRM")];
    ଅକ୍ଷର stringpool_str39[ଆକାର("IO")];
    ଅକ୍ଷର stringpool_str40[ଆକାର("RTMIN+8")];
    ଅକ୍ଷର stringpool_str41[ଆକାର("RTMIN+7")];
    ଅକ୍ଷର stringpool_str42[ଆକାର("RTMAX-8")];
    ଅକ୍ଷର stringpool_str43[ଆକାର("RTMAX-7")];
    ଅକ୍ଷର stringpool_str44[ଆକାର("RTMIN+6")];
    ଅକ୍ଷର stringpool_str45[ଆକାର("RTMIN+10")];
    ଅକ୍ଷର stringpool_str46[ଆକାର("RTMAX-6")];
    ଅକ୍ଷର stringpool_str47[ଆକାର("RTMAX-10")];
    ଅକ୍ଷର stringpool_str48[ଆକାର("SYS")];
    ଅକ୍ଷର stringpool_str49[ଆକାର("XFSZ")];
    ଅକ୍ଷର stringpool_str50[ଆକାର("RTMAX")];
    ଅକ୍ଷର stringpool_str51[ଆକାର("PWR")];
    ଅକ୍ଷର stringpool_str52[ଆକାର("SEGV")];
    ଅକ୍ଷର stringpool_str53[ଆକାର("XCPU")];
    ଅକ୍ଷର stringpool_str54[ଆକାର("WINCH")];
    ଅକ୍ଷର stringpool_str55[ଆକାର("URG")];
    ଅକ୍ଷର stringpool_str56[ଆକାର("CHLD")];
    ଅକ୍ଷର stringpool_str57[ଆକାର("TTIN")];
    ଅକ୍ଷର stringpool_str58[ଆକାର("PIPE")];
    ଅକ୍ଷର stringpool_str59[ଆକାର("ABRT")];
    ଅକ୍ଷର stringpool_str60[ଆକାର("TERM")];
    ଅକ୍ଷର stringpool_str61[ଆକାର("PROF")];
  };
ସ୍ଥିର ଅଚଳ ସଂରଚନା stringpool_t stringpool_contents =
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
#define stringpool ((ଅଚଳ ଅକ୍ଷର *) &stringpool_contents)

ସ୍ଥିର ଅଚଳ ସଂରଚନା signal_s wordlist[] =
  {
#line 36 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str0, 4},
#line 33 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str1, 1},
#line 41 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str2, 9},
#line 54 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str3, 22},
#line 35 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str4, 3},
#line 51 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str5, 19},
#line 66 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str6, 36},
#line 76 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str7, 46},
#line 92 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str8, 62},
#line 82 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str9, 52},
#line 65 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str10, 35},
#line 75 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str11, 45},
#line 93 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str12, 63},
#line 83 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str13, 53},
#line 68 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str14, 38},
#line 78 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str15, 48},
#line 90 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str16, 60},
#line 80 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str17, 50},
#line 67 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str18, 37},
#line 77 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str19, 47},
#line 91 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str20, 61},
#line 81 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str21, 51},
#line 39 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str22, 7},
#line 58 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str23, 26},
#line 34 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str24, 2},
#line 40 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str25, 8},
#line 50 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str26, 18},
#line 48 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str27, 16},
#line 44 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str28, 12},
#line 37 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str29, 5},
#line 52 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str30, 20},
#line 64 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str31, 34},
#line 69 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str32, 39},
#line 79 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str33, 49},
#line 89 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str34, 59},
#line 73 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str35, 43},
#line 42 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str36, 10},
#line 85 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str37, 55},
#line 46 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str38, 14},
#line 61 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str39, 29},
#line 72 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str40, 42},
#line 71 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str41, 41},
#line 86 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str42, 56},
#line 87 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str43, 57},
#line 70 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str44, 40},
#line 74 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str45, 44},
#line 88 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str46, 58},
#line 84 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str47, 54},
#line 63 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str48, 31},
#line 57 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str49, 25},
#line 94 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str50, 64},
#line 62 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str51, 30},
#line 43 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str52, 11},
#line 56 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str53, 24},
#line 60 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str54, 28},
#line 55 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str55, 23},
#line 49 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str56, 17},
#line 53 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str57, 21},
#line 45 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str58, 13},
#line 38 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str59, 6},
#line 47 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str60, 15},
#line 59 "src/libcrun/signals.perf"
    {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)&((ସଂରଚନା stringpool_t *)0)->stringpool_str61, 27}
  };

ଅଚଳ ସଂରଚନା signal_s *
libcrun_signal_in_word_set (ପଞ୍ଜିକରଣ ଅଚଳ ଅକ୍ଷର *str, ପଞ୍ଜିକରଣ size_t len)
{
  ଯଦି (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      ପଞ୍ଜିକରଣ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଚାବି = hash (str, len);

      ଯଦି (ଚାବି <= MAX_HASH_ମୂଲ୍ୟ && ଚାବି >= MIN_HASH_ମୂଲ୍ୟ)
        {
          ପଞ୍ଜିକରଣ ଅଚଳ ସଂରଚନା signal_s *resword;

          ବାଛ (ଚାବି - 13)
            {
              ସ୍ଥିତି 0:
                resword = &wordlist[0];
                ଯାଅ compare;
              ସ୍ଥିତି 1:
                resword = &wordlist[1];
                ଯାଅ compare;
              ସ୍ଥିତି 6:
                resword = &wordlist[2];
                ଯାଅ compare;
              ସ୍ଥିତି 7:
                resword = &wordlist[3];
                ଯାଅ compare;
              ସ୍ଥିତି 8:
                resword = &wordlist[4];
                ଯାଅ compare;
              ସ୍ଥିତି 9:
                resword = &wordlist[5];
                ଯାଅ compare;
              ସ୍ଥିତି 10:
                resword = &wordlist[6];
                ଯାଅ compare;
              ସ୍ଥିତି 11:
                resword = &wordlist[7];
                ଯାଅ compare;
              ସ୍ଥିତି 12:
                resword = &wordlist[8];
                ଯାଅ compare;
              ସ୍ଥିତି 13:
                resword = &wordlist[9];
                ଯାଅ compare;
              ସ୍ଥିତି 14:
                resword = &wordlist[10];
                ଯାଅ compare;
              ସ୍ଥିତି 15:
                resword = &wordlist[11];
                ଯାଅ compare;
              ସ୍ଥିତି 16:
                resword = &wordlist[12];
                ଯାଅ compare;
              ସ୍ଥିତି 17:
                resword = &wordlist[13];
                ଯାଅ compare;
              ସ୍ଥିତି 18:
                resword = &wordlist[14];
                ଯାଅ compare;
              ସ୍ଥିତି 19:
                resword = &wordlist[15];
                ଯାଅ compare;
              ସ୍ଥିତି 20:
                resword = &wordlist[16];
                ଯାଅ compare;
              ସ୍ଥିତି 21:
                resword = &wordlist[17];
                ଯାଅ compare;
              ସ୍ଥିତି 22:
                resword = &wordlist[18];
                ଯାଅ compare;
              ସ୍ଥିତି 23:
                resword = &wordlist[19];
                ଯାଅ compare;
              ସ୍ଥିତି 24:
                resword = &wordlist[20];
                ଯାଅ compare;
              ସ୍ଥିତି 25:
                resword = &wordlist[21];
                ଯାଅ compare;
              ସ୍ଥିତି 26:
                resword = &wordlist[22];
                ଯାଅ compare;
              ସ୍ଥିତି 27:
                resword = &wordlist[23];
                ଯାଅ compare;
              ସ୍ଥିତି 28:
                resword = &wordlist[24];
                ଯାଅ compare;
              ସ୍ଥିତି 29:
                resword = &wordlist[25];
                ଯାଅ compare;
              ସ୍ଥିତି 30:
                resword = &wordlist[26];
                ଯାଅ compare;
              ସ୍ଥିତି 31:
                resword = &wordlist[27];
                ଯାଅ compare;
              ସ୍ଥିତି 32:
                resword = &wordlist[28];
                ଯାଅ compare;
              ସ୍ଥିତି 33:
                resword = &wordlist[29];
                ଯାଅ compare;
              ସ୍ଥିତି 34:
                resword = &wordlist[30];
                ଯାଅ compare;
              ସ୍ଥିତି 35:
                resword = &wordlist[31];
                ଯାଅ compare;
              ସ୍ଥିତି 36:
                resword = &wordlist[32];
                ଯାଅ compare;
              ସ୍ଥିତି 37:
                resword = &wordlist[33];
                ଯାଅ compare;
              ସ୍ଥିତି 38:
                resword = &wordlist[34];
                ଯାଅ compare;
              ସ୍ଥିତି 39:
                resword = &wordlist[35];
                ଯାଅ compare;
              ସ୍ଥିତି 40:
                resword = &wordlist[36];
                ଯାଅ compare;
              ସ୍ଥିତି 41:
                resword = &wordlist[37];
                ଯାଅ compare;
              ସ୍ଥିତି 42:
                resword = &wordlist[38];
                ଯାଅ compare;
              ସ୍ଥିତି 43:
                resword = &wordlist[39];
                ଯାଅ compare;
              ସ୍ଥିତି 44:
                resword = &wordlist[40];
                ଯାଅ compare;
              ସ୍ଥିତି 45:
                resword = &wordlist[41];
                ଯାଅ compare;
              ସ୍ଥିତି 46:
                resword = &wordlist[42];
                ଯାଅ compare;
              ସ୍ଥିତି 47:
                resword = &wordlist[43];
                ଯାଅ compare;
              ସ୍ଥିତି 48:
                resword = &wordlist[44];
                ଯାଅ compare;
              ସ୍ଥିତି 49:
                resword = &wordlist[45];
                ଯାଅ compare;
              ସ୍ଥିତି 50:
                resword = &wordlist[46];
                ଯାଅ compare;
              ସ୍ଥିତି 51:
                resword = &wordlist[47];
                ଯାଅ compare;
              ସ୍ଥିତି 52:
                resword = &wordlist[48];
                ଯାଅ compare;
              ସ୍ଥିତି 53:
                resword = &wordlist[49];
                ଯାଅ compare;
              ସ୍ଥିତି 54:
                resword = &wordlist[50];
                ଯାଅ compare;
              ସ୍ଥିତି 55:
                resword = &wordlist[51];
                ଯାଅ compare;
              ସ୍ଥିତି 56:
                resword = &wordlist[52];
                ଯାଅ compare;
              ସ୍ଥିତି 57:
                resword = &wordlist[53];
                ଯାଅ compare;
              ସ୍ଥିତି 58:
                resword = &wordlist[54];
                ଯାଅ compare;
              ସ୍ଥିତି 59:
                resword = &wordlist[55];
                ଯାଅ compare;
              ସ୍ଥିତି 60:
                resword = &wordlist[56];
                ଯାଅ compare;
              ସ୍ଥିତି 61:
                resword = &wordlist[57];
                ଯାଅ compare;
              ସ୍ଥିତି 62:
                resword = &wordlist[58];
                ଯାଅ compare;
              ସ୍ଥିତି 63:
                resword = &wordlist[59];
                ଯାଅ compare;
              ସ୍ଥିତି 70:
                resword = &wordlist[60];
                ଯାଅ compare;
              ସ୍ଥିତି 73:
                resword = &wordlist[61];
                ଯାଅ compare;
            }
          ଫେରନ୍ତୁ 0;
        compare:
          {
            ପଞ୍ଜିକରଣ ଅଚଳ ଅକ୍ଷର *s = resword->name + stringpool;

            ଯଦି (*str == *s && !strcmp (str + 1, s + 1))
              ଫେରନ୍ତୁ resword;
          }
        }
    }
  ଫେରନ୍ତୁ 0;
}
#line 95 "src/libcrun/signals.perf"

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
str2sig (ଅଚଳ ଅକ୍ଷର *name)
{
  ଅଚଳ ସଂରଚନା signal_s *s;

  ଯଦି (has_prefix (name, "SIG"))
    name += 3;

  s = libcrun_signal_in_word_set (name, strlen (name));
  ଯଦି (s == ଖାଲି)
    {
      long ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ;

      ଯଦି (!isdigit(name[0]))
        {
          errno = EINVAL;
          ଫେରନ୍ତୁ -1;
        }

      errno = 0;
      ମୂଲ୍ୟ = strtol (name, ଖାଲି, 10);
      ଯଦି (errno != 0)
        ଫେରନ୍ତୁ -1;

      ଫେରନ୍ତୁ ମୂଲ୍ୟ;
    }

  ଫେରନ୍ତୁ s->ମୂଲ୍ୟ;
}
