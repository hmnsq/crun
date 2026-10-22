/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --lookup-function-name libcrun_signal_in_word_set -m 100 --null-strings --pic -tCEG -S1 src/libcrun/signals.perf  */
/* Computed positions: -k'2,4,$' */

#إذا !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
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

#ضمن <config.h>
#ضمن <stddef.h>
#ضمن <stdlib.h>
#ضمن "utils.h"
#line 27 "src/libcrun/signals.perf"
بنية signal_s
{
  صحيح name;
  صحيح قدر;
};
تعداد
  {
    TOTAL_KEYWORDS = 62,
    MIN_WORD_LENGTH = 2,
    MAX_WORD_LENGTH = 8,
    MIN_HASH_قدر = 13,
    MAX_HASH_قدر = 86
  };

/* maximum key range = 74, duplicates = 0 */

#ifdef __GNUC__
__inline
#وإلا
#ifdef __cplusplus
مضمن
#endif
#endif
ساكن unsigned صحيح
hash (سجل ثابت حرف *str, سجل size_t len)
{
  ساكن ثابت unsigned حرف asso_اقدار[] =
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
  سجل unsigned صحيح hval = len;

  بدل (hval)
    {
      افتراضي:
        hval += asso_اقدار[(unsigned حرف)str[3]];
      /*FALLTHROUGH*/
      حالة 3:
      حالة 2:
        hval += asso_اقدار[(unsigned حرف)str[1]];
        اكسر;
    }
  أرجع hval + asso_اقدار[(unsigned حرف)str[len - 1]];
}

بنية stringpool_t
  {
    حرف stringpool_str0[حجم("ILL")];
    حرف stringpool_str1[حجم("HUP")];
    حرف stringpool_str2[حجم("KILL")];
    حرف stringpool_str3[حجم("TTOU")];
    حرف stringpool_str4[حجم("QUIT")];
    حرف stringpool_str5[حجم("STOP")];
    حرف stringpool_str6[حجم("RTMIN+2")];
    حرف stringpool_str7[حجم("RTMIN+12")];
    حرف stringpool_str8[حجم("RTMAX-2")];
    حرف stringpool_str9[حجم("RTMAX-12")];
    حرف stringpool_str10[حجم("RTMIN+1")];
    حرف stringpool_str11[حجم("RTMIN+11")];
    حرف stringpool_str12[حجم("RTMAX-1")];
    حرف stringpool_str13[حجم("RTMAX-11")];
    حرف stringpool_str14[حجم("RTMIN+4")];
    حرف stringpool_str15[حجم("RTMIN+14")];
    حرف stringpool_str16[حجم("RTMAX-4")];
    حرف stringpool_str17[حجم("RTMAX-14")];
    حرف stringpool_str18[حجم("RTMIN+3")];
    حرف stringpool_str19[حجم("RTMIN+13")];
    حرف stringpool_str20[حجم("RTMAX-3")];
    حرف stringpool_str21[حجم("RTMAX-13")];
    حرف stringpool_str22[حجم("BUS")];
    حرف stringpool_str23[حجم("VTALRM")];
    حرف stringpool_str24[حجم("INT")];
    حرف stringpool_str25[حجم("FPE")];
    حرف stringpool_str26[حجم("CONT")];
    حرف stringpool_str27[حجم("STKFLT")];
    حرف stringpool_str28[حجم("USR2")];
    حرف stringpool_str29[حجم("TRAP")];
    حرف stringpool_str30[حجم("TSTP")];
    حرف stringpool_str31[حجم("RTMIN")];
    حرف stringpool_str32[حجم("RTMIN+5")];
    حرف stringpool_str33[حجم("RTMIN+15")];
    حرف stringpool_str34[حجم("RTMAX-5")];
    حرف stringpool_str35[حجم("RTMIN+9")];
    حرف stringpool_str36[حجم("USR1")];
    حرف stringpool_str37[حجم("RTMAX-9")];
    حرف stringpool_str38[حجم("ALRM")];
    حرف stringpool_str39[حجم("IO")];
    حرف stringpool_str40[حجم("RTMIN+8")];
    حرف stringpool_str41[حجم("RTMIN+7")];
    حرف stringpool_str42[حجم("RTMAX-8")];
    حرف stringpool_str43[حجم("RTMAX-7")];
    حرف stringpool_str44[حجم("RTMIN+6")];
    حرف stringpool_str45[حجم("RTMIN+10")];
    حرف stringpool_str46[حجم("RTMAX-6")];
    حرف stringpool_str47[حجم("RTMAX-10")];
    حرف stringpool_str48[حجم("SYS")];
    حرف stringpool_str49[حجم("XFSZ")];
    حرف stringpool_str50[حجم("RTMAX")];
    حرف stringpool_str51[حجم("PWR")];
    حرف stringpool_str52[حجم("SEGV")];
    حرف stringpool_str53[حجم("XCPU")];
    حرف stringpool_str54[حجم("WINCH")];
    حرف stringpool_str55[حجم("URG")];
    حرف stringpool_str56[حجم("CHLD")];
    حرف stringpool_str57[حجم("TTIN")];
    حرف stringpool_str58[حجم("PIPE")];
    حرف stringpool_str59[حجم("ABRT")];
    حرف stringpool_str60[حجم("TERM")];
    حرف stringpool_str61[حجم("PROF")];
  };
ساكن ثابت بنية stringpool_t stringpool_contents =
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
#define stringpool ((ثابت حرف *) &stringpool_contents)

ساكن ثابت بنية signal_s wordlist[] =
  {
#line 36 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str0, 4},
#line 33 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str1, 1},
#line 41 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str2, 9},
#line 54 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str3, 22},
#line 35 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str4, 3},
#line 51 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str5, 19},
#line 66 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str6, 36},
#line 76 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str7, 46},
#line 92 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str8, 62},
#line 82 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str9, 52},
#line 65 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str10, 35},
#line 75 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str11, 45},
#line 93 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str12, 63},
#line 83 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str13, 53},
#line 68 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str14, 38},
#line 78 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str15, 48},
#line 90 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str16, 60},
#line 80 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str17, 50},
#line 67 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str18, 37},
#line 77 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str19, 47},
#line 91 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str20, 61},
#line 81 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str21, 51},
#line 39 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str22, 7},
#line 58 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str23, 26},
#line 34 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str24, 2},
#line 40 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str25, 8},
#line 50 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str26, 18},
#line 48 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str27, 16},
#line 44 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str28, 12},
#line 37 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str29, 5},
#line 52 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str30, 20},
#line 64 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str31, 34},
#line 69 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str32, 39},
#line 79 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str33, 49},
#line 89 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str34, 59},
#line 73 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str35, 43},
#line 42 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str36, 10},
#line 85 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str37, 55},
#line 46 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str38, 14},
#line 61 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str39, 29},
#line 72 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str40, 42},
#line 71 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str41, 41},
#line 86 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str42, 56},
#line 87 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str43, 57},
#line 70 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str44, 40},
#line 74 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str45, 44},
#line 88 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str46, 58},
#line 84 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str47, 54},
#line 63 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str48, 31},
#line 57 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str49, 25},
#line 94 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str50, 64},
#line 62 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str51, 30},
#line 43 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str52, 11},
#line 56 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str53, 24},
#line 60 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str54, 28},
#line 55 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str55, 23},
#line 49 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str56, 17},
#line 53 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str57, 21},
#line 45 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str58, 13},
#line 38 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str59, 6},
#line 47 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str60, 15},
#line 59 "src/libcrun/signals.perf"
    {(صحيح)(size_t)&((بنية stringpool_t *)0)->stringpool_str61, 27}
  };

ثابت بنية signal_s *
libcrun_signal_in_word_set (سجل ثابت حرف *str, سجل size_t len)
{
  إذا (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      سجل unsigned صحيح کلید = hash (str, len);

      إذا (کلید <= MAX_HASH_قدر && کلید >= MIN_HASH_قدر)
        {
          سجل ثابت بنية signal_s *resword;

          بدل (کلید - 13)
            {
              حالة 0:
                resword = &wordlist[0];
                اذهب compare;
              حالة 1:
                resword = &wordlist[1];
                اذهب compare;
              حالة 6:
                resword = &wordlist[2];
                اذهب compare;
              حالة 7:
                resword = &wordlist[3];
                اذهب compare;
              حالة 8:
                resword = &wordlist[4];
                اذهب compare;
              حالة 9:
                resword = &wordlist[5];
                اذهب compare;
              حالة 10:
                resword = &wordlist[6];
                اذهب compare;
              حالة 11:
                resword = &wordlist[7];
                اذهب compare;
              حالة 12:
                resword = &wordlist[8];
                اذهب compare;
              حالة 13:
                resword = &wordlist[9];
                اذهب compare;
              حالة 14:
                resword = &wordlist[10];
                اذهب compare;
              حالة 15:
                resword = &wordlist[11];
                اذهب compare;
              حالة 16:
                resword = &wordlist[12];
                اذهب compare;
              حالة 17:
                resword = &wordlist[13];
                اذهب compare;
              حالة 18:
                resword = &wordlist[14];
                اذهب compare;
              حالة 19:
                resword = &wordlist[15];
                اذهب compare;
              حالة 20:
                resword = &wordlist[16];
                اذهب compare;
              حالة 21:
                resword = &wordlist[17];
                اذهب compare;
              حالة 22:
                resword = &wordlist[18];
                اذهب compare;
              حالة 23:
                resword = &wordlist[19];
                اذهب compare;
              حالة 24:
                resword = &wordlist[20];
                اذهب compare;
              حالة 25:
                resword = &wordlist[21];
                اذهب compare;
              حالة 26:
                resword = &wordlist[22];
                اذهب compare;
              حالة 27:
                resword = &wordlist[23];
                اذهب compare;
              حالة 28:
                resword = &wordlist[24];
                اذهب compare;
              حالة 29:
                resword = &wordlist[25];
                اذهب compare;
              حالة 30:
                resword = &wordlist[26];
                اذهب compare;
              حالة 31:
                resword = &wordlist[27];
                اذهب compare;
              حالة 32:
                resword = &wordlist[28];
                اذهب compare;
              حالة 33:
                resword = &wordlist[29];
                اذهب compare;
              حالة 34:
                resword = &wordlist[30];
                اذهب compare;
              حالة 35:
                resword = &wordlist[31];
                اذهب compare;
              حالة 36:
                resword = &wordlist[32];
                اذهب compare;
              حالة 37:
                resword = &wordlist[33];
                اذهب compare;
              حالة 38:
                resword = &wordlist[34];
                اذهب compare;
              حالة 39:
                resword = &wordlist[35];
                اذهب compare;
              حالة 40:
                resword = &wordlist[36];
                اذهب compare;
              حالة 41:
                resword = &wordlist[37];
                اذهب compare;
              حالة 42:
                resword = &wordlist[38];
                اذهب compare;
              حالة 43:
                resword = &wordlist[39];
                اذهب compare;
              حالة 44:
                resword = &wordlist[40];
                اذهب compare;
              حالة 45:
                resword = &wordlist[41];
                اذهب compare;
              حالة 46:
                resword = &wordlist[42];
                اذهب compare;
              حالة 47:
                resword = &wordlist[43];
                اذهب compare;
              حالة 48:
                resword = &wordlist[44];
                اذهب compare;
              حالة 49:
                resword = &wordlist[45];
                اذهب compare;
              حالة 50:
                resword = &wordlist[46];
                اذهب compare;
              حالة 51:
                resword = &wordlist[47];
                اذهب compare;
              حالة 52:
                resword = &wordlist[48];
                اذهب compare;
              حالة 53:
                resword = &wordlist[49];
                اذهب compare;
              حالة 54:
                resword = &wordlist[50];
                اذهب compare;
              حالة 55:
                resword = &wordlist[51];
                اذهب compare;
              حالة 56:
                resword = &wordlist[52];
                اذهب compare;
              حالة 57:
                resword = &wordlist[53];
                اذهب compare;
              حالة 58:
                resword = &wordlist[54];
                اذهب compare;
              حالة 59:
                resword = &wordlist[55];
                اذهب compare;
              حالة 60:
                resword = &wordlist[56];
                اذهب compare;
              حالة 61:
                resword = &wordlist[57];
                اذهب compare;
              حالة 62:
                resword = &wordlist[58];
                اذهب compare;
              حالة 63:
                resword = &wordlist[59];
                اذهب compare;
              حالة 70:
                resword = &wordlist[60];
                اذهب compare;
              حالة 73:
                resword = &wordlist[61];
                اذهب compare;
            }
          أرجع 0;
        compare:
          {
            سجل ثابت حرف *s = resword->name + stringpool;

            إذا (*str == *s && !strcmp (str + 1, s + 1))
              أرجع resword;
          }
        }
    }
  أرجع 0;
}
#line 95 "src/libcrun/signals.perf"

صحيح
str2sig (ثابت حرف *name)
{
  ثابت بنية signal_s *s;

  إذا (has_prefix (name, "SIG"))
    name += 3;

  s = libcrun_signal_in_word_set (name, strlen (name));
  إذا (s == عدم)
    {
      long صحيح قدر;

      إذا (!isdigit(name[0]))
        {
          errno = EINVAL;
          أرجع -1;
        }

      errno = 0;
      قدر = strtol (name, عدم, 10);
      إذا (errno != 0)
        أرجع -1;

      أرجع قدر;
    }

  أرجع s->قدر;
}
