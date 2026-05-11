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
    HashingNode *next = nullptr;
};

/********************************************/
void PrintMenu();
std::string RemoveSpace(std::string target);
std::string RemoveDotTab(std::string target);
bool IsInt(std::string num);
void GetN(std::string temp , int &n);
bool SetInfo(std::vector<HashingNode> &HashingInfo , std::string &num);
bool ChangeFile(std::string filename , std::string outputName);
void ReadBin(std::string outputName , std::vector<HashingNode> &HashingInfo);

bool IsPrime(int n);
int BtPrime(int num);
/*********************************************/

class Hash { //以線性為基礎款
  protected:
    int tableSize;
    std::vector<HashingNode> hashingTable;
    int datasize;
    void WriteInfo(std::string outputName);
    int HashFunction(char key[]); //計算初始格
    virtual bool Collision(int index);//判斷有沒有撞 撞了是true
    virtual int Step(int n, char key[]) {//計算要加幾格 覆寫
        return n;
    }
    float CountExist(std::vector<HashingNode>& HashingInfo); //計算插入資料中存在部分平均搜尋次數
    
    
  public:
    Hash(int infoNum) {
        datasize = infoNum;
        tableSize = BtPrime(infoNum * 1.15);
        hashingTable.resize(tableSize);
    }

    void Insert(std::vector<HashingNode> HashingInfo);
};

class QuadraticHash : public Hash {
  private:
    float CountNotExist();//計算不存在的平均比較次數
    int Step(int n, char key[]) override { //覆寫線性的版本，此處是N平方
        return n * n % tableSize;
    }

  public:
    QuadraticHash(int infoNum) : Hash(infoNum) {}

    void Create(std::vector<HashingNode> HashingInfo);
    void WriteFile(std::string num);
};

class DoubleHash : public Hash {
  private:
    int Step(int n, char key[]) override;//覆寫線性的版本，最高步階 - ((學號每個數字對應的ASCII編碼相乘)除以最高步階取餘數)

  public:
    DoubleHash(int infoNum) : Hash(infoNum) {}

    void Create(std::vector<HashingNode> HashingInfo);
    void WriteFile(std::string num);
};

class SeperateChainingHash : public Hash {
  private:
    int Step(int n, char key[]) override {
        return 0;
    }
    bool Collision(int index) override {//永遠沒有碰撞的可能
        return false;
    }

  public:
    SeperateChainingHash(int infoNum) : Hash(infoNum) {}

    void Create(std::vector<HashingNode> HashingInfo);
    void WriteFile(std::string num);
};

int main() {
    std::string verb;
    std::vector<HashingNode> HashingInfo;

    std::string num; // 檔名,Ex.301
    while(true) {
        PrintMenu();
        std::cin >> verb;
        verb = RemoveSpace(verb);       
        if (verb == "0") break;
        else if (verb == "1") {
            HashingInfo.clear();
            if (!SetInfo(HashingInfo , num)) {//讀到0的情況要停止執行
                std::cout << std::endl;
                continue; 
            }

            QuadraticHash quadraticHash = QuadraticHash(HashingInfo.size());
            quadraticHash.Create(HashingInfo);
            quadraticHash.WriteFile(num);

        } else if (verb == "2") {
            if (HashingInfo.size() == 0) {
                std::cout << "### Command 1 first. ###\n\n";
                continue;
            }
            DoubleHash doubleHash = DoubleHash(HashingInfo.size());
            doubleHash.Create(HashingInfo);
            doubleHash.WriteFile(num);
        } 
        else if (verb == "3") {
            if (HashingInfo.size() == 0) {
                std::cout << "### Command 1 first. ###\n\n";
                continue;
            }
            SeperateChainingHash seperateChainingHash = SeperateChainingHash(HashingInfo.size());
            seperateChainingHash.Create(HashingInfo);
            seperateChainingHash.WriteFile(num);
        } /*else if (verb == "4") {

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
    std::cout << "************ Hash Table **********" << std::endl;
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

bool SetInfo(std::vector<HashingNode> &HashingInfo , std::string &num) {
    std::string fileName;
    std::string outputName;

    std::cout << "\nInput a file number ([0] Quit): ";
    std::cin >> num;
    num = RemoveSpace(num);
    fileName = "input" + num + ".txt";
    outputName = "input" + num + ".bin";
    if (fileName == "input0.txt") return false;

    std::ifstream fin(outputName);
    if (fin.is_open()) {
        ReadBin(outputName , HashingInfo); //讀二進位檔的
        return true;
    }
    // 沒有二進位檔
    if (!ChangeFile(fileName , outputName)) return false;

    ReadBin(outputName , HashingInfo);
    return true;
}

bool ChangeFile(std::string filename , std::string outputName) {
    std::cout << "\n### " << outputName << " does not exist! ###\n";
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cout << "\n### " << filename << " does not exist! ###\n";
        return false;
    }// 連txt都沒有

    std::ofstream fout(outputName, std::ios::binary);

    Node info;
    std::string temp; //暫存

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
    while (fin.read((char*)&node, sizeof(Node))) {
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
    for (int i = 3; i <= sqrt(n) + 1 ; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

int BtPrime(int num) {
    num = num + 1; // 大於
    while(true) {
        if (IsPrime(num)) {
            return num;
        }else {
            num++;
        }
    }
}

//---------------------------Hash-----------------------------
void Hash::WriteInfo(std::string outputName) {
    std::ofstream fout(outputName, std::ios::app); // 接在後面
    for (int i = 0 ; i < tableSize ; i++) {
        fout << "[" << std::setw(3) << i << "]";
        if (!hashingTable[i].used) {
            fout << " " << std::endl;
            continue;
        }
        HashingNode *temp = &hashingTable[i];
        do {
            fout << std::setw(11) << temp->hvalue << ", " << std::setw(10) << temp->sid << ", " <<
                std::setw(10) << temp->sname << ", " << std::setw(10) << temp->average << std::endl;
            temp = temp->next;
        }
        while (temp != nullptr);
    }
    fout.close();
}

int Hash::HashFunction(char key[]) { //計算初始格
    int i = 0;
    int index = 1;
    while (key[i] != '\0') {
        index = index * (key[i] - '0' + 48) % tableSize;
        i++;
    }
    return index;
}

bool Hash::Collision(int index) {//判斷有沒有撞 撞了是true
    if (hashingTable[index].used == true) return true;
    return false;
}

//public
void Hash::Insert(std::vector<HashingNode> HashingInfo) {
    for (int i = 0 ; i < HashingInfo.size() ; i++) {
        HashingNode toInsert = HashingInfo[i];
        int index = HashFunction(toInsert.sid);
        toInsert.hvalue = index;

        bool find = true;
        int n = 1;
        while (Collision(index)) { //碰撞找下一個
            //無窮迴圈
            if (n == tableSize) {
                std::cout << "### Failed at [" << i <<"]###\n";
                find = false;
                break;
            }
            int step = Step(n, toInsert.sid);
            index = toInsert.hvalue + step;
            index %= tableSize;
            n++;
        }
        if (!find) continue; //找不到位置忽略
        if (hashingTable[index].used == true) {
            HashingNode *newNode = new HashingNode;//建一個節點
            *newNode = toInsert;
            newNode->used = true;
            newNode->next = nullptr;

            HashingNode *temp = &hashingTable[index];
            while(temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        } else {
            hashingTable[index] = toInsert;
            hashingTable[index].used = true;
            hashingTable[index].next = nullptr;
        }
    }
}

float Hash::CountExist(std::vector<HashingNode>& HashingInfo) {
    int total = 0;
    int exist = 0;

    for (int i = 0; i < HashingInfo.size(); i++) {
        HashingNode target = HashingInfo[i];

        int initial = HashFunction(target.sid);
        int index = initial;
        int compare = 1;

        int n = 1;

        while (hashingTable[index].used &&strcmp(hashingTable[index].sid, target.sid) != 0) {
            if (n > tableSize) break; // 找太多次
            index = (initial + Step(n, target.sid)) % tableSize;
            n++;
            compare++;
        }

        if (hashingTable[index].used && strcmp(hashingTable[index].sid, target.sid) == 0) {
            total += compare;
            exist++;
        }
    }

    return (float)total / exist;
}


//------------------------QuadraticHash--------------------------
float QuadraticHash::CountNotExist() {
    int total = 0;
    for (int i = 0; i < tableSize; i++) {//看每一格開始找到確定能停下來(沒使用)
        int index = i;

        if (!hashingTable[index].used) {
            continue;
        }

        int n = 1;
        int compare = 0;
        while (hashingTable[index].used) {
            index = (i + Step(n, hashingTable[i].sid)) % tableSize;
            compare++;
            n++;

            if (n > tableSize) break;
        }
        total += compare;
    }
    return (float)total / tableSize;
}

void QuadraticHash::Create(std::vector<HashingNode> HashingInfo) {
    std::cout << "\nHash table has been successfully created by Quadratic probing" << std::endl;
    Insert(HashingInfo);
    std::cout << "unsuccessful search: " << std::fixed << std::setprecision(4) << CountNotExist() <<
                " comparisons on average" << std::endl;
    std::cout << "successful search: " << std::fixed << std::setprecision(4) << CountExist(HashingInfo) <<
                " comparisons on average" << std::endl;
}

void QuadraticHash::WriteFile(std::string num) {
    std::string output = "quadratic" + num + ".txt";
    std::ofstream fout1(output);
    fout1 << " --- Hash table created by Quadratic probing ---" << std::endl;
    fout1.close();
    WriteInfo(output);
    std::ofstream fout2(output, std::ios::app);
    fout2 << " ----------------------------------------------------- " << std::endl;
    fout2.close();
}

//------------------------DoubleHash--------------------------
int DoubleHash::Step(int n, char key[]) { //覆寫線性的版本，最高步階 - ((學號每個數字對應的ASCII編碼相乘)除以最高步階取餘數)
    int max_step = BtPrime(datasize / 5); //最高步階
    int i = 0;
    int step = 1;
    while (key[i] != '\0') {
        step = step * (key[i] - '0' + 48) % max_step;
        i++;
    }
    return n * (max_step - step);
}

void DoubleHash::Create(std::vector<HashingNode> HashingInfo) {
    std::cout << "\nHash table has been successfully created by Double hashing   " << std::endl;
    Insert(HashingInfo);

    std::cout << "successful search: " << std::fixed << std::setprecision(4) << CountExist(HashingInfo) <<
                " comparisons on average" << std::endl;
}

void DoubleHash::WriteFile(std::string num) {
    std::string output = "double" + num + ".txt";
    std::ofstream fout1(output);
    fout1 << " --- Hash table created by Double hashing    ---" << std::endl;
    fout1.close();
    WriteInfo(output);
    std::ofstream fout2(output, std::ios::app);
    fout2 << " ----------------------------------------------------- " << std::endl;
    fout2.close();
}


//------------------------SeperateChainingHash --------------------------

void SeperateChainingHash::Create(std::vector<HashingNode> HashingInfo) {
    std::cout << "\nHash table has been successfully created by Seperate Chaining hashing   " << std::endl;
    Insert(HashingInfo);
}

void SeperateChainingHash::WriteFile(std::string num) {
    std::string output = "SeperateChaining" + num + ".txt";
    std::ofstream fout1(output);
    fout1 << " --- Hash table created by Seperate Chaining hashing    ---" << std::endl;
    fout1.close();
    WriteInfo(output);
    std::ofstream fout2(output, std::ios::app);
    fout2 << " ----------------------------------------------------- " << std::endl;
    fout2.close();
}
