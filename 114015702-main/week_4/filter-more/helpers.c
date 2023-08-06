#include "helpers.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>

void swap(uint8_t *a, uint8_t *b);
void avrg_pixel_values_3x3(int height, int width, RGBTRIPLE original_image[height][width], RGBTRIPLE new_image[height][width],
                           int row, int col);
void avrg_pixel_multi_by_kenels_values_3x3(int height, int width, RGBTRIPLE original_image[height][width],
                                           RGBTRIPLE new_image[height][width], int row, int col);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE curr_pixel = image[i][j];
            int avrg = (int) round((curr_pixel.rgbtBlue + curr_pixel.rgbtRed + curr_pixel.rgbtGreen) / 3.0);
            image[i][j].rgbtBlue = avrg;
            image[i][j].rgbtRed = avrg;
            image[i][j].rgbtGreen = avrg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            swap(&image[i][j].rgbtBlue, &image[i][width - 1 - j].rgbtBlue);
            swap(&image[i][j].rgbtRed, &image[i][width - 1 - j].rgbtRed);
            swap(&image[i][j].rgbtGreen, &image[i][width - 1 - j].rgbtGreen);
        }
    }
    return;
}

void swap(uint8_t *a, uint8_t *b)
{
    uint8_t tmp = *a;
    *a = *b;
    *b = tmp;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE original_image[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            original_image[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            avrg_pixel_values_3x3(height, width, original_image, image, i, j);
        }
    }

    return;
}

void avrg_pixel_values_3x3(int height, int width, RGBTRIPLE original_image[height][width], RGBTRIPLE new_image[height][width],
                           int row, int col)
{
    int avrgGreen = 0;
    int avrgRed = 0;
    int avrgBlue = 0;
    int iter_count = 0;

    for (int i = row - 1; i <= row + 1; i++)
    {
        for (int j = col - 1; j <= col + 1; j++)
        {
            if (i >= 0 && i < height && j >= 0 && j < width)
            {
                avrgGreen += original_image[i][j].rgbtGreen;
                avrgRed += original_image[i][j].rgbtRed;
                avrgBlue += original_image[i][j].rgbtBlue;
                iter_count++;
            }
        }
    }

    new_image[row][col].rgbtGreen = (uint8_t) round(avrgGreen / (iter_count * 1.0));
    new_image[row][col].rgbtRed = (uint8_t) round(avrgRed / (iter_count * 1.0));
    new_image[row][col].rgbtBlue = (uint8_t) round(avrgBlue / (iter_count * 1.0));

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE original_image[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            original_image[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            avrg_pixel_multi_by_kenels_values_3x3(height, width, original_image, image, i, j);
        }
    }

    return;
}

int kernelX[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
int kernelY[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

void avrg_pixel_multi_by_kenels_values_3x3(int height, int width, RGBTRIPLE original_image[height][width],
                                           RGBTRIPLE new_image[height][width], int row, int col)
{
    int avrgGreenX = 0;
    int avrgRedX = 0;
    int avrgBlueX = 0;
    int avrgGreenY = 0;
    int avrgRedY = 0;
    int avrgBlueY = 0;
    int iter_count = 0;

    for (int i = row - 1; i <= row + 1; i++)
    {
        for (int j = col - 1; j <= col + 1; j++)
        {
            if (i >= 0 && i < height && j >= 0 && j < width)
            {
                avrgGreenX += original_image[i][j].rgbtGreen * kernelX[iter_count];
                avrgRedX += original_image[i][j].rgbtRed * kernelX[iter_count];
                avrgBlueX += original_image[i][j].rgbtBlue * kernelX[iter_count];

                avrgGreenY += original_image[i][j].rgbtGreen * kernelY[iter_count];
                avrgRedY += original_image[i][j].rgbtRed * kernelY[iter_count];
                avrgBlueY += original_image[i][j].rgbtBlue * kernelY[iter_count];
            }
            iter_count++;
        }
    }
    int avrgGreen = round(sqrt(pow(avrgGreenX, 2) + pow(avrgGreenY, 2)));
    int avrgRed = round(sqrt(pow(avrgRedX, 2) + pow(avrgRedY, 2)));
    int avrgBlue = round(sqrt(pow(avrgBlueX, 2) + pow(avrgBlueY, 2)));

    if (avrgGreen > 255)
    {
         avrgGreen = 255;
    }
    if (avrgRed > 255)
    {
         avrgRed = 255;
    }
    if (avrgBlue > 255)
    {
         avrgBlue = 255;
    }


    new_image[row][col].rgbtGreen = (uint8_t) avrgGreen;
    new_image[row][col].rgbtRed = (uint8_t) avrgRed;
    new_image[row][col].rgbtBlue = (uint8_t) avrgBlue;
}