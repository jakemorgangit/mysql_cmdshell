#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Explicitly declare mkstemp prototype if not found
#ifndef __USE_XOPEN2K
extern int mkstemp (char *__template);
#endif

bool mysql_cmdshell_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
void mysql_cmdshell_deinit(UDF_INIT *initid);
char *mysql_cmdshell(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error);
int is_command_allowed(const char *command);

static const char *allowed_commands[] = {
};

bool mysql_cmdshell_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    if (args->arg_count != 1 || args->arg_type[0] != STRING_RESULT) {
        strcpy(message, "mysql_cmdshell() requires one string argument");
        return 1;
    }
    initid->maybe_null = 1;     // Allow NULL values
    initid->max_length = 8192;  // Initial max length for the result
    return 0;
}

void mysql_cmdshell_deinit(UDF_INIT *initid) {}

char *mysql_cmdshell(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error) {
    FILE *fp;
    char buffer[256];
    char temp_filename[] = "/tmp/mysql_cmdshell_output_XXXXXX";
    char debug_filename[] = "/tmp/mysql_cmdshell_debug.log";
    int fd = mkstemp(temp_filename);

    if (fd == -1) {
        *is_null = 1;
        return NULL;
    }
    close(fd); // Close the file descriptor

    // Check if the command is allowed
    if (!is_command_allowed(args->args[0])) {
        FILE *debug_fp = fopen(debug_filename, "a");
        if (debug_fp) {
            fprintf(debug_fp, "Rejected command: %s\n", args->args[0]);
            fclose(debug_fp);
        }
        *is_null = 1;
        unlink(temp_filename);
        return NULL;
    }

    // Construct command to redirect output to temp file
    char command[512];
    snprintf(command, sizeof(command), "%s > %s 2>&1", args->args[0], temp_filename);

    // Log the command execution
    FILE *debug_fp = fopen(debug_filename, "a");
    if (debug_fp) {
        fprintf(debug_fp, "Running command: %s\n", command);
        fclose(debug_fp);
    }

    // Run the command
    int sys_result = system(command);
    if (sys_result != 0) {
        unlink(temp_filename);
        *is_null = 1;
        return NULL;
    }

    // Read the output from the temp file
    fp = fopen(temp_filename, "r");
    if (fp == NULL) {
        unlink(temp_filename);
        *is_null = 1;
        return NULL;
    }

    // Initialize the result buffer
    size_t result_size = initid->max_length;
    result = malloc(result_size);
    if (result == NULL) {
        fclose(fp);
        unlink(temp_filename);
        *is_null = 1;
        return NULL;
    }
    result[0] = '\0';
    size_t total_length = 0;

    // Read the output incrementally
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        size_t len = strlen(buffer);
        if (total_length + len >= result_size) { // Ensuring we do not exceed result buffer size
            result_size = total_length + len + 1; // Adjust buffer size to exactly fit the output
            result = realloc(result, result_size);
            if (result == NULL) {
                fclose(fp);
                unlink(temp_filename);
                *is_null = 1;
                return NULL;
            }
        }
        strcat(result, buffer);
        total_length += len;
    }

    fclose(fp);
    unlink(temp_filename);

    if (total_length == 0) {
        *is_null = 1;
        free(result);
        return NULL;
    }

    // Null-terminate the result to remove any extraneous characters
    result[total_length] = '\0';

    // Log the result to debug
    debug_fp = fopen(debug_filename, "a");
    if (debug_fp) {
        fprintf(debug_fp, "Total length: %zu\n", total_length);
        fprintf(debug_fp, "Result: %s\n", result);
        fclose(debug_fp);
    }

    *length = total_length;
    return result;
}

int is_command_allowed(const char *command) {
    const char **allowed;
    for (allowed = allowed_commands; *allowed != NULL; ++allowed) {
        if (strcmp(command, *allowed) == 0) {
            return 1; // Command is allowed
        }
    }
    return 0; // Command is not allowed
}

#ifdef __cplusplus
}
#endif
