#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int spawn(char *program, char **arg_list) {
    pid_t child_pid;

    child_pid = fork();
    if (child_pid != 0)
        return child_pid;
    else {
        execvp(program,  arg_list);
        /* The execvp  function returns only if an error occurs.  */
        fprintf(stderr, "An error occurred in execvp\n");
        abort();
    }
}

int main() {
    char *arg_list[] = {
        "ls",     /* argv[0], the name of the program.  */
        "-l",
        "/",
        NULL      /* The argument list must end with a NULL.  */
    };

    spawn("ls", arg_list);

    printf("Done with main program\n");

    return 0;
}
