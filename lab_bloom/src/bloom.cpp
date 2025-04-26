/**
 * @file bloom.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

 #include "bloom.h"
 #include <algorithm>

BF::BF(uint64_t size, std::vector<hashFunction> hashList){

    // Your code here 

    // make vector to be this size;
    int s = size;
    for (int i = 0; i < s; i++) {
        bv.push_back(0);
    }

    hv = hashList;
}

BF::BF(const BF& other){
    // Your code here 

    for (unsigned int i = 0; i < other.bv.size(); i++) {
        bv.push_back(other.bv[i]);
    }

    for (unsigned int j = 0; j < other.hv.size(); j++) {
        hv.push_back(other.hv[j]);
    }
}

BF::~BF(){
    return;
}

void BF::add(const int & key){

    // Your code here 

    // has it and in every position, set it to one

    for (unsigned int i = 0; i < hv.size(); i++) {
        int idx = hv[i](key) % bv.size();

        bv[idx] = true;
    }

}

bool BF::contains(const int& key) const{
    // Your code here

    // all should be the ones or zeros

    bool in = true;
    for (unsigned int i = 0; i < hv.size(); i++) {
        int idx = hv[i](key) % bv.size();
        in = in && bv[idx];
    }

    return in;
}

void BF::bit_union(const BF& other){
    // Your code here 

    for (unsigned int i = 0; i < bv.size(); i++) {
        if (other.bv[i]) {
            bv[i] = true;
        }
    }
}

void BF::intersect(const BF& other){
    // Your code here 
    for (unsigned int i = 0; i < bv.size(); i++)  {
        if (bv[i] == true && other.bv[i] == true) {
            bv[i] = true;
        } else {
            bv[i] = false;
        }
    }
}

float measureFPR(std::vector<int> inList, uint64_t size, std::vector<hashFunction> hashList, unsigned max){
    // Your code here
    BF filter = BF(size, hashList);

    float fp = 0.0;
    float tn = 0.0;

    // push everything into filter
    for (int key : inList) {
        filter.add(key);
    }

    for (unsigned int i = 0; i < max; i++) {
        if (filter.contains(i) && (std::find(inList.begin(), inList.end(), i) == inList.end())) {
            fp++;
        } else if (!filter.contains(i)) {
            tn++;
        }
    }





    return fp / (fp + tn);

    // return 99.9;

}

bool getBitFromArray(std::vector<char> bv, int index){
    // Your code here

    int idx = index / 8;

    return getBitFromByte(bv[idx], index % 8);
}

bool getBitFromByte(char in, int index){
    // Your code here
    uint8_t input = static_cast<uint8_t>(in) << index;


    
    std::uint8_t tmp{ 0b10000000 };

    return (input & tmp) != 0;

}

