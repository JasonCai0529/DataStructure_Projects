#include "Image.h"

using namespace cs225;

/**
     * Lighten an Image by increasing the luminance of every pixel by 0.1.
     * This function ensures that the luminance remains in the range [0, 1].
    **/

void Image::lighten() {
    for (unsigned int y = 0; y < height(); y++) {
        for (unsigned int x = 0; x < width(); x++) {
            HSLAPixel& oldpixel = getPixel(x, y);
            // Increment luminance of the original pixel by 0.1
            double plus_pt_one_lum = oldpixel.l + 0.1;
            // Make sure lum doesn't go beyond one
            if (plus_pt_one_lum > 1.0) {
                plus_pt_one_lum = 1.0;
            }
            HSLAPixel lighten_pixel = HSLAPixel(oldpixel.h, oldpixel.s, plus_pt_one_lum);
            oldpixel = lighten_pixel;
        }
    }
}


/**
     * Lighten an Image by increasing the luminance of every pixel by `amount`.
     * This function ensures that the luminance remains in the range [0, 1].
     *
     * @param amount The desired increase in luminance.
    **/
void Image::lighten(double amount) {
    for (unsigned int y = 0; y < height(); y++) {
        for (unsigned int x = 0; x < width(); x++) {
            HSLAPixel& oldpixel = getPixel(x, y);
            // Increment luminance of the original pixel by one, and make sure is within the range of [0, 1]
            double plus_amount_lum = oldpixel.l + amount;
            if (plus_amount_lum > 1.0) {
                plus_amount_lum = 1.0;
            }
            HSLAPixel lighten_pixel = HSLAPixel(oldpixel.h, oldpixel.s, plus_amount_lum);
            oldpixel = lighten_pixel;
        }
    }
}

/**
     * Darken an Image by decreasing the luminance of every pixel by 0.1.
     * This function ensures that the luminance remains in the range [0, 1].
    **/
void Image::darken() {
    for (unsigned int y = 0; y < height(); y++) {
        for (unsigned int x = 0; x < width(); x++) {
            HSLAPixel& oldpixel = getPixel(x, y);
            // Ensure lum doesn't go below zero
            // LOGIC MIGHT NOT BE CORRECT
            double minus_pt_one_lum = oldpixel.l - 0.1;
            if (minus_pt_one_lum < 0.0) {
                minus_pt_one_lum = 0.0;
            }

            HSLAPixel darken_pixel = HSLAPixel(oldpixel.h, oldpixel.s, minus_pt_one_lum);
            oldpixel = darken_pixel;
        }
    }
}

/**
     * Darkens an Image by decreasing the luminance of every pixel by `amount`.
     * This function ensures that the luminance remains in the range [0, 1].
     *
    **/
void Image::darken(double amount) {
    for (unsigned int y = 0; y < height(); y++) {
        for (unsigned int x = 0; x < width(); x++) {
            HSLAPixel& oldpixel = getPixel(x, y);
            // Ensure lum doesn't go below zero
            // LOGIC MIGHT NOT BE CORRECT
            double minus_amount_lum = oldpixel.l - amount;
            if (minus_amount_lum < 0.0) {
                minus_amount_lum = 0.0;
            }
            HSLAPixel darken_pixel = HSLAPixel(oldpixel.h, oldpixel.s, minus_amount_lum);
            oldpixel = darken_pixel;
        }
    }
}

/**
     * Saturates an Image by increasing the saturation of every pixel by 0.1.
     * This function ensures that the saturation remains in the range [0, 1].
    **/
void Image::saturate() {
    for (unsigned int x = 0; x < width(); x++) {
        for (unsigned int y = 0; y < height(); y++) {
            HSLAPixel& oldpixel = getPixel(x, y);
            double plus_pt_one_sat = oldpixel.s + 0.1;
            if (plus_pt_one_sat > 1.0) {
                plus_pt_one_sat = 1.0;
            }

            HSLAPixel saturate_pixel = HSLAPixel(oldpixel.h, plus_pt_one_sat, oldpixel.l);
            oldpixel = saturate_pixel;
        }
    }
}

/**
 * Saturates an Image by increasing the saturation of every pixel by `amount`.
 * This function ensures that the saturation remains in the range [0, 1].
 *
 * @param amount The desired increase in saturation.
**/
void Image::saturate(double amount) {
    for (unsigned int x = 0; x < width(); x++) {
        for (unsigned int y = 0; y < height(); y++) {
            HSLAPixel& oldpixel = getPixel(x, y);
            double plus_amount_sat = oldpixel.s + amount;
            if (plus_amount_sat > 1.0) {
                plus_amount_sat = 1.0;
            }

            HSLAPixel saturate_pixel = HSLAPixel(oldpixel.h, plus_amount_sat, oldpixel.l);
            oldpixel = saturate_pixel;
        }
    }
}

/**
 * Desaturates an Image by decreasing the saturation of every pixel by 0.1.
 * This function ensures that the saturation remains in the range [0, 1].
**/
void Image::desaturate() {
    for (unsigned int x = 0; x < width(); x++) {
        for (unsigned int y = 0; y < height(); y++) {
            HSLAPixel& pixel = getPixel(x, y);
            double minus_pt_one_sat = pixel.s - 0.1;
            if (minus_pt_one_sat < 0.0) {
                minus_pt_one_sat = 0.0;
            }
            pixel.s = minus_pt_one_sat;
        }
    }
}

/**
 * Desaturates an Image by decreasing the saturation of every pixel by `amount`.
 * This function ensures that the saturation remains in the range [0, 1].
 *
 * @param amount The desired decrease in saturation.
**/
void Image::desaturate(double amount) {
    for (unsigned int x = 0; x < width(); x++) {
        for (unsigned int y = 0; y < height(); y++) {
            HSLAPixel& pixel = getPixel(x, y);
            double minus_pt_one_sat = pixel.s - amount;
            if (minus_pt_one_sat < 0.0) {
                minus_pt_one_sat = 0.0;
            }
            pixel.s = minus_pt_one_sat;
        }
    }
}

/**
 * Turns the image grayscale.
**/
// To grayscale the image is to change all pixel's saturation value to be zero
void Image::grayscale() {
    for (unsigned int x = 0; x < width(); x++) {
        for (unsigned int y = 0; y < height(); y++) {
            HSLAPixel& pixel = getPixel(x, y);
            // Set all pixel's saturation value = 0.0
            pixel.s = 0.0;
        }
    }
}

/**
 * Rotates the color wheel by `degrees`. Rotating in a positive direction increases the degree of the hue.
 * This function ensures that the hue remains in the range [0, 360].
 *
 * @param degrees The desired amount of rotation.
**/
void Image::rotateColor(double degrees) {
    // rotates hue of all pixels
    for (unsigned int x = 0; x < width(); x++) {
        for (unsigned int y = 0; y < height(); y++) {
            HSLAPixel& pixel = getPixel(x, y);
            double rotated_hue = (pixel.h + degrees);
            // Check for negative case
            if (rotated_hue < 0.0) {
                rotated_hue = rotated_hue + 360.0;
            }
            // make sure hue is within the range from 0 to 360
            // THE MODULE OPERATION MIGHT NOT BE CORRECT
            rotated_hue = fmod(rotated_hue, 360);

            // update the hue value of the pixel
            pixel.h = rotated_hue;
        }
    }
}

//“Illini Orange” has a hue of 11
//“Illini Blue” has a hue of 216
// Hue values are in circle
void Image::illinify() {
    // Determine if current hue is closer to orange or blue
    for (unsigned int x = 0; x < width(); x++) {
        for (unsigned int y = 0; y < height(); y++) {
            HSLAPixel& pixel = getPixel(x, y);
            // Determine if current hue is closer to orange or blue
            // Find the abs value of the difference


            // Find the distance in both direction
            // Logic here might not be correct
            double orange_difference = fabs(pixel.h - 11.0);
            double orange_difference_back = (360.0 - pixel.h) + 11.0;

            double blue_difference = fabs(pixel.h - 216.0);
            double blue_difference_back = (360.0 - pixel.h) + 216.0;

            
            double orange_diff = fmin(orange_difference, orange_difference_back);
            double blue_diff = fmin(blue_difference, blue_difference_back);

            // the pixel should be set to blue
            if (orange_diff > blue_diff) {
                pixel.h = 216.0;
            } else {
                // pixel is set to orange
                pixel.h = 11.0;
            }

        }
    }
}

void Image::scale(double factor) {
    if (factor == 1.0) {
        return;
    }
    double newWidth = factor * width();
    double newHeight = factor * height();
    Image updated_image = Image(newWidth, newHeight);

    unsigned int newWidth_int = factor * width();
    unsigned int newHeight_int = factor * height();

    for (unsigned int y = 0; y < newHeight_int; y++) {
        for (unsigned int x = 0; x < newWidth_int; x++) {
            unsigned int factor_int = factor;
            unsigned int prev_x = x / factor;
            unsigned int prev_y = y / factor;
            HSLAPixel& previous_pixel = getPixel(prev_x, prev_y);
            updated_image.getPixel(x, y) = previous_pixel;
        }
    }

    *this = updated_image;
}

void Image::scale(unsigned w, unsigned h) {
    double d_w = w;
    double d_h = h;
    double width = this->width();
    double height = this->height();
    // Only valid if width and height != 0
    // first need to find the ratio of current image
    double curr_ratio = width / height;
    // ratio of the passed in param
    double para_ratio = d_w / d_h;
    if (curr_ratio == para_ratio) {
        double factor = d_w / width;
        this->scale(factor);
        return;
    }
    // Determine which of w or h should kept and use the ratio of that element to determine the value
    // of the other
    // remain w, but scale ORIGINAL HEIGHT BASED ON W"S RATIO WITH CURRENT WIDTH
    double w_width_ratio = d_w / width;
    double height_scale_by_w = height * w_width_ratio;
    if (height_scale_by_w <= d_h) {
        this->scale(w_width_ratio);
        return;
    }
    // remain h, and scale width
    double h_height_ratio = d_h / height;
    double width_scale_by_h = width * h_height_ratio;
    if (width_scale_by_h <= d_w) {
        this->scale(h_height_ratio);
        return;
    }
}


Image& Image::fillSticker(const Image& sticker, unsigned int x_start, unsigned int y_start) {
    unsigned int x_end = x_start + sticker.width();
    unsigned int y_end = y_start + sticker.height();
    for (unsigned int x = 0; x < this->width(); x++) {
        for (unsigned int y = 0; y < this->height(); y++) {
            // sticker use a difference indx tnan the based image
            unsigned int sticker_x = x - x_start;
            unsigned int sticker_y = y - y_start;
            if (x >= x_start && x < x_end && y >= y_start && y < y_end) {
                // only do this if alpha is not zero
                HSLAPixel sticker_pix = sticker.getPixel(sticker_x, sticker_y);
                if (sticker_pix.a != 0) {
                    this->getPixel(x, y) = sticker_pix;
                }
            }
        }
    }
    return *this;
}


void Image::resizeHelper(unsigned int newWidth, unsigned int newHeight, unsigned int off_x, unsigned int off_y) {
    // Create a new vector to store the image data for the new (resized) image
    // Copy the current data to the new image data, using the existing pixel
    // make a new white png with new width and height
    Image white_png = Image(newWidth, newHeight);

    for (unsigned x = 0; x < newWidth; x++) {
      for (unsigned y = 0; y < newHeight; y++) {
        if (x >= off_x && y >= off_y) {
          unsigned int old_x = x - off_x;
          unsigned int old_y = y - off_y;
          HSLAPixel & oldPixel = this->getPixel(old_x, old_y);
          HSLAPixel & newPixel = white_png.getPixel(x, y);
          newPixel = oldPixel;
        }
      }
    }

    *this = white_png;
}






