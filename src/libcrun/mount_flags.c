/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --lookup-function-name libcrun_mount_flag_in_word_set -m 100 -tCEG -S1 src/libcrun/mount_flags.perf  */
/* Computed positions: -k'1-4' */

#නම් !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
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

#line 19 "src/libcrun/mount_flags.perf"

#define _GNU_SOURCE

#include <config.h>
#include <stddef.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/mount.h>

#include "utils.h"
#include "mount_flags.h"

#line 34 "src/libcrun/mount_flags.perf"
sa_nracanaa propagation_flags_s;
ga_nanaa
  {
    TOTAL_KEYWORDS = 63,
    MIN_WORD_LENGTH = 2,
    MAX_WORD_LENGTH = 14,
    MIN_HASH_VALUE = 2,
    MAX_HASH_VALUE = 96
  };

/* maximum key range = 95, duplicates = 0 */

#ifdef __GNUC__
__inline
#නැතහොත්
#ifdef __cplusplus
anta_hstha
#endif
#endif
static unsigned පූර්ණ_සංඛ්‍යා
hash (pa_njikaa const var_na *str, pa_njikaa size_t len)
{
  static const unsigned var_na asso_values[] =
    {
      97, 97, 97, 97, 97, 97, 97, 97, 97, 97,
      97, 97, 97, 97, 97, 97, 97, 97, 97, 97,
      97, 97, 97, 97, 97, 97, 97, 97, 97, 97,
      97, 97, 97, 97, 97, 97, 97, 97, 97, 97,
      97, 97, 97, 97, 97, 33, 97, 97, 97, 97,
      97, 97, 97, 97, 97, 97, 97, 97, 97, 97,
      97, 97, 97, 97, 97, 97, 97, 97, 97, 97,
      97, 97, 97, 97, 97, 97, 97, 97, 97, 97,
      97, 97, 97, 97, 97, 97, 97, 97, 97, 97,
      97, 97, 97, 97, 97, 97, 97, 11, 35, 10,
       3, 20,  1, 97, 36,  0, 97, 97, 15, 17,
       0,  4, 37, 97,  0,  9,  4, 41, 25,  0,
      16, 33, 97, 97, 97, 97, 97, 97, 97, 97,
      97, 97, 97, 97, 97, 97, 97, 97, 97, 97,
      97, 97, 97, 97, 97, 97, 97, 97, 97, 97,
      97, 97, 97, 97, 97, 97, 97, 97, 97, 97,
      97, 97, 97, 97, 97, 97, 97, 97, 97, 97,
      97, 97, 97, 97, 97, 97, 97, 97, 97, 97,
      97, 97, 97, 97, 97, 97, 97, 97, 97, 97,
      97, 97, 97, 97, 97, 97, 97, 97, 97, 97,
      97, 97, 97, 97, 97, 97, 97, 97, 97, 97,
      97, 97, 97, 97, 97, 97, 97, 97, 97, 97,
      97, 97, 97, 97, 97, 97, 97, 97, 97, 97,
      97, 97, 97, 97, 97, 97, 97, 97, 97, 97,
      97, 97, 97, 97, 97, 97, 97, 97, 97, 97,
      97, 97, 97, 97, 97, 97
    };
  pa_njikaa unsigned පූර්ණ_සංඛ්‍යා hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned var_na)str[3]];
      /*FALLTHROUGH*/
      pak_sa 3:
        hval += asso_values[(unsigned var_na)str[2]];
      /*FALLTHROUGH*/
      pak_sa 2:
        hval += asso_values[(unsigned var_na)str[1]];
      /*FALLTHROUGH*/
      pak_sa 1:
        hval += asso_values[(unsigned var_na)str[0]];
        bha_nga;
    }
  ආපසු hval;
}

static const sa_nracanaa propagation_flags_s wordlist[] =
  {
#line 40 "src/libcrun/mount_flags.perf"
    {"rw", 1, MS_RDONLY, 0},
#line 74 "src/libcrun/mount_flags.perf"
    {"rrw", 1, MS_RDONLY, OPTION_RECURSIVE},
#line 39 "src/libcrun/mount_flags.perf"
    {"ro", 0, MS_RDONLY, 0},
#line 73 "src/libcrun/mount_flags.perf"
    {"rro", 0, MS_RDONLY, OPTION_RECURSIVE},
#line 83 "src/libcrun/mount_flags.perf"
    {"rdirsync", 0, MS_DIRSYNC, OPTION_RECURSIVE},
#line 88 "src/libcrun/mount_flags.perf"
    {"rdiratime", 1, MS_NODIRATIME, OPTION_RECURSIVE},
#line 78 "src/libcrun/mount_flags.perf"
    {"rnodev", 0, MS_NODEV, OPTION_RECURSIVE},
#line 91 "src/libcrun/mount_flags.perf"
    {"rnorelatime", 1, MS_RELATIME, OPTION_RECURSIVE},
#line 60 "src/libcrun/mount_flags.perf"
    {"nodiratime", 0, MS_NODIRATIME, 0},
#line 89 "src/libcrun/mount_flags.perf"
    {"rnodiratime", 0, MS_NODIRATIME, OPTION_RECURSIVE},
#line 53 "src/libcrun/mount_flags.perf"
    {"dirsync", 0, MS_DIRSYNC, 0},
#line 76 "src/libcrun/mount_flags.perf"
    {"rnosuid", 0, MS_NOSUID, OPTION_RECURSIVE},
#line 86 "src/libcrun/mount_flags.perf"
    {"ratime", 1, MS_NOATIME, OPTION_RECURSIVE},
#line 59 "src/libcrun/mount_flags.perf"
    {"diratime", 1, MS_NODIRATIME, 0},
#line 87 "src/libcrun/mount_flags.perf"
    {"rnoatime", 0, MS_NOATIME, OPTION_RECURSIVE},
#line 63 "src/libcrun/mount_flags.perf"
    {"strictatime", 0, MS_STRICTATIME, 0},
#line 92 "src/libcrun/mount_flags.perf"
    {"rstrictatime", 0, MS_STRICTATIME, OPTION_RECURSIVE},
#line 58 "src/libcrun/mount_flags.perf"
    {"noatime", 0, MS_NOATIME, 0},
#line 93 "src/libcrun/mount_flags.perf"
    {"rnostrictatime", 1, MS_STRICTATIME, OPTION_RECURSIVE},
#line 85 "src/libcrun/mount_flags.perf"
    {"rnomand", 1, MS_MANDLOCK, OPTION_RECURSIVE},
#line 64 "src/libcrun/mount_flags.perf"
    {"nostrictatime", 1, MS_STRICTATIME, 0},
#line 80 "src/libcrun/mount_flags.perf"
    {"rnoexec", 0, MS_NOEXEC, OPTION_RECURSIVE},
#line 44 "src/libcrun/mount_flags.perf"
    {"nodev", 0, MS_NODEV, 0},
#line 84 "src/libcrun/mount_flags.perf"
    {"rmand", 0, MS_MANDLOCK, OPTION_RECURSIVE},
#line 62 "src/libcrun/mount_flags.perf"
    {"norelatime", 1, MS_RELATIME, 0},
#line 55 "src/libcrun/mount_flags.perf"
    {"mand", 0, MS_MANDLOCK, 0},
#line 95 "src/libcrun/mount_flags.perf"
    {"idmap", 0, 0, OPTION_IDMAP},
#line 57 "src/libcrun/mount_flags.perf"
    {"atime", 1, MS_NOATIME, 0},
#line 56 "src/libcrun/mount_flags.perf"
    {"nomand", 1, MS_MANDLOCK, 0},
#line 50 "src/libcrun/mount_flags.perf"
    {"noiversion", 0, 0, 0},
#line 38 "src/libcrun/mount_flags.perf"
    {"rbind", 0, MS_REC|MS_BIND, 0},
#line 68 "src/libcrun/mount_flags.perf"
    {"rslave", 0, MS_REC|MS_SLAVE, 0},
#line 37 "src/libcrun/mount_flags.perf"
    {"bind", 0, MS_BIND, 0},
#line 36 "src/libcrun/mount_flags.perf"
    {"defaults", 0, 0, 0},
#line 90 "src/libcrun/mount_flags.perf"
    {"rrelatime", 0, MS_RELATIME, OPTION_RECURSIVE},
#line 70 "src/libcrun/mount_flags.perf"
    {"rprivate", 0, MS_REC|MS_PRIVATE, 0},
#line 46 "src/libcrun/mount_flags.perf"
    {"noexec", 0, MS_NOEXEC, 0},
#line 81 "src/libcrun/mount_flags.perf"
    {"rsync", 0, MS_SYNCHRONOUS, OPTION_RECURSIVE},
#line 54 "src/libcrun/mount_flags.perf"
    {"remount", 0, MS_REMOUNT, 0},
#line 98 "src/libcrun/mount_flags.perf"
    {"dest-nofollow", 0, 0, OPTION_DEST_NOFOLLOW},
#line 51 "src/libcrun/mount_flags.perf"
    {"silent", 0, 0, 0},
#line 43 "src/libcrun/mount_flags.perf"
    {"dev", 1, MS_NODEV, 0},
#line 77 "src/libcrun/mount_flags.perf"
    {"rdev", 1, MS_NODEV, OPTION_RECURSIVE},
#line 49 "src/libcrun/mount_flags.perf"
    {"iversion", 0, 0, 0},
#line 61 "src/libcrun/mount_flags.perf"
    {"relatime", 0, MS_RELATIME, 0},
#line 75 "src/libcrun/mount_flags.perf"
    {"rsuid", 1, MS_NOSUID, OPTION_RECURSIVE},
#line 47 "src/libcrun/mount_flags.perf"
    {"sync", 0, MS_SYNCHRONOUS, 0},
#line 41 "src/libcrun/mount_flags.perf"
    {"suid", 1, MS_NOSUID, 0},
#line 48 "src/libcrun/mount_flags.perf"
    {"async", 1, MS_SYNCHRONOUS, 0},
#line 82 "src/libcrun/mount_flags.perf"
    {"rasync", 1, MS_SYNCHRONOUS, OPTION_RECURSIVE},
#line 42 "src/libcrun/mount_flags.perf"
    {"nosuid", 0, MS_NOSUID, 0},
#line 79 "src/libcrun/mount_flags.perf"
    {"rexec", 1, MS_NOEXEC, OPTION_RECURSIVE},
#line 65 "src/libcrun/mount_flags.perf"
    {"shared", 0, MS_SHARED, 0},
#line 66 "src/libcrun/mount_flags.perf"
    {"rshared", 0, MS_REC|MS_SHARED, 0},
#line 97 "src/libcrun/mount_flags.perf"
    {"src-nofollow", 0, 0, OPTION_SRC_NOFOLLOW},
#line 67 "src/libcrun/mount_flags.perf"
    {"slave", 0, MS_SLAVE, 0},
#line 52 "src/libcrun/mount_flags.perf"
    {"loud", 0, 0, 0},
#line 69 "src/libcrun/mount_flags.perf"
    {"private", 0, MS_PRIVATE, 0},
#line 45 "src/libcrun/mount_flags.perf"
    {"exec", 1, MS_NOEXEC, 0},
#line 94 "src/libcrun/mount_flags.perf"
    {"tmpcopyup", 0, 0, OPTION_TMPCOPYUP},
#line 71 "src/libcrun/mount_flags.perf"
    {"unbindable", 0, MS_UNBINDABLE, 0},
#line 72 "src/libcrun/mount_flags.perf"
    {"runbindable", 0, MS_REC|MS_UNBINDABLE, 0},
#line 96 "src/libcrun/mount_flags.perf"
    {"copy-symlink", 0, 0, OPTION_COPY_SYMLINK}
  };

const sa_nracanaa propagation_flags_s *
libcrun_mount_flag_in_word_set (pa_njikaa const var_na *str, pa_njikaa size_t len)
{
  නම් (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      pa_njikaa unsigned පූර්ණ_සංඛ්‍යා key = hash (str, len);

      නම් (key <= MAX_HASH_VALUE && key >= MIN_HASH_VALUE)
        {
          pa_njikaa const sa_nracanaa propagation_flags_s *resword;

          switch (key - 2)
            {
              pak_sa 0:
                resword = &wordlist[0];
                goto compare;
              pak_sa 1:
                resword = &wordlist[1];
                goto compare;
              pak_sa 4:
                resword = &wordlist[2];
                goto compare;
              pak_sa 5:
                resword = &wordlist[3];
                goto compare;
              pak_sa 9:
                resword = &wordlist[4];
                goto compare;
              pak_sa 10:
                resword = &wordlist[5];
                goto compare;
              pak_sa 11:
                resword = &wordlist[6];
                goto compare;
              pak_sa 13:
                resword = &wordlist[7];
                goto compare;
              pak_sa 15:
                resword = &wordlist[8];
                goto compare;
              pak_sa 16:
                resword = &wordlist[9];
                goto compare;
              pak_sa 17:
                resword = &wordlist[10];
                goto compare;
              pak_sa 18:
                resword = &wordlist[11];
                goto compare;
              pak_sa 19:
                resword = &wordlist[12];
                goto compare;
              pak_sa 20:
                resword = &wordlist[13];
                goto compare;
              pak_sa 21:
                resword = &wordlist[14];
                goto compare;
              pak_sa 22:
                resword = &wordlist[15];
                goto compare;
              pak_sa 23:
                resword = &wordlist[16];
                goto compare;
              pak_sa 24:
                resword = &wordlist[17];
                goto compare;
              pak_sa 25:
                resword = &wordlist[18];
                goto compare;
              pak_sa 26:
                resword = &wordlist[19];
                goto compare;
              pak_sa 28:
                resword = &wordlist[20];
                goto compare;
              pak_sa 29:
                resword = &wordlist[21];
                goto compare;
              pak_sa 30:
                resword = &wordlist[22];
                goto compare;
              pak_sa 31:
                resword = &wordlist[23];
                goto compare;
              pak_sa 32:
                resword = &wordlist[24];
                goto compare;
              pak_sa 33:
                resword = &wordlist[25];
                goto compare;
              pak_sa 34:
                resword = &wordlist[26];
                goto compare;
              pak_sa 35:
                resword = &wordlist[27];
                goto compare;
              pak_sa 36:
                resword = &wordlist[28];
                goto compare;
              pak_sa 37:
                resword = &wordlist[29];
                goto compare;
              pak_sa 38:
                resword = &wordlist[30];
                goto compare;
              pak_sa 39:
                resword = &wordlist[31];
                goto compare;
              pak_sa 40:
                resword = &wordlist[32];
                goto compare;
              pak_sa 41:
                resword = &wordlist[33];
                goto compare;
              pak_sa 42:
                resword = &wordlist[34];
                goto compare;
              pak_sa 43:
                resword = &wordlist[35];
                goto compare;
              pak_sa 44:
                resword = &wordlist[36];
                goto compare;
              pak_sa 45:
                resword = &wordlist[37];
                goto compare;
              pak_sa 46:
                resword = &wordlist[38];
                goto compare;
              pak_sa 47:
                resword = &wordlist[39];
                goto compare;
              pak_sa 48:
                resword = &wordlist[40];
                goto compare;
              pak_sa 49:
                resword = &wordlist[41];
                goto compare;
              pak_sa 50:
                resword = &wordlist[42];
                goto compare;
              pak_sa 51:
                resword = &wordlist[43];
                goto compare;
              pak_sa 52:
                resword = &wordlist[44];
                goto compare;
              pak_sa 53:
                resword = &wordlist[45];
                goto compare;
              pak_sa 54:
                resword = &wordlist[46];
                goto compare;
              pak_sa 55:
                resword = &wordlist[47];
                goto compare;
              pak_sa 56:
                resword = &wordlist[48];
                goto compare;
              pak_sa 57:
                resword = &wordlist[49];
                goto compare;
              pak_sa 58:
                resword = &wordlist[50];
                goto compare;
              pak_sa 59:
                resword = &wordlist[51];
                goto compare;
              pak_sa 60:
                resword = &wordlist[52];
                goto compare;
              pak_sa 61:
                resword = &wordlist[53];
                goto compare;
              pak_sa 62:
                resword = &wordlist[54];
                goto compare;
              pak_sa 63:
                resword = &wordlist[55];
                goto compare;
              pak_sa 65:
                resword = &wordlist[56];
                goto compare;
              pak_sa 67:
                resword = &wordlist[57];
                goto compare;
              pak_sa 68:
                resword = &wordlist[58];
                goto compare;
              pak_sa 75:
                resword = &wordlist[59];
                goto compare;
              pak_sa 84:
                resword = &wordlist[60];
                goto compare;
              pak_sa 85:
                resword = &wordlist[61];
                goto compare;
              pak_sa 94:
                resword = &wordlist[62];
                goto compare;
            }
          ආපසු 0;
        compare:
          {
            pa_njikaa const var_na *s = resword->name;

            නම් (*str == *s && !strcmp (str + 1, s + 1))
              ආපසු resword;
          }
        }
    }
  ආපසු 0;
}
#line 99 "src/libcrun/mount_flags.perf"


const sa_nracanaa propagation_flags_s *
libcrun_str2mount_flags (const var_na *name)
{
  ආපසු libcrun_mount_flag_in_word_set (name, strlen (name));
}

const sa_nracanaa propagation_flags_s *
get_mount_flags_from_wordlist (void)
{
  sa_nracanaa propagation_flags_s *flags;
  size_t i;
  size_t num_wordlist_flags = parimaa_na (wordlist) / parimaa_na (wordlist[0]);

  flags = xmalloc0 ((parimaa_na (sa_nracanaa propagation_flags_s) + 1) * num_wordlist_flags);
  සඳහා (i = 0; i < num_wordlist_flags; i++)
    flags[i].name = wordlist[i].name;

  ආපසු flags;
}
