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

unsigned int getNumRows(vector<vector<double>> data){
    return data.size();
}

vector<vector<double>> readData(){
    string line, featureValue;
    vector<double> currRow;
    vector<vector<double>> fullData;
    ifstream file ("CS170_SuperSmall_Data__43.txt");
    while (getline(file,line)) {
        stringstream ss(line);
        while (ss >> featureValue) {
            currRow.push_back(stod(featureValue));
        }
        fullData.push_back(currRow);
        currRow.clear();
    }
    file.close();
    return fullData;
}

vector<vector<double>> editData(vector<vector<double>> data){
    for (unsigned int i = 0; i < data.size(); ++i) {
        data[i].erase(data[i].begin());
    }
    return data;
}

vector<vector<double>> removeFeatures(vector<vector<double>> featureData, vector<int> classSet, int featureToAdd){
    vector<int> columnsToRemove = classSet;
    columnsToRemove.push_back(featureToAdd);
    for (unsigned int i = 0; i < featureData.size(); ++i) {
        for (unsigned int j = 0; j < featureData[i].size(); ++j) {
            if (find(columnsToRemove.begin(), columnsToRemove.end(), j) != columnsToRemove.end()){
                featureData[i][j] = 0;
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
            if (k != i) {
                distance = 0.0;
                objectToCompare = featureData[k];
                for (unsigned int l = 0; l < objectToClassify.size(); ++l) {
                    distance += pow(objectToClassify[l] - objectToCompare[l],2);
                }
                distance = sqrt(distance);
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
    double accuracy = 0.0;
    
    for (unsigned int i = 0; i < featureData[0].size(); ++i) {
        int featureToAddAtCurrentLevel = -1;
        double bestAccuracy = 0.0;
        cout << "On the " << i << "th level of the search tree" << endl;

        for (unsigned int k = 0; k < featureData[0].size(); ++k) {
            if (find(currentSetOfFeatures.begin(),currentSetOfFeatures.end(),k) == currentSetOfFeatures.end()){ // If the value is not in the current set of features
                cout << "--Considering adding the  " << k << " feature" << endl;
                accuracy = crossValidation(featureData,currentSetOfFeatures,featureToAddAtCurrentLevel,classLabels);
                if (accuracy > bestAccuracy){
                    bestAccuracy = accuracy;
                    featureToAddAtCurrentLevel = k;
                }
            }
        }
        cout << "On level " << i << " I added feature " << featureToAddAtCurrentLevel << " to current set" << endl;
        currentSetOfFeatures.push_back(featureToAddAtCurrentLevel);
    }
}

int main(){

    vector<vector<double>> data = readData(); //2 dimensional vector containing row x column

    unsigned int numRows = getNumRows(data);

    vector<double> classLabels = getClassLabels(data); //Vector containing only class labels

    vector<vector<double>> dataMinusClass = editData(data); //2 dimensional vector containing only features

    featureSearch(dataMinusClass, classLabels);

    return 0;
}