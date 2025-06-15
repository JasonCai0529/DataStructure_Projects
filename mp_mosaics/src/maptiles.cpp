/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>

#include "maptiles.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}


MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{

    int rows = theSource.getRows();
    int cols = theSource.getColumns();


    MosaicCanvas* mosaic = new MosaicCanvas(rows, cols);


    // each point corresponds to a TileImage
    vector<Point<3>> tile_point(theTiles.size());


    // maps a point to an index of the TileImage array for retrive later
    map<Point<3>, size_t> tile_map;
    for (size_t i = 0; i < theTiles.size(); i++) {
        tile_point[i] = convertToXYZ(theTiles[i].getAverageColor());
        tile_map[tile_point[i]] = i;  // store index instead of copy
    }


    KDTree<3> tree(tile_point);

    // for each point in the sourceImg, identify the best match of image and set that tile to the mosaic canva
    for (int i = 0; i < theSource.getRows(); i++) {
        for (int j = 0; j < theSource.getColumns(); j++) {
            Point<3> query = convertToXYZ(theSource.getRegionColor(i, j));
            Point<3> best_point = tree.findNearestNeighbor(query);
            size_t index = tile_map[best_point];
            // &theTiles[index] gives a pointer to a tileImage
            mosaic->setTile(i, j, &theTiles[index]);
        }
    }

    return mosaic;
}



