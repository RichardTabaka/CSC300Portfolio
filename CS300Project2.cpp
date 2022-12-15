// CS300Project2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <climits>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string> // atoi
#include <time.h>
#include <vector>
using namespace std;

unsigned int courseLimit = 300;

// Structure to hold Course info:
struct Course {
    string courseId;
    string courseName;
    vector<string> prerequisiteCourseIds;
};

// Define hash table class
class HashTable {
private:
    // Create Node structure
    struct Node {
        Course course;
        unsigned int key;
        Node* next;

        // Default Constructor:
        Node() {
            key = courseLimit;
            next = nullptr;
        }

        // Initialize with a Course parameter:
        Node(Course aCourse) {
            course = aCourse;
            key = courseLimit;
            next = nullptr;
        }

        // Initialize with a Course and a Key:
        Node(Course aCourse, unsigned int aKey) {
            course = aCourse;
            key = aKey;
            next = nullptr;
        }
    };

    vector<Node> nodes;

    unsigned int tableSize = courseLimit;

    unsigned int hash(int key);

public:
    HashTable();
    virtual ~HashTable();
    void Insert(Course course);
    void PrintCourseList();
    void PrintCourse(string courseId);
    void LoadCourses(string filePath);
};

// HashTable Default Constructor:
HashTable::HashTable() {
    nodes.resize(tableSize);
}

// HashTable Destructor:
HashTable::~HashTable() {
    nodes.erase(nodes.begin());
}

// Hashing Algorithm
unsigned int HashTable::hash(int key) {
    return key % tableSize;
}

// Course Insertion Algo, params: Course(course to be inserted)
void HashTable::Insert(Course course) {
    // Create key for course based on courseId
    unsigned int key = hash(atoi(course.courseId.c_str()));

    // Grab the node at key
    Node* node = &nodes.at(key);

    // If the node was 'empty'
    if (node == nullptr) {
        // This is where the Course belongs, insert it
        node = new Node(course, key);
        nodes.insert(nodes.begin() + key, *node);
    } // else, if node is not used
    else if (node->key == courseLimit) {
        // Assign course to that node
        node->course = course;
        node->key = key;
        node->next = nullptr;
    } // else find the next open node
    else {
        // add new node to end
        while (node->next != nullptr) {
            node = node->next;
        }
        Node* newNode = new Node(course, node->key + 1);
        node->next = newNode;
        nodes.insert(nodes.begin() + newNode->key, *newNode);
    }
}

// Print all courses, sorted
void HashTable::PrintCourseList() {
    for (int i = 0; i < nodes.size(); ++i) {
        if (nodes[i].key != courseLimit) {
            cout << nodes[i].course.courseId << ", " << nodes[i].course.courseName << endl;
        }
    }
    cout << endl;
}

// Print course and prerequisites
void HashTable::PrintCourse(string courseId) {
    Course course;

    // Create the key for the given courseId
    unsigned key = hash(atoi(courseId.c_str()));

    // Grab the node at that key
    Node* node = &(nodes.at(key));

    // If this node has the requested course
    if (node != nullptr && node->key != courseLimit && node->course.courseId == courseId) {
        // Print its info and prereqs
        cout << node->course.courseId << ", " << node->course.courseName 
            << endl << "Prerequisites: ";
        for (string courseId : node->course.prerequisiteCourseIds)
            cout << courseId << ", ";
        return;
    }
    // if no entry is found for the key, return
    if (node == nullptr || atoi(node->course.courseId.c_str()) == courseLimit)
        return;

    // while node isn't null but doesn't have requested course
    while (node != nullptr) {
        // If the current node has the correct course
        if (node->course.courseId == courseId) {
            // Print its info and prereqs
            cout << node->course.courseId << ", " << node->course.courseName
                << endl << "Prerequisites: ";
            for (string courseId : node->course.prerequisiteCourseIds)
                cout << courseId << ", ";
            // And exit the loop
            cout << endl;
            return;
        }
        // If the current node doesn't have the correct courseId, check the next
        node = node->next;
    }
    cout << endl;
}

// Algo to load courses from CSV at filePath
void HashTable::LoadCourses(string filePath) {
    string line;
    ifstream csvStream (filePath);

    // Check to see if the file was found
    if (csvStream.fail()) {
        // If not, report it to the user and return
        cout << "No file found at " << filePath << endl;
        return;
    }

    // While there are lines left to parse
    while (getline(csvStream, line)) {
        string courseNum, courseName, preReq;
        vector<string> preReqs;

        // If the line wasn't empty
        if (line != "") {
            // Read it into a string stream for use
            stringstream inputString(line);
            // Then pull course num and name from the string stream
            getline(inputString, courseNum, ',');
            getline(inputString, courseName, ',');

            // Then grab any pre requisite course info
            while (getline(inputString, preReq, ','))
                preReqs.push_back(preReq);

            // Instantiate the course with info
            Course course;
            course.courseId = courseNum;
            course.courseName = courseName;
            course.prerequisiteCourseIds = preReqs;

            // Insert the course into the hash table
            Insert(course);
        }
        // reset line to blank line in case
        line = "";
    }
    cout << "Successfully loaded courses from " << filePath << endl << endl;
}



int main()
{
    HashTable courseTable;
    string userInput; // C:\\Users\\Richa\\Downloads\\course_documents\\ABCU_Advising_Program_Input.txt

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            cout << "Enter file path for CSV: " << endl;
            cin >> userInput;
            courseTable.LoadCourses(userInput);
            break;

        case 2:
            courseTable.PrintCourseList();
            break;

        case 3:
            cout << "Enter Course ID: " << endl;
            cin >> userInput;
            courseTable.PrintCourse(userInput);
            break;
        }
    }

}
