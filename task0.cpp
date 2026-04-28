//11327104林采寧 11327111林方晴

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <cstring>

struct Node {
	char sid[10];
	char sname[10];
	unsigned char score[6];
	float average;
};


/********************************************/
void PrintMenu();
std::string RemoveSpace(std::string target);
std::string RemoveDotTab(std::string target);
bool IsInt(std::string num);
void GetN(std::string temp , int &n);
bool SetInfo();
std::string GetFileName();
bool LoadFile(std::string filename , std::string outputName);
/*********************************************/

int main() {
    std::string verb;

    while(true) {
        PrintMenu();
        std::cin >> verb;
        verb = RemoveSpace(verb);
        
        if (verb == "0") break;
        else if (verb == "1") {
            if (!SetInfo()) {//讀到0的情況要停止執行
                std::cout << std::endl;
                continue; 
            }


        } else if (verb == "2") {

        } 
        /*else if (verb == "3") {
            
        } else if (verb == "4") {

        }
        */
        else {
            std::cout << "\nCommand does not exist!\n\n";
        }
    }

    return 0;
}

/**********************************************************************/

void PrintMenu() {
    std::cout << "* Data Structures and Algorithms *" << std::endl;
    std::cout << "****** Balanced Search Tree ******" << std::endl;
    std::cout << "* 0. QUIT                        *" << std::endl;
    std::cout << "* 1. Quadratic probing           *" << std::endl;
    std::cout << "* 2. Double hashing              *" << std::endl;
    //std::cout << "* 3. Build a min-max heap        *" << std::endl;
    //std::cout << "* 4: Top-K max from min-max heap *" << std::endl;
    std::cout << "**********************************" << std::endl;
    std::cout << "Input a choice(0, 1, 2): ";
    return;
}

bool IsInt(std::string num) {
    if(num.size() == 0) {
        return false;
    }
    for (int i = 0; i < num.size(); i++) {
        if (i == 0 && (num[i] == '+' || num[i] == '-')) {
        continue;
    }
    if(num[i] > '9' || num[i] < '0') {
        return false;
    }
}
return true;
}

std::string RemoveSpace(std::string target) {
    std::string to_return;
    for (int i = 0; i < target.size() ;i++) {
        if (target[i] != ' ' && target[i] != '\t' && target[i] != '\n' && target[i] != '\r') {
            to_return = to_return + target[i];
        }
    }
    return to_return;
}

std::string RemoveDotTab(std::string target) {
    std::string to_return;
    for (int i = 0 ; i < target.size() ; i++) {
        if (target[i] == '.' || target[i] == '\t' || target[i] == ' ' || (target[i] == '\"' && i != 0)) {
            break;
        }
        if (target[i] != ',' && target[i] != '\"')
            to_return = to_return + target[i];
    }
    return to_return;
}

void GetN(std::string temp , int &n) {
    temp = RemoveDotTab(temp);
    temp = RemoveSpace(temp);
    if (IsInt(temp)) {
        n = stoi(temp);
    }
}

bool SetInfo() {
    std::string fileName;
    std::string outputName;
    do {
        std::cout << "\nInput a file number ([0] Quit): ";
        std::string num;
        std::cin >> num;
        num = RemoveSpace(num);
        fileName = "input" + num + ".txt";
        outputName = "input" + num + ".bin";
        if (fileName == "input0.txt") return false;
    }
    while (!LoadFile(fileName , outputName));
    return true;
}

bool LoadFile(std::string filename , std::string outputName) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cout << "\n### " << filename << " does not exist! ###\n";
        return false;
    }

    std::ofstream fout(outputName, std::ios::binary);

    Node info;
    std::string temp; //站存

    while (getline(fin , temp , '\t')) {

        memset(&info, 0, sizeof(info));

        strcpy(info.sid, temp.c_str());

        getline(fin , temp , '\t');
        strcpy(info.sname, temp.c_str());

        for (int i = 0; i < 6; i++) {
            getline(fin, temp, '\t');
            temp = RemoveSpace(temp);
            info.score[i] = stoi(temp);
        }

        getline(fin, temp);
        temp = RemoveSpace(temp);
        info.average = stof(temp);

        fout.write((char*)&info, sizeof(Node));
    }
    fin.close();
    fout.close();
    return true;
}

