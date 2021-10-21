#include "helpers.h"
#include <math.h>

int valid = 1;

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0; row < height; row++)
    {
        for (int pixel = 0; pixel < width; pixel++)
        {
            //calculate the average of the three colors
            int average = round((image[row][pixel].rgbtBlue + image[row][pixel].rgbtGreen + image[row][pixel].rgbtRed) / 3.0);

            image[row][pixel].rgbtRed = average;
            image[row][pixel].rgbtGreen = average;
            image[row][pixel].rgbtBlue = average;
        }
    }
    return;
}

//cap the values at 255
int cap_values(int value)
{
    if (value > 255)
    {
        return 255;
    }
    else
    {
        return value;
    }
    return 0;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0; row < height; row++)
    {
        for (int pixel = 0; pixel < width; pixel++)
        {
            //original values
            int originalRed = image[row][pixel].rgbtRed;
            int originalGreen = image[row][pixel].rgbtGreen;
            int originalBlue = image[row][pixel].rgbtBlue;

            //sepia values
            int sepiaRed = cap_values(round(0.393 * originalRed + 0.769 * originalGreen + 0.189 * originalBlue));
            int sepiaGreen = cap_values(round(0.349 * originalRed + 0.686 * originalGreen + 0.168 * originalBlue));
            int sepiaBlue = cap_values(round(0.272 * originalRed + 0.534 * originalGreen + 0.131 * originalBlue));

            //update the image values
            image[row][pixel].rgbtRed = sepiaRed;
            image[row][pixel].rgbtGreen = sepiaGreen;
            image[row][pixel].rgbtBlue = sepiaBlue;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int tmpColor = 0;

    for (int row = 0; row < height; row++)
    {
        for (int leftPixel = 0, rightPixel = width - 1; leftPixel < width / 2; leftPixel++, rightPixel--)
        {
            //switching the red values between the left and the right
            tmpColor = image[row][rightPixel].rgbtRed;
            image[row][rightPixel].rgbtRed = image[row][leftPixel].rgbtRed;
            image[row][leftPixel].rgbtRed = tmpColor;

            //switching the green values between the left and the right
            tmpColor = image[row][rightPixel].rgbtGreen;
            image[row][rightPixel].rgbtGreen = image[row][leftPixel].rgbtGreen;
            image[row][leftPixel].rgbtGreen = tmpColor;

            //switching the blue values between the left and the right
            tmpColor = image[row][rightPixel].rgbtBlue;
            image[row][rightPixel].rgbtBlue = image[row][leftPixel].rgbtBlue;
            image[row][leftPixel].rgbtBlue = tmpColor;
        }
    }

    return;
}

//calculate the avergae for the blur function
int averageBlur(int i, int j, int height, int width, RGBTRIPLE image[height][width], int color_position)
{
    //counter stores the amount of pixels that exsits around the required pixel
    float counter = 0;
    int sum = 0;

    for (int k = i - 1; k < (i + 2); k++)
    {
        for (int l = j - 1; l < (j + 2); l ++)
        {
            if (k < 0 || l < 0 || k >= height || l >= width)
            {
                //pixel doesn't exsit
                continue;
            }
            if (color_position == 0)
            {
                sum += image[k][l].rgbtRed;
            }
            else if (color_position == 1)
            {
                sum += image[k][l].rgbtGreen;
            }
            else
            {
                sum += image[k][l].rgbtBlue;
            }
            counter++;

        }
    }

    return round(sum / counter);
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //create a temporary copy of the image array
    RGBTRIPLE tmpImage[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmpImage[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = averageBlur(i, j, height, width, tmpImage, 0);
            image[i][j].rgbtGreen = averageBlur(i, j, height, width, tmpImage, 1);
            image[i][j].rgbtBlue = averageBlur(i, j, height, width, tmpImage, 2);
        }
    }

    return;
}



