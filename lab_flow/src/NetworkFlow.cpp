#include <vector>
#include <algorithm>
#include <set>

#include "cs225_graph/graph.h"
#include "cs225_graph/edge.h"

#include "NetworkFlow.h"

int min(int a, int b) {
  if (a<b)
    return a;
  else return b;
}

NetworkFlow::NetworkFlow(Graph & startingGraph, Vertex source, Vertex sink) :
  g_(startingGraph), residual_(Graph(true,true)), flow_(Graph(true,true)), source_(source), sink_(sink) {

  // YOUR CODE HERE
  for (Vertex v : g_.getVertices()) {
    // flow_.insertVertex(v);

    for (Vertex w : g_.getAdjacent(v)) {
      // flow_.insertVertex(w);

      flow_.insertEdge(v, w);
      flow_.setEdgeWeight(v, w, 0);
    }
  }


  for (Vertex v : g_.getVertices()) {
    // residual_.insertVertex(v);

    for (Vertex w : g_.getAdjacent(v)) {
      residual_.insertEdge(v, w);
      residual_.insertEdge(w, v);
      int weight_out = g_.getEdgeWeight(v, w);
      residual_.setEdgeWeight(v, w, weight_out);
      residual_.setEdgeWeight(w, v, 0);
    }
  }

  
}

  /**
   * findAugmentingPath - use DFS to find a path in the residual graph with leftover capacity.
   *  This version is the helper function.
   *
   * @param source  The starting (current) vertex
   * @param sink    The destination vertex
   * @param path    The vertices in the path
   * @param visited A set of vertices we have visited
   */

bool NetworkFlow::findAugmentingPath(Vertex source, Vertex sink, 
  std::vector<Vertex> &path, std::set<Vertex> &visited) {

  if (visited.count(source) != 0)
    return false;
  visited.insert(source);

  if (source == sink) {
    return true;
  }

  vector<Vertex> adjs = residual_.getAdjacent(source);
  for(auto it = adjs.begin(); it != adjs.end(); it++) {
    if (visited.count(*it) == 0 && residual_.getEdgeWeight(source,*it) > 0) {
      path.push_back(*it);
      if (findAugmentingPath(*it,sink,path,visited))
        return true;
      else {
        path.pop_back();
      }
    }
  }

  return false;
}

  /**
   * findAugmentingPath - use DFS to find a path in the residual graph with leftover capacity.
   *  This version is the main function.  It initializes a set to keep track of visited vertices.
   *
   * @param source The starting (current) vertex
   * @param sink   The destination vertex
   * @param path   The vertices in the path
   */

bool NetworkFlow::findAugmentingPath(Vertex source, Vertex sink, std::vector<Vertex> &path) {
   std::set<Vertex> visited;
   path.clear();
   path.push_back(source);
   return findAugmentingPath(source,sink,path,visited);
}

  /**
   * pathCapacity - Determine the capacity of a path in the residual graph.
   *
   * @param path   The vertices in the path
   */

int NetworkFlow::pathCapacity(const std::vector<Vertex> & path) const {
  // YOUR CODE HERE

  int min = INT_MAX;
  for (unsigned int i = 0; i < (path.size() - 1); i++) {
    Vertex v = path[i];
    Vertex w = path[i + 1];
    // int cur_min = std::min(residual_.getEdgeWeight(v, w), residual_.getEdgeWeight(w, v));
    int cur_min = residual_.getEdgeWeight(v, w);
    if (cur_min < min) {
      min = cur_min;
    }
  }

  return min;
}

  /**
   * calculuateFlow - Determine the capacity of a path in the residual graph.
   * Sets the member function `maxFlow_` to be the flow, and updates the
   * residual graph and flow graph according to the algorithm.
   *
   * @return The network flow graph.
   */

const Graph & NetworkFlow::calculateFlow() {
  // YOUR CODE HERE
  int max_flow = 0;
  std::vector<Vertex> path;

  while (findAugmentingPath(source_, sink_, path)) {
    // go through path
    int capacity = pathCapacity(path);
    
    for (unsigned int i = 0; i < (path.size() - 1); i++) {
      Vertex v = path[i];
      Vertex w = path[i + 1];
      if (flow_.edgeExists(v, w)) {
        int flow_capacity = flow_.getEdgeWeight(v, w);
        flow_.setEdgeWeight(v, w, flow_capacity + capacity);
        
        int forward_capacity = residual_.getEdgeWeight(v, w) - capacity;
        residual_.setEdgeWeight(v, w, forward_capacity);

        
        int back_capacity = residual_.getEdgeWeight(w, v) + capacity;
        residual_.setEdgeWeight(w, v, back_capacity);
        
      } else {
          int flow_capacity = flow_.getEdgeWeight(w, v);
          flow_.setEdgeWeight(w, v, flow_capacity - capacity);
          
          int forward_capacity = residual_.getEdgeWeight(w, v) - capacity;
          residual_.setEdgeWeight(w, v, forward_capacity);

          int back_capacity = residual_.getEdgeWeight(v, w) + capacity;
          residual_.setEdgeWeight(v, w, back_capacity);

      }
    }

      max_flow += capacity;
      path.clear();







  }

  maxFlow_ = max_flow;
  

  return flow_;
}

int NetworkFlow::getMaxFlow() const {
  return maxFlow_;
}

const Graph & NetworkFlow::getGraph() const {
  return g_;
}

const Graph & NetworkFlow::getFlowGraph() const {
  return flow_;
}

const Graph & NetworkFlow::getResidualGraph() const {
  return residual_;
}

