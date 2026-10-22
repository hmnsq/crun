/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --lookup-function-name libcrun_signal_in_word_set -m 100 --null-strings --pic -tCEG -S1 src/libcrun/signals.perf  */
/* Computed positions: -k'2,4,$' */

#اگر !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
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

#شامل <config.h>
#شامل <stddef.h>
#شامل <stdlib.h>
#شامل "utils.h"
#line 27 "src/libcrun/signals.perf"
ساختار signal_s
{
  صحیح name;
  صحیح مقدار;
};
شمارش
  {
    TOTAL_KEYWORDS = 62,
    MIN_WORD_LENGTH = 2,
    MAX_WORD_LENGTH = 8,
    MIN_HASH_مقدار = 13,
    MAX_HASH_مقدار = 86
  };

/* maximum key range = 74, duplicates = 0 */

#ifdef __GNUC__
__inline
#وگرنه
#ifdef __cplusplus
درونخطی
#endif
#endif
ایستا unsigned صحیح
hash (ثبات ثابت نویسه *str, ثبات size_t len)
{
  ایستا ثابت unsigned نویسه asso_مقادیر[] =
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
  ثبات unsigned صحیح hval = len;

  انتخاب (hval)
    {
      پیشفرض:
        hval += asso_مقادیر[(unsigned نویسه)str[3]];
      /*FALLTHROUGH*/
      حالت 3:
      حالت 2:
        hval += asso_مقادیر[(unsigned نویسه)str[1]];
        بشکن;
    }
  بازگشت hval + asso_مقادیر[(unsigned نویسه)str[len - 1]];
}

ساختار stringpool_t
  {
    نویسه stringpool_str0[اندازه("ILL")];
    نویسه stringpool_str1[اندازه("HUP")];
    نویسه stringpool_str2[اندازه("KILL")];
    نویسه stringpool_str3[اندازه("TTOU")];
    نویسه stringpool_str4[اندازه("QUIT")];
    نویسه stringpool_str5[اندازه("STOP")];
    نویسه stringpool_str6[اندازه("RTMIN+2")];
    نویسه stringpool_str7[اندازه("RTMIN+12")];
    نویسه stringpool_str8[اندازه("RTMAX-2")];
    نویسه stringpool_str9[اندازه("RTMAX-12")];
    نویسه stringpool_str10[اندازه("RTMIN+1")];
    نویسه stringpool_str11[اندازه("RTMIN+11")];
    نویسه stringpool_str12[اندازه("RTMAX-1")];
    نویسه stringpool_str13[اندازه("RTMAX-11")];
    نویسه stringpool_str14[اندازه("RTMIN+4")];
    نویسه stringpool_str15[اندازه("RTMIN+14")];
    نویسه stringpool_str16[اندازه("RTMAX-4")];
    نویسه stringpool_str17[اندازه("RTMAX-14")];
    نویسه stringpool_str18[اندازه("RTMIN+3")];
    نویسه stringpool_str19[اندازه("RTMIN+13")];
    نویسه stringpool_str20[اندازه("RTMAX-3")];
    نویسه stringpool_str21[اندازه("RTMAX-13")];
    نویسه stringpool_str22[اندازه("BUS")];
    نویسه stringpool_str23[اندازه("VTALRM")];
    نویسه stringpool_str24[اندازه("INT")];
    نویسه stringpool_str25[اندازه("FPE")];
    نویسه stringpool_str26[اندازه("CONT")];
    نویسه stringpool_str27[اندازه("STKFLT")];
    نویسه stringpool_str28[اندازه("USR2")];
    نویسه stringpool_str29[اندازه("TRAP")];
    نویسه stringpool_str30[اندازه("TSTP")];
    نویسه stringpool_str31[اندازه("RTMIN")];
    نویسه stringpool_str32[اندازه("RTMIN+5")];
    نویسه stringpool_str33[اندازه("RTMIN+15")];
    نویسه stringpool_str34[اندازه("RTMAX-5")];
    نویسه stringpool_str35[اندازه("RTMIN+9")];
    نویسه stringpool_str36[اندازه("USR1")];
    نویسه stringpool_str37[اندازه("RTMAX-9")];
    نویسه stringpool_str38[اندازه("ALRM")];
    نویسه stringpool_str39[اندازه("IO")];
    نویسه stringpool_str40[اندازه("RTMIN+8")];
    نویسه stringpool_str41[اندازه("RTMIN+7")];
    نویسه stringpool_str42[اندازه("RTMAX-8")];
    نویسه stringpool_str43[اندازه("RTMAX-7")];
    نویسه stringpool_str44[اندازه("RTMIN+6")];
    نویسه stringpool_str45[اندازه("RTMIN+10")];
    نویسه stringpool_str46[اندازه("RTMAX-6")];
    نویسه stringpool_str47[اندازه("RTMAX-10")];
    نویسه stringpool_str48[اندازه("SYS")];
    نویسه stringpool_str49[اندازه("XFSZ")];
    نویسه stringpool_str50[اندازه("RTMAX")];
    نویسه stringpool_str51[اندازه("PWR")];
    نویسه stringpool_str52[اندازه("SEGV")];
    نویسه stringpool_str53[اندازه("XCPU")];
    نویسه stringpool_str54[اندازه("WINCH")];
    نویسه stringpool_str55[اندازه("URG")];
    نویسه stringpool_str56[اندازه("CHLD")];
    نویسه stringpool_str57[اندازه("TTIN")];
    نویسه stringpool_str58[اندازه("PIPE")];
    نویسه stringpool_str59[اندازه("ABRT")];
    نویسه stringpool_str60[اندازه("TERM")];
    نویسه stringpool_str61[اندازه("PROF")];
  };
ایستا ثابت ساختار stringpool_t stringpool_contents =
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
#define stringpool ((ثابت نویسه *) &stringpool_contents)

ایستا ثابت ساختار signal_s wordlist[] =
  {
#line 36 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str0, 4},
#line 33 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str1, 1},
#line 41 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str2, 9},
#line 54 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str3, 22},
#line 35 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str4, 3},
#line 51 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str5, 19},
#line 66 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str6, 36},
#line 76 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str7, 46},
#line 92 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str8, 62},
#line 82 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str9, 52},
#line 65 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str10, 35},
#line 75 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str11, 45},
#line 93 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str12, 63},
#line 83 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str13, 53},
#line 68 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str14, 38},
#line 78 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str15, 48},
#line 90 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str16, 60},
#line 80 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str17, 50},
#line 67 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str18, 37},
#line 77 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str19, 47},
#line 91 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str20, 61},
#line 81 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str21, 51},
#line 39 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str22, 7},
#line 58 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str23, 26},
#line 34 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str24, 2},
#line 40 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str25, 8},
#line 50 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str26, 18},
#line 48 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str27, 16},
#line 44 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str28, 12},
#line 37 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str29, 5},
#line 52 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str30, 20},
#line 64 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str31, 34},
#line 69 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str32, 39},
#line 79 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str33, 49},
#line 89 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str34, 59},
#line 73 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str35, 43},
#line 42 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str36, 10},
#line 85 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str37, 55},
#line 46 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str38, 14},
#line 61 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str39, 29},
#line 72 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str40, 42},
#line 71 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str41, 41},
#line 86 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str42, 56},
#line 87 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str43, 57},
#line 70 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str44, 40},
#line 74 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str45, 44},
#line 88 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str46, 58},
#line 84 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str47, 54},
#line 63 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str48, 31},
#line 57 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str49, 25},
#line 94 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str50, 64},
#line 62 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str51, 30},
#line 43 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str52, 11},
#line 56 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str53, 24},
#line 60 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str54, 28},
#line 55 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str55, 23},
#line 49 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str56, 17},
#line 53 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str57, 21},
#line 45 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str58, 13},
#line 38 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str59, 6},
#line 47 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str60, 15},
#line 59 "src/libcrun/signals.perf"
    {(صحیح)(size_t)&((ساختار stringpool_t *)0)->stringpool_str61, 27}
  };

ثابت ساختار signal_s *
libcrun_signal_in_word_set (ثبات ثابت نویسه *str, ثبات size_t len)
{
  اگر (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      ثبات unsigned صحیح کلید = hash (str, len);

      اگر (کلید <= MAX_HASH_مقدار && کلید >= MIN_HASH_مقدار)
        {
          ثبات ثابت ساختار signal_s *resword;

          انتخاب (کلید - 13)
            {
              حالت 0:
                resword = &wordlist[0];
                برو compare;
              حالت 1:
                resword = &wordlist[1];
                برو compare;
              حالت 6:
                resword = &wordlist[2];
                برو compare;
              حالت 7:
                resword = &wordlist[3];
                برو compare;
              حالت 8:
                resword = &wordlist[4];
                برو compare;
              حالت 9:
                resword = &wordlist[5];
                برو compare;
              حالت 10:
                resword = &wordlist[6];
                برو compare;
              حالت 11:
                resword = &wordlist[7];
                برو compare;
              حالت 12:
                resword = &wordlist[8];
                برو compare;
              حالت 13:
                resword = &wordlist[9];
                برو compare;
              حالت 14:
                resword = &wordlist[10];
                برو compare;
              حالت 15:
                resword = &wordlist[11];
                برو compare;
              حالت 16:
                resword = &wordlist[12];
                برو compare;
              حالت 17:
                resword = &wordlist[13];
                برو compare;
              حالت 18:
                resword = &wordlist[14];
                برو compare;
              حالت 19:
                resword = &wordlist[15];
                برو compare;
              حالت 20:
                resword = &wordlist[16];
                برو compare;
              حالت 21:
                resword = &wordlist[17];
                برو compare;
              حالت 22:
                resword = &wordlist[18];
                برو compare;
              حالت 23:
                resword = &wordlist[19];
                برو compare;
              حالت 24:
                resword = &wordlist[20];
                برو compare;
              حالت 25:
                resword = &wordlist[21];
                برو compare;
              حالت 26:
                resword = &wordlist[22];
                برو compare;
              حالت 27:
                resword = &wordlist[23];
                برو compare;
              حالت 28:
                resword = &wordlist[24];
                برو compare;
              حالت 29:
                resword = &wordlist[25];
                برو compare;
              حالت 30:
                resword = &wordlist[26];
                برو compare;
              حالت 31:
                resword = &wordlist[27];
                برو compare;
              حالت 32:
                resword = &wordlist[28];
                برو compare;
              حالت 33:
                resword = &wordlist[29];
                برو compare;
              حالت 34:
                resword = &wordlist[30];
                برو compare;
              حالت 35:
                resword = &wordlist[31];
                برو compare;
              حالت 36:
                resword = &wordlist[32];
                برو compare;
              حالت 37:
                resword = &wordlist[33];
                برو compare;
              حالت 38:
                resword = &wordlist[34];
                برو compare;
              حالت 39:
                resword = &wordlist[35];
                برو compare;
              حالت 40:
                resword = &wordlist[36];
                برو compare;
              حالت 41:
                resword = &wordlist[37];
                برو compare;
              حالت 42:
                resword = &wordlist[38];
                برو compare;
              حالت 43:
                resword = &wordlist[39];
                برو compare;
              حالت 44:
                resword = &wordlist[40];
                برو compare;
              حالت 45:
                resword = &wordlist[41];
                برو compare;
              حالت 46:
                resword = &wordlist[42];
                برو compare;
              حالت 47:
                resword = &wordlist[43];
                برو compare;
              حالت 48:
                resword = &wordlist[44];
                برو compare;
              حالت 49:
                resword = &wordlist[45];
                برو compare;
              حالت 50:
                resword = &wordlist[46];
                برو compare;
              حالت 51:
                resword = &wordlist[47];
                برو compare;
              حالت 52:
                resword = &wordlist[48];
                برو compare;
              حالت 53:
                resword = &wordlist[49];
                برو compare;
              حالت 54:
                resword = &wordlist[50];
                برو compare;
              حالت 55:
                resword = &wordlist[51];
                برو compare;
              حالت 56:
                resword = &wordlist[52];
                برو compare;
              حالت 57:
                resword = &wordlist[53];
                برو compare;
              حالت 58:
                resword = &wordlist[54];
                برو compare;
              حالت 59:
                resword = &wordlist[55];
                برو compare;
              حالت 60:
                resword = &wordlist[56];
                برو compare;
              حالت 61:
                resword = &wordlist[57];
                برو compare;
              حالت 62:
                resword = &wordlist[58];
                برو compare;
              حالت 63:
                resword = &wordlist[59];
                برو compare;
              حالت 70:
                resword = &wordlist[60];
                برو compare;
              حالت 73:
                resword = &wordlist[61];
                برو compare;
            }
          بازگشت 0;
        compare:
          {
            ثبات ثابت نویسه *s = resword->name + stringpool;

            اگر (*str == *s && !strcmp (str + 1, s + 1))
              بازگشت resword;
          }
        }
    }
  بازگشت 0;
}
#line 95 "src/libcrun/signals.perf"

صحیح
str2sig (ثابت نویسه *name)
{
  ثابت ساختار signal_s *s;

  اگر (has_prefix (name, "SIG"))
    name += 3;

  s = libcrun_signal_in_word_set (name, strlen (name));
  اگر (s == پوچ)
    {
      long صحیح مقدار;

      اگر (!isdigit(name[0]))
        {
          errno = EINVAL;
          بازگشت -1;
        }

      errno = 0;
      مقدار = strtol (name, پوچ, 10);
      اگر (errno != 0)
        بازگشت -1;

      بازگشت مقدار;
    }

  بازگشت s->مقدار;
}
