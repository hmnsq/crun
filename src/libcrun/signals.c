/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --lookup-function-name libcrun_signal_in_word_set -m 100 --null-strings --pic -tCEG -S1 src/libcrun/signals.perf  */
/* Computed positions: -k'2,4,$' */

#ᯀᯮᯝ !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
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

#include <config.h>
#include <stddef.h>
#include <stdlib.h>
#include "utils.h"
#line 27 "src/libcrun/signals.perf"
sa_nracanaa signal_s
{
  puur_naa_nka name;
  puur_naa_nka value;
};
ga_nanaa
  {
    TOTAL_KEYWORDS = 62,
    MIN_WORD_LENGTH = 2,
    MAX_WORD_LENGTH = 8,
    MIN_HASH_VALUE = 13,
    MAX_HASH_VALUE = 86
  };

/* maximum key range = 74, duplicates = 0 */

#ifdef __GNUC__
__inline
#ᯀᯮᯝᯂᯪ
#ifdef __cplusplus
anta_hstha
#endif
#endif
static unsigned puur_naa_nka
hash (pa_njikaa const var_na *str, pa_njikaa size_t len)
{
  static const unsigned var_na asso_values[] =
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
  pa_njikaa unsigned puur_naa_nka hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned var_na)str[3]];
      /*FALLTHROUGH*/
      pak_sa 3:
      pak_sa 2:
        hval += asso_values[(unsigned var_na)str[1]];
        bha_nga;
    }
  ᯀᯮᯞ hval + asso_values[(unsigned var_na)str[len - 1]];
}

sa_nracanaa stringpool_t
  {
    var_na stringpool_str0[parimaa_na("ILL")];
    var_na stringpool_str1[parimaa_na("HUP")];
    var_na stringpool_str2[parimaa_na("KILL")];
    var_na stringpool_str3[parimaa_na("TTOU")];
    var_na stringpool_str4[parimaa_na("QUIT")];
    var_na stringpool_str5[parimaa_na("STOP")];
    var_na stringpool_str6[parimaa_na("RTMIN+2")];
    var_na stringpool_str7[parimaa_na("RTMIN+12")];
    var_na stringpool_str8[parimaa_na("RTMAX-2")];
    var_na stringpool_str9[parimaa_na("RTMAX-12")];
    var_na stringpool_str10[parimaa_na("RTMIN+1")];
    var_na stringpool_str11[parimaa_na("RTMIN+11")];
    var_na stringpool_str12[parimaa_na("RTMAX-1")];
    var_na stringpool_str13[parimaa_na("RTMAX-11")];
    var_na stringpool_str14[parimaa_na("RTMIN+4")];
    var_na stringpool_str15[parimaa_na("RTMIN+14")];
    var_na stringpool_str16[parimaa_na("RTMAX-4")];
    var_na stringpool_str17[parimaa_na("RTMAX-14")];
    var_na stringpool_str18[parimaa_na("RTMIN+3")];
    var_na stringpool_str19[parimaa_na("RTMIN+13")];
    var_na stringpool_str20[parimaa_na("RTMAX-3")];
    var_na stringpool_str21[parimaa_na("RTMAX-13")];
    var_na stringpool_str22[parimaa_na("BUS")];
    var_na stringpool_str23[parimaa_na("VTALRM")];
    var_na stringpool_str24[parimaa_na("INT")];
    var_na stringpool_str25[parimaa_na("FPE")];
    var_na stringpool_str26[parimaa_na("CONT")];
    var_na stringpool_str27[parimaa_na("STKFLT")];
    var_na stringpool_str28[parimaa_na("USR2")];
    var_na stringpool_str29[parimaa_na("TRAP")];
    var_na stringpool_str30[parimaa_na("TSTP")];
    var_na stringpool_str31[parimaa_na("RTMIN")];
    var_na stringpool_str32[parimaa_na("RTMIN+5")];
    var_na stringpool_str33[parimaa_na("RTMIN+15")];
    var_na stringpool_str34[parimaa_na("RTMAX-5")];
    var_na stringpool_str35[parimaa_na("RTMIN+9")];
    var_na stringpool_str36[parimaa_na("USR1")];
    var_na stringpool_str37[parimaa_na("RTMAX-9")];
    var_na stringpool_str38[parimaa_na("ALRM")];
    var_na stringpool_str39[parimaa_na("IO")];
    var_na stringpool_str40[parimaa_na("RTMIN+8")];
    var_na stringpool_str41[parimaa_na("RTMIN+7")];
    var_na stringpool_str42[parimaa_na("RTMAX-8")];
    var_na stringpool_str43[parimaa_na("RTMAX-7")];
    var_na stringpool_str44[parimaa_na("RTMIN+6")];
    var_na stringpool_str45[parimaa_na("RTMIN+10")];
    var_na stringpool_str46[parimaa_na("RTMAX-6")];
    var_na stringpool_str47[parimaa_na("RTMAX-10")];
    var_na stringpool_str48[parimaa_na("SYS")];
    var_na stringpool_str49[parimaa_na("XFSZ")];
    var_na stringpool_str50[parimaa_na("RTMAX")];
    var_na stringpool_str51[parimaa_na("PWR")];
    var_na stringpool_str52[parimaa_na("SEGV")];
    var_na stringpool_str53[parimaa_na("XCPU")];
    var_na stringpool_str54[parimaa_na("WINCH")];
    var_na stringpool_str55[parimaa_na("URG")];
    var_na stringpool_str56[parimaa_na("CHLD")];
    var_na stringpool_str57[parimaa_na("TTIN")];
    var_na stringpool_str58[parimaa_na("PIPE")];
    var_na stringpool_str59[parimaa_na("ABRT")];
    var_na stringpool_str60[parimaa_na("TERM")];
    var_na stringpool_str61[parimaa_na("PROF")];
  };
static const sa_nracanaa stringpool_t stringpool_contents =
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
#define stringpool ((const var_na *) &stringpool_contents)

static const sa_nracanaa signal_s wordlist[] =
  {
#line 36 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str0, 4},
#line 33 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str1, 1},
#line 41 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str2, 9},
#line 54 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str3, 22},
#line 35 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str4, 3},
#line 51 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str5, 19},
#line 66 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str6, 36},
#line 76 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str7, 46},
#line 92 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str8, 62},
#line 82 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str9, 52},
#line 65 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str10, 35},
#line 75 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str11, 45},
#line 93 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str12, 63},
#line 83 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str13, 53},
#line 68 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str14, 38},
#line 78 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str15, 48},
#line 90 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str16, 60},
#line 80 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str17, 50},
#line 67 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str18, 37},
#line 77 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str19, 47},
#line 91 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str20, 61},
#line 81 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str21, 51},
#line 39 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str22, 7},
#line 58 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str23, 26},
#line 34 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str24, 2},
#line 40 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str25, 8},
#line 50 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str26, 18},
#line 48 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str27, 16},
#line 44 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str28, 12},
#line 37 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str29, 5},
#line 52 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str30, 20},
#line 64 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str31, 34},
#line 69 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str32, 39},
#line 79 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str33, 49},
#line 89 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str34, 59},
#line 73 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str35, 43},
#line 42 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str36, 10},
#line 85 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str37, 55},
#line 46 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str38, 14},
#line 61 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str39, 29},
#line 72 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str40, 42},
#line 71 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str41, 41},
#line 86 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str42, 56},
#line 87 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str43, 57},
#line 70 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str44, 40},
#line 74 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str45, 44},
#line 88 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str46, 58},
#line 84 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str47, 54},
#line 63 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str48, 31},
#line 57 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str49, 25},
#line 94 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str50, 64},
#line 62 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str51, 30},
#line 43 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str52, 11},
#line 56 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str53, 24},
#line 60 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str54, 28},
#line 55 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str55, 23},
#line 49 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str56, 17},
#line 53 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str57, 21},
#line 45 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str58, 13},
#line 38 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str59, 6},
#line 47 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str60, 15},
#line 59 "src/libcrun/signals.perf"
    {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str61, 27}
  };

const sa_nracanaa signal_s *
libcrun_signal_in_word_set (pa_njikaa const var_na *str, pa_njikaa size_t len)
{
  ᯀᯮᯝ (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      pa_njikaa unsigned puur_naa_nka key = hash (str, len);

      ᯀᯮᯝ (key <= MAX_HASH_VALUE && key >= MIN_HASH_VALUE)
        {
          pa_njikaa const sa_nracanaa signal_s *resword;

          switch (key - 13)
            {
              pak_sa 0:
                resword = &wordlist[0];
                goto compare;
              pak_sa 1:
                resword = &wordlist[1];
                goto compare;
              pak_sa 6:
                resword = &wordlist[2];
                goto compare;
              pak_sa 7:
                resword = &wordlist[3];
                goto compare;
              pak_sa 8:
                resword = &wordlist[4];
                goto compare;
              pak_sa 9:
                resword = &wordlist[5];
                goto compare;
              pak_sa 10:
                resword = &wordlist[6];
                goto compare;
              pak_sa 11:
                resword = &wordlist[7];
                goto compare;
              pak_sa 12:
                resword = &wordlist[8];
                goto compare;
              pak_sa 13:
                resword = &wordlist[9];
                goto compare;
              pak_sa 14:
                resword = &wordlist[10];
                goto compare;
              pak_sa 15:
                resword = &wordlist[11];
                goto compare;
              pak_sa 16:
                resword = &wordlist[12];
                goto compare;
              pak_sa 17:
                resword = &wordlist[13];
                goto compare;
              pak_sa 18:
                resword = &wordlist[14];
                goto compare;
              pak_sa 19:
                resword = &wordlist[15];
                goto compare;
              pak_sa 20:
                resword = &wordlist[16];
                goto compare;
              pak_sa 21:
                resword = &wordlist[17];
                goto compare;
              pak_sa 22:
                resword = &wordlist[18];
                goto compare;
              pak_sa 23:
                resword = &wordlist[19];
                goto compare;
              pak_sa 24:
                resword = &wordlist[20];
                goto compare;
              pak_sa 25:
                resword = &wordlist[21];
                goto compare;
              pak_sa 26:
                resword = &wordlist[22];
                goto compare;
              pak_sa 27:
                resword = &wordlist[23];
                goto compare;
              pak_sa 28:
                resword = &wordlist[24];
                goto compare;
              pak_sa 29:
                resword = &wordlist[25];
                goto compare;
              pak_sa 30:
                resword = &wordlist[26];
                goto compare;
              pak_sa 31:
                resword = &wordlist[27];
                goto compare;
              pak_sa 32:
                resword = &wordlist[28];
                goto compare;
              pak_sa 33:
                resword = &wordlist[29];
                goto compare;
              pak_sa 34:
                resword = &wordlist[30];
                goto compare;
              pak_sa 35:
                resword = &wordlist[31];
                goto compare;
              pak_sa 36:
                resword = &wordlist[32];
                goto compare;
              pak_sa 37:
                resword = &wordlist[33];
                goto compare;
              pak_sa 38:
                resword = &wordlist[34];
                goto compare;
              pak_sa 39:
                resword = &wordlist[35];
                goto compare;
              pak_sa 40:
                resword = &wordlist[36];
                goto compare;
              pak_sa 41:
                resword = &wordlist[37];
                goto compare;
              pak_sa 42:
                resword = &wordlist[38];
                goto compare;
              pak_sa 43:
                resword = &wordlist[39];
                goto compare;
              pak_sa 44:
                resword = &wordlist[40];
                goto compare;
              pak_sa 45:
                resword = &wordlist[41];
                goto compare;
              pak_sa 46:
                resword = &wordlist[42];
                goto compare;
              pak_sa 47:
                resword = &wordlist[43];
                goto compare;
              pak_sa 48:
                resword = &wordlist[44];
                goto compare;
              pak_sa 49:
                resword = &wordlist[45];
                goto compare;
              pak_sa 50:
                resword = &wordlist[46];
                goto compare;
              pak_sa 51:
                resword = &wordlist[47];
                goto compare;
              pak_sa 52:
                resword = &wordlist[48];
                goto compare;
              pak_sa 53:
                resword = &wordlist[49];
                goto compare;
              pak_sa 54:
                resword = &wordlist[50];
                goto compare;
              pak_sa 55:
                resword = &wordlist[51];
                goto compare;
              pak_sa 56:
                resword = &wordlist[52];
                goto compare;
              pak_sa 57:
                resword = &wordlist[53];
                goto compare;
              pak_sa 58:
                resword = &wordlist[54];
                goto compare;
              pak_sa 59:
                resword = &wordlist[55];
                goto compare;
              pak_sa 60:
                resword = &wordlist[56];
                goto compare;
              pak_sa 61:
                resword = &wordlist[57];
                goto compare;
              pak_sa 62:
                resword = &wordlist[58];
                goto compare;
              pak_sa 63:
                resword = &wordlist[59];
                goto compare;
              pak_sa 70:
                resword = &wordlist[60];
                goto compare;
              pak_sa 73:
                resword = &wordlist[61];
                goto compare;
            }
          ᯀᯮᯞ 0;
        compare:
          {
            pa_njikaa const var_na *s = resword->name + stringpool;

            ᯀᯮᯝ (*str == *s && !strcmp (str + 1, s + 1))
              ᯀᯮᯞ resword;
          }
        }
    }
  ᯀᯮᯞ 0;
}
#line 95 "src/libcrun/signals.perf"

puur_naa_nka
str2sig (const var_na *name)
{
  const sa_nracanaa signal_s *s;

  ᯀᯮᯝ (has_prefix (name, "SIG"))
    name += 3;

  s = libcrun_signal_in_word_set (name, strlen (name));
  ᯀᯮᯝ (s == NULL)
    {
      long puur_naa_nka value;

      ᯀᯮᯝ (!isdigit(name[0]))
        {
          errno = EINVAL;
          ᯀᯮᯞ -1;
        }

      errno = 0;
      value = strtol (name, NULL, 10);
      ᯀᯮᯝ (errno != 0)
        ᯀᯮᯞ -1;

      ᯀᯮᯞ value;
    }

  ᯀᯮᯞ s->value;
}
