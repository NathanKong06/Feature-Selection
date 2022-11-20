#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

double crossValidation(vector<int> currentSet, int featureToAdd){\
    double accuracy;
    ifstream data ("CS170_Small_Data__43.txt");
    data.close();
    accuracy = rand();
    return accuracy;
}

int main(){
    string line;
    ifstream file ("CS170_Small_Data__43.txt");
    
    while (getline(file,line)){
        cout << line << endl;
    }

    file.close();

    return 0;
}