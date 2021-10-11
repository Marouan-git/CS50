#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "helpers.h"


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float avg;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            avg = (float) (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3;
            image[i][j].rgbtBlue = (uint8_t) round(avg);
            image[i][j].rgbtGreen = (uint8_t) round(avg);
            image[i][j].rgbtRed = (uint8_t) round(avg);
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
            RGBTRIPLE *tmp = malloc(sizeof(RGBTRIPLE));
            *tmp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = *tmp;
            free(tmp);
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Let's create a temporary pointer to store into the values contained in the image tab
    RGBTRIPLE (*tmp)[width] = malloc(sizeof(RGBTRIPLE) * height * width);
    if (tmp == NULL)
    {
        printf("Something went wrong with malloc.");
        return;
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // If I am at the top of the tab (at the first line)
            if (i == 0 )
            {
                // If I am at the top left corner
                if (j == 0)
                {
                    image[i][j].rgbtBlue = (uint8_t) round((float)(tmp[i][j].rgbtBlue + tmp[i][j+1].rgbtBlue + tmp[i+1][j].rgbtBlue + tmp[i+1][j+1].rgbtBlue) / 4);
                    image[i][j].rgbtGreen = (uint8_t) round((float)(tmp[i][j].rgbtGreen + tmp[i][j+1].rgbtGreen + tmp[i+1][j].rgbtGreen + tmp[i+1][j+1].rgbtGreen) / 4);
                    image[i][j].rgbtRed = (uint8_t) round((float)(tmp[i][j].rgbtRed + tmp[i][j+1].rgbtRed + tmp[i+1][j].rgbtRed + tmp[i+1][j+1].rgbtRed) / 4);
                }
                // If I am at the top right corner
                else if (j == width - 1)
                {
                    image[i][j].rgbtBlue = (uint8_t) round((float)(tmp[i][j].rgbtBlue + tmp[i][j-1].rgbtBlue + tmp[i+1][j].rgbtBlue + tmp[i+1][j-1].rgbtBlue) / 4);
                    image[i][j].rgbtGreen = (uint8_t) round((float)(tmp[i][j].rgbtGreen + tmp[i][j-1].rgbtGreen + tmp[i+1][j].rgbtGreen + tmp[i+1][j-1].rgbtGreen) / 4);
                    image[i][j].rgbtRed = (uint8_t) round((float)(tmp[i][j].rgbtRed + tmp[i][j-1].rgbtRed + tmp[i+1][j].rgbtRed + tmp[i+1][j-1].rgbtRed) / 4);
                }
                else
                {
                    image[i][j].rgbtBlue = (uint8_t) round((float)(tmp[i][j].rgbtBlue + tmp[i][j-1].rgbtBlue + tmp[i+1][j].rgbtBlue + tmp[i+1][j-1].rgbtBlue + tmp[i+1][j+1].rgbtBlue + tmp[i][j+1].rgbtBlue) / 6);
                    image[i][j].rgbtGreen = (uint8_t) round((float)(tmp[i][j].rgbtGreen + tmp[i][j-1].rgbtGreen + tmp[i+1][j].rgbtGreen + tmp[i+1][j-1].rgbtGreen + tmp[i+1][j+1].rgbtGreen + tmp[i][j+1].rgbtGreen) / 6);
                    image[i][j].rgbtRed = (uint8_t) round((float)(tmp[i][j].rgbtRed + tmp[i][j-1].rgbtRed + tmp[i+1][j].rgbtRed + tmp[i+1][j-1].rgbtRed + tmp[i+1][j+1].rgbtRed + tmp[i][j+1].rgbtRed) / 6);
                }
            }
            // If I am at the bottom of the tab (last line)
            else if (i == height - 1)
            {
                // If I am at the bottom left corner
                if (j == 0)
                {
                    image[i][j].rgbtBlue = (uint8_t) round((float)(tmp[i][j].rgbtBlue + tmp[i-1][j].rgbtBlue + tmp[i-1][j+1].rgbtBlue + tmp[i][j+1].rgbtBlue) / 4);
                    image[i][j].rgbtGreen = (uint8_t) round((float)(tmp[i][j].rgbtGreen + tmp[i-1][j].rgbtGreen + tmp[i-1][j+1].rgbtGreen + tmp[i][j+1].rgbtGreen) / 4);
                    image[i][j].rgbtRed = (uint8_t) round((float)(tmp[i][j].rgbtRed + tmp[i-1][j].rgbtRed + tmp[i-1][j+1].rgbtRed + tmp[i][j+1].rgbtRed) / 4);
                }
                // If I am at the bottom right corner
                else if (j == width - 1)
                {
                    image[i][j].rgbtBlue = (uint8_t) round((float)(tmp[i][j].rgbtBlue + tmp[i][j-1].rgbtBlue + tmp[i-1][j-1].rgbtBlue + tmp[i-1][j].rgbtBlue) / 4);
                    image[i][j].rgbtGreen = (uint8_t) round((float)(tmp[i][j].rgbtGreen + tmp[i][j-1].rgbtGreen + tmp[i-1][j-1].rgbtGreen + tmp[i-1][j].rgbtGreen) / 4);
                    image[i][j].rgbtRed = (uint8_t) round((float)(tmp[i][j].rgbtRed + tmp[i][j-1].rgbtRed + tmp[i-1][j-1].rgbtRed + tmp[i-1][j].rgbtRed) / 4);
                }
                else
                {
                    image[i][j].rgbtBlue = (uint8_t) round((float)(tmp[i][j].rgbtBlue + tmp[i][j-1].rgbtBlue + tmp[i-1][j-1].rgbtBlue + tmp[i-1][j].rgbtBlue + tmp[i-1][j+1].rgbtBlue + tmp[i][j+1].rgbtBlue) / 6);
                    image[i][j].rgbtGreen = (uint8_t) round((float)(tmp[i][j].rgbtGreen + tmp[i][j-1].rgbtGreen + tmp[i-1][j-1].rgbtGreen + tmp[i-1][j].rgbtGreen + tmp[i-1][j+1].rgbtGreen + tmp[i][j+1].rgbtGreen) / 6);
                    image[i][j].rgbtRed = (uint8_t) round((float)(tmp[i][j].rgbtRed + tmp[i][j-1].rgbtRed + tmp[i-1][j-1].rgbtRed + tmp[i-1][j].rgbtRed + tmp[i-1][j+1].rgbtRed + tmp[i][j+1].rgbtRed) / 6);
                }
            }
            // If I am not at the top of the tab nor the bottom of the tab
            else
            {
                // If I am at the left of the tab (first column)
                if (j == 0)
                {
                    image[i][j].rgbtBlue = (uint8_t) round((float)(tmp[i][j].rgbtBlue + tmp[i-1][j].rgbtBlue + tmp[i-1][j+1].rgbtBlue + tmp[i][j+1].rgbtBlue + tmp[i+1][j+1].rgbtBlue + tmp[i+1][j].rgbtBlue) / 6);
                    image[i][j].rgbtGreen = (uint8_t) round((float)(tmp[i][j].rgbtGreen + tmp[i-1][j].rgbtGreen + tmp[i-1][j+1].rgbtGreen + tmp[i][j+1].rgbtGreen + tmp[i+1][j+1].rgbtGreen + tmp[i+1][j].rgbtGreen) / 6);
                    image[i][j].rgbtRed = (uint8_t) round((float)(tmp[i][j].rgbtRed + tmp[i-1][j].rgbtRed + tmp[i-1][j+1].rgbtRed + tmp[i][j+1].rgbtRed + tmp[i+1][j+1].rgbtRed + tmp[i+1][j].rgbtRed) / 6);
                }
                // If I am at the right of the tab (last column)
                else if (j == width - 1)
                {
                    image[i][j].rgbtBlue = (uint8_t) round((float)(tmp[i][j].rgbtBlue + tmp[i-1][j].rgbtBlue + tmp[i-1][j-1].rgbtBlue + tmp[i][j-1].rgbtBlue + tmp[i+1][j-1].rgbtBlue + tmp[i+1][j].rgbtBlue) / 6);
                    image[i][j].rgbtGreen = (uint8_t) round((float)(tmp[i][j].rgbtGreen + tmp[i-1][j].rgbtGreen + tmp[i-1][j-1].rgbtGreen + tmp[i][j-1].rgbtGreen + tmp[i+1][j-1].rgbtGreen + tmp[i+1][j].rgbtGreen) / 6);
                    image[i][j].rgbtRed = (uint8_t) round((float)(tmp[i][j].rgbtRed + tmp[i-1][j].rgbtRed + tmp[i-1][j-1].rgbtRed + tmp[i][j-1].rgbtRed + tmp[i+1][j-1].rgbtRed + tmp[i+1][j].rgbtRed) / 6);
                }
                else
                {
                    image[i][j].rgbtBlue = (uint8_t) round((float)(tmp[i][j].rgbtBlue + tmp[i-1][j].rgbtBlue + tmp[i-1][j-1].rgbtBlue + tmp[i][j-1].rgbtBlue + tmp[i+1][j-1].rgbtBlue + tmp[i+1][j].rgbtBlue + tmp[i+1][j+1].rgbtBlue + tmp[i][j+1].rgbtBlue + tmp[i-1][j+1].rgbtBlue) / 9);
                    image[i][j].rgbtGreen = (uint8_t) round((float)(tmp[i][j].rgbtGreen + tmp[i-1][j].rgbtGreen + tmp[i-1][j-1].rgbtGreen + tmp[i][j-1].rgbtGreen + tmp[i+1][j-1].rgbtGreen + tmp[i+1][j].rgbtGreen + tmp[i+1][j+1].rgbtGreen + tmp[i][j+1].rgbtGreen + tmp[i-1][j+1].rgbtGreen) / 9);
                    image[i][j].rgbtRed = (uint8_t) round((float)(tmp[i][j].rgbtRed + tmp[i-1][j].rgbtRed + tmp[i-1][j-1].rgbtRed + tmp[i][j-1].rgbtRed + tmp[i+1][j-1].rgbtRed + tmp[i+1][j].rgbtRed + tmp[i+1][j+1].rgbtRed + tmp[i][j+1].rgbtRed + tmp[i-1][j+1].rgbtRed) / 9);
                }
            }
        }

    }
    free(tmp);
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
     // Let's create a temporary pointer to store into the values contained in the image tab
    RGBTRIPLE (*tmp)[width] = malloc(sizeof(RGBTRIPLE) * height * width);
    if (tmp == NULL)
    {
        printf("Something went wrong with malloc.");
        return;
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp[i][j] = image[i][j];
        }
    }


    int gxRed;
    int gxGreen;
    int gxBlue;
    int gyRed;
    int gyGreen;
    int gyBlue;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // If I am at the top of the tab (at the first line)
            if (i == 0 )
            {
                // If I am at the top left corner
                if (j == 0)
                {
                    gxBlue = (0 * tmp[i][j].rgbtBlue + 2 * tmp[i][j+1].rgbtBlue + 0 * tmp[i+1][j].rgbtBlue + tmp[i+1][j+1].rgbtBlue);
                    gyBlue = (0 * tmp[i][j].rgbtBlue + 0 * tmp[i][j+1].rgbtBlue + 2 * tmp[i+1][j].rgbtBlue + tmp[i+1][j+1].rgbtBlue);
                    if (sqrt(pow(gxBlue,2) + pow(gyBlue,2)) > 255)
                    {
                        image[i][j].rgbtBlue = 255;
                    }
                    else
                    {
                        image[i][j].rgbtBlue = round(sqrt(pow(gxBlue,2) + pow(gyBlue,2)));
                    }

                    gxGreen = (0 * tmp[i][j].rgbtGreen + 2 * tmp[i][j+1].rgbtGreen + 0 * tmp[i+1][j].rgbtGreen + tmp[i+1][j+1].rgbtGreen);
                    gyGreen = (0 * tmp[i][j].rgbtGreen + 0 * tmp[i][j+1].rgbtGreen + 2 * tmp[i+1][j].rgbtGreen + tmp[i+1][j+1].rgbtGreen);
                    if (sqrt(pow(gxGreen,2) + pow(gyGreen,2)) > 255)
                    {
                        image[i][j].rgbtGreen = 255;
                    }
                    else
                    {
                        image[i][j].rgbtGreen = round(sqrt(pow(gxGreen,2) + pow(gyGreen,2)));
                    }

                    gxRed = (0 * tmp[i][j].rgbtRed + 2 * tmp[i][j+1].rgbtRed + 0 * tmp[i+1][j].rgbtRed + tmp[i+1][j+1].rgbtRed);
                    gyRed = (0 * tmp[i][j].rgbtRed + 0 * tmp[i][j+1].rgbtRed + 2 * tmp[i+1][j].rgbtRed + tmp[i+1][j+1].rgbtRed);
                    if (sqrt(pow(gxRed,2) + pow(gyRed,2)) > 255)
                    {
                        image[i][j].rgbtRed = 255;
                    }
                    else
                    {
                        image[i][j].rgbtRed = round(sqrt(pow(gxRed,2) + pow(gyRed,2)));
                    }

                }
                // If I am at the top right corner
                else if (j == width - 1)
                {
                    gxBlue = (0 * tmp[i][j].rgbtBlue + (-2) * tmp[i][j-1].rgbtBlue + 0 * tmp[i+1][j].rgbtBlue + (-1) * tmp[i+1][j-1].rgbtBlue);
                    gyBlue = (0 * tmp[i][j].rgbtBlue + 0 * tmp[i][j-1].rgbtBlue + 2 * tmp[i+1][j].rgbtBlue + tmp[i+1][j-1].rgbtBlue);
                    if (sqrt(pow(gxBlue,2) + pow(gyBlue,2)) > 255)
                    {
                        image[i][j].rgbtBlue = 255;
                    }
                    else
                    {
                        image[i][j].rgbtBlue = round(sqrt(pow(gxBlue,2) + pow(gyBlue,2)));
                    }

                    gxGreen = (0 * tmp[i][j].rgbtGreen + (-2) * tmp[i][j-1].rgbtGreen + 0 * tmp[i+1][j].rgbtGreen + (-1) * tmp[i+1][j-1].rgbtGreen);
                    gyGreen = (0 * tmp[i][j].rgbtGreen + 0 * tmp[i][j-1].rgbtGreen + 2 * tmp[i+1][j].rgbtGreen + tmp[i+1][j-1].rgbtGreen);
                    if (sqrt(pow(gxGreen,2) + pow(gyGreen,2)) > 255)
                    {
                        image[i][j].rgbtGreen = 255;
                    }
                    else
                    {
                        image[i][j].rgbtGreen = round(sqrt(pow(gxGreen,2) + pow(gyGreen,2)));
                    }

                    gxRed = (0 * tmp[i][j].rgbtRed + (-2) * tmp[i][j-1].rgbtRed + 0 * tmp[i+1][j].rgbtRed + (-1) * tmp[i+1][j-1].rgbtRed);
                    gyRed = (0 * tmp[i][j].rgbtRed + 0 * tmp[i][j-1].rgbtRed + 2 * tmp[i+1][j].rgbtRed + tmp[i+1][j-1].rgbtRed);
                    if (sqrt(pow(gxRed,2) + pow(gyRed,2)) > 255)
                    {
                        image[i][j].rgbtRed = 255;
                    }
                    else
                    {
                        image[i][j].rgbtRed = round(sqrt(pow(gxRed,2) + pow(gyRed,2)));
                    }

                }
                else
                {
                    gxBlue = (0 * tmp[i][j].rgbtBlue + (-2) * tmp[i][j-1].rgbtBlue + 0 * tmp[i+1][j].rgbtBlue + (-1) * tmp[i+1][j-1].rgbtBlue + tmp[i+1][j+1].rgbtBlue + 2 * tmp[i][j+1].rgbtBlue);
                    gyBlue = (0 * tmp[i][j].rgbtBlue + 0 * tmp[i][j-1].rgbtBlue + 2 * tmp[i+1][j].rgbtBlue + tmp[i+1][j-1].rgbtBlue + tmp[i+1][j+1].rgbtBlue + 0 * tmp[i][j+1].rgbtBlue);
                    if (sqrt(pow(gxBlue,2) + pow(gyBlue,2)) > 255)
                    {
                        image[i][j].rgbtBlue = 255;
                    }
                    else
                    {
                        image[i][j].rgbtBlue = round(sqrt(pow(gxBlue,2) + pow(gyBlue,2)));
                    }

                    gxGreen = (0 * tmp[i][j].rgbtGreen + (-2) * tmp[i][j-1].rgbtGreen + 0 * tmp[i+1][j].rgbtGreen + (-1) * tmp[i+1][j-1].rgbtGreen + tmp[i+1][j+1].rgbtGreen + 2 * tmp[i][j+1].rgbtGreen);
                    gyGreen = (0 * tmp[i][j].rgbtGreen + 0 * tmp[i][j-1].rgbtGreen + 2 * tmp[i+1][j].rgbtGreen + tmp[i+1][j-1].rgbtGreen + tmp[i+1][j+1].rgbtGreen + 0 * tmp[i][j+1].rgbtGreen);
                    if (sqrt(pow(gxGreen,2) + pow(gyGreen,2)) > 255)
                    {
                        image[i][j].rgbtGreen = 255;
                    }
                    else
                    {
                        image[i][j].rgbtGreen = round(sqrt(pow(gxGreen,2) + pow(gyGreen,2)));
                    }

                    gxRed = (0 * tmp[i][j].rgbtRed + (-2) * tmp[i][j-1].rgbtRed + 0 * tmp[i+1][j].rgbtRed + (-1) * tmp[i+1][j-1].rgbtRed + tmp[i+1][j+1].rgbtRed + 2 * tmp[i][j+1].rgbtRed);
                    gyRed = (0 * tmp[i][j].rgbtRed + 0 * tmp[i][j-1].rgbtRed + 2 * tmp[i+1][j].rgbtRed + tmp[i+1][j-1].rgbtRed + tmp[i+1][j+1].rgbtRed + 0 * tmp[i][j+1].rgbtRed);
                    if (sqrt(pow(gxRed,2) + pow(gyRed,2)) > 255)
                    {
                        image[i][j].rgbtRed = 255;
                    }
                    else
                    {
                        image[i][j].rgbtRed = round(sqrt(pow(gxRed,2) + pow(gyRed,2)));
                    }

                }
            }
            // If I am at the bottom of the tab (last line)
            else if (i == height - 1)
            {
                // If I am at the bottom left corner
                if (j == 0)
                {
                    gxBlue = (0 * tmp[i][j].rgbtBlue + 0 * tmp[i-1][j].rgbtBlue + tmp[i-1][j+1].rgbtBlue + 2 * tmp[i][j+1].rgbtBlue);
                    gyBlue = (0 * tmp[i][j].rgbtBlue + (-2) * tmp[i-1][j].rgbtBlue + (-1) * tmp[i-1][j+1].rgbtBlue + 0 * tmp[i][j+1].rgbtBlue);
                    if (sqrt(pow(gxBlue,2) + pow(gyBlue,2)) > 255)
                    {
                        image[i][j].rgbtBlue = 255;
                    }
                    else
                    {
                        image[i][j].rgbtBlue = round(sqrt(pow(gxBlue,2) + pow(gyBlue,2)));
                    }

                    gxGreen = (0 * tmp[i][j].rgbtGreen + 0 * tmp[i-1][j].rgbtGreen + tmp[i-1][j+1].rgbtGreen + 2 * tmp[i][j+1].rgbtGreen);
                    gyGreen = (0 * tmp[i][j].rgbtGreen + (-2) * tmp[i-1][j].rgbtGreen + (-1) * tmp[i-1][j+1].rgbtGreen + 0 * tmp[i][j+1].rgbtGreen);
                    if (sqrt(pow(gxGreen,2) + pow(gyGreen,2)) > 255)
                    {
                        image[i][j].rgbtGreen = 255;
                    }
                    else
                    {
                        image[i][j].rgbtGreen = round(sqrt(pow(gxGreen,2) + pow(gyGreen,2)));
                    }

                    gxRed = (0 * tmp[i][j].rgbtRed + 0 * tmp[i-1][j].rgbtRed + tmp[i-1][j+1].rgbtRed + 2 * tmp[i][j+1].rgbtRed);
                    gyRed = (0 * tmp[i][j].rgbtRed + (-2) * tmp[i-1][j].rgbtRed + (-1) * tmp[i-1][j+1].rgbtRed + 0 * tmp[i][j+1].rgbtRed);
                    if (sqrt(pow(gxRed,2) + pow(gyRed,2)) > 255)
                    {
                        image[i][j].rgbtRed = 255;
                    }
                    else
                    {
                        image[i][j].rgbtRed = round(sqrt(pow(gxRed,2) + pow(gyRed,2)));
                    }

                }
                // If I am at the bottom right corner
                else if (j == width - 1)
                {
                    gxBlue = (0 * tmp[i][j].rgbtBlue + (-2) * tmp[i][j-1].rgbtBlue + (-1) * tmp[i-1][j-1].rgbtBlue + 0 * tmp[i-1][j].rgbtBlue);
                    gyBlue = (0 * tmp[i][j].rgbtBlue + 0 * tmp[i][j-1].rgbtBlue + (-1) * tmp[i-1][j-1].rgbtBlue + (-2) * tmp[i-1][j].rgbtBlue);
                    if (sqrt(pow(gxBlue,2) + pow(gyBlue,2)) > 255)
                    {
                        image[i][j].rgbtBlue = 255;
                    }
                    else
                    {
                        image[i][j].rgbtBlue = round(sqrt(pow(gxBlue,2) + pow(gyBlue,2)));
                    }

                    gxGreen = (0 * tmp[i][j].rgbtGreen + (-2) * tmp[i][j-1].rgbtGreen + (-1) * tmp[i-1][j-1].rgbtGreen + 0 * tmp[i-1][j].rgbtGreen);
                    gyGreen = (0 * tmp[i][j].rgbtGreen + 0 * tmp[i][j-1].rgbtGreen + (-1) * tmp[i-1][j-1].rgbtGreen + (-2) * tmp[i-1][j].rgbtGreen);
                    if (sqrt(pow(gxGreen,2) + pow(gyGreen,2)) > 255)
                    {
                        image[i][j].rgbtGreen = 255;
                    }
                    else
                    {
                        image[i][j].rgbtGreen = round(sqrt(pow(gxGreen,2) + pow(gyGreen,2)));
                    }

                    gxRed = (0 * tmp[i][j].rgbtRed + (-2) * tmp[i][j-1].rgbtRed + (-1) * tmp[i-1][j-1].rgbtRed + 0 * tmp[i-1][j].rgbtRed);
                    gyRed = (0 * tmp[i][j].rgbtRed + 0 * tmp[i][j-1].rgbtRed + (-1) * tmp[i-1][j-1].rgbtRed + (-2) * tmp[i-1][j].rgbtRed);
                    if (sqrt(pow(gxRed,2) + pow(gyRed,2)) > 255)
                    {
                        image[i][j].rgbtRed = 255;
                    }
                    else
                    {
                        image[i][j].rgbtRed = round(sqrt(pow(gxRed,2) + pow(gyRed,2)));
                    }



                }
                else
                {
                    gxBlue = (0 * tmp[i][j].rgbtBlue + (-2) * tmp[i][j-1].rgbtBlue + (-1) * tmp[i-1][j-1].rgbtBlue + 0 * tmp[i-1][j].rgbtBlue + tmp[i-1][j+1].rgbtBlue + 2 * tmp[i][j+1].rgbtBlue);
                    gyBlue = (0 * tmp[i][j].rgbtBlue + 0 * tmp[i][j-1].rgbtBlue + (-1) * tmp[i-1][j-1].rgbtBlue + (-2) * tmp[i-1][j].rgbtBlue + (-1) * tmp[i-1][j+1].rgbtBlue + 0 * tmp[i][j+1].rgbtBlue);
                    if (sqrt(pow(gxBlue,2) + pow(gyBlue,2)) > 255)
                    {
                        image[i][j].rgbtBlue = 255;
                    }
                    else
                    {
                        image[i][j].rgbtBlue = round(sqrt(pow(gxBlue,2) + pow(gyBlue,2)));
                    }

                    gxGreen = (0 * tmp[i][j].rgbtGreen + (-2) * tmp[i][j-1].rgbtGreen + (-1) * tmp[i-1][j-1].rgbtGreen + 0 * tmp[i-1][j].rgbtGreen + tmp[i-1][j+1].rgbtGreen + 2 * tmp[i][j+1].rgbtGreen);
                    gyGreen = (0 * tmp[i][j].rgbtGreen + 0 * tmp[i][j-1].rgbtGreen + (-1) * tmp[i-1][j-1].rgbtGreen + (-2) * tmp[i-1][j].rgbtGreen + (-1) * tmp[i-1][j+1].rgbtGreen + 0 * tmp[i][j+1].rgbtGreen);
                    if (sqrt(pow(gxGreen,2) + pow(gyGreen,2)) > 255)
                    {
                        image[i][j].rgbtGreen = 255;
                    }
                    else
                    {
                        image[i][j].rgbtGreen = round(sqrt(pow(gxGreen,2) + pow(gyGreen,2)));
                    }

                    gxRed = (0 * tmp[i][j].rgbtRed + (-2) * tmp[i][j-1].rgbtRed + (-1) * tmp[i-1][j-1].rgbtRed + 0 * tmp[i-1][j].rgbtRed + tmp[i-1][j+1].rgbtRed + 2 * tmp[i][j+1].rgbtRed);
                    gyRed = (0 * tmp[i][j].rgbtRed + 0 * tmp[i][j-1].rgbtRed + (-1) * tmp[i-1][j-1].rgbtRed + (-2) * tmp[i-1][j].rgbtRed + (-1) * tmp[i-1][j+1].rgbtRed + 0 * tmp[i][j+1].rgbtRed);
                    if (sqrt(pow(gxRed,2) + pow(gyRed,2)) > 255)
                    {
                        image[i][j].rgbtRed = 255;
                    }
                    else
                    {
                        image[i][j].rgbtRed = round(sqrt(pow(gxRed,2) + pow(gyRed,2)));
                    }

                }
            }
            // If I am neither at the top of the tab (first line) nor the bottom of the tab (last line)
            else
            {
                // If I am at the left of the tab (first column)
                if (j == 0)
                {
                    gxBlue = (0 * tmp[i][j].rgbtBlue + 0 * tmp[i-1][j].rgbtBlue + tmp[i-1][j+1].rgbtBlue + 2 * tmp[i][j+1].rgbtBlue + tmp[i+1][j+1].rgbtBlue + 0 * tmp[i+1][j].rgbtBlue);
                    gyBlue = (0 * tmp[i][j].rgbtBlue + (-2) * tmp[i-1][j].rgbtBlue + (-1) * tmp[i-1][j+1].rgbtBlue + 0 * tmp[i][j+1].rgbtBlue + tmp[i+1][j+1].rgbtBlue + 2 * tmp[i+1][j].rgbtBlue);
                    if (sqrt(pow(gxBlue,2) + pow(gyBlue,2)) > 255)
                    {
                        image[i][j].rgbtBlue = 255;
                    }
                    else
                    {
                        image[i][j].rgbtBlue = round(sqrt(pow(gxBlue,2) + pow(gyBlue,2)));
                    }

                    gxGreen = (0 * tmp[i][j].rgbtGreen + 0 * tmp[i-1][j].rgbtGreen + tmp[i-1][j+1].rgbtGreen + 2 * tmp[i][j+1].rgbtGreen + tmp[i+1][j+1].rgbtGreen + 0 * tmp[i+1][j].rgbtGreen);
                    gyGreen = (0 * tmp[i][j].rgbtGreen + (-2) * tmp[i-1][j].rgbtGreen + (-1) * tmp[i-1][j+1].rgbtGreen + 0 * tmp[i][j+1].rgbtGreen + tmp[i+1][j+1].rgbtGreen + 2 * tmp[i+1][j].rgbtGreen);
                    if (sqrt(pow(gxGreen,2) + pow(gyGreen,2)) > 255)
                    {
                        image[i][j].rgbtGreen = 255;
                    }
                    else
                    {
                        image[i][j].rgbtGreen = round(sqrt(pow(gxGreen,2) + pow(gyGreen,2)));
                    }

                    gxRed = (0 * tmp[i][j].rgbtRed + 0 * tmp[i-1][j].rgbtRed + tmp[i-1][j+1].rgbtRed + 2 * tmp[i][j+1].rgbtRed + tmp[i+1][j+1].rgbtRed + 0 * tmp[i+1][j].rgbtRed);
                    gyRed = (0 * tmp[i][j].rgbtRed + (-2) * tmp[i-1][j].rgbtRed + (-1) * tmp[i-1][j+1].rgbtRed + 0 * tmp[i][j+1].rgbtRed + tmp[i+1][j+1].rgbtRed + 2 * tmp[i+1][j].rgbtRed);
                    if (sqrt(pow(gxRed,2) + pow(gyRed,2)) > 255)
                    {
                        image[i][j].rgbtRed = 255;
                    }
                    else
                    {
                        image[i][j].rgbtRed = round(sqrt(pow(gxRed,2) + pow(gyRed,2)));
                    }
                }
                // If I am at the right of the tab (last column)
                else if (j == width - 1)
                {
                    gxBlue = (0 * tmp[i][j].rgbtBlue + 0 * tmp[i-1][j].rgbtBlue + (-1) * tmp[i-1][j-1].rgbtBlue + (-2) * tmp[i][j-1].rgbtBlue + (-1) * tmp[i+1][j-1].rgbtBlue + 0 * tmp[i+1][j].rgbtBlue);
                    gyBlue = (0 * tmp[i][j].rgbtBlue + (-2) * tmp[i-1][j].rgbtBlue + (-1) * tmp[i-1][j-1].rgbtBlue + 0 * tmp[i][j-1].rgbtBlue +  tmp[i+1][j-1].rgbtBlue + 2 * tmp[i+1][j].rgbtBlue);
                    if (sqrt(pow(gxBlue,2) + pow(gyBlue,2)) > 255)
                    {
                        image[i][j].rgbtBlue = 255;
                    }
                    else
                    {
                        image[i][j].rgbtBlue = round(sqrt(pow(gxBlue,2) + pow(gyBlue,2)));
                    }

                    gxGreen = (0 * tmp[i][j].rgbtGreen + 0 * tmp[i-1][j].rgbtGreen + (-1) * tmp[i-1][j-1].rgbtGreen + (-2) * tmp[i][j-1].rgbtGreen + (-1) * tmp[i+1][j-1].rgbtGreen + 0 * tmp[i+1][j].rgbtGreen);
                    gyGreen = (0 * tmp[i][j].rgbtGreen + (-2) * tmp[i-1][j].rgbtGreen + (-1) * tmp[i-1][j-1].rgbtGreen + 0 * tmp[i][j-1].rgbtGreen +  tmp[i+1][j-1].rgbtGreen + 2 * tmp[i+1][j].rgbtGreen);
                    if (sqrt(pow(gxGreen,2) + pow(gyGreen,2)) > 255)
                    {
                        image[i][j].rgbtGreen = 255;
                    }
                    else
                    {
                        image[i][j].rgbtGreen = round(sqrt(pow(gxGreen,2) + pow(gyGreen,2)));
                    }

                    gxRed = (0 * tmp[i][j].rgbtRed + 0 * tmp[i-1][j].rgbtRed + (-1) * tmp[i-1][j-1].rgbtRed + (-2) * tmp[i][j-1].rgbtRed + (-1) * tmp[i+1][j-1].rgbtRed + 0 * tmp[i+1][j].rgbtRed);
                    gyRed = (0 * tmp[i][j].rgbtRed + (-2) * tmp[i-1][j].rgbtRed + (-1) * tmp[i-1][j-1].rgbtRed + 0 * tmp[i][j-1].rgbtRed +  tmp[i+1][j-1].rgbtRed + 2 * tmp[i+1][j].rgbtRed);
                    if (sqrt(pow(gxRed,2) + pow(gyRed,2)) > 255)
                    {
                        image[i][j].rgbtRed = 255;
                    }
                    else
                    {
                        image[i][j].rgbtRed = round(sqrt(pow(gxRed,2) + pow(gyRed,2)));
                    }

                }
                else
                {
                    gxBlue = (0 * tmp[i][j].rgbtBlue + 0 * tmp[i-1][j].rgbtBlue + (-1) * tmp[i-1][j-1].rgbtBlue + (-2) * tmp[i][j-1].rgbtBlue + (-1) * tmp[i+1][j-1].rgbtBlue + 0 * tmp[i+1][j].rgbtBlue + tmp[i+1][j+1].rgbtBlue + 2 * tmp[i][j+1].rgbtBlue + tmp[i-1][j+1].rgbtBlue);
                    gyBlue = (0 * tmp[i][j].rgbtBlue + (-2) * tmp[i-1][j].rgbtBlue + (-1) * tmp[i-1][j-1].rgbtBlue + 0 * tmp[i][j-1].rgbtBlue + tmp[i+1][j-1].rgbtBlue + 2 * tmp[i+1][j].rgbtBlue + tmp[i+1][j+1].rgbtBlue + 0 * tmp[i][j+1].rgbtBlue + (-1) * tmp[i-1][j+1].rgbtBlue);
                    if (sqrt(pow(gxBlue,2) + pow(gyBlue,2)) > 255)
                    {
                        image[i][j].rgbtBlue = 255;
                    }
                    else
                    {
                        image[i][j].rgbtBlue = round(sqrt(pow(gxBlue,2) + pow(gyBlue,2)));
                    }

                    gxGreen = (0 * tmp[i][j].rgbtGreen + 0 * tmp[i-1][j].rgbtGreen + (-1) * tmp[i-1][j-1].rgbtGreen + (-2) * tmp[i][j-1].rgbtGreen + (-1) * tmp[i+1][j-1].rgbtGreen + 0 * tmp[i+1][j].rgbtGreen + tmp[i+1][j+1].rgbtGreen + 2 * tmp[i][j+1].rgbtGreen + tmp[i-1][j+1].rgbtGreen);
                    gyGreen = (0 * tmp[i][j].rgbtGreen + (-2) * tmp[i-1][j].rgbtGreen + (-1) * tmp[i-1][j-1].rgbtGreen + 0 * tmp[i][j-1].rgbtGreen + tmp[i+1][j-1].rgbtGreen + 2 * tmp[i+1][j].rgbtGreen + tmp[i+1][j+1].rgbtGreen + 0 * tmp[i][j+1].rgbtGreen + (-1) * tmp[i-1][j+1].rgbtGreen);
                    if (sqrt(pow(gxGreen, 2) + pow(gyGreen, 2)) > 255)
                    {
                        image[i][j].rgbtGreen = 255;
                    }
                    else
                    {
                        image[i][j].rgbtGreen = round(sqrt(pow(gxGreen, 2) + pow(gyGreen, 2)));
                    }

                    gxRed = (0 * tmp[i][j].rgbtRed + 0 * tmp[i-1][j].rgbtRed + (-1) * tmp[i-1][j-1].rgbtRed + (-2) * tmp[i][j-1].rgbtRed + (-1) * tmp[i+1][j-1].rgbtRed + 0 * tmp[i+1][j].rgbtRed + tmp[i+1][j+1].rgbtRed + 2 * tmp[i][j+1].rgbtRed + tmp[i-1][j+1].rgbtRed);
                    gyRed = (0 * tmp[i][j].rgbtRed + (-2) * tmp[i-1][j].rgbtRed + (-1) * tmp[i-1][j-1].rgbtRed + 0 * tmp[i][j-1].rgbtRed + tmp[i+1][j-1].rgbtRed + 2 * tmp[i+1][j].rgbtRed + tmp[i+1][j+1].rgbtRed + 0 * tmp[i][j+1].rgbtRed + (-1) * tmp[i-1][j+1].rgbtRed);
                    if (sqrt(pow(gxRed, 2) + pow(gyRed, 2)) > 255)
                    {
                        image[i][j].rgbtRed = 255;
                    }
                    else
                    {
                        image[i][j].rgbtRed = round(sqrt(pow(gxRed, 2) + pow(gyRed, 2)));
                    }
                }
            }
        }

    }
    free(tmp);
    return;
}
