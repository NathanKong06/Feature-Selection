#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>
using namespace std;

vector<double> getClassLabels(vector<vector<double>> data){
    string line, classLabel;
    vector<double> classLabels;
    for (unsigned int i = 0; i < data.size(); ++i) {
        classLabels.push_back(data[i][0]); //Only push back first element for each row which is the class label
    }
    return classLabels;
}

// unsigned int getNumRows(vector<vector<double>> data){
//     return data.size();
// }

vector<vector<double>> readData(string fileName){
    string line, featureValue;
    vector<double> currRow;
    vector<vector<double>> fullData;
    ifstream file (fileName);
    while (getline(file,line)) { //Loop through every row
        stringstream ss(line);
        while (ss >> featureValue) { //Add row values into currRow
            currRow.push_back(stod(featureValue)); //Add curr rows into fullData
        }
        fullData.push_back(currRow);
        currRow.clear();
    }
    file.close();
    return fullData;
}

vector<vector<double>> editData(vector<vector<double>> data){
    for (unsigned int i = 0; i < data.size(); ++i) {
        data[i].erase(data[i].begin()); //Remove class label from current row (Only features in row left)
    }
    return data;
}

vector<vector<double>> removeFeatures(vector<vector<double>> featureData, vector<int> classSet, int featureToAdd){
    vector<int> columnsToKeep = classSet;
    columnsToKeep.push_back(featureToAdd);
    for (unsigned int i = 0; i < featureData.size(); ++i) {
        for (unsigned int j = 0; j < featureData[i].size(); ++j) {
            if (find(columnsToKeep.begin(), columnsToKeep.end(), j) == columnsToKeep.end()){
                featureData[i][j] = 0; //Set collumns not in classSet and featureToAdd to 0
            }
        }
    }
    return featureData;
}

double crossValidation(vector<vector<double>> featureData, vector<int> currentSet, int featureToAdd, vector<double> classLabels){
    featureData = removeFeatures(featureData, currentSet, featureToAdd);
    double accuracy = 0.0, distance = 0.0, classLabelToClassify, nearestNeighborDistance, nearestNeighborLocation, nearestNeighborLabel, numCorrectlyClassified = 0.0;
    vector <double> objectToClassify, objectToCompare;

    for (unsigned int i = 0; i < featureData.size(); ++i) {
        objectToClassify = featureData[i];
        classLabelToClassify = classLabels[i];
        nearestNeighborDistance = 4294967295;
        nearestNeighborLocation = 4294967295;

        for (unsigned int k = 0; k < featureData.size(); ++k) {
            if (k != i) { //Don't compare same feature to itself
                distance = 0.0;
                objectToCompare = featureData[k];
                for (unsigned int l = 0; l < objectToClassify.size(); ++l) {
                    distance += pow(objectToClassify[l] - objectToCompare[l],2);
                }
                distance = sqrt(distance); //Calculate distance
                if (distance < nearestNeighborDistance) {
                    nearestNeighborDistance = distance;
                    nearestNeighborLocation = k;
                    nearestNeighborLabel = classLabels[k];
                }
            }
        }
        if (classLabelToClassify == nearestNeighborLabel) {
            numCorrectlyClassified++;
        }
    }
    accuracy = numCorrectlyClassified / featureData.size();
    return accuracy;
}

void featureSearch(vector<vector<double>> featureData, vector<double> classLabels){
    vector<int> currentSetOfFeatures;
    vector<int> bestFeatures;
    double accuracy = 0.0, bestSoFarAccuracy = 0.0, bestTotalAccuracy = 0.0;
    int bestFeatureAtLevel;
    
    for (unsigned int i = 0; i < featureData[0].size(); ++i) {
        int featureToAddAtCurrentLevel = -1;
        int bestFeatureAtLevel = -1;
        bestSoFarAccuracy = 0.0;
        cout << "On the " << i << "th level of the search tree" << endl;

        for (unsigned int k = 0; k < featureData[0].size(); ++k) {
            if (find(currentSetOfFeatures.begin(),currentSetOfFeatures.end(),k) == currentSetOfFeatures.end()){ // If the value is not in the current set of features
                accuracy = crossValidation(featureData,currentSetOfFeatures,k,classLabels);
                cout << "--Testing the  " << k << " feature with current accuracy of: " << accuracy << endl;
                if (accuracy > bestSoFarAccuracy){
                    bestSoFarAccuracy = accuracy;
                    featureToAddAtCurrentLevel = k;
                }
                if (accuracy > bestTotalAccuracy){
                    bestTotalAccuracy = accuracy;
                    bestFeatureAtLevel = k;
                }
            }
        }
        cout << "On level " << i << " I added feature " << featureToAddAtCurrentLevel << endl;
        currentSetOfFeatures.push_back(featureToAddAtCurrentLevel);
        if (bestFeatureAtLevel >= 0){
            bestFeatures.push_back(bestFeatureAtLevel);
        }
        bestFeatureAtLevel = -1;
    }
    cout << endl << "Best features to use: " << endl;
    for (unsigned int i = 0; i < bestFeatures.size(); ++i){ 
        cout << bestFeatures[i] + 1 << " " ;
    }
    cout << endl << "The best accuracy from this data set is: " << bestTotalAccuracy << endl;
}

int main(){
    string fileName;
    int algorithmNum;
    cout << "Welcome to Nathan's Feature Selection Algorithm." << endl;
    cout << "Type in the name of the file to test: " << endl;
    cin >> fileName;
    cout << "Type the number of the algorithm you want to run." << endl;
    cout << "1. Forward Selection" << endl << "2. Backward Elimination" << endl;
    cin >> algorithmNum;
    vector<vector<double>> data = readData(fileName); //2 dimensional vector containing row x column
    // unsigned int numRows = getNumRows(data);
    vector<double> classLabels = getClassLabels(data); //Vector containing only class labels
    vector<vector<double>> dataMinusClass = editData(data); //2 dimensional vector containing only features
    if (algorithmNum == 1) {
        featureSearch(dataMinusClass, classLabels);
    }
    else if (algorithmNum == 2) {

    }
    else 
        cout << "Invalid input. Exiting" << endl;

    return 0;
}