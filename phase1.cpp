#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

const int MEMORY_SIZE = 100;
const int WORD_SIZE = 4;
vector<vector<char>> M(MEMORY_SIZE, vector<char>(WORD_SIZE, '-')); // Memory
int IC;
vector<char> IR(WORD_SIZE, '-'); // Instruction Register
bool C;
vector<char> R(WORD_SIZE, '-'); // General Purpose Register
int indexForM;
string buffer;
bool flag1;
bool flag2;
int SI;
string opcode;
int operand;

void INIT() {
    // Initialize memory, instruction register, and other variables
    for (auto &row : M)
        fill(row.begin(), row.end(), '-');

    fill(IR.begin(), IR.end(), '-');
    fill(R.begin(), R.end(), '-');

    IC = 0;
    C = false;
    buffer.clear();

    SI = -1;
    flag1 = false;
    flag2 = false;
    indexForM = 0;
}

void READ() {
    IR[3] = '0';
    getline(cin, buffer);
    const char *array = buffer.c_str();
    int indexForArray = 0;
    bool flag = false;
    int startingAddress = stoi(string(1, IR[2]) + string(1, IR[3]));
    int endingAddress = startingAddress + 10;

    for (int i = startingAddress; i < endingAddress; ++i) {
        for (int j = 0; j < WORD_SIZE; ++j) {
            M[i][j] = array[indexForArray];
            indexForArray++;
            if (indexForArray >= buffer.length()) {
                flag = true;
                break;
            }
        }
        if (flag)
            break;
    }
}

void WRITE() {
    IR[3] = '0';
    stringstream ss;
    int startingAddress = stoi(string(1, IR[2]) + string(1, IR[3]));
    int endingAddress = startingAddress + 10;

    for (int i = startingAddress; i < endingAddress; ++i) {
        for (int j = 0; j < WORD_SIZE; ++j) {
            if (M[i][j] == '-')
                ss << " ";
            else
                ss << M[i][j];
        }
    }
    ss << "\n";
    cout << ss.str();
}

void TERMINATE() {
    cout << "\n\n\n";
    for (int k = 0; k < MEMORY_SIZE; ++k) {
        cout << setw(2) << k << ": ";
        for (const auto &ch : M[k])
            cout << ch;
        cout << endl;
    }
    cout << string(83, '-') << "Job Over" << string(83, '-') << endl;

    INIT();
    getline(cin, buffer);
}

void MOS() {
    switch (SI) {
    case 1:
        READ();
        break;
    case 2:
        WRITE();
        break;
    case 3:
        TERMINATE();
        break;
    }
}

void LOAD() {
    while (getline(cin, buffer)) {
        if (buffer.length() >= 4) {
            string first4CharOfInputLine = buffer.substr(0, 4);
            if (first4CharOfInputLine == "$AMJ") {
                flag1 = true;
            } else if (first4CharOfInputLine == "$DTA") {
                flag1 = true;
            }
            if (!flag1) {
                const char *arrayOfBuffer = buffer.c_str();
                int indexForArrayOfBuffer = 0;
                int i = indexForM, j;
                while (true) {
                    for (j = 0; j < WORD_SIZE; ++j) {
                        M[i][j] = arrayOfBuffer[indexForArrayOfBuffer];
                        indexForArrayOfBuffer++;
                        if (indexForArrayOfBuffer >= buffer.length()) {
                            flag2 = true;
                            break;
                        }
                    }
                    if (flag2)
                        break;
                    i++;
                }

                i++;
                indexForM = i;

                if (indexForM == MEMORY_SIZE) {
                    cerr << "No space, out of memory\nExiting......" << endl;
                    exit(1);
                }
            }
            flag2 = false;
            flag1 = false;
        }
    }
}

int main() {
    // Static input simulation
    vector<string> inputLines = {
        "$AMJ",
        "Some data line that is more than four characters long",
        "$DTA",
        "Another line with some data",
        "Last line of input"
    };

    stringstream inputStream;
    for (const auto &line : inputLines)
        inputStream << line << "\n";

    cin.rdbuf(inputStream.rdbuf()); // Redirect cin to use the input simulation

    INIT();
    LOAD();

    // Normally this would be the place where you can run MOS or other operations
    // Example: MOS(); based on your specific logic
    TERMINATE(); // Ending the process as an example

    return 0;
}
