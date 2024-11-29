#include <stdio.h>
#include <stdlib.h>

#define WIDTH 512
#define HEIGHT 512

int readPGMText(const char *filename, unsigned char *pixels, int width, int height) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return 1;
    }

    char buffer[16];
    if (!fgets(buffer, sizeof(buffer), file) || buffer[0] != 'P' || buffer[1] != '2') {
        fprintf(stderr, "Error: Invalid PGM format in file %s\n", filename);
        fclose(file);
        return 1;
    }

    // Skip comments
    do {
        if (!fgets(buffer, sizeof(buffer), file)) {
            fclose(file);
            return 1;
        }
    } while (buffer[0] == '#');

    int w, h, maxVal;
    if (sscanf(buffer, "%d %d", &w, &h) != 2 || w != width || h != height || fscanf(file, "%d", &maxVal) != 1 || maxVal != 255) {
        fprintf(stderr, "Error: Invalid dimensions or max value in %s\n", filename);
        fclose(file);
        return 1;
    }

    for (int i = 0; i < width * height; ++i) {
        if (fscanf(file, "%hhu", &pixels[i]) != 1) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int writePGMText(const char *filename, unsigned char *pixels, int width, int height) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s for writing\n", filename);
        return 1;
    }

    fprintf(file, "P2\n# Created by stego_lsb.c\n%d %d\n255\n", width, height);

    for (int i = 0; i < width * height; ++i) {
        fprintf(file, "%d\n", pixels[i]);
    }

    fclose(file);
    return 0;
}

int writePGMBinary(const char *filename, unsigned char *pixels, int width, int height) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s for writing\n", filename);
        return 1;
    }

    fprintf(file, "P5\n# Created by stego_lsb.c\n%d %d\n255\n", width, height);
    fwrite(pixels, sizeof(unsigned char), width * height, file);

    fclose(file);
    return 0;
}

void embedLSB(unsigned char *coverPixels, unsigned char *secretPixels, int width, int height) {
    for (int i = 0; i < width * height; ++i) {
        coverPixels[i] = (coverPixels[i] & 0xF0) | (secretPixels[i] >> 4);
    }
}

void extractLSB(unsigned char *coverPixels, unsigned char *outputPixels, int width, int height) {
    for (int i = 0; i < width * height; ++i) {
        outputPixels[i] = (coverPixels[i] & 0x0F) << 4;
    }
}

int main() {
    char cover_image[] = "baboon.pgm";
    char secret_image[] = "farm.pgm";
    char stego_image[] = "stego_image_bin.pgm";
    char extracted_secret[] = "extracted_secret.pgm";
    
    unsigned char *coverPixels, *secretPixels, *outputPixels;
    int coverWidth = WIDTH, coverHeight = HEIGHT, secretWidth = WIDTH, secretHeight = HEIGHT;

    // Dynamically allocate memory to store the cover image pixel values
    coverPixels = (unsigned char *)malloc(WIDTH * HEIGHT * sizeof(unsigned char));
    if (!coverPixels) {
        fprintf(stderr, "Error allocating memory for coverPixels.\n");
        return 1;
    }

    // Read the cover image file (text PGM) and store its pixels in 'coverPixels'
    if (readPGMText(cover_image, coverPixels, coverWidth, coverHeight)) {
        fprintf(stderr, "Error reading cover image.\n");
        free(coverPixels);
        return 1;
    }
    printf("Cover image loaded successfully.\n");

    // Dynamically allocate memory to store the secret image pixel values
    secretPixels = (unsigned char *)malloc(WIDTH * HEIGHT * sizeof(unsigned char));
    if (!secretPixels) {
        fprintf(stderr, "Error allocating memory for secretPixels.\n");
        free(coverPixels);
        return 1;
    }

    // Read the secret image file (text PGM) and store its pixels in 'secretPixels'
    if (readPGMText(secret_image, secretPixels, secretWidth, secretHeight)) {
        fprintf(stderr, "Error reading secret image.\n");
        free(coverPixels);
        free(secretPixels);
        return 1;
    }
    printf("Secret image loaded successfully.\n");

    // Check if the dimensions of both images match
    if (coverWidth != secretWidth || coverHeight != secretHeight) {
        fprintf(stderr, "Error: Image dimensions do not match.\n");
        free(coverPixels);
        free(secretPixels);
        return 1;
    }

    // Call the function embedLSB to embed the secret image into the cover image
    embedLSB(coverPixels, secretPixels, coverWidth, coverHeight);
    printf("LSB embedding completed.\n");

    // Save the stego image into binary format
    if (writePGMBinary(stego_image, coverPixels, coverWidth, coverHeight)) {
        fprintf(stderr, "Error writing stego image.\n");
        free(coverPixels);
        free(secretPixels);
        return 1;
    }
    printf("Stego image written successfully.\n");

    // Dynamically allocate memory to store the extracted secret image pixels
    outputPixels = (unsigned char *)malloc(WIDTH * HEIGHT * sizeof(unsigned char));
    if (!outputPixels) {
        fprintf(stderr, "Error allocating memory for outputPixels.\n");
        free(coverPixels);
        free(secretPixels);
        return 1;
    }

    // Call the function extractLSB to extract the secret image from the stego image
    extractLSB(coverPixels, outputPixels, coverWidth, coverHeight);
    printf("LSB extraction completed.\n");

    // Save the extracted secret image in text format
    if (writePGMText(extracted_secret, outputPixels, coverWidth, coverHeight)) {
        fprintf(stderr, "Error writing extracted secret image.\n");
    }
    printf("Extracted secret image written successfully.\n");

    // Free allocated memory
    free(coverPixels);
    free(secretPixels);
    free(outputPixels);

    return 0;
}
