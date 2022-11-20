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
        classLabels.push_back(stod(classLabel)); //Gets all class labels in file and adds to vector
    }
    data.close();
    return classLabels;
}

double crossValidation(vector<int> currentSet, int featureToAdd, double numRows){
    double accuracy = 0.0, distance = 0.0;

    unsigned int rowValue = 0, secondRowValue = 0, numCorrectlyClassified = 0, nearestNeighborDistance, nearestNeighborLocation, nearestNeighborLabel;

    string line, classLabelToClassify, featureValue;

    vector<double> objectToClassify, objectToCompare;
    vector<double> classLabels = getClassLabels();

    ifstream data ("CS170_SuperSmall_Data__43.txt"); //Open File

    while (getline(data,line)){  //Will only loop #Rows times
        stringstream ss(line);
        ss >> classLabelToClassify; //Class label at current row
        while (ss >> featureValue){
            objectToClassify.push_back(stod(featureValue)); //Gets all feature values for current instance (row)
        }
        rowValue++;
        nearestNeighborDistance = 4294967295; //Unsigned Int Max Value
        nearestNeighborLocation = 4294967295;

        ifstream data2 ("CS170_SuperSmall_Data__43.txt"); //Open file again

        while (getline(data2,line)) { 
            stringstream ss(line);
            ss >> classLabelToClassify;
            while (ss >> featureValue){
                objectToCompare.push_back(stod(featureValue)); 
            }
            secondRowValue++;

            if (secondRowValue != rowValue) { //Not comparing same value to itself
                for (unsigned int l = 0; l < objectToClassify.size(); ++l){ 
                    distance += pow(objectToClassify.at(l) - objectToCompare.at(l),2);
                }
                distance = sqrt(distance); //Calculate distance using Euclidian Distance
                if (distance < nearestNeighborDistance){ //Update nearest neighbor values
                    nearestNeighborDistance = distance;
                    nearestNeighborLocation = secondRowValue;
                    nearestNeighborLabel = classLabels.at(nearestNeighborLocation-1);
                }
            }
            objectToCompare.clear(); //Clear vector since comparing new row
        }

        if (stoi(classLabelToClassify) == nearestNeighborLabel){
            numCorrectlyClassified++;
        }

        secondRowValue = 0;
        objectToClassify.clear(); //Clear vector since classifying new row
        data2.close();
        cout << "Object " << rowValue << " is class " << classLabels.at(rowValue - 1) << endl;
        cout << "It's nearest neighbor is " << nearestNeighborLocation << " which is in class " << nearestNeighborLabel << endl;
    }
    data.close();
    accuracy = numCorrectlyClassified / numRows;
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

double getNumRows(){
    double numRows = 0.0;
    string line;

    ifstream file ("CS170_SuperSmall_Data__43.txt");
    while (getline(file,line)){
        numRows++;
    }
    file.close();
    return numRows;
}

int main(){

    double numRows = getNumRows();

    // featureSearch(numRows);
    vector<int>test;
    cout << crossValidation(test,0,numRows) << endl;

    return 0;
}