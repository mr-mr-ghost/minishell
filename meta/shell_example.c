#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 64

// Function declarations
void prompt();
char *read_input();
char **parse_input(char *input);
int execute_command(char **args);
void handle_redirection(char **args);

int main() {
    char *input;
    char **args;
    int status = 1;

    while (status) {
        prompt();

        // Read input from the user
        input = read_input();

        // Parse the input into arguments
        args = parse_input(input);

        // Execute the command
        status = execute_command(args);

        // Free allocated memory
        free(input);
        free(args);
    }

    return 0;
}

// Function to display the prompt
void prompt() {
    printf("> ");
}

// Function to read input from the user
char *read_input() {
    char *input = malloc(MAX_COMMAND_LENGTH);
    if (!fgets(input, MAX_COMMAND_LENGTH, stdin)) {
        perror("fgets");
        exit(EXIT_FAILURE);
    }
    input[strcspn(input, "\n")] = '\0'; // Remove the newline character
    return input;
}

// Function to parse the input into a list of arguments
char **parse_input(char *input) {
    char **args = malloc(MAX_ARGS * sizeof(char *));
    char *arg;
    int position = 0;

    arg = strtok(input, " ");
    while (arg != NULL) {
        args[position++] = arg;
        arg = strtok(NULL, " ");
    }
    args[position] = NULL;
    return args;
}

// Function to execute commands
int execute_command(char **args) {
    pid_t pid;
    int status;

    if (args[0] == NULL) {
        // Empty command
        return 1;
    }

    if (strcmp(args[0], "exit") == 0) {
        // Exit the shell
        return 0;
    }

    pid = fork();
    if (pid == 0) {
        // Child process
        handle_redirection(args);
        if (execvp(args[0], args) == -1) {
            perror("execvp");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Forking error
        perror("fork");
    } else {
        // Parent process
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

// Function to handle redirection
void handle_redirection(char **args) {
    int i = 0;
    while (args[i] != NULL) {
        if (strcmp(args[i], ">") == 0) {
            int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            args[i] = NULL;
            break;
        } else if (strcmp(args[i], "<") == 0) {
            int fd = open(args[i + 1], O_RDONLY);
            if (fd < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
            args[i] = NULL;
            break;
        }
        i++;
    }
}
