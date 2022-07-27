#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

class CTTT {
private:    
    static bool shareCharacter(const std::string& firstString, const std::string& secondString);
    static void incrementString(std::string& string);
    static bool shareMiddle(std::vector<std::pair<const std::string,int>> &stringVector, std::pair<const std::string,int>& string, bool lengthIsThree);
public:  
    static void encode(std::ifstream& into, std::ofstream& outo, std::string& intoFileName, std::string& outoFileName);
    static void decode(std::ifstream& into, std::ofstream& outo);
};