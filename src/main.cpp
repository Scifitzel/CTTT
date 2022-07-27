#include "CTTT.cpp"

int main() {
    std::string fileName{""};
    std::ifstream into{""};
    std::ofstream outo{""};

    std::cout << "Enter 'Compress' or 'Decompress'" << "\n";
    while (true) {
        std::cin >> fileName;
      if (fileName == "Compress" || fileName == "Decompress") {
        break;
      }
      std::cout << "Couldn't recognize input. Please enter 'Compress' or 'Decompress'" << "\n";
    }

     if (fileName == "Compress") {
        std::string outputFileName{""};
            std::cout << "Enter name of file to compress.\n";
            std::getline(std::cin,fileName);
            std::getline(std::cin,fileName);
            into.open(fileName);
            int position = fileName.length()-1;
            while (position >= 0) { 
                if (fileName[position] == '.') {
                    break;
                }
                position--;
            }
            outputFileName = fileName.substr(0,position)+".cttt";
            
            // makes sure we don't overwrite another file
            std::ifstream testingFile{outputFileName};
            if (testingFile.good()) {
               outputFileName = "1"+outputFileName; 
            } 
                outo.open(outputFileName);
            CTTT::encode(into,outo,fileName,outputFileName);
            std::cout << "Finished compressing file. Press any key and then enter to exit.\n";
        } else if (fileName == "Decompress") {
            std::cout << "Enter name of file to decompress.\n";
            std::getline(std::cin,fileName);
            while (true) {
                std::getline(std::cin,fileName);
                if (fileName[fileName.length()-4] == 'c' && fileName[fileName.length()-3] == 't'  && fileName[fileName.length()-2] == 't' && fileName[fileName.length()-1] == 't') {
                    break;
                }
                std::cout << "Incorrect file extension. Please enter '.cttt' file." << "\n";
            }
            into.open(fileName);
            CTTT::decode(into,outo);
            std::cout << "Finished decompressing file. Press any key and then enter to exit.\n";
        } 
    std::cin >> fileName;

    into.close();
    outo.close();
    return 0;
}