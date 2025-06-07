/**
 * @file puzzle.cpp
 * Implementation of puzzle class.
 */
#include "puzzle.h"

#include <algorithm>
#include <queue>
#include <map>
#include <set>
#include <cmath>

/**
    * Default constructor for the puzzle state. This should initialize the
    * puzzle to the solved state.
    */
using namespace std;

void PuzzleState::print() const {
    std::cout << "< ";
    for (unsigned int i = 0; i < 16; i++) {
        int cur = board[i];
        std::cout << cur << " ";
    }

    std::cout << ">" << std::endl;
}

std::pair<unsigned int, unsigned int> getCoordinate(unsigned int idx) {
    unsigned int x = idx % 4;
    unsigned int y = idx / 4;

    return {x, y};
}

std::pair<unsigned int, unsigned int> PuzzleState::getElemCord(uint8_t elem) const {
    for (unsigned int i = 0; i < 16; i++) {
        if (board[i] == elem) {
            return getCoordinate(i);
        }
    }

    return {};
}

PuzzleState::PuzzleState() {
    uint8_t i = 1;
    for (unsigned int j = 0; j < 15; j++) {
        board[j] = i;
        i++;
    }
    board[15] = 0;
}


PuzzleState::PuzzleState(const std::array<uint8_t, 16> state) {
    board = state;
}


std::array<uint8_t, 16> PuzzleState::asArray() const {
    return board;
}



bool PuzzleState::operator==(const PuzzleState &rhs) const {
    // check if they have the same array

    for (unsigned int i = 0; i < 16; i++) {
        if (board[i] != rhs.board[i]) {
            return false;
        }
    }
    return true;

    // return board == rhs.board;
}


bool PuzzleState::operator!=(const PuzzleState &rhs) const {
    // if one position not the same, return true

    for (unsigned int i = 0; i < 16; i++) {
        // one position does not align -> not equal
        if (board[i] != rhs.board[i]) {
            return true;
        }
    }
    return false;

    // return board != rhs.board;

}


bool PuzzleState::operator<(const PuzzleState &rhs) const {
    for (unsigned int i = 0; i < 16; i++) {
        if (board[i] == rhs.board[i]) {
            continue;
        }
        if (board[i] < rhs.board[i]) {
            return true;
        } else {
            return false;
        }
    }

    return false;

    // return board < rhs.board;
}


PuzzleState PuzzleState::getNeighbor(Direction direction) const {
    // TODO
    PuzzleState neighbour = *this;
   
    uint8_t empty_tile = 0;

    // get coordinates of empty space;
    int pos = 0;
    for (int i = 0; i < 16; i++) {
        if (board[i] == empty_tile) {
            pos = i;
            break;
        }
    }

    int up = pos - 4;
    int down = pos + 4;
    int left = pos - 1;
    int right = pos + 1;


    std::pair<unsigned int, unsigned int> cor = getCoordinate(pos);
    // bool invalid = false;

    // int left_cord = cor.first;
    // int right_cord = cor.first;
    // int up_cor = cor.second;
    // int down_cor = cor.second;

    // left_cord -= 1;
    // right_cord += 1;
    // up_cor -= 1;
    // down_cor += 1;
    
    // if (direction == Direction::UP) {
    //     // sth blow empty space
    //     if (down_cor <= 3) {
    //         uint8_t down_elem = neighbour.board[down];
    //         neighbour.board[down] = neighbour.board[pos];
    //         neighbour.board[pos] = down_elem;
    //     } else {
    //         invalid = true;
    //     }
    // } else if (direction == Direction::DOWN) {
    //     // sth up
    //     if (up_cor >= 0) {
    //         uint8_t up_elem = neighbour.board[up];
    //         neighbour.board[up] = neighbour.board[pos];
    //         neighbour.board[pos] = up_elem;
    //     } else {
    //         invalid = true;
    //     }
    // } else if (direction == Direction::LEFT) {
    //     // sth right
    //     if (right_cord >= 0 && right_cord <= 3) {
    //         uint8_t right_elem = neighbour.board[right];
    //         neighbour.board[right] = neighbour.board[pos];
    //         neighbour.board[pos] = right_elem;
    //     } else {
    //         invalid = true;
    //     }
    // } else {
    //     // sth on the left
    //     // direction == Direction::RIGHT
    //     if (left_cord >= 0 && left_cord <= 3) {
    //         uint8_t left_elem = neighbour.board[left];
    //         neighbour.board[left] = neighbour.board[pos];
    //         neighbour.board[pos] = left_elem;
    //     } else {
    //         invalid = true;
    //     }
    // }

    // if (invalid) {
    //     neighbour.board = empty;
    // }


    if (direction == Direction::UP) {
        // sth blow empty space
        if (cor.second < 3) {
            // uint8_t down_elem = neighbour.board[down];
            // neighbour.board[down] = neighbour.board[pos];
            // neighbour.board[pos] = down_elem;
            std::swap(neighbour.board[down], neighbour.board[pos]);

        }
    } else if (direction == Direction::DOWN) {
        // sth up
        if (cor.second > 0) {
            // uint8_t up_elem = neighbour.board[up];
            // neighbour.board[up] = neighbour.board[pos];
            // neighbour.board[pos] = up_elem;

            std::swap(neighbour.board[up], neighbour.board[pos]);

        }
    } else if (direction == Direction::LEFT) {
        // sth right
        if (cor.first < 3) {
            // uint8_t right_elem = neighbour.board[right];
            // neighbour.board[right] = neighbour.board[pos];
            // neighbour.board[pos] = right_elem;
            std::swap(neighbour.board[right], neighbour.board[pos]);
        }
    } else {
        // sth on the left
        // direction == Direction::RIGHT
        if (cor.first > 0) {
            // uint8_t left_elem = neighbour.board[left];
            // neighbour.board[left] = neighbour.board[pos];
            // neighbour.board[pos] = left_elem;
            std::swap(neighbour.board[left], neighbour.board[pos]);
        }
    }

    return neighbour;
}

std::vector<PuzzleState> PuzzleState::getNeighbors() const {
    // should be 2, 3, or 4
    std::vector<PuzzleState> states;
    PuzzleState up = getNeighbor(Direction::UP);
    PuzzleState down = getNeighbor(Direction::DOWN);
    PuzzleState left = getNeighbor(Direction::LEFT);
    PuzzleState right = getNeighbor(Direction::RIGHT);

    // // MAY I USE != for array directly?
    // std::array<uint8_t, 16> empty;
    // for (unsigned int i = 0; i < 16; i++) {
    //     empty[i] = 0;
    // }
    // if (up.board != empty) {
    //     states.push_back(up);
    // }
    // if (down.board != empty) {
    //     states.push_back(down);
    // }

    // if (left.board != empty) {
    //     states.push_back(left);
    // }

    // if (right.board != empty) {
    //     states.push_back(right);
    // }


    if (up != *this) {
        states.push_back(up);
    }
    if (down != *this) {
        states.push_back(down);
    }

    if (left != *this) {
        states.push_back(left);
    }

    if (right != *this) {
        states.push_back(right);
    }

    return states;
}


int PuzzleState::manhattanDistance(const PuzzleState desiredState) const {
    // TODO
    int distance = 0;
    for (unsigned int i = 0; i < 16; i++) {
        std::pair<unsigned int, unsigned int> other_cor = desiredState.getElemCord(board[i]);
        std::pair<unsigned int, unsigned int> cur_cor = getCoordinate(i);

        int ox = other_cor.first;
        int oy = other_cor.second;
        int x = cur_cor.first;
        int y = cur_cor.second;

        if (board[i] != 0) {
            distance += abs(x - ox) + abs(y - oy);
        }

    }

    return distance;
}



std::vector<PuzzleState> solveAstar(const PuzzleState& startState, const PuzzleState &desiredState, size_t *iterations) {
    std::cout << " ASTAR______________________________________" << std::endl;
    startState.print();
    desiredState.print();
    std::vector<PuzzleState> states;

    auto comp = [](const std::pair<PuzzleState, int>& p1, const std::pair<PuzzleState, int>& p2) {
        return p1.second > p2.second;
    };
    

    // std::priority_queue<std::pair<PuzzleState, int>, std::vector<std::pair<PuzzleState, int>>, decltype(comp)> q;  // Queue to go through all nodes // associate a Puzzle state to its f cost
    std::priority_queue<std::pair<int, PuzzleState>, std::vector<std::pair<int, PuzzleState>>, std::greater<std::pair<int, PuzzleState>>> q;
    std::map<PuzzleState, int> pathCostOf;

    std::map<PuzzleState, bool> visited; // map to track visited state // used as closed


    std::set<PuzzleState> closedList;

    std::map<PuzzleState, PuzzleState> parent; // parent[p] = its parent;

    pathCostOf[startState] = 0;



    size_t it = 0;

    // q.push({startState, startState.manhattanDistance(desiredState)});
    q.push({0, startState});
    q.emplace(0, startState);

    pathCostOf[startState] = 0;

    PuzzleState desire;

    while (!q.empty()) {
        PuzzleState cur = q.top().second;
        it++;
        q.pop();


        if (cur == desiredState) {
            std::cout << "INSIDE" << std::endl;
            desire = cur;
            break;
        }

        visited[cur] = true;


        std::vector<PuzzleState> neighbours = cur.getNeighbors();

        for (const PuzzleState& neighbour : neighbours) {
            // skip the visited ones
            if (visited[neighbour]) {
                continue;
            }

            int nextPathCost = pathCostOf[cur] + 1;

            if (pathCostOf.find(neighbour) == pathCostOf.end() || nextPathCost < pathCostOf[neighbour]) {

                pathCostOf[neighbour] = nextPathCost;
                
                q.emplace(nextPathCost + neighbour.manhattanDistance(desiredState), neighbour);

                parent[neighbour] = cur;


            }
        }

    }

    // For each node, get all its neibours find the mind F cost -> distance from start node + distance from end node
    // go the next one with the lowest cost

    states.insert(states.begin(), desiredState);
    while (desire != startState) {
        desire = parent[desire];
        states.insert(states.begin(), desire);
    }

    if (iterations) {
        *iterations = states.size();
    }

    return states;

}







std::vector<PuzzleState> solveBFS(const PuzzleState &startState, const PuzzleState &desiredState, size_t *iterations) {
    std::vector<PuzzleState> states;
    std::cout << "ENTER FUNCTION BFS_____________________________________" << std::endl;


    std::queue<PuzzleState> q;  // Queue to go through all nodes
    std::map<PuzzleState, bool> visited; // map to track visited state
    std::map<PuzzleState, PuzzleState> parent; // parent[p] = its parent;

    size_t it = 0;

    q.push(startState);

    PuzzleState desire = PuzzleState(startState);

    // q will never be empty ??
    while (!q.empty()) {

        PuzzleState p = q.front();
        // q.pop();
        it++;

        if (p == desiredState) {
            desire = p;
            break;
        }

        q.pop();


        if (!visited[p]) {
            visited[p] = true;

            for (const PuzzleState& neighbour : p.getNeighbors()) {
                if (!visited[neighbour]) {
                    parent[neighbour] = p;
                    q.push(neighbour);
                    // visited[neighbour] = true;
                }
            }
        }
    }

    // startState.print();

    // back trace the track
    // while (desire != startState) {

    //     if (parent.find(desire) == parent.end()) {
    //         break;
    //     }

    //     desire = parent[desire];
    //     states.insert(states.begin(), desire);
    // }



    states.insert(states.begin(), desiredState);

    while (desire != startState) {

        if (parent.find(desire) == parent.end()) {
            break;
        }

        desire = parent[desire];
        states.insert(states.begin(), desire);
    }

    if (iterations) {
        *iterations = it;
    }


    return states;
}


