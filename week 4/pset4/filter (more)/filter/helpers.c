#include "helpers.h"
#include <math.h>

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

//A function to calculate the value for the pixels that creates an edge
int calculate_edge(int i, int j, int height, int width, RGBTRIPLE image[height][width], int color_position)
{
    //variables for Sobel operator
    int Gx = 0;
    int Gy = 0;

    for (int k = i - 1; k < (i + 2); k++)
    {
        for (int l = j - 1; l < (j + 2); l ++)
        {
            if (k < 0 || l < 0 || k >= height || l >= width)
            {
                //pixel doesn't exsit, skip to the next pixel
                continue;
            }

            //calculate the Gx value

            //compute the values of the upper-left and lower-left pixels
            if ((l < j && k < i) || (l < j && k > i))
            {
                if (color_position == 0)
                {
                    Gx += image[k][l].rgbtRed * -1;
                }
                else if (color_position == 1)
                {
                    Gx += image[k][l].rgbtGreen * -1;
                }
                else
                {
                    Gx += image[k][l].rgbtBlue * -1;
                }
            }
            else if (l < j && k == i)//compute the values of the middle-left pixel
            {
                if (color_position == 0)
                {
                    Gx += image[k][l].rgbtRed * -2;
                }
                else if (color_position == 1)
                {
                    Gx += image[k][l].rgbtGreen * -2;
                }
                else
                {
                    Gx += image[k][l].rgbtBlue * -2;
                }
            }
            else if ((l > j && k < i) || (l > j && k > i))//compute the values of the upper-right and lower-right pixels
            {
                if (color_position == 0)
                {
                    Gx += image[k][l].rgbtRed * 1;
                }
                else if (color_position == 1)
                {
                    Gx += image[k][l].rgbtGreen * 1;
                }
                else
                {
                    Gx += image[k][l].rgbtBlue * 1;
                }
            }
            else if (l > j && k == i)//compute the values of the middle-right pixel
            {
                if (color_position == 0)
                {
                    Gx += image[k][l].rgbtRed * 2;
                }
                else if (color_position == 1)
                {
                    Gx += image[k][l].rgbtGreen * 2;
                }
                else
                {
                    Gx += image[k][l].rgbtBlue * 2;
                }
            }

            //calculate the Gx value


            if ((l < j && k < i) || (l > j && k < i))//compute the values of the upper-left and upper-right pixels
            {
                if (color_position == 0)
                {
                    Gy += image[k][l].rgbtRed * -1;
                }
                else if (color_position == 1)
                {
                    Gy += image[k][l].rgbtGreen * -1;
                }
                else
                {
                    Gy += image[k][l].rgbtBlue * -1;
                }
            }
            else if (l == j && k < i)//compute the values of the upper-middle pixel
            {
                if (color_position == 0)
                {
                    Gy += image[k][l].rgbtRed * -2;
                }
                else if (color_position == 1)
                {
                    Gy += image[k][l].rgbtGreen * -2;
                }
                else
                {
                    Gy += image[k][l].rgbtBlue * -2;
                }
            }
            else if ((l < j && k > i) || (l > j && k > i))//compute the values of the lower-left and lower-right pixels
            {
                if (color_position == 0)
                {
                    Gy += image[k][l].rgbtRed * 1;
                }
                else if (color_position == 1)
                {
                    Gy += image[k][l].rgbtGreen * 1;
                }
                else
                {
                    Gy += image[k][l].rgbtBlue * 1;
                }
            }
            else if (l == j && k > i)//compute the values of the lower-middle pixel
            {
                if (color_position == 0)
                {
                    Gy += image[k][l].rgbtRed * 2;
                }
                else if (color_position == 1)
                {
                    Gy += image[k][l].rgbtGreen * 2;
                }
                else
                {
                    Gy += image[k][l].rgbtBlue * 2;
                }
            }
        }
    }

    //return the square root of Gx^2 and Gy^2
    return cap_values(round(sqrt(Gx * Gx + Gy * Gy)));
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
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
            image[i][j].rgbtRed = calculate_edge(i, j, height, width, tmpImage, 0);
            image[i][j].rgbtGreen = calculate_edge(i, j, height, width, tmpImage, 1);
            image[i][j].rgbtBlue = calculate_edge(i, j, height, width, tmpImage, 2);
        }
    }

    return;
}




