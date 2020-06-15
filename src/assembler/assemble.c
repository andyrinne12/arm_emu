#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "constants.h"
#include "label_table.h"
#include "encoder.h"
#include "parser.h"

/*
 * 1. verify the number of passed arguments
 * 2. initialize the source and output filenames
 * 3. load the assembly instructions
 * 4. generate symbol table
 * 5. binary encoding + save the files
 * 6. free the memory + exit
 */

int main(int argc, char **argv) {

    //1. verify the number of passed arguments
    if (argc != 3) {
        fprintf(stderr, "The number of provided arguments is not correct\n");
        return EXIT_FAILURE;
    }

    //2. initialize the source and output filenames
    char *filename_source = argv[1];
    char *filename_output = argv[2];

    //3. load the assembly instructions
    //4. generate symbol table

    FILE *input;
    if ((input = fopen(filename_source, "r")) == NULL) {
        fprintf(stderr, "Error while opening the file\n");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    label_dict *dict = new_dict();
    int count = 0;

    while (!feof(input)) {
        char *p = fgets(line, MAX_LINE_LENGTH, input);
        if (p == NULL) {
            break;
        }
        int n = strlen(line);
        if (line[n - 2] == ':') {
            line[n - 2] = '\0';
            add(line, count, dict);
        }
        count++;
    }

    // parse file again
    if ((input = fopen(filename_source, "r")) == NULL) {
        fprintf(stderr, "Error while opening the file\n");
        exit(EXIT_FAILURE);
    }

    FILE *output;
    output = fopen(filename_output, "wb");
    uint32_t binary;
    Token token;
    uint16_t address = 0;

    while (!feof(input)) {
        char *p = fgets(line, MAX_LINE_LENGTH, input);
        if (p == NULL) {
            break;
        }
        int n = strlen(line);
        line[n - 1] = '\0';
        printf("Length line: %d", n);
        if (line[n - 2] == ':' || n == 1) {
            continue;
        }
        printf("%s\n", line);
        printf("%s\n", line);
        token.address = address;
        parse_general(&token, line);
        printf("dupa parser: \n");
        printf("%x\n", binary);
        instr_to_bits(&token, dict, address, &binary);
        printf("dupa encoder: \n");
        printf("%x\n", binary);
        fwrite(&binary, sizeof(uint32_t), 1, output);
        address += 4;
    }


    return EXIT_SUCCESS;
}
