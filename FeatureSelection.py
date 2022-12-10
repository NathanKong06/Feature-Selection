import numpy as np
import copy
from math import sqrt

def getLabels(data):
    classLabels = []
    for i in range(len(data)):
        classLabels.append(data[i][0])
    return classLabels

def editData(data):
    for i in range(len(data)):
        del data[i][0]
    return data

def removeFeatures(data, current_set, feature_to_add):
    columns_to_keep = copy.deepcopy(current_set)
    columns_to_keep.append(feature_to_add)
    for i in range(len(data)):
        for j in range(len(data[0])):
            if j not in columns_to_keep:
                data[i][j] = 0
    return data

def removeFromSet(data, current_set, feature_to_remove):
    columns_to_keep = copy.deepcopy(current_set)
    columns_to_keep.remove(feature_to_remove)
    for i in range(len(data)):
        for j in range(len(data[0])):
            if j not in columns_to_keep:
                data[i][j] = 0
    return data    

def crossValidation(datas,current_sets,feature_to_adds, class_label, userNum):
    data = copy.deepcopy(datas)        
    current_set = copy.deepcopy(current_sets)
    feature_to_add = copy.deepcopy(feature_to_adds)
    class_labels = copy.deepcopy(class_label)
    if userNum == "1":
        data = removeFeatures(data,current_set,feature_to_add)
    elif userNum == "2":
        data = removeFromSet(data,current_set,feature_to_add)
    distance = 0.0
    num_correctly_classified = 0
    object_to_classify = []
    object_to_compare = []
    for i in range(len(data)):
        object_to_classify = data[i]
        class_label_to_classify = class_labels[i]
        nearest_neighbor_distance = 99999
        for j in range(len(data)):
            if j != i:
                distance = 0.0
                object_to_compare = data[j]
                for l in range(len(object_to_classify)):
                    distance = distance + pow(object_to_classify[l] - object_to_compare[l],2)
                distance = sqrt(distance)
                if distance < nearest_neighbor_distance:
                    nearest_neighbor_distance = distance
                    nearest_neighbor_label = class_labels[j]
        if class_label_to_classify == nearest_neighbor_label:
            num_correctly_classified = num_correctly_classified + 1
    accuracy = num_correctly_classified / len(data)
    return accuracy
                
def forwardSelection(data, class_labels, userNum):
    class_label = copy.deepcopy(class_labels)
    current_set_of_features = []
    best_features = []
    best_total_accuracy = 0.0
    accuracy = 0.0
    best_so_far_accuracy = 0.0
    for i in range(len(data[0])): 
        feature_to_add_at_current_level = -1
        best_feature_at_level = -1
        best_so_far_accuracy = 0
        print("On the", i + 1, "level of the search tree")
        
        for k in range(len(data[0])):
            if (k not in current_set_of_features):
                accuracy = crossValidation(data, current_set_of_features, k, class_label, userNum)

                print("Testing adding feature", k + 1, "with current accuracy of:", accuracy, "using feature(s) { ", end = "")
                for j in current_set_of_features:
                    print(j + 1, end = ", ")
                print(k + 1, "}")

                if accuracy > best_so_far_accuracy:
                    best_so_far_accuracy = accuracy
                    feature_to_add_at_current_level = k
                if accuracy > best_total_accuracy:
                    best_total_accuracy = accuracy
                    best_feature_at_level = k
                
        print("On level", i + 1, "I added feature", feature_to_add_at_current_level+1, "with accuracy of",best_so_far_accuracy)
        current_set_of_features.append(feature_to_add_at_current_level)
        if best_feature_at_level >= 0:
            best_features.append(best_feature_at_level)
        best_feature_at_level = -1
    print("-------------------------")
    print("Best features to use:")
    for l in best_features:
        print(l + 1, end = " ")
    print("\nThe best accuracy from this data set is", best_total_accuracy)
    
    
def backwardElimination(data, class_labels, user_num):
    class_label = copy.deepcopy(class_labels)
    current_set_of_features = []
    best_features = []
    best_total_accuracy = 0.0
    accuracy = 0.0
    best_so_far_accuracy = 0.0
    
    for  i in range(len(data[0])):
        current_set_of_features.append(i)
        
    for i in range(len(data[0])): 
        feature_to_remove_at_current_level = -1
        best_so_far_accuracy = 0
        print("On the", i + 1, "level of the search tree")
        
        for k in range(len(data[0])):
            if (k in current_set_of_features):
                accuracy = crossValidation(data, current_set_of_features, k, class_label, user_num)
                print("Testing removing feature", k + 1, "with current accuracy of:", accuracy, "using feature(s) { ", end = "")
                print_features = copy.deepcopy(current_set_of_features)
                print_features.remove(k)
                for l in range(len(print_features)):
                    if l < len(print_features)-1:
                        print(print_features[l] + 1, end = ", ")
                    else:
                        print(print_features[l] + 1, end = "")
                print(" }")

                if accuracy > best_so_far_accuracy:
                    best_so_far_accuracy = accuracy
                    feature_to_remove_at_current_level = k
                if accuracy > best_total_accuracy:
                    best_total_accuracy = accuracy
                    best_features = copy.deepcopy(current_set_of_features)
                    best_features.remove(k)
                    
        print("On level", i + 1, "I removed feature", feature_to_remove_at_current_level+1)
        current_set_of_features.remove(feature_to_remove_at_current_level)
    print("-------------------------")
    print("Best features to use:")
    for l in best_features:
        print(l + 1, end = " ")
    print("\nThe best accuracy from this data set is", best_total_accuracy)
    
def main():
    print("Welcome to Nathan's Feature Selection Algorithm.")
    user_file = input("Type in the name of the file to test: ")
    print("Type the number of the algorithm you want to run.")
    user_num = input("1. Forward Selection \n2. Backward Elimination\n")
    data = np.loadtxt(user_file)
    list_data = np.ndarray.tolist(data)
    class_labels = getLabels(list_data)
    data_minus_class = editData(list_data)
    if user_num == "1":
        forwardSelection(data_minus_class, class_labels, user_num)
    elif user_num == "2":
        backwardElimination(data_minus_class, class_labels, user_num)
    else:
        print("Invalid input. Exiting")
    
if __name__ == "__main__":
    main()