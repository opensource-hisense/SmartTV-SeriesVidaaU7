/* tinymix.c
**
** Copyright 2011, The Android Open Source Project
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of The Android Open Source Project nor the names of
**       its contributors may be used to endorse or promote products derived
**       from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY The Android Open Source Project ``AS IS'' AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL The Android Open Source Project BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
** CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
** OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
** DAMAGE.
*/

#include <tinyalsa/asoundlib.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

#define PARTIAL_PRINT_LENGTH 64
int g_partial_print = 0;
#define MAX_STRING_LENGTH     255
#define LENGTH_OF_ORG_CMD       3 //strlen("get")
int g_print_string = 0;

#define OPTPARSE_IMPLEMENTATION
#include "optparse.h"

static void tinymix_list_controls(struct mixer *mixer, int print_all);

static void tinymix_detail_control(struct mixer *mixer, const char *control);

static int tinymix_set_value(struct mixer *mixer, const char *control,
                             char **values, unsigned int num_values);

static void tinymix_print_enum(struct mixer_ctl *ctl);

void usage(void)
{
    printf("usage: tinymix [options] <command>\n");
    printf("options:\n");
    printf("\t-h, --help        : prints this help message and exits\n");
    printf("\t-v, --version     : prints this version of tinymix and exits\n");
    printf("\t-D, --card NUMBER : specifies the card number of the mixer\n");
    printf("commands:\n");
    printf("\tget NAME|ID       : prints the values of a control\n");
    printf("\tset NAME|ID VALUE : sets the value of a control\n");
    printf("\tcontrols          : lists controls of the mixer\n");
    printf("\tcontents          : lists controls of the mixer and their contents\n");
    printf("\tcontents_s        : lists controls of the mixer and their shrinking contents\n");
    printf("\tget_s NAME|ID       : prints the value as string of a control\n");
    printf("\tset_s NAME|ID VALUE : sets the value as string of a control\n");
}

void version(void)
{
    printf("tinymix version 2.0 (tinyalsa version %s)\n", TINYALSA_VERSION_STRING);
}

int main(int argc, char **argv)
{
    struct mixer *mixer;
    int card = 0, c;
    char *cmd;
    struct optparse opts;
    static struct optparse_long long_options[] = {
        { "card",    'D', OPTPARSE_REQUIRED },
        { "version", 'v', OPTPARSE_NONE     },
        { "help",    'h', OPTPARSE_NONE     },
        { 0, 0, 0 }
    };

    optparse_init(&opts, argv);
    /* Detect the end of the options. */
    while ((c = optparse_long(&opts, long_options, NULL)) != -1) {
        switch (c) {
        case 'D':
            card = atoi(opts.optarg);
            break;
        case 'h':
            usage();
            return EXIT_SUCCESS;
        case 'v':
            version();
            return EXIT_SUCCESS;
        case '?':
            fprintf(stderr, "%s\n", opts.errmsg);
            return EXIT_FAILURE;
        }
    }

    mixer = mixer_open(card);
    if (!mixer) {
        fprintf(stderr, "Failed to open mixer\n");
        return EXIT_FAILURE;
    }

    cmd = argv[opts.optind];
    if (cmd != NULL) {
        if ((strcmp(cmd, "get_s") == 0) ||
            (strcmp(cmd, "set_s") == 0)) {
            g_print_string = 1;
            cmd[LENGTH_OF_ORG_CMD] = 0x00; //goes to "get" or "set" flow.
        }
    }
    if (cmd == NULL) {
        fprintf(stderr, "no command specified (see --help)\n");
        mixer_close(mixer);
        return EXIT_FAILURE;
    } else if (strcmp(cmd, "get") == 0) {
        if ((opts.optind + 1) >= argc) {
            fprintf(stderr, "no control specified\n");
            mixer_close(mixer);
            return EXIT_FAILURE;
        }
        tinymix_detail_control(mixer, argv[opts.optind + 1]);
        printf("\n");
    } else if (strcmp(cmd, "set") == 0) {
        if ((opts.optind + 1) >= argc) {
            fprintf(stderr, "no control specified\n");
            mixer_close(mixer);
            return EXIT_FAILURE;
        }
        if ((opts.optind + 2) >= argc) {
            fprintf(stderr, "no value(s) specified\n");
            mixer_close(mixer);
            return EXIT_FAILURE;
        }
        int res = tinymix_set_value(mixer, argv[opts.optind + 1], &argv[opts.optind + 2], argc - opts.optind - 2);
        if (res != 0) {
            mixer_close(mixer);
            return EXIT_FAILURE;
        }
    } else if (strcmp(cmd, "controls") == 0) {
        tinymix_list_controls(mixer, 0);
    } else if (strcmp(cmd, "contents") == 0) {
        tinymix_list_controls(mixer, 1);
    } else if (strcmp(cmd, "contents_s") == 0) {
        g_partial_print = 1;
        tinymix_list_controls(mixer, 1);
    } else {
        fprintf(stderr, "unknown command '%s' (see --help)\n", cmd);
        mixer_close(mixer);
        return EXIT_FAILURE;
    }

    mixer_close(mixer);
    return EXIT_SUCCESS;
}

static void tinymix_list_controls(struct mixer *mixer, int print_all)
{
    struct mixer_ctl *ctl;
    const char *name, *type;
    unsigned int num_ctls, num_values;
    unsigned int i;

    num_ctls = mixer_get_num_ctls(mixer);

    printf("Number of controls: %u\n", num_ctls);

    if (print_all)
        printf("ctl\ttype\tnum\t%-40svalue\n", "name");
    else
        printf("ctl\ttype\tnum\t%-40s\n", "name");

    for (i = 0; i < num_ctls; i++) {
        ctl = mixer_get_ctl(mixer, i);

        name = mixer_ctl_get_name(ctl);
        type = mixer_ctl_get_type_string(ctl);
        num_values = mixer_ctl_get_num_values(ctl);
        printf("%u\t%s\t%u\t%-40s", i, type, num_values, name);
        if (print_all)
            tinymix_detail_control(mixer, name);
        printf("\n");
    }
}

static void tinymix_print_enum(struct mixer_ctl *ctl)
{
    unsigned int num_enums;
    unsigned int i;
    unsigned int value;
    const char *string;

    num_enums = mixer_ctl_get_num_enums(ctl);
    value = mixer_ctl_get_value(ctl, 0);

    for (i = 0; i < num_enums; i++) {
        string = mixer_ctl_get_enum_string(ctl, i);
        printf("%s%s, ", value == i ? "> " : "", string);
    }
}

static int check_valid_string_format(char *buf, unsigned int num_values)
{
    unsigned int i;
    unsigned int string_length;

    for ( i = 0;i < num_values;i++) {
        if (buf[i] == 0x00)
            break;
        if(!isprint(buf[i]))
            return 0;
    }
    string_length = strlen(buf);
    if (string_length > MAX_STRING_LENGTH)
        return -1;
    return string_length;
}

static int tinymix_set_string_ctl(struct mixer_ctl *ctl,
                                 char **values, unsigned int num_values)
{
    int ret;
    int string_length;

    if (num_values != 1) // only support 1 parameter.
        return -1;

    string_length = check_valid_string_format(values[0], MAX_STRING_LENGTH);

    if (string_length < 0) {
        printf("not a valid string\n");
        return -1;
    }

    // for empty string
    if (string_length == 0)
        string_length++;

    ret = mixer_ctl_set_array(ctl, values[0], string_length);
    if (ret < 0) {
        printf("Failed to set binary control\n");
        return -1;
    }
    return 0;
}
static void tinymix_detail_control(struct mixer *mixer, const char *control)
{
    struct mixer_ctl *ctl;
    enum mixer_ctl_type type;
    unsigned int num_values;
    unsigned int i;
    int min, max;
    int ret;
    char *buf = NULL;
    unsigned int skipped_length = 0;
    int string_length = 0;

    if (isdigit(control[0]))
        ctl = mixer_get_ctl(mixer, atoi(control));
    else
        ctl = mixer_get_ctl_by_name(mixer, control);

    if (!ctl) {
        fprintf(stderr, "Invalid mixer control\n");
        return;
    }

    type = mixer_ctl_get_type(ctl);
    num_values = mixer_ctl_get_num_values(ctl);

    if ((type == MIXER_CTL_TYPE_BYTE) && (num_values > 0)) {
        buf = calloc(1, num_values);
        if (buf == NULL) {
            fprintf(stderr, "Failed to alloc mem for bytes %u\n", num_values);
            return;
        }

        ret = mixer_ctl_get_array(ctl, buf, num_values);
        if (ret < 0) {
            fprintf(stderr, "Failed to mixer_ctl_get_array\n");
            free(buf);
            return;
        }
    }

    if ((g_partial_print) &&
        (type == MIXER_CTL_TYPE_BYTE)) {
        if (num_values > PARTIAL_PRINT_LENGTH) {
            skipped_length = num_values - PARTIAL_PRINT_LENGTH;
            num_values = PARTIAL_PRINT_LENGTH;
        }
    }

    if (g_print_string) {
        if (type == MIXER_CTL_TYPE_BYTE) {
            if (buf && (num_values > 0)) {
                string_length = check_valid_string_format(buf, num_values);
                if (string_length < 0) {
                    printf("not a valid string\n");
                } else {
                    printf("%s",buf);
                }
            }
        } else {
            printf("Only support BYTE type to show string format.");
        }
        num_values = 0; //case handled, to skip for-loop
    }
    for (i = 0; i < num_values; i++) {
        switch (type)
        {
        case MIXER_CTL_TYPE_INT:
            printf("%d", mixer_ctl_get_value(ctl, i));
            break;
        case MIXER_CTL_TYPE_BOOL:
            printf("%s", mixer_ctl_get_value(ctl, i) ? "On" : "Off");
            break;
        case MIXER_CTL_TYPE_ENUM:
            tinymix_print_enum(ctl);
            break;
        case MIXER_CTL_TYPE_BYTE:
            printf(" %02x", buf[i]);
            break;
        default:
            printf("unknown");
            break;
        };
        if ((i + 1) < num_values) {
           printf(", ");
        }
    }

    if (type == MIXER_CTL_TYPE_INT) {
        min = mixer_ctl_get_range_min(ctl);
        max = mixer_ctl_get_range_max(ctl);
        printf(" (range %d->%d)", min, max);
    }

    if ((g_partial_print) &&
        (type == MIXER_CTL_TYPE_BYTE)) {
        if (skipped_length) {
            printf(" (%d bytes skipped)", skipped_length);
        }
    }

    free(buf);
}

static void tinymix_set_byte_ctl(struct mixer_ctl *ctl,
                                 char **values, unsigned int num_values)
{
    int ret;
    char *buf;
    char *end;
    unsigned int i;
    long n;

    buf = calloc(1, num_values);
    if (buf == NULL) {
        fprintf(stderr, "set_byte_ctl: Failed to alloc mem for bytes %u\n", num_values);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < num_values; i++) {
        errno = 0;
        n = strtol(values[i], &end, 0);
        if (*end) {
            fprintf(stderr, "%s not an integer\n", values[i]);
            goto fail;
        }
        if (errno) {
            fprintf(stderr, "strtol: %s: %s\n", values[i],
                strerror(errno));
            goto fail;
        }
        if (n < 0 || n > 0xff) {
            fprintf(stderr, "%s should be between [0, 0xff]\n",
                values[i]);
            goto fail;
        }
        buf[i] = n;
    }

    ret = mixer_ctl_set_array(ctl, buf, num_values);
    if (ret < 0) {
        fprintf(stderr, "Failed to set binary control\n");
        goto fail;
    }

    free(buf);
    return;

fail:
    free(buf);
    exit(EXIT_FAILURE);
}

static int is_int(const char *value)
{
    return (value[0] >= '0') || (value[0] <= '9');
}

struct parsed_int
{
  /** Wether or not the integer was valid. */
  int valid;
  /** The value of the parsed integer. */
  int value;
  /** The number of characters that were parsed. */
  unsigned int length;
  /** The number of characters remaining in the string. */
  unsigned int remaining_length;
  /** The remaining characters (or suffix) of the integer. */
  const char* remaining;
};

static struct parsed_int parse_int(const char* str)
{
  struct parsed_int out = {
    0 /* valid */,
    0 /* value */,
    0 /* length */,
    0 /* remaining length */,
    "" /* remaining characters */
  };

  unsigned int max = strlen(str);

  for (unsigned int i = 0; i < max; i++) {

    char c = str[i];

    if ((c < '0') || (c > '9')) {
      break;
    }

    out.value *= 10;
    out.value += c - '0';

    out.length++;
  }

  out.valid = out.length > 0;
  out.remaining_length = max - out.length;
  out.remaining = str + out.length;

  return out;
}

struct control_value
{
    int value;
    int is_percent;
    int is_relative;
};

static struct control_value to_control_value(const char* value_string)
{
    struct parsed_int parsed_int = parse_int(value_string);

    struct control_value out = {
        0 /* value */,
        0 /* is percent */,
        0 /* is relative */
    };

    out.value = parsed_int.value;

    unsigned int i = 0;

    if (parsed_int.remaining[i] == '%') {
      out.is_percent = 1;
      i++;
    }

    if (parsed_int.remaining[i] == '+') {
      out.is_relative = 1;
    } else if (parsed_int.remaining[i] == '-') {
      out.is_relative = 1;
      out.value *= -1;
    }

    return out;
}

static int set_control_value(struct mixer_ctl* ctl, unsigned int i, const struct control_value* value)
{
    int next_value = value->value;

    if (value->is_relative) {

        int prev_value = value->is_percent ? mixer_ctl_get_percent(ctl, i)
                                           : mixer_ctl_get_value(ctl, i);

        if (prev_value < 0) {
          return prev_value;
        }

        next_value += prev_value;
    }

    return value->is_percent ? mixer_ctl_set_percent(ctl, i, next_value)
                             : mixer_ctl_set_value(ctl, i, next_value);
}

static int set_control_values(struct mixer_ctl* ctl,
                              char** values,
                              unsigned int num_values)
{
    unsigned int num_ctl_values = mixer_ctl_get_num_values(ctl);

    if (num_values == 1) {

        /* Set all values the same */
        struct control_value value = to_control_value(values[0]);

        for (unsigned int i = 0; i < num_values; i++) {
            int res = set_control_value(ctl, i, &value);
            if (res != 0) {
                fprintf(stderr, "Error: invalid value\n");
                return -1;
            }
        }

    } else {

        /* Set multiple values */
        if (num_values > num_ctl_values) {
            fprintf(stderr,
                    "Error: %u values given, but control only takes %u\n",
                    num_values, num_ctl_values);
            return -1;
        }

        for (unsigned int i = 0; i < num_values; i++) {

            struct control_value v = to_control_value(values[i]);

            int res = set_control_value(ctl, i, &v);
            if (res != 0) {
                fprintf(stderr, "Error: invalid value for index %u\n", i);
                return -1;
            }
        }
    }

    return 0;
}

static int tinymix_set_value(struct mixer *mixer, const char *control,
                             char **values, unsigned int num_values)
{
    struct mixer_ctl *ctl;
    enum mixer_ctl_type type;

    if (isdigit(control[0]))
        ctl = mixer_get_ctl(mixer, atoi(control));
    else
        ctl = mixer_get_ctl_by_name(mixer, control);

    if (!ctl) {
        fprintf(stderr, "Invalid mixer control\n");
        return -1;
    }

    type = mixer_ctl_get_type(ctl);
    if (g_print_string) {
        if (type != MIXER_CTL_TYPE_BYTE) {
            fprintf(stderr, "Only support BYTE type to set string\n");
            return 0;
        }
        if (tinymix_set_string_ctl(ctl, values, num_values) != 0) {
            fprintf(stderr, "Not a valid string format.\n");
        }
        return 0;
    }

    if (type == MIXER_CTL_TYPE_BYTE) {
        tinymix_set_byte_ctl(ctl, values, num_values);
        return 0;
    }

    if (is_int(values[0])) {
        set_control_values(ctl, values, num_values);
    } else {
        if (type == MIXER_CTL_TYPE_ENUM) {
            if (num_values != 1) {
                fprintf(stderr, "Enclose strings in quotes and try again\n");
                return -1;
            }
            if (mixer_ctl_set_enum_by_string(ctl, values[0])) {
                fprintf(stderr, "Error: invalid enum value\n");
                return -1;
            }
        } else {
            fprintf(stderr, "Error: only enum types can be set with strings\n");
            return -1;
        }
    }

    return 0;
}

