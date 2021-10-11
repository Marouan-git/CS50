#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

// Recovers lost images from a memory card

int main(int argc, char *argv[])
{
    // Check that the user entered only one command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover file.raw\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    // Check if the file has been correctly opened
    if (input == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

    // Creating a buffer to read the file by blocks of 512 bytes
    BYTE buffer[512];
    int i = 0;
    FILE *jpeg = NULL;
    char *filename = malloc(sizeof(char) * 7);
    while (fread(&buffer, sizeof(BYTE), 512, input))
    {
        // If we find a jpeg header
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0))
        {
            sprintf(filename, "%03i.jpg", i);
            if (jpeg != NULL)
            {
                fclose(jpeg);
            }
            jpeg = fopen(filename, "w");
            fwrite(&buffer, sizeof(BYTE), 512, jpeg);
            i++;
        }
        else
        {
            if (jpeg != NULL)
            {
                fwrite(&buffer, sizeof(BYTE), 512, jpeg);
            }
        }
    }

    free(filename);
    fclose(jpeg);

    return 0;

}
