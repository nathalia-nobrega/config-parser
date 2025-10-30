#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Config {
    char key[50];
    char value[50];
};

struct Config configs[20];

int find_config(struct Config c[], int count_loaded_configs, char *key) {
    for (int i = 0; i < count_loaded_configs; i++) {
        if (strcmp(c[i].key, key) == 0) {
            return i;
        }
    }
    return -1;
}

void find_any_missing_configs(struct Config c[], int total_loaded_configs) {
    FILE *required_configs = fopen("required_configs.txt", "r");

    if (required_configs == NULL) {
        fprintf(stderr, "Unable to read required configs file. The program will now exit.");
        exit(1);
    }

    char required_keys[10][50];

    char buffer[50];

    int count_required_configs = 0;
    while (fgets(buffer, 50, required_configs)) {
        size_t size_buffer = strlen(buffer);
        if (buffer[size_buffer - 1] == '\n') buffer[--size_buffer] = '\0'; 

        strcpy(required_keys[count_required_configs], buffer);
        count_required_configs++;
    }

    fclose(required_configs);


    for (int i = 0; i < count_required_configs; i++) {
        if (find_config(c, total_loaded_configs, required_keys[i]) == -1) {
            fprintf(stderr, "%s configuration is missing!\n", required_keys[i]);
        };
    }
}

// PHASE 1 - STDOUT
void print_to_json_format(struct Config c[], int total_loaded_configs) {
    printf("{\n");

    for (int i = 0; i < total_loaded_configs; i++) {
        printf("\t\"%s\": \"%s\"", c[i].key, c[i].value);
        if (i != total_loaded_configs - 1) printf(",\n");
    }

    printf("\n}\n");
};

// PHASE 2 - .json FILE
void write_to_json_format(struct Config c[], int total_loaded_configs, char *filename) {
    FILE *output = fopen(filename, "w");

    if (output == NULL) {
        fprintf(stderr, "The output file could not be open to write");
        exit(1);
    }

    fprintf(output, "{\n");

    for (int i = 0; i < total_loaded_configs; i++) {
        fprintf(output, "\t\"%s\": \"%s\"", c[i].key, c[i].value);
        if (i != total_loaded_configs - 1) fprintf(output, ",\n"); 
    }

    fprintf(output, "\n}\n");

    fclose(output);

    printf("FILE %s was created successfully!\n", filename);
}

int main(int argc, char *argv[]) {
    if (argv[1] == NULL || strcmp(argv[1], "") == 0) {
        fprintf(stderr, "Please, don't forget to enter the name of the file!");
        exit(1);
    }

    char *path = argv[1];

    FILE *config_file = fopen(path, "r");

    if (config_file == NULL) {
        fprintf(stderr, "The file could not be open. The program will now exit.");
        exit(1);
    };

    char single_line[100];

    int i = 0, count_configs = 0;

    while (fgets(single_line, 100, config_file)) {
        // verify if the line is a comment or if it's empty
        // NOTE: EOL character in DOS (Windows) is '\r\n' and in UNIX is '\n'
        if (strcmp(single_line, "\n") == 0 || 
        strcmp(single_line, "\r\n") == 0 || 
        single_line[0] == '#') {
            continue;
        }

        char *ptr = strtok(single_line, "="); // keys
        strcpy(configs[i].key, ptr);

        ptr = strtok(NULL, "="); // values
        strcpy(configs[i].value, ptr);

        // removing trailling new line character
        size_t length = strlen(configs[i].value);
        if (configs[i].value[length - 1] == '\n') configs[i].value[--length] = '\0';

        count_configs++;
        i++;
    }

    fclose(config_file);

    find_any_missing_configs(configs, count_configs);

    printf("Amount of configs found: %d\n", count_configs);

    write_to_json_format(configs, count_configs, "output.json");

    return 0;
}

