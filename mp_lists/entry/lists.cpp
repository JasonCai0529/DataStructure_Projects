#include "List.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
  List<int> list = List<int>();
  // insert back -> insert front
  for (unsigned int i = 30; i < 35; i+=2) {
    list.insertBack(i);
  }
  list.print();
  std::cout << std::endl;

  List<int> second_list = List<int>();
  for (unsigned int i = 1 ; i < 10; i+=2) {
    second_list.insertBack(i);
  }
  second_list.print();
  std::cout << std::endl;
  list.print();

  second_list.mergeWith(list);
  std::cout << "LIST AFTER MERGE is ";
  second_list.print();
  std::cout << std::endl;
  
  return 0;
}
