#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to check if a string is a valid float
int isFloat(const char *str) {
    char *end;
    strtof(str, &end); // Attempt to convert string to float
    return *end == '\0'; // Return true if entire string is valid
}

void processFile(const char *inputPath, const char *outputPath) {
    FILE *inputFile = fopen(inputPath, "r");
    FILE *outputFile = fopen(outputPath, "w");
    char word[256];
    int validCount = 0, invalidCount = 0;

    if (!inputFile) {
        printf("Could not open input file: %s\n", inputPath);
        return;
    }

    if (!outputFile) {
        printf("Could not open or create output file: %s\n", outputPath);
        fclose(inputFile);
        return;
    }

    // Read words and process each
    while (fscanf(inputFile, "%255s", word) == 1) {
        if (isFloat(word)) {
            float number = strtof(word, NULL); // Convert valid string to float
            fprintf(outputFile, "%.2f\n", number); // Write float to output file
            validCount++;
        } else {
            invalidCount++;
        }
    }

    // Check for errors while reading
    if (ferror(inputFile)) {
        printf("Error reading from input file.\n");
    }

    fclose(inputFile);
    fclose(outputFile);

    // Print summary
    printf("Valid float values: %d\n", validCount);
    printf("Invalid entries: %d\n", invalidCount);
}

int main() {
    const char *inputFile = "data.txt"; // Path to input file
    const char *outputFile = "validdata.txt"; // Path to output file

    processFile(inputFile, outputFile); // Process the file

    return 0;
}