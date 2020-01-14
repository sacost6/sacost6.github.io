
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

/**
 This file contains some toy code which illustrate example
 usage of the standard template library unordered_map class
 among other things.  Some other C++ concepts illustrated
 include:
 
 the std::sort function
 command-line arguments
 opening and reading from files
 the "foreach" construct to iterate over elements
 in an STL "container"
 
 The program itself reads a text file and builds a
 "frequency-count" data structure using an unordered_map.
 
 For each ditsinct string in the input file, the map
 keeps track of the number of time that string appears
 in the file.
 **/


/*
 *  this function rearranged the characters in a string
 *    so that they are sorted (according to their ASCII
 *    value).
 *
 *  Resource:  https://en.cppreference.com/w/cpp/algorithm/sort
 *
 *  Note:  this function is not actually used the the program
 *    in this file.  But you might find it useful... :)
 */
void ssort(std::string &s) {
  
  /*
   strings are "iterable" objects and so have the begin()
   and end() functions.  These functions (in a pretty
   abstract way) return "iterators" which specify the
   "beginning" and "end" of the associated object).
   
   Thus, this call is asking the sort function to sort
   the entire string s.
   */
  std::sort(s.begin(), s.end());
}


/*
 *   usage:   ./freq <filename>
 *
 *   example:  ./freq words.txt
 *
 */
int main(int argc, char *argv[]){
  std::string largestClass;
  int largestC = 0;
  int count = -1;
  int classCount = 0;
  
  /*
   the variable word2freq is an unordered_map
   from strings to integers.
   */
  std::unordered_map<std::string, std::vector<std::string>> word2freq;
  std::ifstream file;
  std::string word;
  
  if(argc != 2) {
    std::cout << "usage:  ./freq <filename>\n";
    std::cout << "goodbye\n";
    return 1;
  }
  
  /*
   *  argv[1] is a C-string which is the filname specified
   *    by the user.  Let's try to open it.
   */
  
  file.open(argv[1], std::ios::in);
  
  if(!file.is_open()){
    std::cout << "Error: could not open file '" <<
    argv[1] << "'\n";
    std::cout << "goodbye\n";
    return 1;
  }
  
  std::cout << "reading input file...\n";
  
  while(file >> word) {
    file >> word;
    std::string temp = word;
    ssort(temp);
    count++;
    if(word2freq.count(temp) == 0) {
      std::vector<std::string> tempVec;
      word2freq[temp] = tempVec;
      word2freq[temp].push_back(word);
      classCount++;
    }
    else {
      word2freq[temp].push_back(word);
      if(word2freq[temp].size() > largestC) {
        largestClass = temp;
        largestC = word2freq[temp].size();
      }
    }
  }
  
  std::cout <<
  "enter a word and I will tell you what I know about it\n";
  std::cout <<
  "  when you are done, type ctrl-d (ctrl-z on windows)\n\n";
  std::cout << "> ";
  
  
  while(std::cin >> word) {
    ssort(word);
    if(word2freq.count(word)==0) {
      std::cout << "sorry, '" << word <<
      "' does not appear in the input file '" <<
      argv[1] << "'\n";
    }
    else {
      ssort(word);
      std::cout << "The following words have been found: \n";
      for(int i = 0; i < word2freq[word].size(); i++) {
        std::cout << word2freq[word][i] << std::endl;
      }
    }
  }
  // user has quit.  Let's dump the map contents
  std::cout << "REPORT: \n\n";
  std::cout << "Number of words                   : " << count << "\n";
  std::cout << "The largest class was             : " << largestClass;
  std::cout << "\nwith                              : " << largestC << " words!\n";
  std::cout << "Number of classes are             : " << classCount << "\n";
  std::cout << "Members of the largest class are: \n\n";
  for(int i = 0; i < word2freq[largestClass].size(); i++) {
    std::cout << word2freq[largestClass][i] << "\n";
  }
  return 0;
}

