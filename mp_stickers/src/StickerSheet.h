/**
 * @file StickerSheet.h
 * Contains your declaration of the interface for the StickerSheet class.
 */
#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

#include "Image.h"


/**
 * Represents an arrangement of many sticker that is output as a single Image
 * when the user calls render().
 */
class StickerSheet
{
private:
  // Maximun amount of layers we can have
  unsigned int max_layer_;
  // used to store x and y coordinates of each sticker
  std::vector<std::pair<int, int>> coordinates_;


public:
  Image base_;
  std::vector<Image*> stickers_;



public:
  // copy constructor
  StickerSheet(const Image& picture);
  // copy assignment operator
  // StickerSheet(const StickerSheet& rhs);

  // ~StickerSheet();

  // void clear();

  // const StickerSheet& operator=(const StickerSheet& rhs);
  int addSticker(Image& sticker, int x, int y);
  int setStickerAtLayer(Image& sticker, unsigned layer, int x, int y);
  bool translate(unsigned index, int x, int y);
  void removeSticker(unsigned index);
  Image* getSticker(unsigned index);
  int layers() const;
  Image render() const;
};
