#include <cmath>
#include <iterator>
#include <iostream>

#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "Point.h"

#include "ImageTraversal.h"

namespace Traversals {
  /**
  * Calculates a metric for the difference between two pixels, used to
  * calculate if a pixel is within a tolerance.
  *
  * @param p1 First pixel
  * @param p2 Second pixel
  * @return the difference between two HSLAPixels
  */
  double calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
    double h = fabs(p1.h - p2.h);
    double s = p1.s - p2.s;
    double l = p1.l - p2.l;

    // Handle the case where we found the bigger angle between two hues:
    if (h > 180) { h = 360 - h; }
    h /= 360;

    return sqrt((h*h) + (s*s) + (l*l));
  }
  
  /**
  * Adds a Point for the bfs traversal to visit at some point in the future.
  * @param work_list the deque storing a list of points to be processed
  * @param point the point to be added
  */
  void bfs_add(std::deque<Point> & work_list, const Point & point) {
    work_list.push_back(point);
  }



  void dfs_add(std::deque<Point> & work_list, const Point & point) {
    work_list.push_back(point);
  }

  /**
  * Removes the current Point in the bfs traversal
  * @param work_list the deque storing a list of points to be processed
  */
  void bfs_pop(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    if (!work_list.empty()) {
      work_list.pop_front();
    }
  }


  void dfs_pop(std::deque<Point> & work_list) {
    if (!work_list.empty()) {
      work_list.pop_back();
    }
  }

  Point bfs_peek(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    if (!work_list.empty()) {
      return work_list.front();
    }
    return Point(100000, 100000);
  }

  /**
   * Returns the current Point in the dfs traversal
   * @param work_list the deque storing a list of points to be processed
   * @return the current Point 
   */
  Point dfs_peek(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    if (!work_list.empty()) {
      return work_list.back();
    }
    return Point(1000000, 1000000);
  }

  
  ImageTraversal::ImageTraversal(const PNG & png, const Point & start, double tolerance, TraversalFunctions fns) {  
    /** @todo [Part 1] */
    // initialize all private fields,
    image = png;
    start_point = start;
    tolerance_ = tolerance;
    function = fns;

  }
 
  ImageTraversal::Iterator ImageTraversal::begin() {
    /** @todo [Part 1] */
    return ImageTraversal::Iterator(start_point, this);
  }

  
  ImageTraversal::Iterator ImageTraversal::end() {
    /** @todo [Part 1] */
    return ImageTraversal::Iterator(start_point, NULL);
  }

  /**
  * Default iterator constructor.
  */
  ImageTraversal::Iterator::Iterator() {
    /** @todo [Part 1] */
  }

  ImageTraversal::Iterator::Iterator(const Point& start, ImageTraversal* traversal) {
    // get the start point in the work_list_
    work_list_.push_back(start);
    current = start;
    start_p = start;
    traversal_ = traversal;


    // all the points are not visited at this point
    if (traversal_) {
      for (unsigned int i = 0; i < traversal_->image.height(); i++) {
        for (unsigned int j = 0; j < traversal_->image.width(); j++) {
          std::pair<unsigned int, unsigned int> pair(i, j);
          visited[pair] = false;
        }
      }
    }
  }


  /**
  * Iterator increment operator.
  *
  * Advances the traversal of the image.
  */
  ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
  
    // old element
    Point cur = traversal_->function.peek(work_list_);
    // set visiting status to be true
    std::pair<unsigned int, unsigned int> p(cur.x, cur.y);
    visited[p] = true;
    // pop the current out
    traversal_->function.pop(work_list_);

    // set to the next element;
    if (!work_list_.empty()) {
      p.first = traversal_->function.peek(work_list_).x;
      p.second = traversal_->function.peek(work_list_).y;
    }
    // check if the remaining list have any visited nodes
    while (!work_list_.empty() && visited[p]) {
      // pop out the element
      traversal_->function.pop(work_list_);

      p.first = traversal_->function.peek(work_list_).x;
      p.second = traversal_->function.peek(work_list_).y;
    }


    Point right = Point(cur.x + 1, cur.y);
    Point below = Point(cur.x, cur.y + 1);
    Point left = Point(cur.x - 1, cur.y);
    Point up = Point(cur.x, cur.y - 1);


    // right first
    if (valid(right)) {
      traversal_->function.add(work_list_, right);
    }

    if (valid(below)) {
      traversal_->function.add(work_list_, below);
    }

    if (valid(left)) {
      traversal_->function.add(work_list_, left);
    }

    if (valid(up)) {
      traversal_->function.add(work_list_, up);
    }

    if (work_list_.empty()) {
      traversal_ = NULL;
      return *this;
    }

    current = traversal_->function.peek(work_list_);

    // std::cout << current.x << " " << current.y << std::endl;

    return *this;
  }

  /**
  * Iterator accessor operator.
  *
  * Accesses the current Point in the ImageTraversal.
  */
  Point ImageTraversal::Iterator::operator*() {
    return current;
  }

  /**
  * Iterator inequality operator.
  *
  * Determines if two iterators are not equal.
  */
  bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
    /** @todo [Part 1] */

    return !(traversal_ == other.traversal_ && other.traversal_ == NULL);
  }

  /**
   * Iterator size function.
   *
   * @return size_t the size of the iterator work queue.
   */
  size_t ImageTraversal::Iterator::size() const {
    return work_list_.size();
  }

  /**
   * Iterator empty function.
   *
   * @return bool whether the iterator work queue is empty.
   */
  bool ImageTraversal::Iterator::empty() const {
    return work_list_.empty();
  }


  bool ImageTraversal::Iterator::valid(const Point& p) {
    unsigned int width = traversal_->image.width();
    unsigned int height = traversal_->image.height();
    // out of bounds, since they are unsigned, they cannot be negative
    if (p.x < width && p.y < height) {
      if (calculateDelta(traversal_->image.getPixel(p.x, p.y), traversal_->image.getPixel(start_p.x, start_p.y)) < traversal_->tolerance_) {
        std::pair<unsigned int, unsigned int> pair(p.x, p.y);
        return !(visited[pair]);
      }
    }
    return false;
  }

}