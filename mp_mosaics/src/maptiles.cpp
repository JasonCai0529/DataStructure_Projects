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

// MosaicCanvas* mapTiles(SourceImage const& theSource,
//                        vector<TileImage>& theTiles)
// {
    
//     cerr << "entering map tiles";
//     // new mosaic has the exact same rows and column
//     MosaicCanvas* mosaic = new MosaicCanvas(theSource.getRows(), theSource.getColumns());


//     // need to loop through every region of the source image, find color closest to it
//     // place it in mosaic
//     vector<Point<3>> tile_point(theTiles.size());

//     map<Point<3>, TileImage> tile_map;

//     cerr << "start of first loop";
//     for (unsigned int i = 0; i < theTiles.size(); i++) {
//         LUVAPixel cur_pixel = theTiles[i].getAverageColor();

//         Point<3> pt = convertToXYZ(cur_pixel);

//         tile_point[i] = pt;
//         // construct map that has tile images to use for later
        
//         pair<Point<3>, TileImage> key_value(pt, theTiles[i]);
//         tile_map.insert(key_value);
//     }



    
//     std::cout << "start of second loop\n" << std::flush;

//     // construct a tree based on theTiles
//     KDTree<3> tree = KDTree<3>(tile_point);

//     for (int i = 0; i < theSource.getRows(); i++) {
//         for (int j = 0; j < theSource.getColumns(); j++) {
//             // for every pixels in the region
//             LUVAPixel source_pix = theSource.getRegionColor(i, j);
//             // convert this to a point
//             Point<3> query = convertToXYZ(source_pix);

//             Point<3> best_point = tree.findNearestNeighbor(query);

//             // use this best point to find tile image

//             mosaic->setTile(i, j, new TileImage(tile_map[best_point]));
//         }
//     }
//     std::cout << "return back from map tiles" << std::endl;

//     return mosaic;
// }


MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    std::cerr << "[mapTiles] entering mapTiles()" << std::endl;

    int rows = theSource.getRows();
    int cols = theSource.getColumns();

    std::cerr << "[mapTiles] Source image is " << rows << " x " << cols << std::endl;

    MosaicCanvas* mosaic = new MosaicCanvas(rows, cols);

    std::cerr << "[mapTiles] Allocated MosaicCanvas" << std::endl;

    vector<Point<3>> tile_point(theTiles.size());
    // std::map<Point<3>, TileImage> tile_map;

    // std::cerr << "[mapTiles] Starting tile preprocessing loop..." << std::endl;

    // for (unsigned int i = 0; i < theTiles.size(); i++) {
    //     std::cerr << "  [tile loop] i = " << i << std::endl;

    //     LUVAPixel cur_pixel = theTiles[i].getAverageColor();
    //     std::cerr << "    got average color: "
    //               << "(" << cur_pixel.l << "," << cur_pixel.u << "," << cur_pixel.v << ")"
    //               << std::endl;

    //     Point<3> pt = convertToXYZ(cur_pixel);
    //     tile_point[i] = pt;

    //     std::cerr << "    inserting into map" << std::endl;
    //     tile_map.insert({pt, theTiles[i]});
    // }


    map<Point<3>, size_t> tile_map;
    for (size_t i = 0; i < theTiles.size(); i++) {
        tile_point[i] = convertToXYZ(theTiles[i].getAverageColor());
        tile_map[tile_point[i]] = i;  // store index instead of copy
    }


    std::cerr << "[mapTiles] Done preprocessing tiles. Building KDTree..." << std::endl;

    KDTree<3> tree(tile_point);
    std::cerr << "[mapTiles] KDTree built. Starting second loop..." << std::endl;

    // for (int i = 0; i < rows; i++) {
    //     for (int j = 0; j < cols; j++) {
    //         std::cerr << "  [source loop] (" << i << ", " << j << ")" << std::endl;

    //         LUVAPixel source_pix = theSource.getRegionColor(i, j);
    //         Point<3> query = convertToXYZ(source_pix);
    //         Point<3> best_point = tree.findNearestNeighbor(query);


    //         auto it = tile_map.find(best_point);
    //         if (it == tile_map.end()) {
    //             std::cerr << "    ERROR: tile_map missing best_point" << std::endl;
    //             continue;
    //         }

    //         // std::cerr << " Setting tile img (" << i << ", " << j << ")" << std::endl;

    //         // TileImage* tile_img = new TileImage(it->second);

    //         // std::cerr << " After tile img (" << i << ", " << j << ")" << std::endl;

    //         // store a pointer to the unique tile object instead of making a copy
    //         TileImage* ref = &tile_map[best_point];
    //         mosaic->setTile(i, j, ref);



    //         mosaic->setTile(i, j, ref);
    //     }
    // }


    for (int i = 0; i < theSource.getRows(); i++) {
    for (int j = 0; j < theSource.getColumns(); j++) {
        Point<3> query = convertToXYZ(theSource.getRegionColor(i, j));
        Point<3> best_point = tree.findNearestNeighbor(query);
        size_t index = tile_map[best_point];
        mosaic->setTile(i, j, &theTiles[index]);  // use original
    }
}


    std::cerr << "[mapTiles] Finished mapping tiles. Returning mosaic." << std::endl;
    return mosaic;
}



