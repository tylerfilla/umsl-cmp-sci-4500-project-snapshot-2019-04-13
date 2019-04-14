/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "op/friend_list.h"
#include "op/friend_remove.h"
#include "op/interact.h"

#include "global.h"
#include "version.h"

/** A program operation. */
enum operation {
  op_nop = 0,
  op_friend_list,
  op_friend_remove,
  op_interact,
};

/** A command-line option. */
struct option {
  /** The number of aliases. */
  size_t num_aliases;

  /** The option aliases. */
  const char** aliases;

  /** The option description. */
  const char* description;

  /** Whether the option is a flag. A flag has no data. */
  int is_flag;

  /** The option data storage. */
  const char** data;
};

/** A command-line subcommand. */
struct subcommand {
  /** The command canonical name. */
  const char* canon_name;

  /** The command description. */
  const char* description;

  /** The number of aliases. */
  size_t num_aliases;

  /** The command aliases. */
  const char** aliases;

  /** The command operation. */
  enum operation operation;

  /** The number of subcommands. */
  size_t num_subcommands;

  /** The command subcommands. */
  const struct subcommand* subcommands;

  /** The number of options. */
  size_t num_options;

  /** The command options. */
  const struct option* options;
};

/** Option data for help flag. */
static const char* g_opt_data_help;

/** Option data for legal flag. */
static const char* g_opt_data_legal;

/** Option data for version flag. */
static const char* g_opt_data_version;

/** Option data for SQL hostname. */
static const char* g_opt_data_sql_host;

/** Option data for SQL server username. */
static const char* g_opt_data_sql_user;

/** Option data for SQL server password. */
static const char* g_opt_data_sql_pass;

/** Option data for SQL database name. */
static const char* g_opt_data_sql_db;

/** Option data for friend IDs. */
static const char* g_opt_data_friend_id;

/** The subcommand layout. */
static const struct subcommand g_subcommand_root = {
  .canon_name = NULL,
  .description = NULL,
  .num_aliases = 0,
  .aliases = NULL,
  .operation = op_nop,
  .num_subcommands = 2,
  .subcommands = (struct subcommand[]) {
    {
      .canon_name = "friend",
      .description = "friend management kit",
      .num_aliases = 2,
      .aliases = (const char* []) {"friend", "fr"},
      .operation = op_nop,
      .num_subcommands = 2,
      .subcommands = (struct subcommand[]) {
        {
          .canon_name = "friend list",
          .description = "list friend details",
          .num_aliases = 2,
          .aliases = (const char* []) {"list", "ls"},
          .operation = op_friend_list,
          .num_subcommands = 0,
          .subcommands = NULL,
          .num_options = 1,
          .options = (struct option[]) {
            {
              .num_aliases = 2,
              .aliases = (const char* []) {"-f", "--friend"},
              .description = "the friend id",
              .is_flag = 0,
              .data = &g_opt_data_friend_id,
            },
          },
        },
        {
          .canon_name = "friend remove",
          .description = "remove friends from memory",
          .num_aliases = 2,
          .aliases = (const char* []) {"remove", "rm"},
          .operation = op_friend_remove,
          .num_subcommands = 0,
          .subcommands = NULL,
          .num_options = 1,
          .options = (struct option[]) {
            {
              .num_aliases = 2,
              .aliases = (const char* []) {"-f", "--friend"},
              .description = "the friend id",
              .is_flag = 0,
              .data = &g_opt_data_friend_id,
            },
          },
        },
      },
    },
    {
      .canon_name = "go",
      .description = "start interactive session",
      .num_aliases = 1,
      .aliases = (const char* []) {"go"},
      .operation = op_interact,
      .num_subcommands = 0,
      .subcommands = NULL,
      .num_options = 0,
      .options = NULL,
    },
  },
  .num_options = 7,
  .options = (struct option[]) {
    {
      .num_aliases = 2,
      .aliases = (const char* []) {"-h", "--help"},
      .description = "show help information",
      .is_flag = 1,
      .data = &g_opt_data_help,
    },
    {
      .num_aliases = 1,
      .aliases = (const char* []) {"--legal"},
      .description = "show legal information",
      .is_flag = 1,
      .data = &g_opt_data_legal,
    },
    {
      .num_aliases = 1,
      .aliases = (const char* []) {"--version"},
      .description = "show version information",
      .is_flag = 1,
      .data = &g_opt_data_version,
    },
    {
      .num_aliases = 1,
      .aliases = (const char* []) {"--sql-host"},
      .description = "set hostname for sql server",
      .is_flag = 0,
      .data = &g_opt_data_sql_host,
    },
    {
      .num_aliases = 1,
      .aliases = (const char* []) {"--sql-user"},
      .description = "set username for sql server",
      .is_flag = 0,
      .data = &g_opt_data_sql_user,
    },
    {
      .num_aliases = 1,
      .aliases = (const char* []) {"--sql-pass"},
      .description = "set password for sql server",
      .is_flag = 0,
      .data = &g_opt_data_sql_pass,
    },
    {
      .num_aliases = 1,
      .aliases = (const char* []) {"--sql-db"},
      .description = "set database for sql server",
      .is_flag = 0,
      .data = &g_opt_data_sql_db,
    },
  },
};

/**
 * Print header text for a subcommand.
 *
 * Header text is the common "usage: ..." text at the beginning of both usage
 * and help text.
 *
 * @param cmd The subcommand
 */
static void print_header(const struct subcommand* cmd) {
  if (cmd->canon_name) {
    printf("usage: cozmo %s <command> [<args>]\n", cmd->canon_name);
    printf("   or: cozmo %s <opts>\n", cmd->canon_name);
  } else {
    printf("usage: cozmo <command> [<args>]\n");
    printf("   or: cozmo <opts>\n");
  }
}

/**
 * Print help text for a subcommand.
 *
 * @param cmd The subcommand
 */
static void print_help(const struct subcommand* cmd) {
  print_header(cmd);

  // If command has neither subcommands nor options
  if (cmd->num_subcommands == 0 && cmd->num_options == 0) {
    printf("\nno subcommands or options\n");
  }

  // If command has subcommands
  if (cmd->num_subcommands > 0) {
    printf("\nsubcommands:\n");

    // The target padding between aliases and description
    int target_pad = 16;

    // Loop through sub-subcommands for subcommand
    for (int i = 0; i < cmd->num_subcommands; ++i) {
      // The subcommand under consideration
      const struct subcommand* c = &cmd->subcommands[i];

      // Left spacing
      printf("  ");

      // Print aliases with separating commas
      int pad = 0;
      for (int j = 0; j < c->num_aliases; ++j) {
        // The alias under consideration
        const char* a = c->aliases[j];

        printf("%s", a);
        pad += strlen(a);

        // If not last, add a comma
        if (j < c->num_aliases - 1) {
          printf(", ");
          pad += 2;
        }
      }

      // Pad the space between aliases and description
      for (; pad < target_pad; ++pad) {
        printf(" ");
      }

      // Print description of sub-subcommand
      printf("%s\n", c->description);
    }
  }

  // If command has options
  if (cmd->num_options > 0) {
    printf("\noptions:\n");

    // The target padding between aliases and description
    int target_pad = 16;

    // Loop through options for subcommand
    for (int i = 0; i < cmd->num_options; ++i) {
      // The option under consideration
      const struct option* o = &cmd->options[i];

      // Left spacing
      printf("  ");

      // Print aliases with separating commas
      int pad = 0;
      for (int j = 0; j < o->num_aliases; ++j) {
        // The alias under consideration
        const char* a = o->aliases[j];

        printf("%s", a);
        pad += strlen(a);

        // If not last, add a comma
        if (j < o->num_aliases - 1) {
          printf(", ");
          pad += 2;
        }
      }

      // Pad the space between aliases and description
      for (; pad < target_pad; ++pad) {
        printf(" ");
      }

      // Print description of option
      printf("%s\n", o->description);
    }
  }
}

/**
 * Print usage text for a subcommand.
 *
 * @param cmd The subcommand
 */
static void print_usage(const struct subcommand* cmd) {
  print_header(cmd);

  if (cmd->canon_name) {
    printf("\ntry `cozmo %s -h' for more info\n", cmd->canon_name);
  } else {
    printf("\ntry `cozmo -h' for more info\n");
  }
}

/** Print legal text. */
static void print_legal() {
  printf("Cozmonaut\n");
  printf("Copyright 2019 The Cozmonaut Contributors\n");
}

/** Print version text. */
static void print_version() {
  printf("cozmo " VERSION_STRING "\n");
  printf("built on " VERSION_TIMESTAMP "\n");
  printf("vcs " VERSION_VCS_HASH " (" VERSION_VCS_BRANCH ")\n");
}

/**
 * Try to match a command-line argument to a subcommand option.
 *
 * @param chain The subcommand chain
 * @param chain_len The length of the subcommand chain
 * @param arg The raw argument text
 * @return The option or NULL on failure
 */
static const struct option* match_option(const struct subcommand** chain, size_t chain_len, const char* arg) {
  // Walk in reverse up the command chain
  // This simulates inheritance (allowing, e.g., friend subcommands to inherit the --help option)
  for (ssize_t i = chain_len - 1; i >= 0; --i) {
    // The subcommand under consideration
    const struct subcommand* c = chain[i];

    // Loop through options on command
    for (int j = 0; j < c->num_options; ++j) {
      // The option under consideration
      const struct option* o = &c->options[j];

      // Loop through aliases on option
      for (int k = 0; k < o->num_aliases; ++k) {
        // The alias under consideration
        const char* a = o->aliases[k];

        // If this alias matches the argument
        if (!strcmp(arg, a)) {
          // Return the option
          return o;
        }
      }
    }
  }

  // Control fell out somewhere without an option
  return NULL;
}

/**
 * Try to match a command-line argument to a subcommand.
 *
 * @param chain The subcommand chain
 * @param chain_len The length of the subcommand chain
 * @param arg The raw argument text
 * @return The subcommand or NULL on failure
 */
static const struct subcommand* match_subcommand(const struct subcommand** chain, size_t chain_len, const char* arg) {
  // We do not walk the subcommand chain when matching subcommands
  // Only the topmost subcommand on the chain can influence the next subcommand
  const struct subcommand* cmd_back = chain[chain_len - 1];

  // Loop through subcommands on subcommand
  for (int i = 0; i < cmd_back->num_subcommands; ++i) {
    // The subcommand under consideration
    const struct subcommand* c = &cmd_back->subcommands[i];

    // Loop through aliases on subcommand
    for (int j = 0; j < c->num_aliases; ++j) {
      // The alias under consideration
      const char* a = c->aliases[j];

      // If this alias matches the argument
      if (!strcmp(arg, a)) {
        // Return the subcommand
        return c;
      }
    }
  }

  // Control fell out somewhere without a subcommand
  return NULL;
}

/**
 * Read command-line arguments.
 *
 * Most data is passed out via parameters. No parameter outputs are modified
 * unless this function indicates success by returning zero.
 *
 * @param [out] op The computed program operation
 * @param [out] out_cmd_chain The command chain
 * @param [out] out_cmd_chain_len The length of the command chain
 * @return Zero on success, otherwise nonzero
 */
static int read_arguments(enum operation* op, const struct subcommand*** out_cmd_chain, size_t* out_cmd_chain_len) {
  // The command chain (initialized to root)
  // This will grow as subcommands are encountered
  // It's job is to act like a running filter for matching subcommands and options
  const struct subcommand** cmd_chain = malloc(sizeof *cmd_chain);
  cmd_chain[0] = &g_subcommand_root;

  // The current command chain length
  size_t cmd_chain_len = 1;

  // Loop through user-given command-line arguments
  // We skip the first argument, as libc sets it to the process path
  for (int i = 1; i < g->argc; ++i) {
    // The argument under consideration
    const char* arg = g->argv[i];
    size_t arg_len = strlen(arg);

    // Try to classify the argument as either an option or a subcommand
    if (arg_len > 2 && arg[0] == '-' && arg[1] == '-') {
      // Argument starts with two dashes, so treat it like a long (GNU-style) option

      // The option name (not null-terminated)
      const char* opt_name = &arg[2];

      // Length of option name
      size_t opt_name_len = arg_len - 2;

      // The option data payload (null-terminated)
      const char* opt_data = NULL;

      // Search the argument for the first equal sign
      // Long options can contain data in the form --key=value
      const char* equal_sign = strchr(opt_name, '=');

      // If there is an equal sign
      if (equal_sign) {
        // Truncate the name, as it erroneously contains the data payload
        opt_name_len = equal_sign - opt_name;

        // Create a view of the data payload
        // This is safe, as we will always hit the null terminator
        opt_data = equal_sign + 1;
      }

      // Make a string buffer framing the double-dash-prefixed name component as an independent argument
      // This is necessary to guarantee that the string is followed by a null terminator
      char* opt_arg = strndup(arg, opt_name_len + 2);

      // Try to match option
      const struct option* opt;
      if ((opt = match_option(cmd_chain, cmd_chain_len, opt_arg)) != NULL) {
        // If option is a flag
        if (opt->is_flag) {
          // Set flag data
          *opt->data = "true";
        } else {
          // Option is not a flag, so it has a data requirement
          // Long options can get data either from their data payloads or from more arguments

          // If data payload is non-empty
          if (opt_data) {
            // Map data payload directly
            *opt->data = opt_data;
          } else {
            // Data payload is empty

            // Read data from next argument
            // This advances the argument iterator above (variable i)
            if (++i >= g->argc) {
              fprintf(stderr, "no data given to long option: %s\n", opt_arg);

              free(opt_arg);
              free(cmd_chain);
              return 1;
            }

            // Map next argument as this argument's data
            *opt->data = g->argv[i];
          }
        }

        // Go to next argument
        free(opt_arg);
        continue;
      }

      fprintf(stderr, "no such long option: %s\n", arg);
      free(opt_arg);
      free(cmd_chain);
      return 1;
    } else if (arg_len > 1 && arg[0] == '-') {
      // Argument starts with one dash and is not a long option, so treat it like a short (POSIX-style) option(s)

      // Loop through characters in argument
      // For short options, compounding can occur (e.g. -abc means -a -b -c)
      // So, we treat each character as its own individual short option
      for (int j = 1; j < arg_len; ++j) {
        // The character under consideration
        char c = arg[j];

        // Make a small string buffer framing the character as an independent argument
        const char* c_arg = (char[]) {'-', c, 0};

        // Try to match option
        const struct option* opt;
        if ((opt = match_option(cmd_chain, cmd_chain_len, c_arg)) != NULL) {
          // If option is a flag
          if (opt->is_flag) {
            // Set flag data
            *opt->data = "true";
          } else {
            // Option is not a flag, so it has a data requirement
            // Short options can only get data from more arguments

            // Read data from next argument
            // This advances the argument iterator above (variable i)
            if (++i >= g->argc) {
              fprintf(stderr, "no data given to short option: -%c\n", c);

              free(cmd_chain);
              return 1;
            }

            // Map next argument as this argument's data
            *opt->data = g->argv[i];
          }

          // Go to next short option
          // This jumps back into the character iteration
          continue;
        }

        fprintf(stderr, "no such short option: -%c\n", c);

        free(cmd_chain);
        return 1;
      }
    } else {
      // Argument is not an option, so treat it like a subcommand

      // Try to match subcommand
      const struct subcommand* cmd;
      if ((cmd = match_subcommand(cmd_chain, cmd_chain_len, arg)) != NULL) {
        // Append to the subcommand chain
        cmd_chain = realloc(cmd_chain, ++cmd_chain_len);
        cmd_chain[cmd_chain_len - 1] = cmd;

        // Go to next argument
        continue;
      }

      fprintf(stderr, "no such subcommand: %s\n", arg);

      free(cmd_chain);
      return 1;
    }
  }

  // Compute operation and return it with the command chain
  *op = cmd_chain[cmd_chain_len - 1]->operation;
  *out_cmd_chain = cmd_chain;
  *out_cmd_chain_len = cmd_chain_len;
  return 0;
}

int main(int argc, char* argv[]) {
  g_mut->argc = argc;
  g_mut->argv = (const char**) argv;

  enum operation op;
  const struct subcommand** cmd_chain;
  size_t cmd_chain_len;

  // Try to read command-line arguments
  if (read_arguments(&op, &cmd_chain, &cmd_chain_len)) {
    printf("\n");

    // Show usage for root command
    print_usage(&g_subcommand_root);
    return 1;
  }

  // If help flag provided
  if (g_opt_data_help) {
    // Show help for topmost command
    print_help(cmd_chain[cmd_chain_len - 1]);
    return 0;
  }

  // If legal flag provided
  if (g_opt_data_legal) {
    // Show legal text
    print_legal();
    return 0;
  }

  // If version flag provided
  if (g_opt_data_version) {
    // Show version text
    print_version();
    return 0;
  }

  // If no operation was computed
  if (op == op_nop) {
    // Show usage for topmost command
    print_usage(cmd_chain[cmd_chain_len - 1]);
    return 0;
  }

  // Dispatch the computed operation
  switch (op) {
    case op_friend_list: {
      // If optional friend ID was given
      int friend_id = -1;
      if (g_opt_data_friend_id) {
        // Convert friend ID to an integer
        char* friend_id_error;
        friend_id = (int) strtol(g_opt_data_friend_id, &friend_id_error, 10);
        if (friend_id_error != g_opt_data_friend_id + strlen(g_opt_data_friend_id)) {
          fprintf(stderr, "malformed friend id: %s\n", g_opt_data_friend_id);
          return 1;
        }

        // Enforce range
        if (friend_id <= 0) {
          fprintf(stderr, "friend id out of range: %s\n", g_opt_data_friend_id);
          return 1;
        }
      }

      // Call friend list operation
      return op_friend_list_main(&(struct op_friend_list_args) {
        .friend_id = friend_id,
      });
    }
    case op_friend_remove: {
      // If friend ID was given
      int friend_id = -1;
      if (g_opt_data_friend_id) {
        // Convert friend ID to an integer
        char* friend_id_error;
        friend_id = (int) strtol(g_opt_data_friend_id, &friend_id_error, 10);
        if (friend_id_error != g_opt_data_friend_id + strlen(g_opt_data_friend_id)) {
          fprintf(stderr, "malformed friend id: %s\n", g_opt_data_friend_id);
          return 1;
        }

        // Enforce range
        if (friend_id <= 0) {
          fprintf(stderr, "friend id out of range: %s\n", g_opt_data_friend_id);
          return 1;
        }
      } else {
        // Friend ID is required
        fprintf(stderr, "friend id is required\n");
        return 1;
      }

      // Call friend removal operation
      return op_friend_remove_main(&(struct op_friend_remove_args) {
        .friend_id = friend_id,
      });
    }
    case op_interact: {
      // Call interaction operation
      return op_interact_main(&(struct op_interact_args) {});
    }
  }
}
