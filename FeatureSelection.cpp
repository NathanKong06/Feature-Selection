#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <chrono>

using namespace std;

vector<double> getClassLabels(vector<vector<double>> data){
    string line, classLabel;
    vector<double> classLabels;
    for (unsigned int i = 0; i < data.size(); ++i) {
        classLabels.push_back(data[i][0]); //Only push back first element for each row which is the class label
    }
    return classLabels;
}

vector<vector<double>> readData(string fileName){
    string line, featureValue;
    vector<double> currRow;
    vector<vector<double>> fullData;
    ifstream file (fileName);
    while (getline(file,line)) { //Loop through every row
        stringstream ss(line);
        while (ss >> featureValue) { //Add row values into currRow
            currRow.push_back(stod(featureValue)); //Add curr rows into fullData as doubles
        }
        fullData.push_back(currRow);
        currRow.clear(); //Clear currRow to get ready for next row's data
    }
    file.close();
    return fullData;
}

vector<vector<double>> editData(vector<vector<double>> data){
    for (unsigned int i = 0; i < data.size(); ++i) { //Remove class label from every row so only features in the row remains
        data[i].erase(data[i].begin()); 
    }
    return data;
}

vector<vector<double>> removeFeatures(vector<vector<double>> featureData, vector<int> classSet, int featureToAdd){
    vector<int> columnsToKeep = classSet;
    columnsToKeep.push_back(featureToAdd); //Columns we want to keep, 0 out every other column
    for (unsigned int i = 0; i < featureData.size(); ++i) {
        for (unsigned int j = 0; j < featureData[i].size(); ++j) {
            if (find(columnsToKeep.begin(), columnsToKeep.end(), j) == columnsToKeep.end()){
                featureData[i][j] = 0; //Set columns not in classSet and featureToAdd to 0
            }
        }
    }
    return featureData;
}

vector<vector<double>> removeFromSet(vector<vector<double>> featureData, vector<int> classSet, int featureToRemove){
    vector<int> columnsToKeep = classSet;
    columnsToKeep.erase(remove(columnsToKeep.begin(),columnsToKeep.end(),featureToRemove),columnsToKeep.end()); 
    for (unsigned int i = 0; i < featureData.size(); ++i) {
        for (unsigned int j = 0; j < featureData[i].size(); ++j) {
            if (find(columnsToKeep.begin(), columnsToKeep.end(), j) == columnsToKeep.end()){
                featureData[i][j] = 0; //Set columns not in classSet to 0
            }
        }
    }
    return featureData;
}

double crossValidation(vector<vector<double>> featureData, vector<int> currentSet, int featureToAddOrRemove, vector<double> classLabels, int userNum){
    if (userNum == 1) //Forward Search
        featureData = removeFeatures(featureData, currentSet, featureToAddOrRemove);
    else if (userNum == 2) //Backward Elimination
        featureData = removeFromSet(featureData, currentSet, featureToAddOrRemove);

    double accuracy = 0.0, distance = 0.0, classLabelToClassify, nearestNeighborDistance, nearestNeighborLocation, nearestNeighborLabel, numCorrectlyClassified = 0.0;
    vector <double> objectToClassify, objectToCompare;

    for (unsigned int i = 0; i < featureData.size(); ++i) {
        objectToClassify = featureData[i];
        classLabelToClassify = classLabels[i];
        nearestNeighborDistance = 4294967295; //Some large value for initialization
        nearestNeighborLocation = 4294967295;

        for (unsigned int k = 0; k < featureData.size(); ++k) {
            if (k != i) { //Don't compare same feature to itself
                distance = 0.0;
                objectToCompare = featureData[k];
                for (unsigned int l = 0; l < objectToClassify.size(); ++l) {
                    distance += pow(objectToClassify[l] - objectToCompare[l],2);
                }
                distance = sqrt(distance); //Calculate distance
                if (distance < nearestNeighborDistance) { //Update nearestNeighborDistance, nearestNeighborLocation, and nearestNeighborLabel
                    nearestNeighborDistance = distance;
                    nearestNeighborLocation = k;
                    nearestNeighborLabel = classLabels[k];
                }
            }
        }
        if (classLabelToClassify == nearestNeighborLabel) { //If correctly classified
            numCorrectlyClassified++;
        }
    }
    accuracy = numCorrectlyClassified / featureData.size();
    return accuracy;
}

void forwardFeatureSearch(vector<vector<double>> featureData, vector<double> classLabels, int userNum){
    vector<int> currentSetOfFeatures, bestFeatures;
    double accuracy = 0.0, bestSoFarAccuracy = 0.0, bestTotalAccuracy = 0.0;
    
    for (unsigned int i = 0; i < featureData[0].size(); ++i) {
        int featureToAddAtCurrentLevel = -1, bestFeatureAtLevel = -1;
        bestSoFarAccuracy = 0.0;
        cout << "On level " << i + 1 << " of the search tree" << endl;

        for (unsigned int k = 0; k < featureData[0].size(); ++k) { //Loop through every feature (column)
            if (find(currentSetOfFeatures.begin(),currentSetOfFeatures.end(),k) == currentSetOfFeatures.end()){ // If the value is not in the current set of features
                accuracy = crossValidation(featureData,currentSetOfFeatures,k,classLabels, userNum);
                cout << "--Testing adding feature " << k + 1 << " with current accuracy of: " << accuracy << " using feature(s) {";
                for (unsigned int i = 0; i < currentSetOfFeatures.size(); ++i) {
                    cout << currentSetOfFeatures[i] + 1 << ", ";
                }
                cout << k + 1 << "} " << endl;
                if (accuracy > bestSoFarAccuracy){ //Update bestSoFarAcccuracy
                    bestSoFarAccuracy = accuracy;
                    featureToAddAtCurrentLevel = k;
                }
                if (accuracy > bestTotalAccuracy){ //Update bestTotalAccuracy
                    bestTotalAccuracy = accuracy;
                    bestFeatureAtLevel = k;
                }
            }
        }
        cout << "On level " << i + 1 << ", I added feature " << featureToAddAtCurrentLevel + 1 << " with accuracy of " << bestSoFarAccuracy << endl;
        currentSetOfFeatures.push_back(featureToAddAtCurrentLevel);
        if (bestFeatureAtLevel >= 0){
            bestFeatures.push_back(bestFeatureAtLevel);
        }
        bestFeatureAtLevel = -1;
    }
    cout << "------------------------------------------------------------" << endl;
    cout << "Best features to use: " << endl;
    for (unsigned int i = 0; i < bestFeatures.size(); ++i){ 
        cout << bestFeatures[i] + 1 << " " ;
    }
    cout << endl << "The best accuracy from this data set is: " << bestTotalAccuracy << endl;
}

void backwardFeatureSearch(vector<vector<double>> featureData, vector<double> classLabels, int userNum){
    vector<int> currentSetOfFeatures, bestFeatures, printFeatures;
    double accuracy = 0, bestSoFarAccuracy = 0.0, bestTotalAccuracy = 0.0;

    for (unsigned int i = 0; i < featureData[0].size(); ++i) { //Start with every feature 
        currentSetOfFeatures.push_back(i);
    }
    
    for (unsigned int i = 0; i < featureData[0].size(); ++i) {
        int featureToRemoveAtCurrentLevel = -1;
        bestSoFarAccuracy = 0.0;
        cout << "On level " << i + 1 << " of the search tree" << endl;

        for (unsigned int k = 0; k < featureData[0].size(); ++k) {
            if (find(currentSetOfFeatures.begin(),currentSetOfFeatures.end(),k) != currentSetOfFeatures.end()){ // If the value is in the current set of features
                accuracy = crossValidation(featureData,currentSetOfFeatures,k,classLabels, userNum);
                cout << "--Testing removing feature " << k + 1 << " with current accuracy of: " << accuracy <<  " after removing the feature from the set {";
                printFeatures = currentSetOfFeatures;
                printFeatures.erase(remove(printFeatures.begin(),printFeatures.end(),k),printFeatures.end());
                for (unsigned int i = 0; i < printFeatures.size(); ++i) {
                    if (i < printFeatures.size() - 1)
                        cout << printFeatures[i] + 1 << ", ";
                    else    
                        cout << printFeatures[i] + 1;
                }
                cout << "}" << endl;
                if (accuracy > bestSoFarAccuracy){ //Update bestSoFarAccuracy
                    bestSoFarAccuracy = accuracy;
                    featureToRemoveAtCurrentLevel = k;
                }
                if (accuracy > bestTotalAccuracy){ //Update bestTotalAccuracy
                    bestTotalAccuracy = accuracy;
                    bestFeatures = currentSetOfFeatures;
                    bestFeatures.erase(remove(bestFeatures.begin(),bestFeatures.end(),k),bestFeatures.end());
                }
            }
        }
        cout << "On level " << i + 1 << ", I removed feature " << featureToRemoveAtCurrentLevel + 1 << endl;
        currentSetOfFeatures.erase(remove(currentSetOfFeatures.begin(),currentSetOfFeatures.end(),featureToRemoveAtCurrentLevel),currentSetOfFeatures.end()); //Remove the feature
    }
    cout << "------------------------------------------------------------" << endl;
    cout << "Best features to use: " << endl;
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
    vector<double> classLabels = getClassLabels(data); //Vector containing only class labels
    vector<vector<double>> dataMinusClass = editData(data); //2 dimensional vector containing only features

    cout << "This dataset has " << dataMinusClass[0].size() << " features (not including the class attribute), with " << dataMinusClass.size() << " instances." << endl;

    if (algorithmNum == 1) {
        cout << "Beginning Search" << endl;
        auto start = std::chrono::high_resolution_clock::now();
        forwardFeatureSearch(dataMinusClass, classLabels, algorithmNum);
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
        cout << "Duration: " << duration.count() << " milliseconds" << endl;
    }
    else if (algorithmNum == 2) {
        cout << "Beginning Search" << endl;
        auto start = std::chrono::high_resolution_clock::now();
        backwardFeatureSearch(dataMinusClass, classLabels, algorithmNum);
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
        cout << "Duration: " << duration.count() << " milliseconds" << endl;
    }
    else 
        cout << "Invalid input. Exiting" << endl;

    return 0;
}