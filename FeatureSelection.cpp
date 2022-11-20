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
    unsigned int counter = 0;
    string line;
    string featureValue;
    ifstream file ("CS170_SuperSmall_Data__43.txt");
    
    while (getline(file,line)) {
        counter++;
        cout << "On the " << counter << "th level of the search tree" << endl;
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