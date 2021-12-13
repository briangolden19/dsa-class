//============================================================================
// Name        : CourseProject.cpp
// Author      : Brian Golden
// Version     : 1.0
// Copyright   : Copyright © 2021 Brian Golde
// Description : Binary Search Tree to hold a list of courses and thier prereqs
//============================================================================


#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

struct Course { // struct to hold the course objects
	string courseID;
	string courseName;
	vector<string> preReqs;
};

struct TreeNode { // a tree node
	Course course; // our course struct
	TreeNode* leftNode; //left side pointer
	TreeNode* rightNode; //right side pointer

	TreeNode() { // set both pointers to null
		leftNode = nullptr;
		rightNode = nullptr;
	}

	TreeNode(Course c) :TreeNode() { // set the course object and set pointers to null
		course = c;
	}
};

class BinarySearchTree {
private:
	TreeNode* root;
	void addNode(TreeNode* node, Course c);
	void inOrder(TreeNode* node);

public:
	BinarySearchTree();
	virtual ~BinarySearchTree();
	void InOrder();
	void Insert(Course c);
	Course Search(string courseID);
	void deleteAll(TreeNode* node);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
	//root is equal to nullptr
	root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
	// recurse from root deleting every node
	deleteAll(root);
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
	// call inOrder fuction and pass root 
	this->inOrder(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
	// if root equarl to null ptr
	if (root == nullptr) {
		root = new TreeNode(course); // root is equal to new node course
	}
	else { // else
		this->addNode(root, course); // add Node root and course
	}
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseID) {
	// set current node equal to root
	TreeNode* curr = root;
	// keep looping downwards until bottom reached or matching courseID found
	while (curr != nullptr) {
		if (curr->course.courseID.compare(courseID) == 0) {// if match found, return current course
			return curr->course;
		}
		if (courseID.compare(curr->course.courseID) < 0) { // if course is smaller than current node then traverse left
			curr = curr->leftNode;
		}
		else {// else larger so traverse right
			curr = curr->rightNode;
		}
	}
	Course course; // return empty course
	return course;
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(TreeNode* node, Course course) {
	// if node is larger then add to left
	if (node->course.courseID.compare(course.courseID) > 0) {
		if (node->leftNode == nullptr) { // if no left node
			node->leftNode = new TreeNode(course);  // this node becomes left
		}
		else {  // else recurse down the left node
			addNode(node->leftNode, course);
		}
	}
	else { // else
		if (node->rightNode == nullptr) {// if no right node
			node->rightNode = new TreeNode(course); // this node becomes right
		}
		else {//else
			addNode(node->rightNode, course); // recurse down the right node                
		}
	}
}

void BinarySearchTree::inOrder(TreeNode* node) {
	//if node is not equal to null ptr
	if (node != nullptr) {
		inOrder(node->leftNode); //InOrder left
		cout << node->course.courseID << ", " << node->course.courseName << endl;
		inOrder(node->rightNode);//InOder right
	}
}

void BinarySearchTree::deleteAll(TreeNode* node) {
	if (node != nullptr) {
		deleteAll(node->leftNode); //InOrder left
		deleteAll(node->rightNode);//InOder right
		delete node;
	}
}
/**
* Print the specified course
* 
* @param course the course to print
*/
void printCourse(Course course) {
	cout << course.courseID << ", " << course.courseName << endl; // print the name
	if (!course.preReqs.empty()) { //if the prereqs are not empty
		cout << "Prerequisites: ";
		int i; // loop through the prerequs
		for (i = 0; i < course.preReqs.size() - 1; i++) {
			cout << course.preReqs.at(i) << ", ";
		}
		cout << course.preReqs.at(i) << endl;
	}
	cout << endl;
}

/**
* Load the courses from a file into the bst
* 
* @param filePath the path to the course file
* @param bst the binary search tree we are adding courses to
*/
void loadCourses(string filePath, BinarySearchTree* bst) {
	ifstream courseFile; // create the input stream object
	courseFile.open(filePath); // open the file
	string line; // string to hold each line
	vector<string> tempCourseIDList; // this will hold all of the courseID's so that we can make sure only valid courses are added at the end
	vector<Course> tempCourseList; // this holds our list of courses before adding to the bst
	if (courseFile.is_open()) { // if the file is open
		while (getline(courseFile, line)) { // while there is a next line in the file
			Course tempCourse; // create a temp course object to be modifying each loop
			if (line.find(",") == std::string::npos) { //if there is not a comma found we assume there is an error in file input
				cout << "Error in file format!!" << endl; // spit out error
				courseFile.close(); // close the file
				return; //return
			}
			else { // if an error was not found
				int tempIndex = line.find(","); // store the index of the first comma found
				tempCourse.courseID = line.substr(0, tempIndex); //get the substring of the start till the first comma and store that as the courseID

				tempCourseIDList.push_back(tempCourse.courseID); //Store that ID in our temporary vector

				int nextComma = line.find(",", tempIndex + 1); // check for another comma in the line (this indactes there is a prereq
				if (nextComma != std::string::npos) { // if a prereq was found in the file
					tempCourse.courseName = line.substr(tempIndex + 1, (nextComma - (tempIndex+1))); //store the course name which is the substring of the first comma to the next comma
					if (line.find(",", nextComma + 1) != std::string::npos) { //if there is another comma after the second one
						while (line.find(",", nextComma + 1) != std::string::npos) { // we loop through for any prereqs found
							tempIndex = line.find(",", nextComma + 1); // store the 2nd or more comma
							tempCourse.preReqs.push_back(line.substr(nextComma + 1, tempIndex - (nextComma + 1))); //add the prereq to the course prereq vector
							nextComma = tempIndex; // flip the indecies
						}
						tempCourse.preReqs.push_back(line.substr(nextComma + 1)); // pushback to add a prereq
					}
					else {
						tempCourse.preReqs.push_back(line.substr(nextComma + 1)); //pushback to add a prereq
					}
				}
				else {  //This is for courses that don't have prereqs
					tempCourse.courseName = line.substr(tempIndex + 1); // add the course name to the temp course
				}
			}
			tempCourseList.push_back(tempCourse); // add the current temp course to the temp list
		}
		courseFile.close(); // close the file
	}

	else { // print this if our file was unable to open
		cout << "Unable to open file";
		return;
	}

	for (Course course : tempCourseList) { // this will go through every course in the temp list
		
		if (course.preReqs.empty()) { //if there are no prereqs we can just add the course
			bst->Insert(course);
		}else{ //otherwise
			bool courseIDFound = false; // boolean for if we found a match
			for (string pre : course.preReqs) { //loop through prereqs
				courseIDFound = false; // if there are more than one we should set this to false at every loop
				for (string id : tempCourseIDList) { // go through each courseID to make sure we are adding valid courses
					if (pre.compare(id) == 0) { // if a match was found set our boolean to true
						courseIDFound = true;
					}
				}
				if (!courseIDFound) { // if we did not find a match we should leave the loop
					break;
				}
			}
			if (courseIDFound) { // if the course was found
				bst->Insert(course); // insert our found course since it was found to be valid
			}
			else { // otherwise print out that we have an invalid course
				cout << "INVALID COURSE!!" << endl;
				cout << "Cannot add: " << course.courseID << endl;
			}
		}
		
		
	}
}


int main(int argc, char* argv[]) {
	// process command line arguments
	string filePath;
	switch (argc) {
	case 2:
		filePath = argv[1]; // gets the file from command line
		break;
	default:
		filePath = "courses2.txt"; // if ran without args use the default file
	}

	BinarySearchTree* bst = new BinarySearchTree(); // create our tree

	Course course; // create a course object

	int choice = 0; // this will be the user's choice
	string courseID; // string to hold the courseID for option 3
	while (choice != 9) { // while our choice is not 9 print menu
		cout << "Welcome to course planner:" << endl;
		cout << "1. Load Data Structure" << endl;
		cout << "2. Print course list" << endl;
		cout << "3. Print course" << endl;
		cout << "9. Exit" << endl;
		cout << "Enter choice: " << endl;
		cin >> choice; // get the users choice

		switch (choice) { // switch choice

		case 1:
			bst = new BinarySearchTree(); // remake the BST
			loadCourses(filePath, bst); // call the load courses with the file and tree object
			break;
		case 2: 
			bst->InOrder(); // call the inorder method which will print everything in alphabetical order
			break;

		case 3:
			cout << "Which course would you like to know about?" << endl;
			cin >> courseID; // get the course ID
			course = bst->Search(courseID); // search for the courseID
			if (!course.courseID.empty()) { // if it's found 
				printCourse(course); // print the info
			}
			else {
				cout << "CourseID: " << courseID << " not found." << endl; // otherwise say it is not found
			}
			break;
		
		case 9:
			cout << "Goodbye" << endl; // user ended loop
			break;
		
		default:
			cout << choice << " is not a valid choice!!" << endl; // used if invalid choice is made
		}
	}
}