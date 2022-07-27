
#include "CTTT.h"


bool CTTT::shareCharacter(const std::string& firstString, const std::string& secondString) {
    for (int a = 0; a < firstString.length(); a++) {
        for (int i = 0; i < secondString.length(); i++) {
            if (firstString[a] == secondString[i]) {
                return true;
            }
        }
    }
    return false;
}

void CTTT::incrementString(std::string& string) {
  if (string[1] == 127) {
    string[0]++;
    if (string[0] == 10) {
        string[0]++;
    }
    string[1] = -128; 
  } else {
    string[1]++;
    if (string[1] == 10) {
        string[1]++;
    }
  }
}

bool CTTT::shareMiddle(std::vector<std::pair<const std::string,int>> &stringVector, std::pair<const std::string,int>& string, bool lengthIsThree) {
    // these are to make sure the string doesn't accidently get compressed with another string
  if (lengthIsThree) {  
    for (int i = 0; i < stringVector.size(); i+=2) {
        if (stringVector[i+1].first[0] == string.first[2] || stringVector[i+1].first[1] == string.first[0] ||  stringVector[i].first[0] == string.first[2] || stringVector[i].first[2] == string.first[0] || stringVector[i].first.substr(0,2) == string.first.substr(1,2) || stringVector[i].first.substr(1,2) == string.first.substr(0,2) ||  stringVector[i].first == string.first) {
            return true;
        }
    }
  } else {
        for (int i = 1; i < stringVector.size(); i+=2) {
        if (stringVector[i-1].first[2] == string.first[0] || stringVector[i-1].first[0] == string.first[1] || stringVector[i].first[1] == string.first[0] || stringVector[i].first[0] == string.first[1]  || stringVector[i].first == string.first || stringVector[i-1].first.find(string.first) != -1) {
            return true;
        }
    }
  } 
    return false;
}


void CTTT::encode(std::ifstream& into, std::ofstream& outo, std::string& intoFileName, std::string& outoFileName) {

    std::map <std::string,int> threeLetterStrings;
    std::map <std::string,int> twoLetterStrings;
    std::string string{""};
    std::string substring{""};
    int howManyTimes = 0;
// fills out ""twoLetterString" with every single 2 character string possible
    string.push_back(-128);
    string.push_back(-128);
    while (string[0] != 127 || string[1] != 127) { 
        twoLetterStrings[string] = 0;
        incrementString(string);
    }

    while(std::getline(into,string)) {
        substring.clear();
        for (int i = 0; i < string.length(); i++) {
            if (i == 0) {
                substring.push_back(string[i]);
                i++;
            } 
            substring.push_back(string[i]);
            twoLetterStrings[substring]++;
            substring.erase(0,1);
        }
        howManyTimes++;
        if (howManyTimes > 100000) { 
            break;
        }
    } 

    into.close();
    into.open(intoFileName);
            howManyTimes = 0;

    while(std::getline(into,string)) {
        substring.clear();
        for (int i = 0; i < string.length(); i++) {
            if (i == 0) {
                substring.push_back(string[i]);
                i++;
                substring.push_back(string[i]);
                i++;
            } 
            substring.push_back(string[i]);
            threeLetterStrings[substring]++;
            substring.erase(0,1);
        }
        howManyTimes++;
        if (howManyTimes > 1000) { 
            break;
        }
    } 
    std::vector <std::pair<const std::string, int>> compressableString{};
    std::pair <std::string,int> mostCommontThreeString {"",0};
    std::pair <std::string,int> leastCommonTwoString {"",10000000};

while (true){
   mostCommontThreeString = std::pair<std::string, int>{"",0};
   leastCommonTwoString = std::pair<std::string, int>{"",10000000};
    for (auto iter: threeLetterStrings) {
        if (mostCommontThreeString.second < iter.second && !shareMiddle(compressableString,iter,true) && iter.first[2] != 'a' && iter.first[0] != 'b' ){
           if ((iter.first[0] != iter.first[1] || iter.first[1] != iter.first[2]) && iter.first[0] != iter.first[2])  {
            mostCommontThreeString = iter;
           }
        }
    }

    for (auto iter: twoLetterStrings) {
        if (leastCommonTwoString.second > iter.second  && !shareCharacter(mostCommontThreeString.first,iter.first) && !shareMiddle(compressableString,iter,false) && iter.first[1] != 'a' && iter.first[0] != 'b') {
          if (iter.first[0] != iter.first[1]) {  
            leastCommonTwoString = iter;
          }
        }
    }

    if (leastCommonTwoString.second+6 >= mostCommontThreeString.second) {
        break;
    }
    compressableString.push_back(mostCommontThreeString);
    compressableString.push_back(leastCommonTwoString);
}
    threeLetterStrings.clear();
    twoLetterStrings.clear();

    if (compressableString.empty()) {
        std::cout << "Didn't find any characters to compress. Aborting compression process.\n";
        std::remove(outoFileName.c_str());
        return;
    }
    into.close();
    into.open(intoFileName);

     int position = intoFileName.length()-1;
    while (position >= 0) {
        if (intoFileName[position] == '/') { // on Windows, replace '/' with '\'
            break;
        }
            position--;
        }  
     outo << intoFileName.substr(0,position)  << std::endl;  

    for (int i = 0; i < compressableString.size(); i+=2) {
        outo << compressableString[i].first << compressableString[i+1].first;
    }
    outo << std::endl;

    int stringPosition = -1;
    std::string tempString{"ab"};
    int temp = 0;
    while (std::getline(into,string)) {
       for (int i = 0; i < compressableString.size(); i+=2) {
        tempString = "ab";
        while (string.find(tempString) != -1 || tempString[0] == tempString[1]) {
        incrementString(tempString);
        }

        stringPosition = -1;
        // we replace the two letter strings with a temporary strings so we don't compress them with the three strings during compression or decompression
        stringPosition = string.find(compressableString[i+1].first);
        while (stringPosition != -1) {
            string.replace(stringPosition,2,tempString);
            stringPosition = string.find(compressableString[i+1].first);
        }

        stringPosition = -1;
        stringPosition = string.find(compressableString[i].first);
        while (stringPosition != -1) {
            string.replace(stringPosition,3,compressableString[i+1].first);
            stringPosition = string.find(compressableString[i].first);
        }

        stringPosition = -1;
        stringPosition = string.find(tempString);
        while (stringPosition != -1) {
            string.replace(stringPosition,2,compressableString[i].first);
            stringPosition = string.find(tempString);
        }
       }

        outo << string << std::endl;

        temp++;
        if (temp == 8190) {
            std::cout << "";
        }

    }
}

void CTTT::decode(std::ifstream& into, std::ofstream& outo){
    std::string string{""};
    std::vector <std::string> compressableString;
    std::getline(into,string);
    outo.close();
    std::ifstream testingFile{string};
    if (testingFile.good()) {
        string = "1"+string;
    }
    outo.open(string);
    std::getline(into,string);

    for (int i = 0; i < string.length(); i+=5) {
        compressableString.push_back(string.substr(i,3));
        compressableString.push_back(string.substr(i+3,2));
    }

    int stringPosition = -1;
    std::string tempString{"ab"};
    int temp = 0;

    while (std::getline(into,string)) {
       for  (int i = 0; i < compressableString.size(); i+=2) {
        tempString = "ab";
        while (string.find(tempString) != -1 || tempString[0] == tempString[1] || compressableString[i].find(tempString) != -1 || compressableString[i+1] == tempString) {
            incrementString(tempString);
        }

        stringPosition = -1;
        stringPosition = string.find(compressableString[i+1]);
        while (stringPosition != -1) {
            string.replace(stringPosition,2,tempString);
            stringPosition = string.find(compressableString[i+1]);
        }

        stringPosition = -1;
        stringPosition = string.find(compressableString[i]);
        while (stringPosition != -1) {
            string.replace(stringPosition,3,compressableString[i+1]);
            stringPosition = string.find(compressableString[i]);
        }
        stringPosition = -1;
        stringPosition = string.find(tempString);
        while (stringPosition != -1) {
            string.replace(stringPosition,2,compressableString[i]);
            stringPosition = string.find(tempString);
        }
       }  
        outo << string << std::endl;

        temp++;
        if (temp == 4511) {
            std::cout << "";
        }

    }

}




