// cs300BSTproject2.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

//moved this from the header file to the main file to be in line with the project requirements.
class courseNode {
	// This class represents a node in the binary search tree (BST) for course data
private:
	std::string courseID;
	std::string courseName;
	courseNode* right;
	courseNode* left;
	std::vector<std::string> prerequisites;

public:
	// Constructor to initialize a course node with ID, name, and prerequisites
	courseNode(std::string id, std::string name) : courseID(id), courseName(name), right(nullptr), left(nullptr) {}
	courseNode() : courseID(NULL), courseName(NULL), right(nullptr), left(nullptr), prerequisites({}) {}
	// Getters for course ID, name, left and right child nodes, and prerequisites
	std::string getCourseID() const { return courseID; }
	std::string getCourseName() const { return courseName; }
	courseNode* getRight() const { return right; }
	courseNode* getLeft() const { return left; }
	std::vector<std::string> getPrerequisites() const { return prerequisites; }
	// Setters for right and left child nodes
	void setRight(courseNode* node) { right = node; }
	void setLeft(courseNode* node) { left = node; }
	void addPrerequisite(const std::string& prereq) { prerequisites.push_back(prereq); }

};

// InsertNode function to insert a course into the BST
void insertNode(courseNode*& root, courseNode* newCourse) {  // had an issue not usoing & to pass by reference
	// If the tree is empty, set the new course as the root
	if (root == nullptr) {
		root = newCourse;
	}
	// if the new course ID is less than the current node's ID, insert into the left subtree
	else if (newCourse->getCourseID() < root->getCourseID()) {
		courseNode* left = root->getLeft();
		insertNode(left, newCourse);
		root->setLeft(left);
	}
	// if the new course ID is greater than the current node's ID, insert into the right subtree
	else {
		courseNode* right = root->getRight();
		insertNode(right, newCourse);
		root->setRight(right);
	}
}


// readFile function to read the CSV file and create the BST
courseNode* readFile(const std::string& filename) {
	courseNode* root = nullptr;
	std::ifstream file(filename);
	// Check if the file is open
	if (!file.is_open()) {
		std::cerr << "Error: Could not open file.\n";
		return nullptr;
	}
	// Read each line from the file
	// this was a separate function in the sudo code. It was factory, but I was overly modular
	std::string line;
	while (std::getline(file, line)) {
		std::istringstream ss(line);
		std::string courseID, courseName, prereq;
		std::getline(ss, courseID, ',');
		std::getline(ss, courseName, ',');
		courseNode* newCourse = new courseNode(courseID, courseName);
		while (std::getline(ss, prereq, ',')) {
			newCourse->addPrerequisite(prereq);
		}
		insertNode(root, newCourse);
	}
	file.close();
	return root;
}


void printDataStructure(courseNode* courseNode) {
	//Save computation time by not printing empty nodes
	if (courseNode == nullptr) return;
	//print in acending order recursively
	printDataStructure(courseNode->getLeft());
	std::cout << courseNode->getCourseID() << ", " << courseNode->getCourseName() << "\n";
	printDataStructure(courseNode->getRight());
}


void searchCourse(courseNode* courseNode, const std::string& targetID) {
	// if it reaches the leaf node without finding the targetID
	if (courseNode == nullptr) {
		std::cout << "Course not found.\n";
		return;
	}
	// if the targetID is found
	if (courseNode->getCourseID() == targetID) {
		std::cout << "Course ID: " << courseNode->getCourseID() << "\n";
		std::cout << "Course Name: " << courseNode->getCourseName() << "\n";
		std::cout << "Prerequisites: ";
		for (const auto& prereq : courseNode->getPrerequisites()) {
			std::cout << prereq << " ";
		}
		std::cout << "\n";
		return;
	}
	// if the targetID is less than the current node's ID, search left
	if (targetID < courseNode->getCourseID()) {
		searchCourse(courseNode->getLeft(), targetID);
	}
	// if the targetID is greater than the current node's ID, search right
	else {
		searchCourse(courseNode->getRight(), targetID);
	}
}

// ease of life function to convert a string to uppercase so seaching courses isn't so painful
std::string toUpperAlpha(const std::string& input) {
	std::string result = input;
	for (char& c : result) {
		if (std::isalpha(static_cast<unsigned char>(c))) {
			c = std::toupper(static_cast<unsigned char>(c));
		}
	}
	return result;
}

int main()
{
	// initialzing static vars
	std::string fileName = "ABCU_Advising_Program_input.csv";
	courseNode* myDataStructure = nullptr;

	// Display the menu
	int choice;
	do {
		// Display menu options
		std::cout << "Menu:\n";
		std::cout << "1 - Load Data from File\n";
		std::cout << "2 - Sort and Display Courses\n";
		std::cout << "3 - Search for Course by ID\n";
		std::cout << "9 - Exit\n";
		std::cout << "Enter your choice: ";
		std::cin >> choice;
		
		// Check for invalid input
		if (std::cin.fail()) {
			std::cin.clear(); // Clear error flag
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Remove invalid input
			std::cout << "Invalid input. Please enter a valid number.\n";
			choice = -1; // Set to invalid option to continue the loop
			continue;
		}

		switch (choice) {
		case 1:
			myDataStructure = readFile(fileName);
			break;
		case 2:
			// presorted via insertNode function and the intrenal structure of the BST
			printDataStructure(myDataStructure);
			break;
		case 3: {
			std::string targetID;
			std::cout << "Enter Course ID to search for: ";
			std::cin >> targetID;
			searchCourse(myDataStructure, toUpperAlpha(targetID)); // convert to uppercase targetID and passes result instead of literal value.
			break;
		}
		case 9:
			std::cout << "Exiting program.\n";
			break;
		default:
			std::cout << "Invalid option. Please try again.\n";
		}
	} while (choice != 9);
	return 0;
}