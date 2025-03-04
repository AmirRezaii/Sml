#define NOB_IMPLEMENTATION
#include "nob.h"

#define BUILD_DIR "build/"

void run(Nob_Cmd *cmd) {
    nob_cmd_append(cmd, BUILD_DIR"sml");
    if (!nob_cmd_run_sync_and_reset(cmd))
      return;
}

int main(int argc, char **argv) {
  NOB_GO_REBUILD_URSELF(argc, argv);


  if (!nob_mkdir_if_not_exists(BUILD_DIR))
    return 1;

  Nob_Cmd cmd = {0};
  nob_cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-o", BUILD_DIR"sml", "main.c",
                 "matrix.c", "-lm");

  if (!nob_cmd_run_sync_and_reset(&cmd))
    return 1;

  nob_shift(argv, argc);
  if (argc > 0) {
    char *arg = nob_shift(argv, argc);

    if (strcmp(arg, "run") == 0) {
      run(&cmd);
    }
  }


  return 0;
}
