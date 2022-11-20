#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

double crossValidation(vector<int> currentSet, int featureToAdd){
    double accuracy;
    ifstream data ("CS170_SuperSmall_Data__43.txt");
    data.close();
    accuracy = rand();
    return accuracy;
}

void feature_search_demo(){
    unsigned int rowCounter = 0, columnCounter = 0, numFeatures = 0;
    int featureToAddAtCurrentLevel = -1;
    double bestAccuracy = 0.0, accuracy = 0.0;

    string line, featureValue, classValue;
    
    vector<int> currentSetOfFeatures;

    ifstream tempFile ("CS170_SuperSmall_Data__43.txt");
    getline(tempFile,line);
    stringstream ss(line);
    while (ss >> featureValue){
        numFeatures++;
    }
    numFeatures = numFeatures - 1;
    tempFile.close();

    ifstream file ("CS170_SuperSmall_Data__43.txt");

    for (unsigned int i = 1; i <= numFeatures; i++) {
        cout << line << endl;
        rowCounter++; //Current Row Number
        cout << "On the " << i << "th level of the search tree" << endl;
        featureToAddAtCurrentLevel = -1;
        bestAccuracy = 0;
        stringstream ss(line);
        ss >> classValue; //Take in Class Value

        while (ss >> featureValue){ //Every Value (Column) in Row Except Class Value
            columnCounter++;
            if (find(currentSetOfFeatures.begin(),currentSetOfFeatures.end(),columnCounter) == currentSetOfFeatures.end()){
                cout << "--Considering adding the " << columnCounter << " feature" << endl;
                accuracy = crossValidation(currentSetOfFeatures,columnCounter + 1);

                if (accuracy > bestAccuracy){
                    bestAccuracy  = accuracy;
                    featureToAddAtCurrentLevel = columnCounter;
                }
            }
        }
        columnCounter = 0;
        currentSetOfFeatures.push_back(featureToAddAtCurrentLevel);
        cout << "On level: " << i << " I added feature " << featureToAddAtCurrentLevel << " to current set" << endl;
    }

    file.close();
}

int main(){

    feature_search_demo();

    return 0;
}