/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
 * crun is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * crun is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with crun.  If not, see <http://www.gnu.org/licenses/>.
 */

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <argp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "crun.h"

ਸਥਿਰ ਅੱਖਰ doc[] = "OCI runtime";

ਗਿਣਤੀ
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

ਬਣਤਰ list_options_s
{
  bool quiet;
  ਪੂਰਨ format;
};

ਗਿਣਤੀ
{
  LIST_ਸਾਰਣੀ = 100,
  LIST_JSON,
};

ਸਥਿਰ ਬਣਤਰ list_options_s list_options;

ਸਥਿਰ ਬਣਤਰ argp_option options[]
    = { { "quiet", 'q', 0, 0, "show only IDs", 0 },
        { "format", 'f', "FORMAT", 0, "select one of: table or json (default: \"table\")", 0 },
        {
            0,
        } };

ਸਥਿਰ ਅੱਖਰ ਦਲੀਲਾਂ_doc[] = "list";

ਸਥਿਰ error_t
parse_opt (ਪੂਰਨ ਕੁੰਜੀ, ਅੱਖਰ *arg, ਬਣਤਰ argp_state *state arg_ਅਣਵਰਤਿਆ)
{
  ਚੋਣ (ਕੁੰਜੀ)
    {
    ਹਾਲਤ 'q':
      list_options.quiet = true;
      ਰੋਕੋ;
    ਹਾਲਤ 'f':
      ਜੇ (strcmp (arg, "table") == 0)
        list_options.format = LIST_ਸਾਰਣੀ;
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (arg, "json") == 0)
        list_options.format = LIST_JSON;
      ਨਹੀਂਤਾਂ
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      ਰੋਕੋ;

    ਮੂਲ:
      ਵਾਪਸ ARGP_ERR_UNKNOWN;
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਬਣਤਰ argp ਲੜੀਵਾਰ_argp = { options, parse_opt, ਦਲੀਲਾਂ_doc, doc, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ };

ਪੂਰਨ
crun_command_list (ਬਣਤਰ crun_global_arguments *global_ਦਲੀਲਾਂ, ਪੂਰਨ argc, ਅੱਖਰ **argv, libcrun_error_t *err)
{
  ਪੂਰਨ ਪਹਿਲਾ_arg;
  ਪੂਰਨ ret, max_ਲੰਮਾਈ = 4;
  cleanup_context libcrun_context_t *crun_context = ਨਿਰਾ;
  libcrun_container_list_t *list = ਨਿਰਾ;
  libcrun_container_iter_t *it;
  ਅਚਲ ਅੱਖਰ *name;

  list_options.format = LIST_ਸਾਰਣੀ;

  argp_parse (&ਲੜੀਵਾਰ_argp, argc, argv, ARGP_ਵਿੱਚ_ਕ੍ਰਮ, &ਪਹਿਲਾ_arg, &list_options);
  crun_assert_n_ਦਲੀਲਾਂ (argc - ਪਹਿਲਾ_arg, 0, 0);

  crun_context = new_libcrun_context (global_ਦਲੀਲਾਂ);

  ret = init_libcrun_context (crun_context, argv[ਪਹਿਲਾ_arg], global_ਦਲੀਲਾਂ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (list_options.format == LIST_JSON)
    {
      cleanup_free ਅੱਖਰ *json = ਨਿਰਾ;

      ret = libcrun_container_list_json (crun_context, &json, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      fputs (json, stdout);
      ਵਾਪਸ 0;
    }

  ret = libcrun_container_list (crun_context, &list, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਲਈ (it = libcrun_container_list_iter (list); libcrun_container_iter_ਅਗਲਾ (it, &name);)
    {
      ਪੂਰਨ l = ਲੜੀ_ਲੰਬਾਈ (name);
      ਜੇ (l > max_ਲੰਮਾਈ)
        max_ਲੰਮਾਈ = l;
    }
  libcrun_container_iter_free (it);

  max_ਲੰਮਾਈ++;

  ਜੇ (! list_options.quiet)
    ਲਿਖੋ ("%-*s%-10s%-8s %-39s %-30s %s\n", max_ਲੰਮਾਈ, "NAME", "PID", "STATUS", "BUNDLE PATH", "CREATED", "OWNER");

  ਲਈ (it = libcrun_container_list_iter (list); libcrun_container_iter_ਅਗਲਾ (it, &name);)
    {
      libcrun_ਸਥਿਤੀ_t *ਸਥਿਤੀ = ਨਿਰਾ;

      ret = libcrun_container_ਸਥਿਤੀ_ਲੱਦੋ (crun_context, name, &ਸਥਿਤੀ, err);
      ਜੇ (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          ਜਾਰੀ;
        }
      ਜੇ (list_options.quiet)
        ਲਿਖੋ ("%s\n", name);
      ਨਹੀਂਤਾਂ
        {
          libcrun_container_state_t state;
          ਪੂਰਨ running = 0;
          ਪੂਰਨ pid = libcrun_ਸਥਿਤੀ_get_pid (ਸਥਿਤੀ);

          ret = libcrun_ਸਥਿਤੀ_get_state (ਸਥਿਤੀ, &state, &running, err);
          ਜੇ (UNLIKELY (ret < 0))
            {
              libcrun_error_report_and_release (err);
              libcrun_container_ਸਥਿਤੀ_free (ਸਥਿਤੀ);
              ਜਾਰੀ;
            }

          ਜੇ (! running)
            pid = 0;

          ਲਿਖੋ ("%-*s%-10d%-8s %-39s %-30s %s\n", max_ਲੰਮਾਈ, name, pid,
                  libcrun_container_state_ਮੰਜ਼ਿਲ_string (state), libcrun_ਸਥਿਤੀ_get_bundle (ਸਥਿਤੀ),
                  libcrun_ਸਥਿਤੀ_get_created (ਸਥਿਤੀ), libcrun_ਸਥਿਤੀ_get_owner (ਸਥਿਤੀ));
        }

      libcrun_container_ਸਥਿਤੀ_free (ਸਥਿਤੀ);
    }
  libcrun_container_iter_free (it);

  libcrun_container_list_free (list);
  ਵਾਪਸ ret >= 0 ? 0 : ret;
}
