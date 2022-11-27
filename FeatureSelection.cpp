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


// double crossValidation(vector<int> currentSet, int featureToAdd, double numRows){
//     double accuracy = 0.0, distance = 0.0;

//     unsigned int rowValue = 0, secondRowValue = 0, numCorrectlyClassified = 0, nearestNeighborDistance, nearestNeighborLocation, nearestNeighborLabel;

//     string line, classLabelToClassify, featureValue;

//     vector<double> objectToClassify, objectToCompare;
//     vector<double> classLabels = getClassLabels();

//     ifstream data ("CS170_SuperSmall_Data__43.txt"); //Open File

//     while (getline(data,line)){  //Will only loop #Rows times
//         stringstream ss(line);
//         ss >> classLabelToClassify; //Class label at current row
//         while (ss >> featureValue){
//             objectToClassify.push_back(stod(featureValue)); //Gets all feature values for current instance (row)
//         }
//         rowValue++;
//         nearestNeighborDistance = 4294967295; //Unsigned Int Max Value
//         nearestNeighborLocation = 4294967295;

//         ifstream data2 ("CS170_SuperSmall_Data__43.txt"); //Open file again

//         while (getline(data2,line)) { 
//             stringstream ss(line);
//             ss >> classLabelToClassify;
//             while (ss >> featureValue){
//                 objectToCompare.push_back(stod(featureValue)); 
//             }
//             secondRowValue++;
//             if (secondRowValue != rowValue) { //Not comparing same value to itself
//                 for (unsigned int l = 0; l < objectToClassify.size(); ++l){ 
//                     distance += pow(objectToClassify.at(l) - objectToCompare.at(l),2);
//                 }
//                 distance = sqrt(distance); //Calculate distance using Euclidian Distance
//                 if (distance < nearestNeighborDistance){ //Update nearest neighbor values
//                     nearestNeighborDistance = distance;
//                     nearestNeighborLocation = secondRowValue;
//                     nearestNeighborLabel = classLabels.at(nearestNeighborLocation-1);
//                 }
//             }
//             objectToCompare.clear(); //Clear vector since comparing new row
//         }

//         if (stoi(classLabelToClassify) == nearestNeighborLabel){
//             numCorrectlyClassified++;
//         }

//         secondRowValue = 0;
//         objectToClassify.clear(); //Clear vector since classifying new row
//         data2.close();
//         cout << "Object " << rowValue << " is class " << classLabels.at(rowValue - 1) << endl;
//         cout << "It's nearest neighbor is " << nearestNeighborLocation << " which is in class " << nearestNeighborLabel << endl;
//     }
//     data.close();
//     accuracy = numCorrectlyClassified / numRows;
//     return accuracy;
// }

// void featureSearch(int numRows){
//     unsigned int numFeatures = 0;
//     int featureToAddAtCurrentLevel = -1;
//     double bestAccuracy = 0.0, accuracy = 0.0;

//     string line, featureValue, classValue;
    
//     vector<int> currentSetOfFeatures;

//     ifstream tempFile ("CS170_SuperSmall_Data__43.txt");
//     getline(tempFile,line); // Retrieves First Line of Data
//     stringstream ss(line);
//     while (ss >> featureValue){
//         numFeatures++;
//     }
//     numFeatures = numFeatures - 1; //Counts the number of features minus class Value
//     tempFile.close();

//     ifstream file ("CS170_SuperSmall_Data__43.txt"); //Open file

//     for (unsigned int i = 1; i <= numFeatures; i++) { //Iterating through down the Search Tree
//         cout << "On the " << i << "th level of the search tree" << endl;
//         featureToAddAtCurrentLevel = -1;
//         bestAccuracy = 0;

//         for (unsigned int k = 1; k <= numFeatures; k++){ //Considers each feature separately
//             if (find(currentSetOfFeatures.begin(),currentSetOfFeatures.end(),k) == currentSetOfFeatures.end()){ // If the value is not in the current set of features
//                 cout << "--Considering adding the " << k << " feature" << endl;  //Consider adding the feature
//                 accuracy = crossValidation(currentSetOfFeatures,k + 1, numRows); //Calculate accuracy
//                 if (accuracy > bestAccuracy){
//                     bestAccuracy  = accuracy;
//                     featureToAddAtCurrentLevel = k;
//                 }
//             }
//         }
//         currentSetOfFeatures.push_back(featureToAddAtCurrentLevel); //Add feature 
//         cout << "On level: " << i << " I added feature " << featureToAddAtCurrentLevel << " to current set" << endl;
//     }
//     file.close(); //Close file
// }

void featureSearch(vector<vector<double>> featureData){
    vector<int> currentSetOfFeatures;
    double accuracy = 0.0;
    
    for (unsigned int i = 0; i < featureData[0].size(); ++i) {
        int featureToAddAtCurrentLevel = -1;
        double bestAccuracy = 0.0;
        cout << "On the " << i << "th level of the search tree" << endl;

        for (unsigned int k = 0; k < featureData[0].size(); ++k) {
            if (find(currentSetOfFeatures.begin(),currentSetOfFeatures.end(),k) == currentSetOfFeatures.end()){ // If the value is not in the current set of features
                cout << "--Considering adding the  " << k << " feature" << endl;
                accuracy = rand();
                if (accuracy > bestAccuracy){
                    bestAccuracy = accuracy;
                    featureToAddAtCurrentLevel = k;
                }
            }
        }
        cout << "On level " << i << "I added feature " << featureToAddAtCurrentLevel << " to current set" << endl;
        currentSetOfFeatures.push_back(featureToAddAtCurrentLevel);
    }
}

int main(){

    vector<vector<double>> data = readData(); //2 dimensional vector containing row x column

    unsigned int numRows = getNumRows(data);

    vector<double> classLabels = getClassLabels(data); //Vector containing only class labels

    vector<vector<double>> dataMinusClass = editData(data); //2 dimensional vector containing only features

    featureSearch(dataMinusClass);

    return 0;
}