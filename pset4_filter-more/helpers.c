#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // vars
    float avg;

    // for each pixel
    for (int i = 0, gray; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // calculate average color
            avg = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0;
            gray = round(avg);

            // set each color to be equal to avg -> grayscale
            image[i][j].rgbtBlue = gray;
            image[i][j].rgbtGreen = gray;
            image[i][j].rgbtRed = gray;
        }
    }

    // end function
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // declare vars for swapping
    RGBTRIPLE swap;

    // for each row of pixel
    for (int i = 0, l = width / 2; i < height; i++)
    {
        // for each corresponding pixels for the flip
        for (int j = 0; j < l; j++)
        {
            // swap the pixels
            swap = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = swap;
        }
    }

    // end function
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // copy image in order to blur using original values
    RGBTRIPLE image_copy[height][width];

    // for each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // copy RGB
            image_copy[i][j].rgbtBlue = image[i][j].rgbtBlue;
            image_copy[i][j].rgbtGreen = image[i][j].rgbtGreen;
            image_copy[i][j].rgbtRed = image[i][j].rgbtRed;
        }
    }

    // blur the original image
    // declare vars
    float r, g, b;
    int num_pixels;

    // for each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // reset vars
            r = 0;
            g = 0;
            b = 0;
            num_pixels = 0;

            // average surrounding RGB values, 3x3 box
            // shift ii & jj back by one in order to get to the beginning of the 3x3 box
            for (int ii = -1; ii < 2 && i + ii < height; ii++)
            {
                // when we've NOT started before an edge
                if (i + ii >= 0)
                {
                    for (int jj = -1; jj < 2 && j + jj < width; jj++)
                    {
                        // when we've NOT started before an edge
                        if (j + jj >= 0)
                        {
                            // add to total RGB sum
                            b += image_copy[i + ii][j + jj].rgbtBlue;
                            g += image_copy[i + ii][j + jj].rgbtGreen;
                            r += image_copy[i + ii][j + jj].rgbtRed;

                            // update number of pixels for the avg
                            num_pixels++;
                        }

                    }
                }
            }

            b /= num_pixels;
            g /= num_pixels;
            r /= num_pixels;

            // add blur to original
            image[i][j].rgbtBlue = round(b);
            image[i][j].rgbtGreen = round(g);
            image[i][j].rgbtRed = round(r);
        }
    }

    // end function
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // declare kernels for edge detection
    int gx[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, -0, 1} };
    int gy[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };

    // copy image w/ borders in order to use edge detection on boundaries of matrix
    RGBTRIPLE image_copy[height + 2][width + 2];

    // for each pixel that's not in the border
    for (int i = 1, l1 = height + 1; i < l1; i++)
    {
        for (int j = 1, l2 = width + 1; j < l2; j++)
        {
            // copy RGB
            image_copy[i][j] = image[i - 1][j - 1];
        }
    }

    // black borders
    // top and bottom row
    for (int i = 0, w = width + 2, h = height + 1; i < 2; i++)
    {
        for (int j = 0; j < w; j++)
        {
            image_copy[i * h][j].rgbtBlue = 0;
            image_copy[i * h][j].rgbtGreen = 0;
            image_copy[i * h][j].rgbtRed = 0;
        }
    }

    // left and right column
    for (int i = 0, w = width + 1, h = height + 2; i < 2; i++)
    {
        for (int j = 0; j < h; j++)
        {
            image_copy[j][i * w].rgbtBlue = 0;
            image_copy[j][i * w].rgbtGreen = 0;
            image_copy[j][i * w].rgbtRed = 0;
        }
    }

    // edge detection on the original image
    // declare variables
    int r, g, b, r_x, b_x, g_x, r_y, b_y, g_y;

    // for each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // reset vars
            r_x = 0;
            r_y = 0;
            g_x = 0;
            g_y = 0;
            b_x = 0;
            b_y = 0;

            // since the image_copy has a border, you can treat i,j as the beginning of the 3x3 box
            // since i,j is effectively like shifting i - 1, j - 1 on the image_copy
            // use kernel on surrounding RGB values, 3x3 box
            for (int ii = 0; ii < 3; ii++)
            {
                for (int jj = 0; jj < 3; jj++)
                {
                    // add to total RGB sum with x kernel
                    b_x += image_copy[i + ii][j + jj].rgbtBlue * gx[ii][jj];
                    g_x += image_copy[i + ii][j + jj].rgbtGreen * gx[ii][jj];
                    r_x += image_copy[i + ii][j + jj].rgbtRed * gx[ii][jj];

                    // add to total RGB sum with y kernel
                    b_y += image_copy[i + ii][j + jj].rgbtBlue * gy[ii][jj];
                    g_y += image_copy[i + ii][j + jj].rgbtGreen * gy[ii][jj];
                    r_y += image_copy[i + ii][j + jj].rgbtRed * gy[ii][jj];
                }
            }

            // channel final value
            r = round(sqrt(r_x * r_x + r_y * r_y));
            r = r > 255 ? 255 : r;
            g = round(sqrt(g_x * g_x + g_y * g_y));
            g = g > 255 ? 255 : g;
            b = round(sqrt(b_x * b_x + b_y * b_y));
            b = b > 255 ? 255 : b;

            // add edge detection to original
            image[i][j].rgbtBlue = b;
            image[i][j].rgbtGreen = g;
            image[i][j].rgbtRed = r;
        }
    }

    // end function
    return;
}
