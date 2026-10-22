/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --lookup-function-name libcrun_signal_in_word_set -m 100 --null-strings --pic -tCEG -S1 src/libcrun/signals.perf  */
/* Computed positions: -k'2,4,$' */

#എങ്കിൽ !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stddef.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ "utils.h"
#line 27 "src/libcrun/signals.perf"
ഘടന signal_s
{
  പൂർണ്ണസംഖ്യ name;
  പൂർണ്ണസംഖ്യ മൂല്യം;
};
എണ്ണൽ
  {
    TOTAL_KEYWORDS = 62,
    MIN_WORD_LENGTH = 2,
    MAX_WORD_LENGTH = 8,
    MIN_HASH_മൂല്യം = 13,
    MAX_HASH_മൂല്യം = 86
  };

/* maximum key range = 74, duplicates = 0 */

#ifdef __GNUC__
__inline
#അല്ലെങ്കിൽ
#ifdef __cplusplus
വരിയിൽ
#endif
#endif
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
hash (രജിസ്റ്റർ സ്ഥിരം അക്ഷരം *str, രജിസ്റ്റർ size_t len)
{
  സ്ഥിതം സ്ഥിരം unsigned അക്ഷരം asso_മൂല്യങ്ങൾ[] =
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
  രജിസ്റ്റർ unsigned പൂർണ്ണസംഖ്യ hval = len;

  തിരഞ്ഞെടുപ്പ് (hval)
    {
      സ്ഥിരസ്ഥിതി:
        hval += asso_മൂല്യങ്ങൾ[(unsigned അക്ഷരം)str[3]];
      /*FALLTHROUGH*/
      അവസ്ഥ 3:
      അവസ്ഥ 2:
        hval += asso_മൂല്യങ്ങൾ[(unsigned അക്ഷരം)str[1]];
        നിർത്തുക;
    }
  തിരികെ hval + asso_മൂല്യങ്ങൾ[(unsigned അക്ഷരം)str[len - 1]];
}

ഘടന stringpool_t
  {
    അക്ഷരം stringpool_str0[വലുപ്പം("ILL")];
    അക്ഷരം stringpool_str1[വലുപ്പം("HUP")];
    അക്ഷരം stringpool_str2[വലുപ്പം("KILL")];
    അക്ഷരം stringpool_str3[വലുപ്പം("TTOU")];
    അക്ഷരം stringpool_str4[വലുപ്പം("QUIT")];
    അക്ഷരം stringpool_str5[വലുപ്പം("STOP")];
    അക്ഷരം stringpool_str6[വലുപ്പം("RTMIN+2")];
    അക്ഷരം stringpool_str7[വലുപ്പം("RTMIN+12")];
    അക്ഷരം stringpool_str8[വലുപ്പം("RTMAX-2")];
    അക്ഷരം stringpool_str9[വലുപ്പം("RTMAX-12")];
    അക്ഷരം stringpool_str10[വലുപ്പം("RTMIN+1")];
    അക്ഷരം stringpool_str11[വലുപ്പം("RTMIN+11")];
    അക്ഷരം stringpool_str12[വലുപ്പം("RTMAX-1")];
    അക്ഷരം stringpool_str13[വലുപ്പം("RTMAX-11")];
    അക്ഷരം stringpool_str14[വലുപ്പം("RTMIN+4")];
    അക്ഷരം stringpool_str15[വലുപ്പം("RTMIN+14")];
    അക്ഷരം stringpool_str16[വലുപ്പം("RTMAX-4")];
    അക്ഷരം stringpool_str17[വലുപ്പം("RTMAX-14")];
    അക്ഷരം stringpool_str18[വലുപ്പം("RTMIN+3")];
    അക്ഷരം stringpool_str19[വലുപ്പം("RTMIN+13")];
    അക്ഷരം stringpool_str20[വലുപ്പം("RTMAX-3")];
    അക്ഷരം stringpool_str21[വലുപ്പം("RTMAX-13")];
    അക്ഷരം stringpool_str22[വലുപ്പം("BUS")];
    അക്ഷരം stringpool_str23[വലുപ്പം("VTALRM")];
    അക്ഷരം stringpool_str24[വലുപ്പം("INT")];
    അക്ഷരം stringpool_str25[വലുപ്പം("FPE")];
    അക്ഷരം stringpool_str26[വലുപ്പം("CONT")];
    അക്ഷരം stringpool_str27[വലുപ്പം("STKFLT")];
    അക്ഷരം stringpool_str28[വലുപ്പം("USR2")];
    അക്ഷരം stringpool_str29[വലുപ്പം("TRAP")];
    അക്ഷരം stringpool_str30[വലുപ്പം("TSTP")];
    അക്ഷരം stringpool_str31[വലുപ്പം("RTMIN")];
    അക്ഷരം stringpool_str32[വലുപ്പം("RTMIN+5")];
    അക്ഷരം stringpool_str33[വലുപ്പം("RTMIN+15")];
    അക്ഷരം stringpool_str34[വലുപ്പം("RTMAX-5")];
    അക്ഷരം stringpool_str35[വലുപ്പം("RTMIN+9")];
    അക്ഷരം stringpool_str36[വലുപ്പം("USR1")];
    അക്ഷരം stringpool_str37[വലുപ്പം("RTMAX-9")];
    അക്ഷരം stringpool_str38[വലുപ്പം("ALRM")];
    അക്ഷരം stringpool_str39[വലുപ്പം("IO")];
    അക്ഷരം stringpool_str40[വലുപ്പം("RTMIN+8")];
    അക്ഷരം stringpool_str41[വലുപ്പം("RTMIN+7")];
    അക്ഷരം stringpool_str42[വലുപ്പം("RTMAX-8")];
    അക്ഷരം stringpool_str43[വലുപ്പം("RTMAX-7")];
    അക്ഷരം stringpool_str44[വലുപ്പം("RTMIN+6")];
    അക്ഷരം stringpool_str45[വലുപ്പം("RTMIN+10")];
    അക്ഷരം stringpool_str46[വലുപ്പം("RTMAX-6")];
    അക്ഷരം stringpool_str47[വലുപ്പം("RTMAX-10")];
    അക്ഷരം stringpool_str48[വലുപ്പം("SYS")];
    അക്ഷരം stringpool_str49[വലുപ്പം("XFSZ")];
    അക്ഷരം stringpool_str50[വലുപ്പം("RTMAX")];
    അക്ഷരം stringpool_str51[വലുപ്പം("PWR")];
    അക്ഷരം stringpool_str52[വലുപ്പം("SEGV")];
    അക്ഷരം stringpool_str53[വലുപ്പം("XCPU")];
    അക്ഷരം stringpool_str54[വലുപ്പം("WINCH")];
    അക്ഷരം stringpool_str55[വലുപ്പം("URG")];
    അക്ഷരം stringpool_str56[വലുപ്പം("CHLD")];
    അക്ഷരം stringpool_str57[വലുപ്പം("TTIN")];
    അക്ഷരം stringpool_str58[വലുപ്പം("PIPE")];
    അക്ഷരം stringpool_str59[വലുപ്പം("ABRT")];
    അക്ഷരം stringpool_str60[വലുപ്പം("TERM")];
    അക്ഷരം stringpool_str61[വലുപ്പം("PROF")];
  };
സ്ഥിതം സ്ഥിരം ഘടന stringpool_t stringpool_contents =
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
#define stringpool ((സ്ഥിരം അക്ഷരം *) &stringpool_contents)

സ്ഥിതം സ്ഥിരം ഘടന signal_s wordlist[] =
  {
#line 36 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str0, 4},
#line 33 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str1, 1},
#line 41 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str2, 9},
#line 54 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str3, 22},
#line 35 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str4, 3},
#line 51 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str5, 19},
#line 66 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str6, 36},
#line 76 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str7, 46},
#line 92 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str8, 62},
#line 82 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str9, 52},
#line 65 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str10, 35},
#line 75 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str11, 45},
#line 93 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str12, 63},
#line 83 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str13, 53},
#line 68 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str14, 38},
#line 78 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str15, 48},
#line 90 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str16, 60},
#line 80 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str17, 50},
#line 67 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str18, 37},
#line 77 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str19, 47},
#line 91 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str20, 61},
#line 81 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str21, 51},
#line 39 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str22, 7},
#line 58 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str23, 26},
#line 34 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str24, 2},
#line 40 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str25, 8},
#line 50 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str26, 18},
#line 48 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str27, 16},
#line 44 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str28, 12},
#line 37 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str29, 5},
#line 52 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str30, 20},
#line 64 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str31, 34},
#line 69 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str32, 39},
#line 79 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str33, 49},
#line 89 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str34, 59},
#line 73 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str35, 43},
#line 42 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str36, 10},
#line 85 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str37, 55},
#line 46 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str38, 14},
#line 61 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str39, 29},
#line 72 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str40, 42},
#line 71 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str41, 41},
#line 86 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str42, 56},
#line 87 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str43, 57},
#line 70 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str44, 40},
#line 74 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str45, 44},
#line 88 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str46, 58},
#line 84 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str47, 54},
#line 63 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str48, 31},
#line 57 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str49, 25},
#line 94 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str50, 64},
#line 62 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str51, 30},
#line 43 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str52, 11},
#line 56 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str53, 24},
#line 60 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str54, 28},
#line 55 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str55, 23},
#line 49 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str56, 17},
#line 53 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str57, 21},
#line 45 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str58, 13},
#line 38 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str59, 6},
#line 47 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str60, 15},
#line 59 "src/libcrun/signals.perf"
    {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str61, 27}
  };

സ്ഥിരം ഘടന signal_s *
libcrun_signal_in_word_set (രജിസ്റ്റർ സ്ഥിരം അക്ഷരം *str, രജിസ്റ്റർ size_t len)
{
  എങ്കിൽ (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      രജിസ്റ്റർ unsigned പൂർണ്ണസംഖ്യ താക്കോൽ = hash (str, len);

      എങ്കിൽ (താക്കോൽ <= MAX_HASH_മൂല്യം && താക്കോൽ >= MIN_HASH_മൂല്യം)
        {
          രജിസ്റ്റർ സ്ഥിരം ഘടന signal_s *resword;

          തിരഞ്ഞെടുപ്പ് (താക്കോൽ - 13)
            {
              അവസ്ഥ 0:
                resword = &wordlist[0];
                പോകുക compare;
              അവസ്ഥ 1:
                resword = &wordlist[1];
                പോകുക compare;
              അവസ്ഥ 6:
                resword = &wordlist[2];
                പോകുക compare;
              അവസ്ഥ 7:
                resword = &wordlist[3];
                പോകുക compare;
              അവസ്ഥ 8:
                resword = &wordlist[4];
                പോകുക compare;
              അവസ്ഥ 9:
                resword = &wordlist[5];
                പോകുക compare;
              അവസ്ഥ 10:
                resword = &wordlist[6];
                പോകുക compare;
              അവസ്ഥ 11:
                resword = &wordlist[7];
                പോകുക compare;
              അവസ്ഥ 12:
                resword = &wordlist[8];
                പോകുക compare;
              അവസ്ഥ 13:
                resword = &wordlist[9];
                പോകുക compare;
              അവസ്ഥ 14:
                resword = &wordlist[10];
                പോകുക compare;
              അവസ്ഥ 15:
                resword = &wordlist[11];
                പോകുക compare;
              അവസ്ഥ 16:
                resword = &wordlist[12];
                പോകുക compare;
              അവസ്ഥ 17:
                resword = &wordlist[13];
                പോകുക compare;
              അവസ്ഥ 18:
                resword = &wordlist[14];
                പോകുക compare;
              അവസ്ഥ 19:
                resword = &wordlist[15];
                പോകുക compare;
              അവസ്ഥ 20:
                resword = &wordlist[16];
                പോകുക compare;
              അവസ്ഥ 21:
                resword = &wordlist[17];
                പോകുക compare;
              അവസ്ഥ 22:
                resword = &wordlist[18];
                പോകുക compare;
              അവസ്ഥ 23:
                resword = &wordlist[19];
                പോകുക compare;
              അവസ്ഥ 24:
                resword = &wordlist[20];
                പോകുക compare;
              അവസ്ഥ 25:
                resword = &wordlist[21];
                പോകുക compare;
              അവസ്ഥ 26:
                resword = &wordlist[22];
                പോകുക compare;
              അവസ്ഥ 27:
                resword = &wordlist[23];
                പോകുക compare;
              അവസ്ഥ 28:
                resword = &wordlist[24];
                പോകുക compare;
              അവസ്ഥ 29:
                resword = &wordlist[25];
                പോകുക compare;
              അവസ്ഥ 30:
                resword = &wordlist[26];
                പോകുക compare;
              അവസ്ഥ 31:
                resword = &wordlist[27];
                പോകുക compare;
              അവസ്ഥ 32:
                resword = &wordlist[28];
                പോകുക compare;
              അവസ്ഥ 33:
                resword = &wordlist[29];
                പോകുക compare;
              അവസ്ഥ 34:
                resword = &wordlist[30];
                പോകുക compare;
              അവസ്ഥ 35:
                resword = &wordlist[31];
                പോകുക compare;
              അവസ്ഥ 36:
                resword = &wordlist[32];
                പോകുക compare;
              അവസ്ഥ 37:
                resword = &wordlist[33];
                പോകുക compare;
              അവസ്ഥ 38:
                resword = &wordlist[34];
                പോകുക compare;
              അവസ്ഥ 39:
                resword = &wordlist[35];
                പോകുക compare;
              അവസ്ഥ 40:
                resword = &wordlist[36];
                പോകുക compare;
              അവസ്ഥ 41:
                resword = &wordlist[37];
                പോകുക compare;
              അവസ്ഥ 42:
                resword = &wordlist[38];
                പോകുക compare;
              അവസ്ഥ 43:
                resword = &wordlist[39];
                പോകുക compare;
              അവസ്ഥ 44:
                resword = &wordlist[40];
                പോകുക compare;
              അവസ്ഥ 45:
                resword = &wordlist[41];
                പോകുക compare;
              അവസ്ഥ 46:
                resword = &wordlist[42];
                പോകുക compare;
              അവസ്ഥ 47:
                resword = &wordlist[43];
                പോകുക compare;
              അവസ്ഥ 48:
                resword = &wordlist[44];
                പോകുക compare;
              അവസ്ഥ 49:
                resword = &wordlist[45];
                പോകുക compare;
              അവസ്ഥ 50:
                resword = &wordlist[46];
                പോകുക compare;
              അവസ്ഥ 51:
                resword = &wordlist[47];
                പോകുക compare;
              അവസ്ഥ 52:
                resword = &wordlist[48];
                പോകുക compare;
              അവസ്ഥ 53:
                resword = &wordlist[49];
                പോകുക compare;
              അവസ്ഥ 54:
                resword = &wordlist[50];
                പോകുക compare;
              അവസ്ഥ 55:
                resword = &wordlist[51];
                പോകുക compare;
              അവസ്ഥ 56:
                resword = &wordlist[52];
                പോകുക compare;
              അവസ്ഥ 57:
                resword = &wordlist[53];
                പോകുക compare;
              അവസ്ഥ 58:
                resword = &wordlist[54];
                പോകുക compare;
              അവസ്ഥ 59:
                resword = &wordlist[55];
                പോകുക compare;
              അവസ്ഥ 60:
                resword = &wordlist[56];
                പോകുക compare;
              അവസ്ഥ 61:
                resword = &wordlist[57];
                പോകുക compare;
              അവസ്ഥ 62:
                resword = &wordlist[58];
                പോകുക compare;
              അവസ്ഥ 63:
                resword = &wordlist[59];
                പോകുക compare;
              അവസ്ഥ 70:
                resword = &wordlist[60];
                പോകുക compare;
              അവസ്ഥ 73:
                resword = &wordlist[61];
                പോകുക compare;
            }
          തിരികെ 0;
        compare:
          {
            രജിസ്റ്റർ സ്ഥിരം അക്ഷരം *s = resword->name + stringpool;

            എങ്കിൽ (*str == *s && !strcmp (str + 1, s + 1))
              തിരികെ resword;
          }
        }
    }
  തിരികെ 0;
}
#line 95 "src/libcrun/signals.perf"

പൂർണ്ണസംഖ്യ
str2sig (സ്ഥിരം അക്ഷരം *name)
{
  സ്ഥിരം ഘടന signal_s *s;

  എങ്കിൽ (has_prefix (name, "SIG"))
    name += 3;

  s = libcrun_signal_in_word_set (name, strlen (name));
  എങ്കിൽ (s == ശൂന്യ)
    {
      long പൂർണ്ണസംഖ്യ മൂല്യം;

      എങ്കിൽ (!isdigit(name[0]))
        {
          errno = EINVAL;
          തിരികെ -1;
        }

      errno = 0;
      മൂല്യം = strtol (name, ശൂന്യ, 10);
      എങ്കിൽ (errno != 0)
        തിരികെ -1;

      തിരികെ മൂല്യം;
    }

  തിരികെ s->മൂല്യം;
}
