/**
 * @file PNG.cpp
 * Implementation of a simple PNG class using HSLAPixels and the lodepng PNG library.
 *
 * @author CS 225: Data Structures
 */


#include <iostream>
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <cassert>
#include <algorithm>
#include <functional>

#include "lodepng/lodepng.h"
#include "PNG.h"
#include "RGB_HSL.h"


namespace cs225 {
  void PNG::_copy(PNG const & other) {
    // Clear self
    delete[] imageData_;

    // Copy `other` to self
    width_ = other.width_;
    height_ = other.height_;
    imageData_ = new HSLAPixel[width_ * height_];
    for (unsigned i = 0; i < width_ * height_; i++) {
      imageData_[i] = other.imageData_[i];
    }
  }

  PNG::PNG() {
    width_ = 0;
    height_ = 0;
    imageData_ = NULL;
  }

  PNG::PNG(unsigned int width, unsigned int height) {
    width_ = width;
    height_ = height;
    imageData_ = new HSLAPixel[width * height];
  }

  PNG::PNG(PNG const & other) {
    imageData_ = NULL;
    _copy(other);
  }

  PNG::~PNG() {
    delete[] imageData_;
  }

  PNG const & PNG::operator=(PNG const & other) {
    if (this != &other) { _copy(other); }
    return *this;
  }

  bool PNG::operator== (PNG const & other) const {
    if (width_ != other.width_) { return false; }
    if (height_ != other.height_) { return false; }

    hslaColor hslaSpace1;
    hslaColor hslaSpace2;
    rgbaColor rgbaSpace1;
    rgbaColor rgbaSpace2;
    for (unsigned i = 0; i < width_ * height_; i++) {
      HSLAPixel & p1 = imageData_[i];
      HSLAPixel & p2 = other.imageData_[i];
      hslaSpace1 = {p1.h, p1.s, p1.l, p1.a};
      hslaSpace2 = {p2.h, p2.s, p2.l, p2.a};
      rgbaSpace1 = hsl2rgb(hslaSpace1);
      rgbaSpace2 = hsl2rgb(hslaSpace2);
      if (rgbaSpace1.r != rgbaSpace2.r
          || rgbaSpace1.g != rgbaSpace2.g
          || rgbaSpace1.b != rgbaSpace2.b
          || rgbaSpace1.a != rgbaSpace2.a) { return false; }
    }

    return true;
  }

  bool PNG::operator!= (PNG const & other) const {
    return !(*this == other);
  }

  HSLAPixel & PNG::_getPixelHelper(unsigned int x, unsigned int y) const {
    if (width_ == 0 || height_ == 0) {
      cerr << "ERROR: Call to cs225::PNG::getPixel() made on an image with no pixels." << endl;
      assert(width_ > 0);
      assert(height_ > 0);
    }

    if (x >= width_) {
      cerr << "ERROR: Call to cs225::PNG::getPixel(" << x << "," << y << ") tries to access x=" << x
          << ", which is outside of the image (image width: " << width_ << ")." << endl;
      
      assert(x < width_);
    }

    if (y >= height_) {
      cerr << "ERROR: Call to cs225::PNG::getPixel(" << x << "," << y << ") tries to access y=" << y
          << ", which is outside of the image (image height: " << height_ << ")." << endl;
      
      assert(y < height_);
    }

    unsigned index = x + (y * width_);
    return imageData_[index];
  }

  HSLAPixel & PNG::getPixel(unsigned int x, unsigned int y) { return _getPixelHelper(x,y); }

  const HSLAPixel & PNG::getPixel(unsigned int x, unsigned int y) const { return _getPixelHelper(x,y); }

  bool PNG::readFromFile(string const & fileName) {
    vector<unsigned char> byteData;
    unsigned error = lodepng::decode(byteData, width_, height_, fileName);

    if (error) {
      cerr << "PNG decoder error " << error << ": " << lodepng_error_text(error) << endl;
      return false;
    }

    delete[] imageData_;
    imageData_ = new HSLAPixel[width_ * height_];

    for (unsigned i = 0; i < byteData.size(); i += 4) {
      rgbaColor rgb;
      rgb.r = byteData[i];
      rgb.g = byteData[i + 1];
      rgb.b = byteData[i + 2];
      rgb.a = byteData[i + 3];

      hslaColor hsl = rgb2hsl(rgb);
      HSLAPixel & pixel = imageData_[i/4];
      pixel.h = hsl.h;
      pixel.s = hsl.s;
      pixel.l = hsl.l;
      pixel.a = hsl.a;
    }

    return true;
  }

  bool PNG::writeToFile(string const & fileName) {
    unsigned char *byteData = new unsigned char[width_ * height_ * 4];

    for (unsigned i = 0; i < width_ * height_; i++) {
      hslaColor hsl;
      hsl.h = imageData_[i].h;
      hsl.s = imageData_[i].s;
      hsl.l = imageData_[i].l;
      hsl.a = imageData_[i].a;

      rgbaColor rgb = hsl2rgb(hsl);

      byteData[(i * 4)]     = rgb.r;
      byteData[(i * 4) + 1] = rgb.g;
      byteData[(i * 4) + 2] = rgb.b;
      byteData[(i * 4) + 3] = rgb.a;
    }

    unsigned error = lodepng::encode(fileName, byteData, width_, height_);
    if (error) {
      cerr << "PNG encoding error " << error << ": " << lodepng_error_text(error) << endl;
    }

    delete[] byteData;
    return (error == 0);
  }

  unsigned int PNG::width() const {
    return width_;
  }

  unsigned int PNG::height() const {
    return height_;
  }

  void PNG::resize(unsigned int newWidth, unsigned int newHeight) {
    // Create a new vector to store the image data for the new (resized) image
    HSLAPixel * newImageData = new HSLAPixel[newWidth * newHeight];

    // Copy the current data to the new image data, using the existing pixel
    // for coordinates within the bounds of the old image size
    for (unsigned x = 0; x < newWidth; x++) {
      for (unsigned y = 0; y < newHeight; y++) {
        if (x < width_ && y < height_) {
          HSLAPixel & oldPixel = this->getPixel(x, y);
          HSLAPixel & newPixel = newImageData[ (x + (y * newWidth)) ];
          newPixel = oldPixel;
        }
      }
    }

    // Clear the existing image
    
    delete[] imageData_;

    // Update the image to reflect the new image size and data
    width_ = newWidth;
    height_ = newHeight;
    imageData_ = newImageData;
  }


  std::ostream & operator << ( std::ostream& os, PNG const& png ) {
    std::hash<double> hashFunction;
    std::size_t hash = 0;

    for (unsigned x = 0; x < png.width(); x++) {
      for (unsigned y = 0; y < png.height(); y++) {
        const HSLAPixel &pixel = png.getPixel(x, y);
        hash ^= hashFunction(pixel.h);
        hash ^= hashFunction(pixel.s);
        hash ^= hashFunction(pixel.l);
        hash ^= hashFunction(pixel.a);
      }
    }

    os << "PNG(w=" << png.width() << ", h=" << png.height() << ", hash=" << std::hex << hash << std::dec << ")";
    return os;
  }

}
