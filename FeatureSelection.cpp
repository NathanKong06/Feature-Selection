#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>
using namespace std;

vector<double> getClassLabels(){
    string line, classLabel;
    vector<double> classLabels;
    ifstream data ("CS170_SuperSmall_Data__43.txt");
    while (getline(data,line)){
        stringstream ss(line);
        ss >> classLabel;
        classLabels.push_back(stod(classLabel));
    }
    data.close();
    return classLabels;
}

double crossValidation(vector<int> currentSet, int featureToAdd, int numRows){
    double accuracy = 0.0, distance = 0.0;

    unsigned int rowValue = 0, secondRowValue = 0, nearestNeighborDistance, nearestNeighborLocation, nearestNeighborLabel;

    string line, classLabelToClassify, featureValue;

    vector<double> objectToClassify;
    vector<double> classLabels = getClassLabels();

    ifstream data ("CS170_SuperSmall_Data__43.txt");

    while (getline(data,line)){
        stringstream ss(line);
        ss >> classLabelToClassify;
        while (ss >> featureValue){
            objectToClassify.push_back(stod(featureValue));
        }

        rowValue++;
        nearestNeighborDistance = 4294967295; //Unsigned Int Max Value
        nearestNeighborLocation = 4294967295;
        ifstream data2 ("CS170_SuperSmall_Data__43.txt");
        while (getline(data2,line)){
            secondRowValue++;
            if (secondRowValue != rowValue) {
                cout << "Ask if " << rowValue << " is nearest neighbor with " << secondRowValue << endl;
                for (unsigned int l = 0; l < objectToClassify.size(); ++l){
                    distance = sqrt(1);
                    if (distance < nearestNeighborDistance){
                        nearestNeighborDistance = distance;
                        nearestNeighborLocation = secondRowValue;
                        nearestNeighborLabel = classLabels.at(nearestNeighborLocation - 1);
                    }
                }
            }
        }
        secondRowValue = 0;
        objectToClassify.clear();
        data2.close();
    }
    data.close();

    
    accuracy = rand();
    return accuracy;
}

void featureSearch(int numRows){
    unsigned int numFeatures = 0;
    int featureToAddAtCurrentLevel = -1;
    double bestAccuracy = 0.0, accuracy = 0.0;

    string line, featureValue, classValue;
    
    vector<int> currentSetOfFeatures;

    ifstream tempFile ("CS170_SuperSmall_Data__43.txt");
    getline(tempFile,line); // Retrieves First Line of Data
    stringstream ss(line);
    while (ss >> featureValue){
        numFeatures++;
    }
    numFeatures = numFeatures - 1; //Counts the number of features minus class Value
    tempFile.close();

    ifstream file ("CS170_SuperSmall_Data__43.txt"); //Open file

    for (unsigned int i = 1; i <= numFeatures; i++) { //Iterating through down the Search Tree
        cout << "On the " << i << "th level of the search tree" << endl;
        featureToAddAtCurrentLevel = -1;
        bestAccuracy = 0;

        for (unsigned int k = 1; k <= numFeatures; k++){ //Considers each feature separately
            if (find(currentSetOfFeatures.begin(),currentSetOfFeatures.end(),k) == currentSetOfFeatures.end()){ // If the value is not in the current set of features
                cout << "--Considering adding the " << k << " feature" << endl;  //Consider adding the feature
                accuracy = crossValidation(currentSetOfFeatures,k + 1, numRows); //Calculate accuracy
                if (accuracy > bestAccuracy){
                    bestAccuracy  = accuracy;
                    featureToAddAtCurrentLevel = k;
                }
            }
        }
        currentSetOfFeatures.push_back(featureToAddAtCurrentLevel); //Add feature 
        cout << "On level: " << i << " I added feature " << featureToAddAtCurrentLevel << " to current set" << endl;
    }
    file.close(); //Close file
}

int getNumRows(){
    int numRows = 0;
    string line;

    ifstream file ("CS170_SuperSmall_Data__43.txt");
    while (getline(file,line)){
        numRows++;
    }
    file.close();
    return numRows;
}

int main(){

    int numRows = getNumRows();

    // featureSearch(numRows);
    vector<int>test;
    crossValidation(test,0,numRows);

    return 0;
}