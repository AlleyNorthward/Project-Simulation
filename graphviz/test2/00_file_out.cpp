#include <iostream>
#include <fstream>

int main(){
  std::ofstream outFile("example.txt", std::ios::app);

  if(!outFile) return 1;

  outFile << "Hello, world!" << std::endl;
  outFile << "This is a test file." << std::endl;
}










