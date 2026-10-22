/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --lookup-function-name libcrun_signal_in_word_set -m 100 --null-strings --pic -tCEG -S1 src/libcrun/signals.perf  */
/* Computed positions: -k'2,4,$' */

#ಯದಿ !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ "utils.h"
#line 27 "src/libcrun/signals.perf"
ಸಂರಚನಾ signal_s
{
  ಪೂರ್ಣಾಙ್ಕ name;
  ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ;
};
ಗಣನಾ
  {
    TOTAL_KEYWORDS = 62,
    MIN_WORD_LENGTH = 2,
    MAX_WORD_LENGTH = 8,
    MIN_HASH_ಮೌಲ್ಯ = 13,
    MAX_HASH_ಮೌಲ್ಯ = 86
  };

/* maximum key range = 74, duplicates = 0 */

#ifdef __GNUC__
__inline
#ಅನ್ಯಥಾ
#ifdef __cplusplus
ಅನ್ತಃಸ್ಥ
#endif
#endif
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ
hash (ಪಞ್ಜಿಕಾ ಧ್ರುವ ವರ್ಣ *str, ಪಞ್ಜಿಕಾ size_t len)
{
  ಸ್ಥಿರ ಧ್ರುವ unsigned ವರ್ಣ asso_ಮೌಲ್ಯಗಳು[] =
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
  ಪಞ್ಜಿಕಾ unsigned ಪೂರ್ಣಾಙ್ಕ hval = len;

  ವಿಕಲ್ಪ (hval)
    {
      ಸ್ವಭಾವ:
        hval += asso_ಮೌಲ್ಯಗಳು[(unsigned ವರ್ಣ)str[3]];
      /*FALLTHROUGH*/
      ಪಕ್ಷ 3:
      ಪಕ್ಷ 2:
        hval += asso_ಮೌಲ್ಯಗಳು[(unsigned ವರ್ಣ)str[1]];
        ಭಙ್ಗ;
    }
  ಪ್ರತ್ಯಾಗಮ hval + asso_ಮೌಲ್ಯಗಳು[(unsigned ವರ್ಣ)str[len - 1]];
}

ಸಂರಚನಾ stringpool_t
  {
    ವರ್ಣ stringpool_str0[ಪರಿಮಾಣ("ILL")];
    ವರ್ಣ stringpool_str1[ಪರಿಮಾಣ("HUP")];
    ವರ್ಣ stringpool_str2[ಪರಿಮಾಣ("KILL")];
    ವರ್ಣ stringpool_str3[ಪರಿಮಾಣ("TTOU")];
    ವರ್ಣ stringpool_str4[ಪರಿಮಾಣ("QUIT")];
    ವರ್ಣ stringpool_str5[ಪರಿಮಾಣ("STOP")];
    ವರ್ಣ stringpool_str6[ಪರಿಮಾಣ("RTMIN+2")];
    ವರ್ಣ stringpool_str7[ಪರಿಮಾಣ("RTMIN+12")];
    ವರ್ಣ stringpool_str8[ಪರಿಮಾಣ("RTMAX-2")];
    ವರ್ಣ stringpool_str9[ಪರಿಮಾಣ("RTMAX-12")];
    ವರ್ಣ stringpool_str10[ಪರಿಮಾಣ("RTMIN+1")];
    ವರ್ಣ stringpool_str11[ಪರಿಮಾಣ("RTMIN+11")];
    ವರ್ಣ stringpool_str12[ಪರಿಮಾಣ("RTMAX-1")];
    ವರ್ಣ stringpool_str13[ಪರಿಮಾಣ("RTMAX-11")];
    ವರ್ಣ stringpool_str14[ಪರಿಮಾಣ("RTMIN+4")];
    ವರ್ಣ stringpool_str15[ಪರಿಮಾಣ("RTMIN+14")];
    ವರ್ಣ stringpool_str16[ಪರಿಮಾಣ("RTMAX-4")];
    ವರ್ಣ stringpool_str17[ಪರಿಮಾಣ("RTMAX-14")];
    ವರ್ಣ stringpool_str18[ಪರಿಮಾಣ("RTMIN+3")];
    ವರ್ಣ stringpool_str19[ಪರಿಮಾಣ("RTMIN+13")];
    ವರ್ಣ stringpool_str20[ಪರಿಮಾಣ("RTMAX-3")];
    ವರ್ಣ stringpool_str21[ಪರಿಮಾಣ("RTMAX-13")];
    ವರ್ಣ stringpool_str22[ಪರಿಮಾಣ("BUS")];
    ವರ್ಣ stringpool_str23[ಪರಿಮಾಣ("VTALRM")];
    ವರ್ಣ stringpool_str24[ಪರಿಮಾಣ("INT")];
    ವರ್ಣ stringpool_str25[ಪರಿಮಾಣ("FPE")];
    ವರ್ಣ stringpool_str26[ಪರಿಮಾಣ("CONT")];
    ವರ್ಣ stringpool_str27[ಪರಿಮಾಣ("STKFLT")];
    ವರ್ಣ stringpool_str28[ಪರಿಮಾಣ("USR2")];
    ವರ್ಣ stringpool_str29[ಪರಿಮಾಣ("TRAP")];
    ವರ್ಣ stringpool_str30[ಪರಿಮಾಣ("TSTP")];
    ವರ್ಣ stringpool_str31[ಪರಿಮಾಣ("RTMIN")];
    ವರ್ಣ stringpool_str32[ಪರಿಮಾಣ("RTMIN+5")];
    ವರ್ಣ stringpool_str33[ಪರಿಮಾಣ("RTMIN+15")];
    ವರ್ಣ stringpool_str34[ಪರಿಮಾಣ("RTMAX-5")];
    ವರ್ಣ stringpool_str35[ಪರಿಮಾಣ("RTMIN+9")];
    ವರ್ಣ stringpool_str36[ಪರಿಮಾಣ("USR1")];
    ವರ್ಣ stringpool_str37[ಪರಿಮಾಣ("RTMAX-9")];
    ವರ್ಣ stringpool_str38[ಪರಿಮಾಣ("ALRM")];
    ವರ್ಣ stringpool_str39[ಪರಿಮಾಣ("IO")];
    ವರ್ಣ stringpool_str40[ಪರಿಮಾಣ("RTMIN+8")];
    ವರ್ಣ stringpool_str41[ಪರಿಮಾಣ("RTMIN+7")];
    ವರ್ಣ stringpool_str42[ಪರಿಮಾಣ("RTMAX-8")];
    ವರ್ಣ stringpool_str43[ಪರಿಮಾಣ("RTMAX-7")];
    ವರ್ಣ stringpool_str44[ಪರಿಮಾಣ("RTMIN+6")];
    ವರ್ಣ stringpool_str45[ಪರಿಮಾಣ("RTMIN+10")];
    ವರ್ಣ stringpool_str46[ಪರಿಮಾಣ("RTMAX-6")];
    ವರ್ಣ stringpool_str47[ಪರಿಮಾಣ("RTMAX-10")];
    ವರ್ಣ stringpool_str48[ಪರಿಮಾಣ("SYS")];
    ವರ್ಣ stringpool_str49[ಪರಿಮಾಣ("XFSZ")];
    ವರ್ಣ stringpool_str50[ಪರಿಮಾಣ("RTMAX")];
    ವರ್ಣ stringpool_str51[ಪರಿಮಾಣ("PWR")];
    ವರ್ಣ stringpool_str52[ಪರಿಮಾಣ("SEGV")];
    ವರ್ಣ stringpool_str53[ಪರಿಮಾಣ("XCPU")];
    ವರ್ಣ stringpool_str54[ಪರಿಮಾಣ("WINCH")];
    ವರ್ಣ stringpool_str55[ಪರಿಮಾಣ("URG")];
    ವರ್ಣ stringpool_str56[ಪರಿಮಾಣ("CHLD")];
    ವರ್ಣ stringpool_str57[ಪರಿಮಾಣ("TTIN")];
    ವರ್ಣ stringpool_str58[ಪರಿಮಾಣ("PIPE")];
    ವರ್ಣ stringpool_str59[ಪರಿಮಾಣ("ABRT")];
    ವರ್ಣ stringpool_str60[ಪರಿಮಾಣ("TERM")];
    ವರ್ಣ stringpool_str61[ಪರಿಮಾಣ("PROF")];
  };
ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ stringpool_t stringpool_contents =
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
#define stringpool ((ಧ್ರುವ ವರ್ಣ *) &stringpool_contents)

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ signal_s wordlist[] =
  {
#line 36 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str0, 4},
#line 33 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str1, 1},
#line 41 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str2, 9},
#line 54 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str3, 22},
#line 35 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str4, 3},
#line 51 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str5, 19},
#line 66 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str6, 36},
#line 76 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str7, 46},
#line 92 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str8, 62},
#line 82 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str9, 52},
#line 65 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str10, 35},
#line 75 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str11, 45},
#line 93 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str12, 63},
#line 83 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str13, 53},
#line 68 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str14, 38},
#line 78 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str15, 48},
#line 90 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str16, 60},
#line 80 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str17, 50},
#line 67 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str18, 37},
#line 77 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str19, 47},
#line 91 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str20, 61},
#line 81 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str21, 51},
#line 39 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str22, 7},
#line 58 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str23, 26},
#line 34 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str24, 2},
#line 40 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str25, 8},
#line 50 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str26, 18},
#line 48 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str27, 16},
#line 44 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str28, 12},
#line 37 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str29, 5},
#line 52 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str30, 20},
#line 64 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str31, 34},
#line 69 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str32, 39},
#line 79 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str33, 49},
#line 89 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str34, 59},
#line 73 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str35, 43},
#line 42 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str36, 10},
#line 85 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str37, 55},
#line 46 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str38, 14},
#line 61 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str39, 29},
#line 72 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str40, 42},
#line 71 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str41, 41},
#line 86 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str42, 56},
#line 87 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str43, 57},
#line 70 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str44, 40},
#line 74 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str45, 44},
#line 88 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str46, 58},
#line 84 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str47, 54},
#line 63 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str48, 31},
#line 57 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str49, 25},
#line 94 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str50, 64},
#line 62 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str51, 30},
#line 43 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str52, 11},
#line 56 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str53, 24},
#line 60 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str54, 28},
#line 55 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str55, 23},
#line 49 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str56, 17},
#line 53 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str57, 21},
#line 45 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str58, 13},
#line 38 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str59, 6},
#line 47 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str60, 15},
#line 59 "src/libcrun/signals.perf"
    {(ಪೂರ್ಣಾಙ್ಕ)(size_t)&((ಸಂರಚನಾ stringpool_t *)0)->stringpool_str61, 27}
  };

ಧ್ರುವ ಸಂರಚನಾ signal_s *
libcrun_signal_in_word_set (ಪಞ್ಜಿಕಾ ಧ್ರುವ ವರ್ಣ *str, ಪಞ್ಜಿಕಾ size_t len)
{
  ಯದಿ (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      ಪಞ್ಜಿಕಾ unsigned ಪೂರ್ಣಾಙ್ಕ ಕೀಲಿ = hash (str, len);

      ಯದಿ (ಕೀಲಿ <= MAX_HASH_ಮೌಲ್ಯ && ಕೀಲಿ >= MIN_HASH_ಮೌಲ್ಯ)
        {
          ಪಞ್ಜಿಕಾ ಧ್ರುವ ಸಂರಚನಾ signal_s *resword;

          ವಿಕಲ್ಪ (ಕೀಲಿ - 13)
            {
              ಪಕ್ಷ 0:
                resword = &wordlist[0];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 1:
                resword = &wordlist[1];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 6:
                resword = &wordlist[2];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 7:
                resword = &wordlist[3];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 8:
                resword = &wordlist[4];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 9:
                resword = &wordlist[5];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 10:
                resword = &wordlist[6];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 11:
                resword = &wordlist[7];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 12:
                resword = &wordlist[8];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 13:
                resword = &wordlist[9];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 14:
                resword = &wordlist[10];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 15:
                resword = &wordlist[11];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 16:
                resword = &wordlist[12];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 17:
                resword = &wordlist[13];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 18:
                resword = &wordlist[14];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 19:
                resword = &wordlist[15];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 20:
                resword = &wordlist[16];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 21:
                resword = &wordlist[17];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 22:
                resword = &wordlist[18];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 23:
                resword = &wordlist[19];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 24:
                resword = &wordlist[20];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 25:
                resword = &wordlist[21];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 26:
                resword = &wordlist[22];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 27:
                resword = &wordlist[23];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 28:
                resword = &wordlist[24];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 29:
                resword = &wordlist[25];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 30:
                resword = &wordlist[26];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 31:
                resword = &wordlist[27];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 32:
                resword = &wordlist[28];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 33:
                resword = &wordlist[29];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 34:
                resword = &wordlist[30];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 35:
                resword = &wordlist[31];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 36:
                resword = &wordlist[32];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 37:
                resword = &wordlist[33];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 38:
                resword = &wordlist[34];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 39:
                resword = &wordlist[35];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 40:
                resword = &wordlist[36];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 41:
                resword = &wordlist[37];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 42:
                resword = &wordlist[38];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 43:
                resword = &wordlist[39];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 44:
                resword = &wordlist[40];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 45:
                resword = &wordlist[41];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 46:
                resword = &wordlist[42];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 47:
                resword = &wordlist[43];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 48:
                resword = &wordlist[44];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 49:
                resword = &wordlist[45];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 50:
                resword = &wordlist[46];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 51:
                resword = &wordlist[47];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 52:
                resword = &wordlist[48];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 53:
                resword = &wordlist[49];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 54:
                resword = &wordlist[50];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 55:
                resword = &wordlist[51];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 56:
                resword = &wordlist[52];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 57:
                resword = &wordlist[53];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 58:
                resword = &wordlist[54];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 59:
                resword = &wordlist[55];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 60:
                resword = &wordlist[56];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 61:
                resword = &wordlist[57];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 62:
                resword = &wordlist[58];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 63:
                resword = &wordlist[59];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 70:
                resword = &wordlist[60];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 73:
                resword = &wordlist[61];
                ಗಚ್ಛ compare;
            }
          ಪ್ರತ್ಯಾಗಮ 0;
        compare:
          {
            ಪಞ್ಜಿಕಾ ಧ್ರುವ ವರ್ಣ *s = resword->name + stringpool;

            ಯದಿ (*str == *s && !strcmp (str + 1, s + 1))
              ಪ್ರತ್ಯಾಗಮ resword;
          }
        }
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}
#line 95 "src/libcrun/signals.perf"

ಪೂರ್ಣಾಙ್ಕ
str2sig (ಧ್ರುವ ವರ್ಣ *name)
{
  ಧ್ರುವ ಸಂರಚನಾ signal_s *s;

  ಯದಿ (has_prefix (name, "SIG"))
    name += 3;

  s = libcrun_signal_in_word_set (name, strlen (name));
  ಯದಿ (s == ರಿಕ್ತ)
    {
      long ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ;

      ಯದಿ (!isdigit(name[0]))
        {
          errno = EINVAL;
          ಪ್ರತ್ಯಾಗಮ -1;
        }

      errno = 0;
      ಮೌಲ್ಯ = strtol (name, ರಿಕ್ತ, 10);
      ಯದಿ (errno != 0)
        ಪ್ರತ್ಯಾಗಮ -1;

      ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
    }

  ಪ್ರತ್ಯಾಗಮ s->ಮೌಲ್ಯ;
}
