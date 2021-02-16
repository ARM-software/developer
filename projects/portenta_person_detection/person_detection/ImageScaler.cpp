#include <stdio.h>
#include <math.h>
#include "ImageScaler.h"

/*
ImageScaler.cpp

Image Scaler for Arduino

Rich Freedman, Chariot Solutions <https://chariotsolutions.com> for ARM
*/

int ImageScaler::get_one_dim_pixel_index(int col, int row, int row_width)
{
    int pixelIndexA = row * row_width + col;
    return pixelIndexA;
}

// The 'source' is a one-dimensional array representing a two-dimensional array
// This is a convenience method to allow us to treat it as a two-dimensional array
int8_t ImageScaler::get_souurce_pixel(uint8_t *source, int col, int row, int source_width)
{
    int sourcePixelIndex = get_one_dim_pixel_index(col, row, source_width);
    return source[sourcePixelIndex];
}

// The 'dest' is a one-dimensional array representing a two-dimensional array
// This is a convenience method to allow us to treat it as a two-dimensional array
void ImageScaler::set_dest_pixel(uint8_t *dest, int col, int row, int dest_width, uint8_t value)
{
    int destPixelIndex = get_one_dim_pixel_index(col, row, dest_width);
    dest[destPixelIndex] = value;
}

// "Nearest Neighbor" scaling
int ImageScaler::scale_image_down(uint8_t *largeImage, int large_width, int large_height, uint8_t *scaledImage, int scaled_width, int scaled_height)
{
    if(scaled_width > large_width || scaled_height > large_height) {
        return -1;
    }
    float x_scale = (float) large_width / (float) scaled_width;
    float y_scale = (float) large_height / (float) scaled_height;

    for(int scaled_x = 0; scaled_x < scaled_width; scaled_x++) {
        for(int scaled_y = 0; scaled_y < scaled_height; scaled_y++) {
            int large_x =  floor(scaled_x * x_scale);
            int large_y = floor(scaled_y * y_scale);
            int8_t source_pixel_value = get_souurce_pixel(largeImage, large_x, large_y, large_width);
            set_dest_pixel(scaledImage, scaled_x, scaled_y, scaled_width, source_pixel_value);
        }
    }
    return 0;
}
