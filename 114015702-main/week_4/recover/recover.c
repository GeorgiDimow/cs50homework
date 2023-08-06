#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL)
    {
        printf("No file with this name\n");
        return 1;
    }

    uint8_t *bufferReader;

    bufferReader = malloc(512 * sizeof(uint8_t));

    if (bufferReader == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    int numberOfImg = 0;
    char filename[8];
    FILE *img = NULL;

    while (fread(bufferReader, sizeof(uint8_t), 512, file) == 512)
    {

        if (bufferReader[0] == 0xff  && bufferReader[1] == 0xd8 && bufferReader[2] == 0xff && (bufferReader[3] & 0xf0) == 0xe0)
        {
            if (img != NULL) {
                fclose(img);
            }

            sprintf(filename, "%03i.jpg", numberOfImg);

            img = fopen(filename, "w");

            if (img == NULL) {
                fclose(file);
                printf("Couldn't create output file\n");
                return 1;
            }

            numberOfImg++;
        }

        if (img != NULL)
        {
            fwrite(bufferReader, 1, 512, img);

        }

    }

    free(bufferReader);
    fclose(file);
    fclose(img);

    return 0;
}