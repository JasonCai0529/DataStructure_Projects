/* Your code here! */
#include "maze.h"

#include <queue>
#include <map>


void print_vect(std::vector<int> vect) {
    for (unsigned int i = 0; i < vect.size(); i++) {
        if (i == 0) {
            std::cout << "< ";
        }

        std::cout << vect[i] << " ";

        if (i == vect.size() - 1) {
            std::cout << ">" << std::endl;
        }
    }
}


SquareMaze::SquareMaze() {

}

    /**
     * Makes a new SquareMaze of the given height and width.
     *
     * If this object already represents a maze it will clear all the
     * existing data before doing so. You will start with a square grid (like
     * graph paper) with the specified height and width. You will select
     * random walls to delete without creating a cycle, until there are no
     * more walls that could be deleted without creating a cycle. Do not
     * delete walls on the perimeter of the grid.
     *
     * Hints: You only need to store 2 bits per square: the "down" and
     * "right" walls. The finished maze is always a tree of corridors.)
     *
     * @param width The width of the SquareMaze (number of cells)
     * @param height The height of the SquareMaze (number of cells)
     */
void SquareMaze::makeMaze(int width, int height) {
    //TODO
    // if there is an existing maze
    // if (!walls.empty()) {
    walls.clear();
    
    width_ = width;
    height_ = height;
    int blocks = width * height;
    // if dimension correct
    for (int i = 0; i < width; i++) {
        std::vector<std::pair<bool, bool>> vect;
        for (int j = 0; j < height; j++) {
            std::pair<bool, bool> p(true, true);
            vect.push_back(p);
        }
        walls.push_back(vect);
    }

    // disjoint set
    set_.addelements(blocks);

    // see if I can remove two walls by checking both its right and down
    // every thing should be connected with each other
    while (set_.size(0) < blocks) {
        int x = rand() % width;
        int y = rand() % height;
        int dir = rand() % 2;  // either 0(right) or 1(down)
        int offset = x + y * width;

        // right neibour
        int r_x = x + 1;
        int r_y = y;
        int r_offset = r_x + r_y * width;

        // // down neighbour
        int d_x = x;
        int d_y = y + 1;
        int d_offset = d_x + d_y * width;

        if (dir == 0) {
            // remove right wall
            if (x < (width - 1)) {
                if (set_.find(offset) != set_.find(r_offset)) {
                    setWall(x, y, 0, false);
                    set_.setunion(offset, r_offset);
                }
            }
        } else {
            // dir == 1
            if (y < (height - 1)) {
                if (set_.find(offset) != set_.find(d_offset)) {
                    setWall(x, y, 1, false);
                    set_.setunion(offset, d_offset);
                }
            }
        }
    }
    
}


    /**
     * This uses your representation of the maze to determine whether it is
     * possible to travel in the given direction from the square at
     * coordinates (x,y).
     *
     * For example, after makeMaze(2,2), the possible input coordinates will
     * be (0,0), (0,1), (1,0), and (1,1).
     *
     * - dir = 0 represents a rightward step (+1 to the x coordinate)
     * - dir = 1 represents a downward step (+1 to the y coordinate)
     * - dir = 2 represents a leftward step (-1 to the x coordinate)
     * - dir = 3 represents an upward step (-1 to the y coordinate)
     *
     * You can not step off of the maze or through a wall.
     *
     * This function will be very helpful in solving the maze. It will also
     * be used by the grading program to verify that your maze is a tree that
     * occupies the whole grid, and to verify your maze solution. So make
     * sure that this function works!
     *
     * @param x The x coordinate of the current cell
     * @param y The y coordinate of the current cell
     * @param dir The desired direction to move from the current cell
     * @return whether you can travel in the specified direction
     */
bool SquareMaze::canTravel(int x, int y, int dir) const {
    // need to consider walls
    if (dir == 0) {
        return (x + 1 < width_) && (x >= 0) && (y < height_) && (y >= 0) && !(walls[x][y].first);
    } else if (dir == 1) {
        return (x < width_) && (x > -1) && (y + 1 < height_) && (y >= 0) && !(walls[x][y].second); 
    } else if (dir == 2) {
        return (x - 1 >= 0) && (x < width_) && (y < height_) && (y > -1) && !(walls[x-1][y].first);
    } else {
        return (x < width_) && (x > -1) && (y - 1 >= 0) && (y < height_) && !(walls[x][y-1].second);
    }
}

    /**
     * Sets whether or not the specified wall exists.
     *
     * This function should be fast (constant time). You can assume that in
     * grading we will not make your maze a non-tree and then call one of the
     * other member functions. setWall should not prevent cycles from
     * occurring, but should simply set a wall to be present or not present.
     * Our tests will call setWall to copy a specific maze into your
     * implementation.
     *
     * @param x The x coordinate of the current cell
     * @param y The y coordinate of the current cell
     * @param dir Either 0 (right) or 1 (down), which specifies which wall to
     * set (same as the encoding explained in canTravel). You only need to
     * support setting the bottom and right walls of every square in the grid.
     * @param exists true if setting the wall to exist, false otherwise
     */
void SquareMaze::setWall(int x, int y, int dir, bool exists) {
    (dir == 0) ? walls[x][y].first = exists : walls[x][y].second = exists;;
}

    /**
     * Solves this SquareMaze.
     *
     * For each square on the bottom row (maximum y coordinate), there is a
     * distance from the origin (i.e. the top-left cell), which is defined as
     * the length (measured as a number of steps) of the only path through
     * the maze from the origin to that square.
     *
     * Select the square in the bottom row with the largest distance from the
     * origin as the destination of the maze. solveMaze() returns the
     * winning path from the origin to the destination as a vector of
     * integers, where each integer represents the direction of a step, using
     * the same encoding as in canTravel().
     *
     * If multiple paths of maximum length exist, use the one with the
     * destination cell that has the smallest x value.
     *
     * Hint: this function should run in time linear in the number of cells
     * in the maze.
     *
     * @return a vector of directions taken to solve the maze
     */
std::vector<int> SquareMaze::solveMaze() {
    //TODO
    std::map<std::pair<int, int>, bool> visited; // initialize all to false

    std::map<std::pair<int, int>, std::vector<int>> paths; // path to get to that point

    std::queue<std::pair<int, int>> cells_queue;

    // start with (0,0) // top left corner
    cells_queue.push({0, 0});

    while (!cells_queue.empty()) {
        std::pair<int, int> cur_cell = cells_queue.front();

        int x = cur_cell.first;
        int y = cur_cell.second;

        cells_queue.pop();

        visited[cur_cell] = true;

        // check right
        if (canTravel(x, y, 0) && !visited[{x + 1, y}]) {
            // add right to queue
            cells_queue.push({x + 1, y});


            paths[{x + 1, y}] = paths[cur_cell];
            paths[{x + 1, y}].push_back(0);
            // update vector that got to that point
        }

        // check down
        if (canTravel(x, y, 1) && !visited[{x, y + 1}]) {
            cells_queue.push({x, y + 1});
            paths[{x, y + 1}] = paths[cur_cell];
            paths[{x, y + 1}].push_back(1);
        }

        // check left
        if (canTravel(x, y, 2) && !visited[{x - 1, y}]) {
            cells_queue.push({x - 1, y});
            paths[{x - 1, y}] = paths[cur_cell];
            paths[{x - 1, y}].push_back(2);
        }
        
        // check up
        if (canTravel(x, y, 3) && !visited[{x, y - 1}]) {
            cells_queue.push({x, y - 1});
            paths[{x, y - 1}] = paths[cur_cell];
            paths[{x, y - 1}].push_back(3);
        }

    }

    int max = 0;
    std::vector<int> solution;

    int y = height_ - 1;
    // find which vector in the last row has the greatest size
    // last element of every vector

    for (int x = 0; x < width_; x++) {

        std::vector<int> cur_sol = paths[{x, y}];
        int cur_max = cur_sol.size();

        if (cur_max > max) {
            solution = cur_sol;
            max = cur_max;
        }
    }

    return solution;
}

    /**
     * Draws the maze without the solution.
     *
     * First, create a new PNG. Set the dimensions of the PNG to
     * (width*10+1,height*10+1). where height and width were the arguments to
     * makeMaze. Blacken the entire topmost row and leftmost column of
     * pixels, except the entrance (1,0) through (9,0).  For each square in
     * the maze, call its maze coordinates (x,y). If the right wall exists,
     * then blacken the pixels with coordinates ((x+1)*10,y*10+k) for k from
     * 0 to 10. If the bottom wall exists, then blacken the pixels with
     * coordinates (x*10+k, (y+1)*10) for k from 0 to 10.
     *
     * The resulting PNG will look like the sample image, except there will
     * be no exit from the maze and the red line will be missing.
     *
     * @return a PNG of the unsolved SquareMaze
     */
cs225::PNG* SquareMaze::drawMaze() const {
    int png_width = width_ * 10 + 1;
    int png_height = height_ * 10 + 1;

    cs225::PNG* maze = new cs225::PNG(png_width, png_height);
    cs225::HSLAPixel black_pixel = cs225::HSLAPixel(0, 0, 0);

    // top_row
    for (int i = 0; i < png_width; i++) {
        if (i >= 1 && i <= 9)
            continue;
        maze->getPixel(i, 0) = black_pixel;
    }

    // leftmost_col
    for (int i = 0; i < png_height; i++) {
        maze->getPixel(0, i) = black_pixel;
    }

    for (int x = 0; x < width_; x++) {
        for (int y = 0; y < height_; y++) {

            bool right = walls[x][y].first;
            bool down = walls[x][y].second;
            if (right) {
                for (unsigned int i = 0; i < 11; i++) {
                    maze->getPixel((x+1)*10, y*10+i) = black_pixel;
                }
            }
            
            if (down) {
                for (unsigned int i = 0; i < 11; i++) {
                    maze->getPixel((x*10) + i, (y + 1)*10) = black_pixel;
                }
            }


        }
    }

    return maze;
}

    /**
     * This function calls drawMaze, then solveMaze; it modifies the PNG
     * from drawMaze to show the solution vector and the exit.
     *
     * Start at pixel (5,5). Each direction in the solution vector
     * corresponds to a trail of 11 red pixels in the given direction. If the
     * first step is downward, color pixels (5,5) through (5,15) red. (Red is
     * 0,1,0.5,1 in HSLA). Then if the second step is right, color pixels (5,15)
     * through (15,15) red. Then if the third step is up, color pixels
     * (15,15) through (15,5) red. Continue in this manner until you get to
     * the end of the solution vector, so that your output looks analogous
     * the above picture.
     *
     * Make the exit by undoing the bottom wall of the destination square:
     * call the destination maze coordinates (x,y), and whiten the pixels
     * with coordinates (x*10+k, (y+1)*10) for k from 1 to 9.
     *
     * @return a PNG of the solved SquareMaze
     */
cs225::PNG* SquareMaze::drawMazeWithSolution() {
    cs225::PNG* maze = drawMaze();

    std::vector<int> solution = solveMaze();

    cs225::HSLAPixel red = cs225::HSLAPixel(0, 1, 0.5, 1);

    int x = 5;
    int y = 5;
    for (int dir : solution) {
        if (dir == 0) {
            for (unsigned int i = 0; i < 11; i++) {
                maze->getPixel(x + i, y) = red;
            }
            x += 10;
        } else if (dir == 1) {
            for (unsigned int i = 0; i < 11; i++) {
                maze->getPixel(x, y + i) = red;
            }
            y += 10;
        } else if (dir == 2) {
            for (unsigned int i = 0; i < 11; i++) {
                maze->getPixel(x - i, y) = red;
            }
            x -= 10;
        } else {
            for (unsigned int i = 0; i < 11; i++) {
                maze->getPixel(x, y - i) = red;
            }
            y -= 10;
        }
    }


    cs225::HSLAPixel white = cs225::HSLAPixel(0, 0, 1);
    int x_offset = x - 5;
    for (int i = 1; i < 10; i++) {
        maze->getPixel(x_offset + i, maze->height() - 1) = white;
    }

    return maze;
}




