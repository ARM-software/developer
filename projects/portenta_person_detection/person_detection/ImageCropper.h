/*
ImageCropper.h

Image Cropper for Arduino

Rich Freedman, Chariot Solutions <https://chariotsolutions.com> for ARM
*/

#ifndef IMAGE_CROPPER_H
#define IMAGE_CROPPER_H

class ImageCropper
{

    private:
        int get_one_dim_pixel_index(int col, int row, int row_width) ;
        int8_t get_source_pixel(uint8_t *source, int source_width, int col, int row);
        void set_dest_pixel(uint8_t *dest, int dest_width, int col, int row, uint8_t value) ;

    public:
        void crop_image(uint8_t *source, int source_width, int source_height, uint8_t *dest, int dest_width, int dest_height);
};

#endif // IMAGE_CROPPER_H
