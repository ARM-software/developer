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
