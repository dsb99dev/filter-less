#include <math.h>
#include <stdarg.h>

#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			// calculate gray shade using the average method
			BYTE gray_hex = round( (image[i][j].rgbtBlue +
						image[i][j].rgbtGreen +
						image[i][j].rgbtRed) / 3.0 );

			// replace rgb with new gray shade
			image[i][j].rgbtBlue = gray_hex;
			image[i][j].rgbtGreen = gray_hex;
			image[i][j].rgbtRed = gray_hex;
		}
	}
	return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			// get rgb values from image[i][j]
			BYTE blue_hex, green_hex, red_hex;
			blue_hex = image[i][j].rgbtBlue;
			green_hex = image[i][j].rgbtGreen;
			red_hex = image[i][j].rgbtRed;

			// calculate sepia values using the harvard formula;
			red_hex = round( (float)(red_hex * 0.393) + (float)(green_hex * 0.769) + (float)(blue_hex * 0.189) );
			green_hex = round( (float)(red_hex * 0.349) + (float)(green_hex * 0.686) + (float)(blue_hex * 0.168) );
			blue_hex = round( (float)(red_hex * 0.272) + (float)(green_hex * 0.534) + (float)(blue_hex * 0.131) );

			// replace rgb with sepia
			image[i][j].rgbtBlue = blue_hex;
			image[i][j].rgbtGreen = green_hex;
			image[i][j].rgbtRed = red_hex;
		}
	}
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
	// create a "mirrored" copy of image (mirror effect)
	RGBTRIPLE mirrored_image[height][width];
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			mirrored_image[i][j] = image[i][width - 1 -j];
		}
	}

	// replace image[][] with mirrored_image[][]
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			image[i][j] = mirrored_image[i][j];
		}
	}
	return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
	int bi_height = height + 2;
	int bi_width = width + 2;
	RGBTRIPLE bordered_image[bi_height][bi_width];
	RGBTRIPLE black_pixel = { .rgbtRed = 0, .rgbtGreen = 0, .rgbtBlue = 0 };

	// Border the image with black pixels
	for (int i = 0; i < bi_height; i++)
	{
		for (int j = 0; j < bi_width; j++)
		{
			if (i == 0 || i == bi_height - 1 || j == 0 || j == bi_width - 1)
			{
				bordered_image[i][j] = black_pixel;
			}
			else
			{
				bordered_image[i][j] = image[i][j];
			}
		}
	}

	// Apply blur to image
	for (int i = 1; i < bi_height - 1; i++)
	{
		for (int j = 1; j < bi_width - 1; j++)
		{
			image[i - 1][j - 1] = calc_blur_rgb(i, j, bi_height, bi_width, bordered_image);
		}
	}
    return;
}

// Calculate blur rgb values for pixel image[i][j]
RGBTRIPLE calc_blur_rgb(int index_i, int index_j, int height, int width, RGBTRIPLE image[height][width])
{
	RGBTRIPLE triple, blur_triple;
	float blur_red = 0.0, blur_green = 0.0, blur_blue = 0.0;
	for (int i = index_i - 1; i <= index_i + 1; i++)
	{
		for (int j = index_j - 1; j <= index_j + 1; j++)
		{
			triple = image[i][j];
			blur_red += triple.rgbtRed / 9.0;
			blur_green += triple.rgbtGreen / 9.0;
			blur_blue += triple.rgbtBlue / 9.0;
		}
	}
	blur_triple.rgbtRed = round(blur_red);
	blur_triple.rgbtGreen = round(blur_green);
	blur_triple.rgbtBlue = round(blur_blue);
	return blur_triple;
}
