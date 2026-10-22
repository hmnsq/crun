/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --lookup-function-name libcrun_signal_in_word_set -m 100 --null-strings --pic -tCEG -S1 src/libcrun/signals.perf  */
/* Computed positions: -k'2,4,$' */

#यदि !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
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
  पूर्णाङ्क name;
  पूर्णाङ्क मूल्य;
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
अन्तःस्थ
#endif
#endif
स्थिर अचिन्हित पूर्णाङ्क
hash (पञ्जिका ध्रुव वर्ण *str, पञ्जिका माप_प्रकार len)
{
  स्थिर ध्रुव अचिन्हित वर्ण asso_मूल्यावली[] =
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
  पञ्जिका अचिन्हित पूर्णाङ्क hval = len;

  विकल्प (hval)
    {
      स्वभाव:
        hval += asso_मूल्यावली[(अचिन्हित वर्ण)str[3]];
      /*FALLTHROUGH*/
      पक्ष 3:
      पक्ष 2:
        hval += asso_मूल्यावली[(अचिन्हित वर्ण)str[1]];
        भङ्ग;
    }
  प्रत्यागम hval + asso_मूल्यावली[(अचिन्हित वर्ण)str[len - 1]];
}

संरचना stringpool_t
  {
    वर्ण stringpool_str0[परिमाण("ILL")];
    वर्ण stringpool_str1[परिमाण("HUP")];
    वर्ण stringpool_str2[परिमाण("KILL")];
    वर्ण stringpool_str3[परिमाण("TTOU")];
    वर्ण stringpool_str4[परिमाण("QUIT")];
    वर्ण stringpool_str5[परिमाण("STOP")];
    वर्ण stringpool_str6[परिमाण("RTMIN+2")];
    वर्ण stringpool_str7[परिमाण("RTMIN+12")];
    वर्ण stringpool_str8[परिमाण("RTMAX-2")];
    वर्ण stringpool_str9[परिमाण("RTMAX-12")];
    वर्ण stringpool_str10[परिमाण("RTMIN+1")];
    वर्ण stringpool_str11[परिमाण("RTMIN+11")];
    वर्ण stringpool_str12[परिमाण("RTMAX-1")];
    वर्ण stringpool_str13[परिमाण("RTMAX-11")];
    वर्ण stringpool_str14[परिमाण("RTMIN+4")];
    वर्ण stringpool_str15[परिमाण("RTMIN+14")];
    वर्ण stringpool_str16[परिमाण("RTMAX-4")];
    वर्ण stringpool_str17[परिमाण("RTMAX-14")];
    वर्ण stringpool_str18[परिमाण("RTMIN+3")];
    वर्ण stringpool_str19[परिमाण("RTMIN+13")];
    वर्ण stringpool_str20[परिमाण("RTMAX-3")];
    वर्ण stringpool_str21[परिमाण("RTMAX-13")];
    वर्ण stringpool_str22[परिमाण("BUS")];
    वर्ण stringpool_str23[परिमाण("VTALRM")];
    वर्ण stringpool_str24[परिमाण("INT")];
    वर्ण stringpool_str25[परिमाण("FPE")];
    वर्ण stringpool_str26[परिमाण("CONT")];
    वर्ण stringpool_str27[परिमाण("STKFLT")];
    वर्ण stringpool_str28[परिमाण("USR2")];
    वर्ण stringpool_str29[परिमाण("TRAP")];
    वर्ण stringpool_str30[परिमाण("TSTP")];
    वर्ण stringpool_str31[परिमाण("RTMIN")];
    वर्ण stringpool_str32[परिमाण("RTMIN+5")];
    वर्ण stringpool_str33[परिमाण("RTMIN+15")];
    वर्ण stringpool_str34[परिमाण("RTMAX-5")];
    वर्ण stringpool_str35[परिमाण("RTMIN+9")];
    वर्ण stringpool_str36[परिमाण("USR1")];
    वर्ण stringpool_str37[परिमाण("RTMAX-9")];
    वर्ण stringpool_str38[परिमाण("ALRM")];
    वर्ण stringpool_str39[परिमाण("IO")];
    वर्ण stringpool_str40[परिमाण("RTMIN+8")];
    वर्ण stringpool_str41[परिमाण("RTMIN+7")];
    वर्ण stringpool_str42[परिमाण("RTMAX-8")];
    वर्ण stringpool_str43[परिमाण("RTMAX-7")];
    वर्ण stringpool_str44[परिमाण("RTMIN+6")];
    वर्ण stringpool_str45[परिमाण("RTMIN+10")];
    वर्ण stringpool_str46[परिमाण("RTMAX-6")];
    वर्ण stringpool_str47[परिमाण("RTMAX-10")];
    वर्ण stringpool_str48[परिमाण("SYS")];
    वर्ण stringpool_str49[परिमाण("XFSZ")];
    वर्ण stringpool_str50[परिमाण("RTMAX")];
    वर्ण stringpool_str51[परिमाण("PWR")];
    वर्ण stringpool_str52[परिमाण("SEGV")];
    वर्ण stringpool_str53[परिमाण("XCPU")];
    वर्ण stringpool_str54[परिमाण("WINCH")];
    वर्ण stringpool_str55[परिमाण("URG")];
    वर्ण stringpool_str56[परिमाण("CHLD")];
    वर्ण stringpool_str57[परिमाण("TTIN")];
    वर्ण stringpool_str58[परिमाण("PIPE")];
    वर्ण stringpool_str59[परिमाण("ABRT")];
    वर्ण stringpool_str60[परिमाण("TERM")];
    वर्ण stringpool_str61[परिमाण("PROF")];
  };
स्थिर ध्रुव संरचना stringpool_t stringpool_contents =
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
#define stringpool ((ध्रुव वर्ण *) &stringpool_contents)

स्थिर ध्रुव संरचना signal_s wordlist[] =
  {
#line 36 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str0, 4},
#line 33 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str1, 1},
#line 41 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str2, 9},
#line 54 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str3, 22},
#line 35 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str4, 3},
#line 51 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str5, 19},
#line 66 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str6, 36},
#line 76 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str7, 46},
#line 92 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str8, 62},
#line 82 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str9, 52},
#line 65 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str10, 35},
#line 75 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str11, 45},
#line 93 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str12, 63},
#line 83 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str13, 53},
#line 68 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str14, 38},
#line 78 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str15, 48},
#line 90 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str16, 60},
#line 80 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str17, 50},
#line 67 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str18, 37},
#line 77 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str19, 47},
#line 91 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str20, 61},
#line 81 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str21, 51},
#line 39 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str22, 7},
#line 58 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str23, 26},
#line 34 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str24, 2},
#line 40 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str25, 8},
#line 50 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str26, 18},
#line 48 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str27, 16},
#line 44 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str28, 12},
#line 37 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str29, 5},
#line 52 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str30, 20},
#line 64 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str31, 34},
#line 69 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str32, 39},
#line 79 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str33, 49},
#line 89 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str34, 59},
#line 73 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str35, 43},
#line 42 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str36, 10},
#line 85 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str37, 55},
#line 46 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str38, 14},
#line 61 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str39, 29},
#line 72 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str40, 42},
#line 71 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str41, 41},
#line 86 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str42, 56},
#line 87 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str43, 57},
#line 70 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str44, 40},
#line 74 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str45, 44},
#line 88 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str46, 58},
#line 84 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str47, 54},
#line 63 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str48, 31},
#line 57 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str49, 25},
#line 94 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str50, 64},
#line 62 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str51, 30},
#line 43 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str52, 11},
#line 56 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str53, 24},
#line 60 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str54, 28},
#line 55 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str55, 23},
#line 49 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str56, 17},
#line 53 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str57, 21},
#line 45 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str58, 13},
#line 38 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str59, 6},
#line 47 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str60, 15},
#line 59 "src/libcrun/signals.perf"
    {(पूर्णाङ्क)(माप_प्रकार)&((संरचना stringpool_t *)0)->stringpool_str61, 27}
  };

ध्रुव संरचना signal_s *
libcrun_signal_in_word_set (पञ्जिका ध्रुव वर्ण *str, पञ्जिका माप_प्रकार len)
{
  यदि (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      पञ्जिका अचिन्हित पूर्णाङ्क कुंजी = hash (str, len);

      यदि (कुंजी <= MAX_HASH_मूल्य && कुंजी >= MIN_HASH_मूल्य)
        {
          पञ्जिका ध्रुव संरचना signal_s *resword;

          विकल्प (कुंजी - 13)
            {
              पक्ष 0:
                resword = &wordlist[0];
                गच्छ compare;
              पक्ष 1:
                resword = &wordlist[1];
                गच्छ compare;
              पक्ष 6:
                resword = &wordlist[2];
                गच्छ compare;
              पक्ष 7:
                resword = &wordlist[3];
                गच्छ compare;
              पक्ष 8:
                resword = &wordlist[4];
                गच्छ compare;
              पक्ष 9:
                resword = &wordlist[5];
                गच्छ compare;
              पक्ष 10:
                resword = &wordlist[6];
                गच्छ compare;
              पक्ष 11:
                resword = &wordlist[7];
                गच्छ compare;
              पक्ष 12:
                resword = &wordlist[8];
                गच्छ compare;
              पक्ष 13:
                resword = &wordlist[9];
                गच्छ compare;
              पक्ष 14:
                resword = &wordlist[10];
                गच्छ compare;
              पक्ष 15:
                resword = &wordlist[11];
                गच्छ compare;
              पक्ष 16:
                resword = &wordlist[12];
                गच्छ compare;
              पक्ष 17:
                resword = &wordlist[13];
                गच्छ compare;
              पक्ष 18:
                resword = &wordlist[14];
                गच्छ compare;
              पक्ष 19:
                resword = &wordlist[15];
                गच्छ compare;
              पक्ष 20:
                resword = &wordlist[16];
                गच्छ compare;
              पक्ष 21:
                resword = &wordlist[17];
                गच्छ compare;
              पक्ष 22:
                resword = &wordlist[18];
                गच्छ compare;
              पक्ष 23:
                resword = &wordlist[19];
                गच्छ compare;
              पक्ष 24:
                resword = &wordlist[20];
                गच्छ compare;
              पक्ष 25:
                resword = &wordlist[21];
                गच्छ compare;
              पक्ष 26:
                resword = &wordlist[22];
                गच्छ compare;
              पक्ष 27:
                resword = &wordlist[23];
                गच्छ compare;
              पक्ष 28:
                resword = &wordlist[24];
                गच्छ compare;
              पक्ष 29:
                resword = &wordlist[25];
                गच्छ compare;
              पक्ष 30:
                resword = &wordlist[26];
                गच्छ compare;
              पक्ष 31:
                resword = &wordlist[27];
                गच्छ compare;
              पक्ष 32:
                resword = &wordlist[28];
                गच्छ compare;
              पक्ष 33:
                resword = &wordlist[29];
                गच्छ compare;
              पक्ष 34:
                resword = &wordlist[30];
                गच्छ compare;
              पक्ष 35:
                resword = &wordlist[31];
                गच्छ compare;
              पक्ष 36:
                resword = &wordlist[32];
                गच्छ compare;
              पक्ष 37:
                resword = &wordlist[33];
                गच्छ compare;
              पक्ष 38:
                resword = &wordlist[34];
                गच्छ compare;
              पक्ष 39:
                resword = &wordlist[35];
                गच्छ compare;
              पक्ष 40:
                resword = &wordlist[36];
                गच्छ compare;
              पक्ष 41:
                resword = &wordlist[37];
                गच्छ compare;
              पक्ष 42:
                resword = &wordlist[38];
                गच्छ compare;
              पक्ष 43:
                resword = &wordlist[39];
                गच्छ compare;
              पक्ष 44:
                resword = &wordlist[40];
                गच्छ compare;
              पक्ष 45:
                resword = &wordlist[41];
                गच्छ compare;
              पक्ष 46:
                resword = &wordlist[42];
                गच्छ compare;
              पक्ष 47:
                resword = &wordlist[43];
                गच्छ compare;
              पक्ष 48:
                resword = &wordlist[44];
                गच्छ compare;
              पक्ष 49:
                resword = &wordlist[45];
                गच्छ compare;
              पक्ष 50:
                resword = &wordlist[46];
                गच्छ compare;
              पक्ष 51:
                resword = &wordlist[47];
                गच्छ compare;
              पक्ष 52:
                resword = &wordlist[48];
                गच्छ compare;
              पक्ष 53:
                resword = &wordlist[49];
                गच्छ compare;
              पक्ष 54:
                resword = &wordlist[50];
                गच्छ compare;
              पक्ष 55:
                resword = &wordlist[51];
                गच्छ compare;
              पक्ष 56:
                resword = &wordlist[52];
                गच्छ compare;
              पक्ष 57:
                resword = &wordlist[53];
                गच्छ compare;
              पक्ष 58:
                resword = &wordlist[54];
                गच्छ compare;
              पक्ष 59:
                resword = &wordlist[55];
                गच्छ compare;
              पक्ष 60:
                resword = &wordlist[56];
                गच्छ compare;
              पक्ष 61:
                resword = &wordlist[57];
                गच्छ compare;
              पक्ष 62:
                resword = &wordlist[58];
                गच्छ compare;
              पक्ष 63:
                resword = &wordlist[59];
                गच्छ compare;
              पक्ष 70:
                resword = &wordlist[60];
                गच्छ compare;
              पक्ष 73:
                resword = &wordlist[61];
                गच्छ compare;
            }
          प्रत्यागम 0;
        compare:
          {
            पञ्जिका ध्रुव वर्ण *s = resword->name + stringpool;

            यदि (*str == *s && !म_भेद (str + 1, s + 1))
              प्रत्यागम resword;
          }
        }
    }
  प्रत्यागम 0;
}
#line 95 "src/libcrun/signals.perf"

पूर्णाङ्क
str2sig (ध्रुव वर्ण *name)
{
  ध्रुव संरचना signal_s *s;

  यदि (has_prefix (name, "SIG"))
    name += 3;

  s = libcrun_signal_in_word_set (name, म_माप (name));
  यदि (s == रिक्त)
    {
      दीर्घ पूर्णाङ्क मूल्य;

      यदि (!है_अंक(name[0]))
        {
          त्रुटि_सं = EINVAL;
          प्रत्यागम -1;
        }

      त्रुटि_सं = 0;
      मूल्य = म_से_दीर्घ (name, रिक्त, 10);
      यदि (त्रुटि_सं != 0)
        प्रत्यागम -1;

      प्रत्यागम मूल्य;
    }

  प्रत्यागम s->मूल्य;
}
