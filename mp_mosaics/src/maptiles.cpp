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
    
    cerr << "entering map tiles";
    // new mosaic has the exact same rows and column
    MosaicCanvas* mosaic = new MosaicCanvas(theSource.getRows(), theSource.getColumns());


    // need to loop through every region of the source image, find color closest to it
    // place it in mosaic
    vector<Point<3>> tile_point(theTiles.size());

    map<Point<3>, TileImage> tile_map;

    cerr << "start of first loop";

    for (unsigned int i = 0; i < theTiles.size(); i++) {
        LUVAPixel cur_pixel = theTiles[i].getAverageColor();

        Point<3> pt = convertToXYZ(cur_pixel);

        tile_point[i] = pt;
        // construct map that has tile images to use for later
        
        pair<Point<3>, TileImage> key_value(pt, theTiles[i]);
        tile_map.insert(key_value);
    }



    
    cerr << "start of second loop";
    // construct a tree based on theTiles
    KDTree<3> tree = KDTree<3>(tile_point);

    for (int i = 0; i < theSource.getRows(); i++) {
        for (int j = 0; j < theSource.getColumns(); j++) {
            // for every pixels in the region
            LUVAPixel source_pix = theSource.getRegionColor(i, j);
            // convert this to a point
            Point<3> query = convertToXYZ(source_pix);

            Point<3> best_point = tree.findNearestNeighbor(query);

            // use this best point to find tile image

            mosaic->setTile(i, j, new TileImage(tile_map[best_point]));
        }
    }

    cerr << "returning back from map tiles";

    return mosaic;
}


