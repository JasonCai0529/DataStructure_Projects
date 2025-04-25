#include "StickerSheet.h"
/**
   * Add any constructors/destructors you may need based on your
   * class design for StickerSheet.
   */

  /**
   * Initializes this StickerSheet with a deep copy of the base picture.
   *
   * @param picture The base picture to use in the StickerSheet
   */
StickerSheet::StickerSheet(const Image& picture) {
    base_ = picture;
    max_layer_ = 0;
}



/**
   * Adds a `sticker` to the StickerSheet, so that the top-left of the sticker's
   * `Image` is at `(x, y)` on the StickerSheet.
   *
   * The sticker must be added to the lowest possible layer available.
   *
   * If all available layers have been filled, a new layer must be added above
   * all existing layers.
   *
   * @param sticker The Image of the sticker.
   * @param x The x location of the sticker on the StickerSheet
   * @param y The y location of the sticker on the StickerSheet
   *
   * @return The zero-based layer index the sticker was added to.
   */
int StickerSheet::addSticker(Image& sticker, int x, int y) {

    stickers_.push_back(&sticker);

    std::pair<int, int> coord(x, y);
    coordinates_.push_back(coord);

    max_layer_++;

    return stickers_.size() - 1;
}

  /**
   * Adds a `sticker` to the StickerSheet at layer `layer`, so that the top-left
   * of the sticker's `Image` is at `(x, y)` on the StickerSheet. If there is already
   * a sticker at `layer` it should be replaced and if `layer` is beyond the top of the
   * existing layers, add a new layer on top.
   *
   * @param sticker The Image of the sticker.
   * @param layer The layer where the sticker is to be placed.
   * @param x The x location of the sticker on the StickerSheet
   * @param y The y location of the sticker on the StickerSheet
   *
   * @return The zero-based layer index the sticker was added to.
   */
int StickerSheet::setStickerAtLayer(Image& sticker, unsigned layer, int x, int y) {
    // if layer is within the range
    if (layer < max_layer_) {
        stickers_[layer] = &sticker;

        std::pair<int, int> coord(x, y);
        coordinates_[layer] = coord;

        return layer;
    }

    // layer is beyond the max, adds it to the existing vector
    // WHAT IF LAYER IS BEYOND MAX_LAYER
    stickers_.push_back(&sticker);

    std::pair<int, int> coord(x, y);
    coordinates_.push_back(coord);
    max_layer_++;
    return (max_layer_ - 1);
}

  /**
   * Changes the x and y coordinates of the Image in the specified layer.
   *
   * If the layer is invalid or does not contain a sticker, this function
   * must return false. Otherwise, this function returns true.
   *
   * @param index Zero-based layer index of the sticker.
   * @param x The new x location of the sticker on the StickerSheet
   * @param y The new y location of the sticker on the StickerSheet
   *
   * @return `true` if the translate was successful; otherwise `false`.
   */
bool StickerSheet::translate(unsigned index, int x, int y) {
    // if index is invalid
    if (index >= stickers_.size()) {
        return false;
    }

    std::pair<int, int> coord(x, y);
    coordinates_[index] = coord;
    return true;
}

  /**
   * Removes the sticker at the given zero-based layer index. Make sure that the
   * other stickers don't change order.
   *
   * @param index The layer in which to delete the png
   */
void StickerSheet::removeSticker(unsigned index) {
    // remove from both vectors
    stickers_.erase(stickers_.begin() + index);
    coordinates_.erase(coordinates_.begin() + index);
    max_layer_--;
}

  /**
   * Returns a pointer to the sticker at the specified index,
   * not a copy of it. That way, the user can modify the Image.
   *
   * If the index is invalid, return NULL.
   *
   * @param index The layer in which to get the sticker.
   *
   * @return A pointer to a specific Image in the StickerSheet
   */
Image* StickerSheet::getSticker(unsigned index) {
    if (index >= max_layer_) {
        return nullptr;
    }
    return stickers_[index];
}

  /**
   * Returns the total number of layers available on the Stickersheet.
   */
int StickerSheet::layers() const {
    return stickers_.size();
}


  /**
   * Renders the whole StickerSheet on one Image and returns that Image.
   *
   * The base picture is drawn first and then each sticker is drawn in order
   * starting with layer zero (0), then layer one (1), and so on.
   *
   * If a pixel's alpha channel in a sticker is zero (0), no pixel is drawn
   * for that sticker at that pixel. If the alpha channel is non-zero, a
   * pixel is drawn. (Alpha blending is awesome, but not required.)
   *
   * The returned image always includes the full contents of the picture and
   * all stickers. It should expand in each corresponding direction if
   * stickers go beyond the edge of the picture.
   *
   * @return An Image object representing the drawn scene
   */
Image StickerSheet::render() const {
    // Each time
    // Check if the sticker go beyond the width and height of the base image
    // resize if needed
    // Change and arrange the based image to produce an image that can be used for the next layer
    // a loop through all the layers of stickers_
    Image base_layer = Image(base_);

    // needs to have a check for negative coordinates

    for (unsigned int i = 0; i < max_layer_; i++) {
        Image sticker = *(stickers_[i]);
        unsigned int width = base_layer.width();
        unsigned int height = base_layer.height();
        int x = coordinates_[i].first;
        int y = coordinates_[i].second;
        // Each time
        // Check if the sticker go beyond the width and height of the base image
        // resize if needed
        unsigned int result_width = abs(x) + sticker.width();
        unsigned int result_height = abs(y) + sticker.height();
        result_width = std::max(result_width, base_layer.width());
        result_height = std::max(result_height, base_layer.height());

        // check if any of them is negative
        // resize still based on the same logic
        if (x < 0 && y < 0) {
            base_layer.resizeHelper((width + abs(x)), (height + abs(y)), abs(x), abs(y));
            base_layer.fillSticker(sticker, 0, 0);
            continue;
        }
        if (result_height != width || result_height != height) {
            base_layer.resize(result_width, result_height);
        }
        base_layer.fillSticker(sticker, x, y);
    }
    return base_layer;
}

