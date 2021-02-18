/*
Copyright 2021 Chariot Solutions Inc. (https://chariotsolutions.com)
and ARM (https://arm.com) All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include <stdio.h>
#include <math.h>
#include "ImageCropper.h"

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
