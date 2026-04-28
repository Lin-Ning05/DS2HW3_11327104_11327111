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

struct HashingNode {
    int hvalue;
	char sid[10] = {'\0'};
	char sname[10];
	float average;
    bool used = false;
};

class Hash {
  private:
    int tableSize;
    std::vector<HashingNode> hashingTable;

  public:
    Hash(int infoNum) {
        tableSize = BtPrime(infoNum * 1.5);
        hashingTable.resize(tableSize);
    }

    void WriteFile();
    int HashFunction(char key[]) { //計算初始格
        int i = 0;
        int index = 1;
        while (key[i] != '\0') {
            index = index * (key[i] - '0' + 48);
            index %= tableSize;
        }
        return index;
    }
    bool Collision(int index) {//判斷有沒有撞 撞了是true
        if (hashingTable[index].used == true) return true;
        return false;
    }
    int Step(int n) {//計算要加幾格 覆寫
        return 1;
    } 
    float CountExist();

    void Insert(std::vector<HashingNode> HashingInfo) {
        for (int i = 0 ; i < HashingInfo.size() ; i++) {
            HashingNode toInsert = HashingInfo[i];
            int index = HashFunction(toInsert.sid);
            toInsert.hvalue = index;

            bool find = true;
            while (Collision(index)) {
                int step = Step(0);
                index = toInsert.hvalue + step;
                index %= tableSize;
                //無窮迴圈
                if (index == toInsert.hvalue) {
                    find = false;
                    break;
                }
            }
            if (!find) continue;
            hashingTable[index] = toInsert;
        }
    }
};

/********************************************/
void PrintMenu();
std::string RemoveSpace(std::string target);
std::string RemoveDotTab(std::string target);
bool IsInt(std::string num);
void GetN(std::string temp , int &n);
bool SetInfo(std::vector<HashingNode> &HashingInfo);
std::string GetFileName();
bool ChangeFile(std::string filename , std::string outputName);
void ReadBin(std::string outputName , std::vector<HashingNode> &HashingInfo);

bool IsPrime(int n);
int BtPrime(int num);
/*********************************************/

int main() {
    std::string verb;
    std::vector<HashingNode> HashingInfo;

    while(true) {
        PrintMenu();
        std::cin >> verb;
        verb = RemoveSpace(verb);
        
        if (verb == "0") break;
        else if (verb == "1") {
            HashingInfo.clear();
            if (!SetInfo(HashingInfo)) {//讀到0的情況要停止執行
                std::cout << std::endl;
                continue; 
            }



        } else if (verb == "2") {
            if (HashingInfo.size() == 0) {
                std::cout << "### Command 1 first. ###\n\n";
                continue;
            }

            int tableSize = BtPrime(HashingInfo.size() * 1.5);
            std::vector<HashingNode> doubleTable(tableSize);
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
    std::cout << std::endl;
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

bool SetInfo(std::vector<HashingNode> &HashingInfo) {
    std::string fileName;
    std::string outputName;

    std::cout << "\nInput a file number ([0] Quit): ";
    std::string num;
    std::cin >> num;
    num = RemoveSpace(num);
    fileName = "input" + num + ".txt";
    outputName = "input" + num + ".bin";
    if (fileName == "input0.txt") return false;

    std::ifstream fin(outputName);
    if (fin.is_open()) {
        ReadBin(outputName , HashingInfo); //讀二進為檔的
        return true;
    }

    if (!ChangeFile(fileName , outputName)) return false;

    ReadBin(outputName , HashingInfo);
    return true;
}

bool ChangeFile(std::string filename , std::string outputName) {
    std::cout << "### " << outputName << " does not exist! ###\n";
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

void ReadBin(std::string outputName , std::vector<HashingNode> &HashingInfo) {
    HashingNode hashNode;
    std::ifstream fin(outputName, std::ios::binary);

    Node node;
    while (fin.read((char*)&node, sizeof(HashingNode))) {
        hashNode.hvalue = -1;
        hashNode.average = node.average;
        strcpy(hashNode.sid , node.sid);
        strcpy(hashNode.sname , node.sname);
        HashingInfo.push_back(hashNode);
    }
    return;
}

bool IsPrime(int n) {
    if (n <= 1) return false; // 0 和 1 不是質數
    if (n == 2) return true;  // 2 是質數
    if (n % 2 == 0) return false; // 排除偶數

    // 只需要檢查到平方根，且步長為2（只檢查奇數）
    for (int i = 3; i <= sqrt(n); i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

int BtPrime(int num) {
    while(true) {
        if (IsPrime(num)) {
            return num;
        }else {
            num++;
        }
    }
}

