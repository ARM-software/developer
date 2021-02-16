#include <stdio.h>
#include <math.h>
#include "ImageCropper.h"

/*
ImageCropper.cpp

Image Cropper for Arduino

Rich Freedman, Chariot Solutions <https://chariotsolutions.com> for ARM
*/

int ImageCropper::get_one_dim_pixel_index(int col, int row, int row_width)
{
    int pixelIndexA = row * row_width + col;
    return pixelIndexA;
}

// The 'source' is a one-dimensional array representing a two-dimensional array
// This is a convenience method to allow us to treat it as a two-dimensional array
int8_t ImageCropper::get_source_pixel(uint8_t *source, int source_width, int col, int row)
{
    int sourcePixelIndex = get_one_dim_pixel_index(col, row, source_width);
    return source[sourcePixelIndex];
}

// The 'dest' is a one-dimensional array representing a two-dimensional array
// This is a convenience method to allow us to treat it as a two-dimensional array
void ImageCropper::set_dest_pixel(uint8_t *dest, int dest_width, int col, int row, uint8_t value)
{
    int destPixelIndex = get_one_dim_pixel_index(col, row, dest_width);
    dest[destPixelIndex] = value;
}

/**
 * Center-Crops to specified dimensions
 */
void ImageCropper::crop_image(uint8_t *source, int source_width, int source_height, uint8_t *dest, int dest_width, int dest_height)
{
    // crop with x and y offset to effect center crop

    const int offsetX = round((source_width - dest_width) / 2);
    const int offsetY = round((source_height - dest_height) / 2);

    for(int row = 0; row < dest_height; row++) {
        for (int col = 0; col < dest_width; col++) {
            uint8_t value = get_source_pixel(source, source_width, col + offsetX, row + offsetY);
            set_dest_pixel(dest, dest_width, col, row, value);
        }
    }
}
