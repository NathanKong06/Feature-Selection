#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

double crossValidation(vector<int> currentSet, int featureToAdd){\
    double accuracy;
    ifstream data ("CS170_SuperSmall_Data__43.txt");
    data.close();
    accuracy = rand();
    return accuracy;
}

void feature_search_demo(){
    unsigned int rowCounter = 0;
    unsigned int columnCounter = 0;
    string line;
    string featureValue;
    string classValue;
    ifstream file ("CS170_SuperSmall_Data__43.txt");
    
    while (getline(file,line)) {
        rowCounter++;
        cout << "On the " << rowCounter << "th level of the search tree" << endl;
        stringstream ss(line);
        ss >> classValue;
        while (ss >> featureValue){
            columnCounter++;
            cout << "--Considering adding the " << columnCounter << " feature" << endl;
        }
        columnCounter = 0;
    }

    // while (ss >> featureValue){
    //     cout << stod(featureValue) << " " ;
    // }

    file.close();
}

int main(){

    feature_search_demo();

    return 0;
}