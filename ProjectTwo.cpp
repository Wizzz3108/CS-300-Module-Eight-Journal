// ProjectTwo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

// ==================
//  Course Structure
// ==================
struct Course {
    string courseNumber;   // Unique ID
    string courseTitle;    // Course Name
    vector<string> prerequisites;   // List of prerequisite course numbers
};

// =====================
// Node STRUCTURE (BST)
// =====================
struct Node {
    Course course;    // Course is stored in this node
    Node* left;     // Pointer to the left child
    Node* right;    // Pointer to the right child

    // Constructor initializes node with course data
    Node(Course c) {
        course = c;
        left = nullptr;
        right = nullptr;
    }
};

// =========================
// Binary Search Tree CLASS
// =========================
class BinarySearchTree {
private:
    Node* root;    // Root node of BST

    // Recursive function to insert a node into a tree
    void addNode(Node* node, Course course) {

        // Compare course numbers to decide placement
        if (course.courseNumber < node->course.courseNumber) {

            // If left node is empty, insert here
            if (node->left == nullptr) {
                node->left = new Node(course);
            }
            else {
                addNode(node->left, course);   // Recurse left
            }
        }
        else {

            // Otherwise go right
            if (node->right == nullptr) {
                node->right = new Node(course);
            }
            else {
                addNode(node->right, course);
            }
        }
    }


     // In-order traversal (Left, Root, Right)
    // This prints courses in sorted order automatically
    void inOrder(Node* node) {
        if (node != nullptr) {

            inOrder(node->left);    // visit left subtree

            cout << node->course.courseNumber << ", "
                << node->course.courseTitle << endl;

            inOrder(node->right);   // visit right subtree
        }
    }

    // Recursive search function
    Course search(Node* node, string courseNumber) {

        // Base case: not found
        if (node == nullptr) {
            return Course();
        }

        // Match found
        if (node->course.courseNumber == courseNumber) {
            return node->course;
        }

        // Traverse left or right depending on comparison
        if (courseNumber < node->course.courseNumber) {
            return search(node->left, courseNumber);
        }
        else {
            return search(node->right, courseNumber);
        }
    }
public:
    // Constructor initializes empty tree
    BinarySearchTree() {
        root = nullptr;
    }

    // Public insert function
    void Insert(Course course) {
        
        // If tree is empty, set root
        if (root == nullptr) {
            root = new Node(course);
        }
        else {
            addNode(root, course);    // Otherwise  insert recursively
        }
    }

    // Print all courses in sorted order
    void PrintCourseList() {
        inOrder(root);
    }

    // Search for a course by course number
    Course SearchCourse(string courseNumber) {
        return search(root, courseNumber);
    }
};

// ==============================
// GLOBAL VECTOR (VALID COURSES)
// ==============================
vector<string> validCourses;

// ================================
// FIRST PASS: LOAD COURSE NUMBERS
// ================================
void loadValidCourses(string fileName) {
    ifstream file(fileName);    // Open file
    string line;

    // Check if file opened successfully
    if (!file.is_open()) {
        cout << "Error: Could not open file. \n";
        return;
    }

    validCourses.clear();         // Clear any previous data

    // Read file line by line
    while (getline(file, line)) {
        stringstream ss(line);
        string courseNumber;

        getline(ss, courseNumber, ',');   // Extract first token

        // ONLY remove leading space after comma 
        if (!courseNumber.empty() && courseNumber[0] == ' ') {
            courseNumber.erase(0, 1);
        }

        validCourses.push_back(courseNumber);     // Store valid course number
    }

    file.close();    // Close file
}

// =======================
// SECOND PASS: BUILD BST
// =======================
void buildTree(string fileName, BinarySearchTree& tree) {
    ifstream file(fileName);
    string line;

    if (!file.is_open()) {
        cout << "Error: Could not open file\n";
        return;
    }

    while (getline(file, line)) {
        stringstream ss(line);
        Course course;
        string token;

        // Read course number and title
        getline(ss, course.courseNumber, ',');
        getline(ss, course.courseTitle, ',');

        // trim whitespace courseNumber (both sides)
        if (!course.courseNumber.empty() && course.courseNumber[0] == ' ') {
            course.courseNumber.erase(0, 1);
        }

        // trim whitespace courseTitle (both sides)
        if (!course.courseTitle.empty() && course.courseTitle[0] == ' ') {
            course.courseTitle.erase(0, 1);
        }

        // Process prerequisites
        while (getline(ss, token, ',')) {

            // Remove whitespace
            if (!token.empty() && token[0] == ' ') {
                token.erase(0, 1);
            }

            // Skip empty tokens 
            if (token.empty()) {
                continue;
            }

            // Check if prerequisite exists in valid list
            if (find(validCourses.begin(), validCourses.end(), token) != validCourses.end()) {
                course.prerequisites.push_back(token);    // Add valid prereq
            }
            else {
                cout << "Invalid prerequisite found: " << token << endl;
            }
        }

        // Insert Course
        tree.Insert(course);
    }

    file.close();
}

// ====================
// PRINT SINGLE COURSE
// =====================
void PrintCourse(BinarySearchTree& tree, string courseNumber) {
    Course c = tree.SearchCourse(courseNumber);    // search BST

    // If course is not found
    if (c.courseNumber == "") {
        cout << "Course not found.\n";
        return;
    }

    // Print course information
    cout << c.courseNumber << ", " << c.courseTitle << endl;

    // Print prerequisites
    if (c.prerequisites.empty()) {
        cout << "Prerequisites: None\n";
    }
    else {
        cout << "Prequisites: ";

        // Loop through prerequisites
        for (int i = 0; i < c.prerequisites.size(); i++) {
            cout << c.prerequisites[i];

            // Add comma between items
            if (i < c.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

// =====================
// MAIN FUNCTION (MENU)
// =====================
int main()
{
    BinarySearchTree tree;    // Create BST object

    int choice = 0;
    string fileName;

    cout << "Welcome to the course planner\n";

    // Menu loop
    while (choice != 9) {    // Loop runs until user chooses to exit (option 9) 
        cout << "\n1. Load Data Structure.\n";
        cout << "2. Print Course List.\n";
        cout << "3. Print Course.\n";
        cout << "9. Exit\n";
        cout << "What would you like to do? ";

        cin >> choice;    // Read user menu selection

        switch (choice) {
        case 1: {
            cout << "Enter file name: ";
            cin.ignore();        // Clear leftover newline from input buffer
            getline(cin, fileName);     // Allows full file name (including spaces)

            loadValidCourses(fileName);     // First pass: collect valid course numbers
            buildTree(fileName, tree);      // Second pass: build BST with course data

            cout << "Data loaded successfully.\n";
            break;
        }
        case 2: {
            cout << "Here is a sample schedule:\n";
            tree.PrintCourseList();        // In-order traversal prints sorted courses
            break;
        }
        case 3: {
            string courseNum;

            cout << "What course do you want to know about? ";
            cin >> courseNum;       // Get course number input

            // Convert to upper case
            for (char& c : courseNum) {
                c = toupper(c);
            }
            PrintCourse(tree, courseNum);      // Search BST and display course info
            break;
        }
        case 9: {
            cout << "Thank you for using the course planner!\n";    // Exit message
            break;
        }
        default: {  
            cout << choice << " is not a valid option.\n";    // Handle invalid menu input
            break;
        }
        }
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
