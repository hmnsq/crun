/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --lookup-function-name libcrun_mount_flag_in_word_set -m 100 -tCEG -S1 src/libcrun/mount_flags.perf  */
/* Computed positions: -k'1-4' */

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

#line 19 "src/libcrun/mount_flags.perf"

#define _GNU_SOURCE

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <sys/mount.h>

#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "mount_flags.h"

#line 34 "src/libcrun/mount_flags.perf"
ಸಂರಚನಾ propagation_flags_s;
ಗಣನಾ
  {
    TOTAL_KEYWORDS = 63,
    MIN_WORD_LENGTH = 2,
    MAX_WORD_LENGTH = 14,
    MIN_HASH_ಮೌಲ್ಯ = 2,
    MAX_HASH_ಮೌಲ್ಯ = 96
  };

/* maximum key range = 95, duplicates = 0 */

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
  ಪಞ್ಜಿಕಾ unsigned ಪೂರ್ಣಾಙ್ಕ hval = len;

  ವಿಕಲ್ಪ (hval)
    {
      ಸ್ವಭಾವ:
        hval += asso_ಮೌಲ್ಯಗಳು[(unsigned ವರ್ಣ)str[3]];
      /*FALLTHROUGH*/
      ಪಕ್ಷ 3:
        hval += asso_ಮೌಲ್ಯಗಳು[(unsigned ವರ್ಣ)str[2]];
      /*FALLTHROUGH*/
      ಪಕ್ಷ 2:
        hval += asso_ಮೌಲ್ಯಗಳು[(unsigned ವರ್ಣ)str[1]];
      /*FALLTHROUGH*/
      ಪಕ್ಷ 1:
        hval += asso_ಮೌಲ್ಯಗಳು[(unsigned ವರ್ಣ)str[0]];
        ಭಙ್ಗ;
    }
  ಪ್ರತ್ಯಾಗಮ hval;
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ propagation_flags_s wordlist[] =
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

ಧ್ರುವ ಸಂರಚನಾ propagation_flags_s *
libcrun_mount_flag_in_word_set (ಪಞ್ಜಿಕಾ ಧ್ರುವ ವರ್ಣ *str, ಪಞ್ಜಿಕಾ size_t len)
{
  ಯದಿ (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      ಪಞ್ಜಿಕಾ unsigned ಪೂರ್ಣಾಙ್ಕ ಕೀಲಿ = hash (str, len);

      ಯದಿ (ಕೀಲಿ <= MAX_HASH_ಮೌಲ್ಯ && ಕೀಲಿ >= MIN_HASH_ಮೌಲ್ಯ)
        {
          ಪಞ್ಜಿಕಾ ಧ್ರುವ ಸಂರಚನಾ propagation_flags_s *resword;

          ವಿಕಲ್ಪ (ಕೀಲಿ - 2)
            {
              ಪಕ್ಷ 0:
                resword = &wordlist[0];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 1:
                resword = &wordlist[1];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 4:
                resword = &wordlist[2];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 5:
                resword = &wordlist[3];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 9:
                resword = &wordlist[4];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 10:
                resword = &wordlist[5];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 11:
                resword = &wordlist[6];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 13:
                resword = &wordlist[7];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 15:
                resword = &wordlist[8];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 16:
                resword = &wordlist[9];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 17:
                resword = &wordlist[10];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 18:
                resword = &wordlist[11];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 19:
                resword = &wordlist[12];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 20:
                resword = &wordlist[13];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 21:
                resword = &wordlist[14];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 22:
                resword = &wordlist[15];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 23:
                resword = &wordlist[16];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 24:
                resword = &wordlist[17];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 25:
                resword = &wordlist[18];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 26:
                resword = &wordlist[19];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 28:
                resword = &wordlist[20];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 29:
                resword = &wordlist[21];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 30:
                resword = &wordlist[22];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 31:
                resword = &wordlist[23];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 32:
                resword = &wordlist[24];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 33:
                resword = &wordlist[25];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 34:
                resword = &wordlist[26];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 35:
                resword = &wordlist[27];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 36:
                resword = &wordlist[28];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 37:
                resword = &wordlist[29];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 38:
                resword = &wordlist[30];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 39:
                resword = &wordlist[31];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 40:
                resword = &wordlist[32];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 41:
                resword = &wordlist[33];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 42:
                resword = &wordlist[34];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 43:
                resword = &wordlist[35];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 44:
                resword = &wordlist[36];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 45:
                resword = &wordlist[37];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 46:
                resword = &wordlist[38];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 47:
                resword = &wordlist[39];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 48:
                resword = &wordlist[40];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 49:
                resword = &wordlist[41];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 50:
                resword = &wordlist[42];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 51:
                resword = &wordlist[43];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 52:
                resword = &wordlist[44];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 53:
                resword = &wordlist[45];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 54:
                resword = &wordlist[46];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 55:
                resword = &wordlist[47];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 56:
                resword = &wordlist[48];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 57:
                resword = &wordlist[49];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 58:
                resword = &wordlist[50];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 59:
                resword = &wordlist[51];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 60:
                resword = &wordlist[52];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 61:
                resword = &wordlist[53];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 62:
                resword = &wordlist[54];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 63:
                resword = &wordlist[55];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 65:
                resword = &wordlist[56];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 67:
                resword = &wordlist[57];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 68:
                resword = &wordlist[58];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 75:
                resword = &wordlist[59];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 84:
                resword = &wordlist[60];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 85:
                resword = &wordlist[61];
                ಗಚ್ಛ compare;
              ಪಕ್ಷ 94:
                resword = &wordlist[62];
                ಗಚ್ಛ compare;
            }
          ಪ್ರತ್ಯಾಗಮ 0;
        compare:
          {
            ಪಞ್ಜಿಕಾ ಧ್ರುವ ವರ್ಣ *s = resword->name;

            ಯದಿ (*str == *s && !strcmp (str + 1, s + 1))
              ಪ್ರತ್ಯಾಗಮ resword;
          }
        }
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}
#line 99 "src/libcrun/mount_flags.perf"


ಧ್ರುವ ಸಂರಚನಾ propagation_flags_s *
libcrun_str2mount_flags (ಧ್ರುವ ವರ್ಣ *name)
{
  ಪ್ರತ್ಯಾಗಮ libcrun_mount_flag_in_word_set (name, strlen (name));
}

ಧ್ರುವ ಸಂರಚನಾ propagation_flags_s *
get_mount_flags_from_wordlist (ಶೂನ್ಯ)
{
  ಸಂರಚನಾ propagation_flags_s *flags;
  size_t i;
  size_t ಅಂಕೆ_wordlist_flags = ಪರಿಮಾಣ (wordlist) / ಪರಿಮಾಣ (wordlist[0]);

  flags = xmalloc0 ((ಪರಿಮಾಣ (ಸಂರಚನಾ propagation_flags_s) + 1) * ಅಂಕೆ_wordlist_flags);
  ಪ್ರತಿ (i = 0; i < ಅಂಕೆ_wordlist_flags; i++)
    flags[i].name = wordlist[i].name;

  ಪ್ರತ್ಯಾಗಮ flags;
}
