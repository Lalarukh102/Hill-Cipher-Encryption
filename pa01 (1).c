

/*=============================================================================
| Assignment: pa01 - Encrypting a plaintext file using the Hill cipher
|
| Author: Your name here
| Language: c, c++, Java, go, python
|
| To Compile: javac pa01.java
| gcc -o pa01 pa01.c
| g++ -o pa01 pa01.cpp
| go build pa01.go
|
| To Execute: java -> java pa01 kX.txt pX.txt
| or c++ -> ./pa01 kX.txt pX.txt
| or c -> ./pa01 kX.txt pX.txt
| or go -> ./pa01 kX.txt pX.txt
| or python -> python3 pa01.py kX.txt pX.txt
| where kX.txt is the keytext file
| and pX.txt is plaintext file
|
| Note: All input files are simple 8 bit ASCII input
|
| Class: CIS3360 - Security in Computing - Summer 2023
| Instructor: McAlpin
| Due Date: per assignment
|
+=============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

void readKeyFile(const char* filename, int key[MAX_SIZE][MAX_SIZE], int* n) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Key file '%s' not found.\n", filename);
        exit(1);
    }
    fscanf(file, "%d", n);
    if (*n < 2 || *n > MAX_SIZE) {
        printf("Invalid key matrix size.\n");
        exit(1);
    }
    for (int i = 0; i < *n; i++) {
        for (int j = 0; j < *n; j++) {
            fscanf(file, "%d", &key[i][j]);
        }
    }
    fclose(file);
}

void readPlaintextFile(const char* filename, char* plaintext) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Plaintext file '%s' not found.\n", filename);
        exit(1);
    }
    char buffer[MAX_SIZE * MAX_SIZE];
    int i = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        for (int j = 0; j < strlen(buffer); j++) {
            if (isalpha(buffer[j])) {
                plaintext[i++] = tolower(buffer[j]);
            }
        }
    }
    plaintext[i] = '\0';
    fclose(file);
}

void padPlaintext(const char* plaintext, int block_size, char* padded_plaintext) {
    int padding_size = (block_size - (strlen(plaintext) % block_size)) % block_size;
    strcpy(padded_plaintext, plaintext);
    for (int i = 0; i < padding_size; i++) {
        strcat(padded_plaintext, "x");
    }
}

void multiplyMatrices(int matrix1[MAX_SIZE][MAX_SIZE], int matrix2[MAX_SIZE][MAX_SIZE], int n, int result[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int element = 0;
            for (int k = 0; k < n; k++) {
                element += matrix1[i][k] * matrix2[k][j];
            }
            result[i][j] = element % 26;  // Perform modulo 26 arithmetic
        }
    }
}

void encrypt(const char* plaintext, int key[MAX_SIZE][MAX_SIZE], int n, char* ciphertext) {
    int block_size = n;
    char padded_plaintext[MAX_SIZE * MAX_SIZE];
    padPlaintext(plaintext, block_size, padded_plaintext);

    int encrypted_block[MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];

    int index = 0;
    for (int i = 0; i < strlen(padded_plaintext); i += block_size) {
        for (int j = 0; j < block_size; j++) {
            encrypted_block[j] = padded_plaintext[i + j] - 'a';
        }
        multiplyMatrices(&encrypted_block, key, n, result);
        for (int j = 0; j < block_size; j++) {
            ciphertext[index++] = result[0][j] + 'a';
        }
    }
    ciphertext[index] = '\0';
}

void formatCiphertext(const char* ciphertext, char* formatted_ciphertext) {
    int index = 0;
    for (int i = 0; i < strlen(ciphertext); i++) {
        formatted_ciphertext[index++] = ciphertext[i];
        if ((i + 1) % 80 == 0) {
            formatted_ciphertext[index++] = '\n';
        }
    }
    formatted_ciphertext[index] = '\0';
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: ./encrypt key_file plaintext_file\n");
        return 1;
    }

    char* key_file = argv[1];
    char* plaintext_file = argv[2];

    int key[MAX_SIZE][MAX_SIZE];
    int n;
    readKeyFile(key_file, key, &n);

    char plaintext[MAX_SIZE * MAX_SIZE];
    readPlaintextFile(plaintext_file, plaintext);

    char ciphertext[MAX_SIZE * MAX_SIZE];
    encrypt(plaintext, key, n, ciphertext);

    char formatted_ciphertext[MAX_SIZE * MAX_SIZE + MAX_SIZE];
    formatCiphertext(ciphertext, formatted_ciphertext);

    printf("%s\n", formatted_ciphertext);

    return 0;
}


/*=============================================================================
| I Lala, (5454398) affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any another person, nor copied any code from any other person, nor permitted
| my code to be copied or otherwise used by any other person, nor have I
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
+=============================================================================*/