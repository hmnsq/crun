/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --lookup-function-name libcrun_signal_in_word_set -m 100 --null-strings --pic -tCEG -S1 src/libcrun/signals.perf  */
/* Computed positions: -k'2,4,$' */

#जर !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stddef.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव "utils.h"
#line 27 "src/libcrun/signals.perf"
संरचना signal_s
{
  पूर्णांक name;
  पूर्णांक मूल्य;
};
गणना
  {
    TOTAL_KEYWORDS = 62,
    MIN_WORD_LENGTH = 2,
    MAX_WORD_LENGTH = 8,
    MIN_HASH_मूल्य = 13,
    MAX_HASH_मूल्य = 86
  };

/* maximum key range = 74, duplicates = 0 */

#ifdef __GNUC__
__inline
#अन्यथा
#ifdef __cplusplus
ओळीत
#endif
#endif
स्थिर अचिन्हित पूर्णांक
hash (नोंदणी स्थिरांक अक्षर *str, नोंदणी माप_प्रकार len)
{
  स्थिर स्थिरांक अचिन्हित अक्षर asso_मूल्ये[] =
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
  नोंदणी अचिन्हित पूर्णांक hval = len;

  निवड (hval)
    {
      पूर्वनिर्धारित:
        hval += asso_मूल्ये[(अचिन्हित अक्षर)str[3]];
      /*FALLTHROUGH*/
      स्थिती 3:
      स्थिती 2:
        hval += asso_मूल्ये[(अचिन्हित अक्षर)str[1]];
        थांबा;
    }
  परत hval + asso_मूल्ये[(अचिन्हित अक्षर)str[len - 1]];
}

संरचना stringpool_t
  {
    अक्षर stringpool_str0[आकार("ILL")];
    अक्षर stringpool_str1[आकार("HUP")];
    अक्षर stringpool_str2[आकार("KILL")];
    अक्षर stringpool_str3[आकार("TTOU")];
    अक्षर stringpool_str4[आकार("QUIT")];
    अक्षर stringpool_str5[आकार("STOP")];
    अक्षर stringpool_str6[आकार("RTMIN+2")];
    अक्षर stringpool_str7[आकार("RTMIN+12")];
    अक्षर stringpool_str8[आकार("RTMAX-2")];
    अक्षर stringpool_str9[आकार("RTMAX-12")];
    अक्षर stringpool_str10[आकार("RTMIN+1")];
    अक्षर stringpool_str11[आकार("RTMIN+11")];
    अक्षर stringpool_str12[आकार("RTMAX-1")];
    अक्षर stringpool_str13[आकार("RTMAX-11")];
    अक्षर stringpool_str14[आकार("RTMIN+4")];
    अक्षर stringpool_str15[आकार("RTMIN+14")];
    अक्षर stringpool_str16[आकार("RTMAX-4")];
    अक्षर stringpool_str17[आकार("RTMAX-14")];
    अक्षर stringpool_str18[आकार("RTMIN+3")];
    अक्षर stringpool_str19[आकार("RTMIN+13")];
    अक्षर stringpool_str20[आकार("RTMAX-3")];
    अक्षर stringpool_str21[आकार("RTMAX-13")];
    अक्षर stringpool_str22[आकार("BUS")];
    अक्षर stringpool_str23[आकार("VTALRM")];
    अक्षर stringpool_str24[आकार("INT")];
    अक्षर stringpool_str25[आकार("FPE")];
    अक्षर stringpool_str26[आकार("CONT")];
    अक्षर stringpool_str27[आकार("STKFLT")];
    अक्षर stringpool_str28[आकार("USR2")];
    अक्षर stringpool_str29[आकार("TRAP")];
    अक्षर stringpool_str30[आकार("TSTP")];
    अक्षर stringpool_str31[आकार("RTMIN")];
    अक्षर stringpool_str32[आकार("RTMIN+5")];
    अक्षर stringpool_str33[आकार("RTMIN+15")];
    अक्षर stringpool_str34[आकार("RTMAX-5")];
    अक्षर stringpool_str35[आकार("RTMIN+9")];
    अक्षर stringpool_str36[आकार("USR1")];
    अक्षर stringpool_str37[आकार("RTMAX-9")];
    अक्षर stringpool_str38[आकार("ALRM")];
    अक्षर stringpool_str39[आकार("IO")];
    अक्षर stringpool_str40[आकार("RTMIN+8")];
    अक्षर stringpool_str41[आकार("RTMIN+7")];
    अक्षर stringpool_str42[आकार("RTMAX-8")];
    अक्षर stringpool_str43[आकार("RTMAX-7")];
    अक्षर stringpool_str44[आकार("RTMIN+6")];
    अक्षर stringpool_str45[आकार("RTMIN+10")];
    अक्षर stringpool_str46[आकार("RTMAX-6")];
    अक्षर stringpool_str47[आकार("RTMAX-10")];
    अक्षर stringpool_str48[आकार("SYS")];
    अक्षर stringpool_str49[आकार("XFSZ")];
    अक्षर stringpool_str50[आकार("RTMAX")];
    अक्षर stringpool_str51[आकार("PWR")];
    अक्षर stringpool_str52[आकार("SEGV")];
    अक्षर stringpool_str53[आकार("XCPU")];
    अक्षर stringpool_str54[आकार("WINCH")];
    अक्षर stringpool_str55[आकार("URG")];
    अक्षर stringpool_str56[आकार("CHLD")];
    अक्षर stringpool_str57[आकार("TTIN")];
    अक्षर stringpool_str58[आकार("PIPE")];
    अक्षर stringpool_str59[आकार("ABRT")];
    अक्षर stringpool_str60[आकार("TERM")];
    अक्षर stringpool_str61[आकार("PROF")];
  };
स्थिर स्थिरांक संरचना stringpool_t stringpool_contents =
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
#define stringpool ((स्थिरांक अक्षर *) &stringpool_contents)

स्थिर स्थिरांक संरचना signal_s wordlist[] =
  {
#line 36 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str0, 4},
#line 33 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str1, 1},
#line 41 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str2, 9},
#line 54 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str3, 22},
#line 35 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str4, 3},
#line 51 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str5, 19},
#line 66 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str6, 36},
#line 76 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str7, 46},
#line 92 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str8, 62},
#line 82 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str9, 52},
#line 65 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str10, 35},
#line 75 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str11, 45},
#line 93 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str12, 63},
#line 83 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str13, 53},
#line 68 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str14, 38},
#line 78 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str15, 48},
#line 90 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str16, 60},
#line 80 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str17, 50},
#line 67 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str18, 37},
#line 77 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str19, 47},
#line 91 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str20, 61},
#line 81 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str21, 51},
#line 39 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str22, 7},
#line 58 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str23, 26},
#line 34 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str24, 2},
#line 40 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str25, 8},
#line 50 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str26, 18},
#line 48 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str27, 16},
#line 44 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str28, 12},
#line 37 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str29, 5},
#line 52 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str30, 20},
#line 64 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str31, 34},
#line 69 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str32, 39},
#line 79 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str33, 49},
#line 89 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str34, 59},
#line 73 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str35, 43},
#line 42 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str36, 10},
#line 85 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str37, 55},
#line 46 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str38, 14},
#line 61 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str39, 29},
#line 72 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str40, 42},
#line 71 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str41, 41},
#line 86 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str42, 56},
#line 87 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str43, 57},
#line 70 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str44, 40},
#line 74 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str45, 44},
#line 88 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str46, 58},
#line 84 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str47, 54},
#line 63 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str48, 31},
#line 57 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str49, 25},
#line 94 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str50, 64},
#line 62 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str51, 30},
#line 43 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str52, 11},
#line 56 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str53, 24},
#line 60 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str54, 28},
#line 55 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str55, 23},
#line 49 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str56, 17},
#line 53 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str57, 21},
#line 45 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str58, 13},
#line 38 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str59, 6},
#line 47 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str60, 15},
#line 59 "src/libcrun/signals.perf"
    {(पूर्णांक)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str61, 27}
  };

स्थिरांक संरचना signal_s *
libcrun_signal_in_word_set (नोंदणी स्थिरांक अक्षर *str, नोंदणी माप_प्रकार len)
{
  जर (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      नोंदणी अचिन्हित पूर्णांक किल्ली = hash (str, len);

      जर (किल्ली <= MAX_HASH_मूल्य && किल्ली >= MIN_HASH_मूल्य)
        {
          नोंदणी स्थिरांक संरचना signal_s *resword;

          निवड (किल्ली - 13)
            {
              स्थिती 0:
                resword = &wordlist[0];
                जा compare;
              स्थिती 1:
                resword = &wordlist[1];
                जा compare;
              स्थिती 6:
                resword = &wordlist[2];
                जा compare;
              स्थिती 7:
                resword = &wordlist[3];
                जा compare;
              स्थिती 8:
                resword = &wordlist[4];
                जा compare;
              स्थिती 9:
                resword = &wordlist[5];
                जा compare;
              स्थिती 10:
                resword = &wordlist[6];
                जा compare;
              स्थिती 11:
                resword = &wordlist[7];
                जा compare;
              स्थिती 12:
                resword = &wordlist[8];
                जा compare;
              स्थिती 13:
                resword = &wordlist[9];
                जा compare;
              स्थिती 14:
                resword = &wordlist[10];
                जा compare;
              स्थिती 15:
                resword = &wordlist[11];
                जा compare;
              स्थिती 16:
                resword = &wordlist[12];
                जा compare;
              स्थिती 17:
                resword = &wordlist[13];
                जा compare;
              स्थिती 18:
                resword = &wordlist[14];
                जा compare;
              स्थिती 19:
                resword = &wordlist[15];
                जा compare;
              स्थिती 20:
                resword = &wordlist[16];
                जा compare;
              स्थिती 21:
                resword = &wordlist[17];
                जा compare;
              स्थिती 22:
                resword = &wordlist[18];
                जा compare;
              स्थिती 23:
                resword = &wordlist[19];
                जा compare;
              स्थिती 24:
                resword = &wordlist[20];
                जा compare;
              स्थिती 25:
                resword = &wordlist[21];
                जा compare;
              स्थिती 26:
                resword = &wordlist[22];
                जा compare;
              स्थिती 27:
                resword = &wordlist[23];
                जा compare;
              स्थिती 28:
                resword = &wordlist[24];
                जा compare;
              स्थिती 29:
                resword = &wordlist[25];
                जा compare;
              स्थिती 30:
                resword = &wordlist[26];
                जा compare;
              स्थिती 31:
                resword = &wordlist[27];
                जा compare;
              स्थिती 32:
                resword = &wordlist[28];
                जा compare;
              स्थिती 33:
                resword = &wordlist[29];
                जा compare;
              स्थिती 34:
                resword = &wordlist[30];
                जा compare;
              स्थिती 35:
                resword = &wordlist[31];
                जा compare;
              स्थिती 36:
                resword = &wordlist[32];
                जा compare;
              स्थिती 37:
                resword = &wordlist[33];
                जा compare;
              स्थिती 38:
                resword = &wordlist[34];
                जा compare;
              स्थिती 39:
                resword = &wordlist[35];
                जा compare;
              स्थिती 40:
                resword = &wordlist[36];
                जा compare;
              स्थिती 41:
                resword = &wordlist[37];
                जा compare;
              स्थिती 42:
                resword = &wordlist[38];
                जा compare;
              स्थिती 43:
                resword = &wordlist[39];
                जा compare;
              स्थिती 44:
                resword = &wordlist[40];
                जा compare;
              स्थिती 45:
                resword = &wordlist[41];
                जा compare;
              स्थिती 46:
                resword = &wordlist[42];
                जा compare;
              स्थिती 47:
                resword = &wordlist[43];
                जा compare;
              स्थिती 48:
                resword = &wordlist[44];
                जा compare;
              स्थिती 49:
                resword = &wordlist[45];
                जा compare;
              स्थिती 50:
                resword = &wordlist[46];
                जा compare;
              स्थिती 51:
                resword = &wordlist[47];
                जा compare;
              स्थिती 52:
                resword = &wordlist[48];
                जा compare;
              स्थिती 53:
                resword = &wordlist[49];
                जा compare;
              स्थिती 54:
                resword = &wordlist[50];
                जा compare;
              स्थिती 55:
                resword = &wordlist[51];
                जा compare;
              स्थिती 56:
                resword = &wordlist[52];
                जा compare;
              स्थिती 57:
                resword = &wordlist[53];
                जा compare;
              स्थिती 58:
                resword = &wordlist[54];
                जा compare;
              स्थिती 59:
                resword = &wordlist[55];
                जा compare;
              स्थिती 60:
                resword = &wordlist[56];
                जा compare;
              स्थिती 61:
                resword = &wordlist[57];
                जा compare;
              स्थिती 62:
                resword = &wordlist[58];
                जा compare;
              स्थिती 63:
                resword = &wordlist[59];
                जा compare;
              स्थिती 70:
                resword = &wordlist[60];
                जा compare;
              स्थिती 73:
                resword = &wordlist[61];
                जा compare;
            }
          परत 0;
        compare:
          {
            नोंदणी स्थिरांक अक्षर *s = resword->name + stringpool;

            जर (*str == *s && !म_भेद (str + 1, s + 1))
              परत resword;
          }
        }
    }
  परत 0;
}
#line 95 "src/libcrun/signals.perf"

पूर्णांक
str2sig (स्थिरांक अक्षर *name)
{
  स्थिरांक संरचना signal_s *s;

  जर (has_prefix (name, "SIG"))
    name += 3;

  s = libcrun_signal_in_word_set (name, म_माप (name));
  जर (s == रिक्त)
    {
      दीर्घ पूर्णांक मूल्य;

      जर (!है_अंक(name[0]))
        {
          त्रुटि_सं = EINVAL;
          परत -1;
        }

      त्रुटि_सं = 0;
      मूल्य = म_से_दीर्घ (name, रिक्त, 10);
      जर (त्रुटि_सं != 0)
        परत -1;

      परत मूल्य;
    }

  परत s->मूल्य;
}
