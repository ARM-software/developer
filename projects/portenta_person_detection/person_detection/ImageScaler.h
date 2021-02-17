/*
ImageScaler.h

Rich Freedman, Chariot Solutions <https://chariotsolutions.com> for ARM
*/

#ifndef IMAGE_SCALER_H
#define IMAGE_SCALER_H

class ImageScaler
{
    private:
        int get_one_dim_pixel_index(int col, int row, int row_width);
        int8_t get_souurce_pixel(uint8_t *source, int col, int row, int source_width);
        void set_dest_pixel(uint8_t *dest, int col, int row, int dest_width, uint8_t value);

    public:
        int scale_image_down(uint8_t *largeImage, int large_width, int large_height, uint8_t *scaledImage, int scaled_width, int scaled_height);
};

#endif // IMAGE_SCALER_H
