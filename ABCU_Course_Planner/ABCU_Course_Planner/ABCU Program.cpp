//============================================================================
// Name        : ABCU Program.cpp
// Author      : Ron Causey
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <time.h>
#include <string>
#include <algorithm>
#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================


// define structure to hold course info
struct Course {
	string courseNum;
	string title;
	string preReqOne, preReqTwo;

};

// structure for tree node
struct Node {
	Course course;
	Node* left;
	Node* right;

	// default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// initialize with a course
	Node(Course aCourse) : Node() {
		this->course = aCourse;
	}

};

//============================================================================
// Binary Search Tree definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
	Node* root;

	void addNode(Node* node, Course course);
	void inOrder(Node* node);
	void preOrder(Node* node);
	void postOrder(Node* node);
	void RemoveSubTree(Node* node);
	Node* removeNode(Node* node, string courseNum);

public:
	BinarySearchTree();
	virtual ~BinarySearchTree();
	void InOrder();
	void PreOrder();
	void PostOrder();

	void Insert(Course course);
	void Remove(string courseNum);
	Course Search(string courseNum);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
	root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
	// recurse from root deleting every node
	RemoveSubTree(root);
}

void BinarySearchTree::RemoveSubTree(Node* node) {
	if (node != nullptr) {
		if (node->left != nullptr) {
			RemoveSubTree(node->left);
		}
		if (node->right != nullptr) {
			RemoveSubTree(node->right);
		}
		delete node;
	}
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
	this->inOrder(root);
}

/**
 * Traverse the tree in post-order
 */
void BinarySearchTree::PostOrder() {
	this->postOrder(root);
}

/**
 * Traverse the tree in pre-order
 */
void BinarySearchTree::PreOrder() {
	this->preOrder(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
	if (root == nullptr) {
		root = new Node(course);
	}
	else {
		this->addNode(root, course);
	}

}

/**
 * Remove a course
 */
void BinarySearchTree::Remove(string courseNum) {
	this->removeNode(root, courseNum);

}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseNum) {
	Node* current = root;

	while (current != nullptr) {
		if (current->course.courseNum.compare(courseNum) == 0) {
			return current->course;
		}
		if (courseNum.compare(current->course.courseNum) < 0) {
			current = current->left;
		}
		else {
			current = current->right;
		}
	}

	Course course;
	return course;
}

/**
 * Add a course to node
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
	if (node->course.courseNum.compare(course.courseNum) > 0) {
		// if no left node
		if (node->left == nullptr) {
			node->left = new Node(course);
		}
		else {
			this->addNode(node->left, course);
		}
	}
	else {
		// if no right node
		if (node->right == nullptr) {
			node->right = new Node(course);
		}
		else {
			this->addNode(node->right, course);
		}

	}
}

Node* BinarySearchTree::removeNode(Node* node, string courseNum) {
	if (node == nullptr) {
		return node;
	}
	if (courseNum.compare(node->course.courseNum) < 0) {
		node->left = removeNode(node->left, courseNum);
	}
	else if (courseNum.compare(node->course.courseNum) > 0) {
		node->right = removeNode(node->right, courseNum);
	}
	else {
		if (node->left == nullptr && node->right == nullptr) {
			delete node;
			node = nullptr;
		}
		// one child to left
		else if (node->left != nullptr && node->right == nullptr) {
			Node* temp = node;
			node = node->left;
			delete temp;
		}
		// one child to right
		else if (node->right != nullptr && node->right == nullptr) {
			Node* temp = node;
			node = node->right;
			delete temp;
		}
		// two children
		else {
			Node* temp = node->right;
			while (temp->left != nullptr) {
				temp = temp->left;
			}
			node->course = temp->course;
			node->right = removeNode(node->right, temp->course.courseNum);
		}
	}
	return node;
}

void BinarySearchTree::inOrder(Node* node) {
	if (node != nullptr) {
		inOrder(node->left);
		std::cout << node->course.courseNum << ", "
			<< node->course.title << std::endl;
		inOrder(node->right);
	}
}

void BinarySearchTree::postOrder(Node* node) {
	if (node != nullptr) {
		postOrder(node->left);
		postOrder(node->right);
		std::cout << node->course.courseNum << ": "
			<< node->course.title << std::endl;
	}

}

void BinarySearchTree::preOrder(Node* node) {
	if (node != nullptr) {
		std::cout << node->course.courseNum << ": "
			<< node->course.title << std::endl;
		preOrder(node->left);
		preOrder(node->right);
	}

}



/**
 * Display the course information to the console (std::out)
 *
 * @param course struct containing the course info
 */
void displayCourse(Course course) {
	std::cout << course.courseNum << ", " << course.title << std::endl;
	if (!course.preReqTwo.empty()) {
		std::cout << "Prerequisites: " << course.preReqOne << ", " << course.preReqTwo << std::endl;
	}
	else if (!course.preReqOne.empty()) {
		std::cout << "Prerequisites: " << course.preReqOne << std::endl;
	}
	std::cout << std::endl;

	return;
}

/**
 * Load a CSV file containing courses into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadCourses(string csvPath, BinarySearchTree* bst) {
	std::cout << "Loading CSV file " << csvPath << std::endl;

	csv::Parser file = csv::Parser(csvPath);

	std::cout << "" << std::endl;

	try {
		for (unsigned int i = 0; i < file.rowCount(); i++) {

			Course course;
			course.courseNum = file[i][0];
			course.title = file[i][1];
			course.preReqOne = file[i][2];
			course.preReqTwo = file[i][3];

			bst->Insert(course);
		}
	}
	catch (csv::Error& e) {
		std::cerr << e.what() << std::endl;
	}
}



/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

	string csvPath, courseKey;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		courseKey = "CSCI100";
		break;
	case 3:
		csvPath = argv[1];
		courseKey = argv[2];
		break;
	default:
		csvPath = "ABCU_Courses.csv";
		courseKey = "CSCI100";
	}

	BinarySearchTree* bst{};

	Course course;

	cout << "Welcome to the course planner \n" << std::endl;

	int choice = 0;
	while (choice != 4) {

		std::cout << "  1. Load Data Structure." << std::endl;
		std::cout << "  2. Print Course List." << std::endl;
		std::cout << "  3. Print Course." << std::endl;
		std::cout << "  9. Exit \n" << std::endl;

		std::cout << "What would you like to do? \n";
		std::cin >> choice;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		switch (choice) {

		case 1:
			bst = new BinarySearchTree();
			loadCourses(csvPath, bst);
			break;

		case 2:
			cout << "Here is a sample schedule: \n";
			cout << endl;
			bst->InOrder();
			cout << endl;
			break;

		case 3:
			cout << "What course do you want to know about? ";
			cin >> courseKey;
			std::for_each(courseKey.begin(), courseKey.end(), [](char& c) {
				c = ::toupper(c);});
			course = bst->Search(courseKey);

			if (!course.courseNum.empty()) {
				displayCourse(course);
			}
			else {
				std::cout << "Course Number " << courseKey << " not found." << std::endl;
			}

			break;

		case 9:
			break;

		default:
			std::cout << "That is not a valid option. \n" << std::endl;

			break;
		}
	}

	std::cout << "Thank you for using the course planner!" << std::endl;

	return 0;
}