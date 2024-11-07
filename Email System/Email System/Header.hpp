#pragma once
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct Email
{
	string sender;
	string recipient;
	string subject;
	string body;
	string priority;
	Email* next; // for linking emails in the stack or queue
};

struct User {
	std::string email;
	std::string password;
	std::string role;
	User* next;
};

// Define a Node structure for the linked list to store matched results
struct Node {
	string sender;
	string recipient;
	string subject;
	string body;
	Node* next;
};


void displayEmail(Email* email) {
	if (email == nullptr) {
		cout << "No email to display.\n";
		return;
	}
	cout << "\n========== Email ==========\n";
	cout << "Priority: " << email->priority << "\n";
	cout << "Sender: " << email->sender << "\n";
	cout << "Recipient: " << email->recipient << "\n";
	cout << "Subject: " << email->subject << "\n\n";
	cout << "Body: " << email->body << "\n";
	cout << string(27, '=') << endl << endl;;
}


class Stack
{
	Email* top;
public:
	Stack() : top(nullptr) {}

	//push email onto stack
	void push(string sender, string recipient, string subject, string body, string priority)
	{
		Email* newEmail = new Email{ sender, recipient, subject, body, priority, nullptr };
		
		// if the stac is empty or new email is high priority
		if (top == nullptr || priority == "High") {
			// insert high priority email at the front
			newEmail->next = top;
			top = newEmail;
		}
		else {
			// transverse to the end of the high priority email to insert the standard ones (recent -> old)
			Email* current = top;
			while (current->next != nullptr && current->next->priority == "High") {
				current = current->next;
			}
			// reach the last high priority email
			newEmail->next = current->next;
			current->next = newEmail;
		}
	}

	//Pop email from stack
	Email* pop()
	{
		if (isEmpty())
		{
			cout << "Inbox is empty\.";
			return nullptr;
		}
		Email* emailToReturn = top;
		top = top->next;
		return emailToReturn;
	}

	Email* peek()
	{
		if (isEmpty())
			return nullptr;
		return top;
	}

	bool isEmpty()
	{
		if (top == nullptr)
			return true;
		return false;
	}
	// Load emails from "Inbox.csv" file
	void loadFromFile(const string& filename)
	{
		ifstream file(filename);
		string sender, recipient, subject, body, priority;

		while (file.good())
		{
			// Read the sender, if the line is empty, skip it
			if (!getline(file, sender, ',')) continue;

			// Ensure that the other fields are read properly, or skip the line
			if (!getline(file, recipient, ',')) continue;
			if (!getline(file, subject, ',')) continue;
			if (!getline(file, body, ',')) continue;
			if (!getline(file, priority, '\n')) continue;

			// Push valid data into the stack
			push(sender, recipient, subject, body, priority);
		}
		file.close();
	}


	void saveToFile(const string& filename)
	{
		// Open in write mode (not append) to overwrite existing content
		ofstream file(filename);

		if (!file.is_open()) {
			cout << "Error: Unable to open file for writing.\n";
			return;
		}

		Email* current = top;
		while (current != nullptr)
		{
			file << current->sender << "," << current->recipient << ","
				<< current->subject << "," << current->body << "," 
				<< current->priority << "\n";
			current = current->next;
		}

		file.close();
	}


	void viewReceivedEmails(const string& userEmail)
	{
		Email* current = top;  // Assuming 'top' points to the first email
		bool found = false;

		// Traverse the linked list or stack/queue of emails
		while (current != nullptr)
		{
			if (current->recipient == userEmail)
			{
				displayEmail(current);  // Assuming this function displays email details
				found = true;
			}

			// Move to the next email in the list
			current = current->next;
		}

		// If no emails were found for the recipient
		if (!found) {
			cout << "No emails found where you are the recipient.\n";
		}
	}


	void displayRecentEmails(Stack& inbox, const string& loggedInEmail) {
		Email* current = inbox.peek(); // Start with the most recent email (top of the stack)
		int index = 1;

		// Loop through the inbox stack and only display emails where the recipient is the logged-in user
		while (index < 1)
		{
			while (current != nullptr) {
				if (current->recipient == loggedInEmail) {
					cout << "Email No." << index << endl;
					cout << ". Sender: " << current->sender << endl;
					cout << "Subject: " << current->subject << endl;
					cout << "Body: " << current->body << endl;
					index++;
				}
				current = current->next; // Move to the next email in the stack
			}

			if (index == 1) {
				cout << "No emails found for " << loggedInEmail << ".\n";
			}
		}
	}
};

class Queue
{
	Email* front;
	Email* rear;
public:
	Queue() : front(nullptr), rear(nullptr) {}

	void clear() {
		Email* current = front;
		while (current != nullptr) {
			Email* nextEmail = current->next; // Store the next email
			delete current; // Delete the current email
			current = nextEmail; // Move to the next email
		}
		front = nullptr; // Reset front pointer
		rear = nullptr;  // Reset rear pointer
	}

	Email* getFront()
	{
		return front;
	}
	Email* peek() {
		if (isEmpty()) {
			cout << "Outbox is empty!\n";
			return nullptr;
		}
		return rear;
	}

	//Enqueue email
	void enqueue(string sender, string recipient, string subject, string body, string priority)
	{
		Email* newEmail = new Email{ sender, recipient, subject, body, priority, nullptr };
		if (rear == nullptr)
		{
			front = rear = newEmail;
		}
		else
		{
			rear->next = newEmail;
			rear = newEmail;
		}
	}

	//Dequeue email
	Email* dequeue() {
		if (isEmpty()) {
			cout << "Outbox is empty!\n";
			return nullptr;
		}
		Email* emailToReturn = front;
		front = front->next;

		if (front == nullptr) {  // If we dequeued the last element
			rear = nullptr;
		}
		emailToReturn->next = nullptr;  // Ensure the dequeued email doesn't point to anything

		return emailToReturn;
	}


	bool isEmpty()
	{
		if (front == nullptr)
			return true;
		return false;
	}

	// Load emails from "Outbox.csv" file
	void loadFromFile(const string& filename)
	{
		ifstream file(filename);
		string sender, recipient, subject, body, priority;

		while (file.good())
		{
			// Read the sender, if the line is empty, skip it
			if (!getline(file, sender, ',')) continue;

			// Ensure that the other fields are read properly, or skip the line
			if (!getline(file, recipient, ',')) continue;
			if (!getline(file, subject, ',')) continue;
			if (!getline(file, body, ',')) continue;
			if (!getline(file, priority, '\n')) continue;

			// Push valid data into the stack
			enqueue(sender, recipient, subject, body, priority);
		}
		file.close();
	}

	Email* getRear()
	{
		return rear;
	}
	void saveToFile(const string& filename)
	{
		ofstream file(filename);  // Open file in default mode, which overwrites the file

		Email* current = front;
		while (current != nullptr) {
			file << current->sender << "," << current->recipient << ","
				<< current->subject << "," << current->body << "\n";
			current = current->next;
		}

		file.close();
	}

	void displayOutboxWithIndex(const string& userEmail) {
		Email* current = front;
		int index = 0;

		cout << "Outbox Emails:\n";
		while (current != nullptr) {
			// Only display emails that belong to the logged-in user
			if (current->sender == userEmail) {
				cout << index + 1 << ". Sender: " << current->sender
					<< ", Recipient: " << current->recipient
					<< ", Subject: " << current->subject 
					<< ", Priority: " << current->priority << endl;
				index++;
			}
			current = current->next;  // Move to the next email
		}

		if (index == 0) {
			cout << "No emails in outbox for " << userEmail << ".\n";
		}
	}
};

class LinkedList {
	struct Node {
		std::string email;
		Node* next;
	};
	Node* head;

public:
	LinkedList() : head(nullptr) {}

	void add(const std::string& email) {
		if (contains(email)) return;  // Avoid duplicates
		Node* newNode = new Node{ email, nullptr };
		if (!head) {
			head = newNode;
		}
		else {
			Node* current = head;
			while (current->next) {
				current = current->next;
			}
			current->next = newNode;
		}
	}

	bool contains(const std::string& email) const {
		Node* current = head;
		while (current) {
			if (current->email == email) return true;
			current = current->next;
		}
		return false;
	}

	bool isEmpty() const {
		return head == nullptr;
	}

	void displayAndChoose(std::string& chosenEmail) {
		if (isEmpty()) {
			cout << "No matches found.\n";
			return;
		}

		int option;
		Node* current;

		// Display options and prompt user until a valid selection is made
		while (true) {
			int displayOption = 1;
			current = head;

			// Display each email with an option number
			while (current) {
				cout << displayOption++ << ". " << current->email << "\n";
				current = current->next;
			}

			cout << "\nSelect an email from the options: ";
			cin >> option;

			// Check if the option is within the valid range
			if (option >= 1 && option < displayOption) {
				// Retrieve the chosen email
				current = head;
				for (int i = 1; i < option; ++i) {
					current = current->next;
				}
				chosenEmail = current->email;
				break; // Valid choice, exit the loop
			}
			else {
				cout << "\nInvalid, please try again.\n\n";
			}
		}
	}
};


class Admin {
	User* head;  // Linked list to store users temporarily

public:
	Admin() : head(nullptr) {}

	// Load users from file into linked list (temporary storage)
	void loadUsers(const std::string& filename) {
		std::ifstream file(filename);
		std::string email, password, role;
		while (file.good())
		{
			// Read the sender, if the line is empty, skip it
			if (!getline(file, email, ',')) continue;

			// Ensure that the other fields are read properly, or skip the line
			if (!getline(file, password, ',')) continue;
			if (!getline(file, role, '\n')) continue;
			addUserToList(email, password, role, false);  // Adding to list silently

		}
		file.close();
	}

	// Add user to linked list (temporary storage)
	void addUserToList(const std::string& email, const std::string& password, const std::string& role, bool displayMessage = true) {
		User* newUser = new User{ email, password, role, nullptr };
		if (head == nullptr) {
			head = newUser;
		}
		else {
			User* current = head;
			while (current->next != nullptr) {
				current = current->next;
			}
			current->next = newUser;
		}
		if (displayMessage) {
			std::cout << "User added.\n";
		}
	}

	// Add new user (temporary)
	void addUser() {
		std::string email, password, role;
		std::cout << "Enter email: ";
		std::cin >> email;
		std::cout << "Enter password: ";
		std::cin >> password;
		std::cout << "Enter role (admin/user): ";
		std::cin >> role;

		addUserToList(email, password, role);
	}

	// Delete user (temporary)
	void deleteUser() {
		std::string email;
		std::cout << "Enter email of user to delete: ";
		std::cin >> email;

		User* current = head;
		User* prev = nullptr;

		while (current != nullptr) {
			if (current->email == email) {
				if (prev == nullptr) {
					head = current->next;
				}
				else {
					prev->next = current->next;
				}
				delete current;
				std::cout << "User deleted.\n";
				return;
			}
			prev = current;
			current = current->next;
		}
		std::cout << "User not found.\n";
	}

	// Modify user (temporary)
	void modifyUser() {
		std::string email;
		std::cout << "Enter email of user to modify: ";
		std::cin >> email;

		User* current = head;
		while (current != nullptr) {
			if (current->email == email) {
				std::cout << "Enter new password: ";
				std::cin >> current->password;
				std::cout << "Enter new role (admin/user): ";
				std::cin >> current->role;
				std::cout << "User modified.\n";
				return;
			}
			current = current->next;
		}
		std::cout << "User not found.\n";
	}

	// Display all users from the temporary list
	void displayUsers() {
		User* current = head;
		if (current == nullptr) {
			std::cout << "No users loaded.\n";
		}
		else {
			std::cout << "\nCurrent Users: \n";
			while (current != nullptr) {
				std::cout << "Email: " << current->email << ", Role: " << current->role << "\n";
				current = current->next;
			}
		}
	}

	// Destructor to free memory
	~Admin() {
		User* current = head;
		while (current != nullptr) {
			User* next = current->next;
			delete current;
			current = next;
		}
	}
};

void displayWelcomeScreen() {
	cout << "======================================================" << endl;
	cout << "||             Welcome to Email Management           ||" << endl;
	cout << "||                  System by Group11                ||" << endl;
	cout << "======================================================" << endl;
	cout << "\n\n";
	cout << "------------------- Login Page -----------------------" << endl;
	cout << "\n\n\nPlease enter your login details to proceed" << endl;
}


void writeEmail(Queue& outbox, const string& email) {

	string recipient, subject, body, priority;

	// Get recipient email
	while (true) {
		cout << "Enter recipient email: ";
		getline(cin, recipient);
		if (!recipient.empty()) {
			break;  // Exit loop if input is valid
		}
		cout << "Recipient email cannot be empty. Please try again.\n";
	}

	// Get subject
	while (true) {
		cout << "Enter subject: ";
		getline(cin, subject);
		if (!subject.empty()) {
			break;  // Exit loop if input is valid
		}
		cout << "Subject cannot be empty. Please try again.\n";
	}

	// Get body
	while (true) {
		cout << "Enter body: ";
		getline(cin, body);
		if (!body.empty()) {
			break;  // Exit loop if input is valid
		}
		cout << "Body cannot be empty. Please try again.\n";
	}

	// Get body
	while (true) {
		cout << "Enter Priority of Email (0 = Standard, 1 = High): ";
		getline(cin, priority);
		if (!priority.empty()) {
			if (priority == "0") {
				priority = "Standard";
				break;
			}
			else if (priority == "1") {
				priority = "High";
				break;
			}
			else {
				cout << "Invalid Input! Please Try Again." << endl;
			}
		}
		cout << "Priority cannot be empty. Please try again.\n";
	}

	// Assuming Email has a constructor that takes these 
	outbox.enqueue(email, recipient, subject, body, priority);
}


void removeEmailFromFile(const string& filename, Email* emailToRemove) {
	ifstream file(filename);
	ofstream tempFile("temp.csv");

	string line;
	bool emailFound = false;

	// Read each line from the original file and write it to the temp file,
	// except for the one that matches the emailToRemove
	while (getline(file, line)) {
		// Assuming the CSV format is sender,recipient,subject,body
		size_t pos = 0;
		string tokens[4];
		int i = 0;

		// Tokenize the line
		while ((pos = line.find(',')) != string::npos) {
			tokens[i++] = line.substr(0, pos);
			line.erase(0, pos + 1);
		}
		tokens[i] = line; // Add the last part of the body

		// Compare the current line with emailToRemove's fields
		if (tokens[0] == emailToRemove->sender &&
			tokens[1] == emailToRemove->recipient &&
			tokens[2] == emailToRemove->subject &&
			tokens[3] == emailToRemove->body) {
			emailFound = true;
			continue; // Skip writing this email to the temp file
		}

		// Write the email to the temp file if it doesn't match emailToRemove
		tempFile << tokens[0] << "," << tokens[1] << "," << tokens[2] << "," << tokens[3] << "\n";
	}

	file.close();
	tempFile.close();

	// Replace the original file with the temp file
	remove(filename.c_str());
	rename("temp.csv", filename.c_str());

	if (emailFound) {
		cout << "Email removed from " << filename << "\n";
	}
	else {
		cout << "Email not found in " << filename << "\n";
	}
}


void appendToFile(const string& filename, Email* email)
{
	ofstream file(filename, ios::app); // Open in append mode to add only the new email

	if (!file.is_open()) {
		cout << "Error: Unable to open file for appending.\n";
		return;
	}

	file << email->sender << "," << email->recipient << ","
		<< email->subject << "," << email->body << "\n";

	file.close();
}


bool login(const string& filename, string& role, string& userEmail) {
	ifstream file(filename);
	string password, storedEmail, storedPassword, storedRole;

	if (!file.is_open()) {
		cout << "Error opening login file.\n";
		return false;
	}

	cout << "Enter email: ";
	cin >> userEmail;
	cout << "Enter password: ";
	cin >> password;

	while (file.good())
	{
		getline(file, storedEmail, ',');
		getline(file, storedPassword, ',');
		getline(file, storedRole, '\n');

		if (storedEmail == userEmail && storedPassword == password)
		{
			role = storedRole;
			userEmail = storedEmail;
			return true;
		}
	}

	file.close();
	cout << "Invalid email or password.\n";
	return false;
}

const int MAX_EMAILS = 1000;

void checkForDuplicates(Stack& inbox) {
	std::string emails[MAX_EMAILS];
	int emailCount = 0;
	bool foundDuplicate = false;

	Stack tempStack;

	while (!inbox.isEmpty()) {
		Email* email = inbox.pop();

		std::string emailString = email->sender + "," + email->recipient + "," + email->subject + "," + email->body;

		bool isDuplicate = false;
		for (int i = 0; i < emailCount; i++) {
			if (emails[i] == emailString) {
				isDuplicate = true;
				foundDuplicate = true;
				std::cout << "Duplicate email found: " << email->subject << "\n";

				std::cout << "Do you want to view this email? (1 = Yes, 0 = No): ";
				int choice;
				std::cin >> choice;
				std::cin.ignore();

				if (choice == 1) {
					displayEmail(email);

					std::cout << "Do you want to remove this email? (1 = Yes, 0 = No): ";
					std::cin >> choice;
					std::cin.ignore();

					if (choice == 1) {
						std::cout << "Email deleted!\n";
						delete email;
						email = nullptr;
						break;
					}
				}

				if (email != nullptr) {
					tempStack.push(email->sender, email->recipient, email->subject, email->body, email->priority);
				}
				break;
			}
		}

		if (!isDuplicate) {
			if (emailCount < MAX_EMAILS) {
				emails[emailCount++] = emailString;
				tempStack.push(email->sender, email->recipient, email->subject, email->body, email->priority);
			}
			else {
				std::cerr << "Error: Maximum email limit reached.\n";
				delete email;
			}
		}
		else {
			delete email;
		}
	}

	while (!tempStack.isEmpty()) {
		Email* email = tempStack.pop();
		inbox.push(email->sender, email->recipient, email->subject, email->body, email->priority);
	}

	if (!foundDuplicate) {
		std::cout << "No duplicate emails found.\n";
	}
}

void loadAndSuggestEmails(const string& filename, const string& partialEmail, LinkedList& suggestions, bool isInbox) {
	ifstream file(filename);
	string sender, recipient, subject, body, priority;

	while (file.good()) {
		getline(file, sender, ',');
		getline(file, recipient, ',');
		getline(file, subject, ',');
		getline(file, body, ',');
		getline(file, priority, '\n');

		string emailField = isInbox ? recipient : sender;
		if (emailField.find(partialEmail) != string::npos) {
			suggestions.add(emailField);  // Add only if not already in list
		}
	}
	file.close();
}

int countOccurrences(const string& filename, const string& email, bool isInbox) {
	ifstream file(filename);
	string sender, recipient, subject, body, priority;
	int count = 0;

	while (file.good()) {
		getline(file, sender, ',');
		getline(file, recipient, ',');
		getline(file, subject, ',');
		getline(file, body, ',');
		getline(file, priority, '\n');

		if ((isInbox && recipient == email) || (!isInbox && sender == email)) {
			count++;
		}
	}
	file.close();
	return count;
}


// Function to add a node to the linked list
void addNode(Node*& head, const string& sender, const string& recipient, const string& subject, const string& body) {
	Node* newNode = new Node{ sender, recipient, subject, body, nullptr };
	if (!head) {
		head = newNode;
	}
	else {
		Node* current = head;
		while (current->next) {
			current = current->next;
		}
		current->next = newNode;
	}
}

// Function to display the linked list and let user choose
void displayAndChoose(Node* head) {
	if (!head) {
		cout << "No matching subjects found.\n";
		return;
	}

	while (true) {
		int index = 1;
		Node* current = head;

		// Display all matched subjects
		cout << "\nMatching Emails:\n";
		while (current) {
			cout << index << ". Subject: " << current->subject << endl;
			index++;
			current = current->next;
		}

		// Get user's choice
		int choice;
		cout << "Enter the number of the email you want to view (or 0 to exit): ";
		cin >> choice;

		if (choice == 0) {
			cout << "Exiting selection.\n";
			return;
		}

		if (choice > 0 && choice < index) {
			// Reset the current pointer to head and navigate to the chosen email
			current = head;
			for (int i = 1; i < choice; ++i) {
				current = current->next;
			}
			// Display the chosen email details
			cout <<endl<< string(55, '=') << endl;
			cout << "\nFrom: " << current->sender << "," << endl;
			cout << "\nSubject: " << current->subject << endl;
			cout << "\n\nBody: " << current->body << endl << endl;
			cout << string(55, '=') << endl << endl << endl;

			// Ask if the user wants to continue
			char continueChoice;
			cout << "Do you want to view another email? (y/n): ";
			cin >> continueChoice;

			if (continueChoice == 'n' || continueChoice == 'N') {
				cout << "Returning to main menu.\n";
				return;
			}
		}
		else {
			cout << "Invalid choice. Please try again.\n";
		}
	}
}

void displayEmailDetails(const string& filename, const string& email, bool isInbox) {
	ifstream file(filename);
	string sender, recipient, subject, body, priority;
	bool emailFound = false;

	while (file.good()) {
		getline(file, sender, ',');
		getline(file, recipient, ',');
		getline(file, subject, ',');
		getline(file, body, ',');
		getline(file, priority, '\n');

		// Check if the email matches the chosen one based on inbox or outbox
		if ((isInbox && recipient == email) || (!isInbox && sender == email)) {

			// Display appropriate information based on inbox or outbox
			cout << string(55, '=') << endl;
			cout << (isInbox ? "From: " : "To: ") << (isInbox ? sender : recipient) << "," << endl;

			cout << "\nSubject: " << subject << endl;

			cout << "\n\nBody: " << body << endl << endl;
			cout << string(55, '=') << endl << endl<<endl;
			emailFound = true;
		}
	}
	file.close();
	// Display message if no emails are found for the selected option
	if (!emailFound) {
		cout << endl << string(55, '=') << endl;
		cout << "Currently nothing inside " << (isInbox ? "inbox" : "outbox") << ".\n";
		cout << endl << string(55, '=') << endl<<endl;
	}

	
}

void SearchAndRetrieveEmail() {
	cout << "Search and Retrieve:\n";
	cout << string(23, '=') << endl;
	cout << "Please enter the email for retrieve: ";
	string partialEmail;
	cin >> partialEmail;

	LinkedList suggestions;
	loadAndSuggestEmails("inbox.csv", partialEmail, suggestions, true);
	loadAndSuggestEmails("Outbox.csv", partialEmail, suggestions, false);

	if (suggestions.isEmpty()) {
		cout << "No email found.\n";
		return;
	}
	cout << "\nWe've found some similiar emails, please choose the options:" << endl << endl;
	string chosenEmail;
	suggestions.displayAndChoose(chosenEmail);

	// Count the number of emails received in the Inbox and Outbox
	int countForInbox = countOccurrences("inbox.csv", chosenEmail, true);
	int countForOutbox = countOccurrences("outbox.csv", chosenEmail, false);

	cout << chosenEmail << " has " << countForInbox << " inbox, and " << countForOutbox << " outbox.\n";

	//Display retrieve options , using loop 
	while (true) {
		// Display retrieve options
		cout << "\nRetrieve from:\n";
		cout << "1. Inbox\n";
		cout << "2. Outbox\n";
		cout << "0. Return back\n";
		cout << "\nPlease enter the options: ";

		int option;
		cin >> option;

		switch (option) {
		case 1:
			if (countForInbox == 0) {
				cout << endl << string(23, '=') << endl;
				cout << "\nNothing else in inbox.\n";
				cout << endl << string(23, '=') << endl << endl;
			}
			else {
				displayEmailDetails("inbox.csv", chosenEmail, true);
			}
			break;

		case 2:
			if (countForOutbox == 0) {
				cout << endl << string(23, '=') << endl;
				cout << "\nNothing else in outbox.\n";
				cout << endl << string(23, '=') << endl << endl;
			}
			else {
				displayEmailDetails("outbox.csv", chosenEmail, false);
			}
			break;

		case 0:
			return; // Exit the retrieve menu

		default:
			cout << "Invalid option, please try again.\n";
			continue; // Go back to the start of the loop
		}

		// If a valid option is chosen, prompt for continuation and break out of the loop
		cout << "Do you want to continue? (y/n): ";
		char choice;
		cin >> choice;
		if (choice == 'y' || choice == 'Y') {
			// Return to main menu or restart function
			break; // Exit the retrieve options loop to return to the main program
		}
		else {
			cout << "\nThank you for using our Email System!" << endl;
			exit(0);  // Exit the program
		}
	}
}

void displayUserEmailDetails(const string& filename, const string& userEmail, bool isInbox) {
	ifstream file(filename);
	if (!file.is_open()) {
		cout << "Error opening file: " << filename << endl;
		return;
	}

	string sender, recipient, subject, body, priority;
	bool emailFound = false;

	while (getline(file, sender, ',')) {
		getline(file, recipient, ',');
		getline(file, subject, ',');
		getline(file, body, ',');
		getline(file, priority, '\n');

		// Check if the email matches the user's email in the inbox or outbox
		if ((isInbox && recipient == userEmail) || (!isInbox && sender == userEmail)) {
			// Display format based on inbox or outbox
			cout << string(55, '=') << endl;
			cout << (isInbox ? "From: " : "To: ") << (isInbox ? sender : recipient) << endl;
			cout << "\nSubject: " << subject << endl;
			cout << "\n\nBody: " << body << endl << endl;
			cout << string(55, '=') << endl << endl;
			emailFound = true;
		}
	}
	file.close();

	if (!emailFound) {
		cout << "No emails found in " << (isInbox ? "inbox" : "outbox") << " for " << userEmail << ".\n";
	}
}


// Function to search for the subject in inbox and outbox files
void searchAndRetrieveSubject(const string& userEmail) {
	string searchTerm;
	cout << "Enter a search term for the subject: ";
	cin >> searchTerm;

	// Linked list to store matching emails
	Node* matches = nullptr;

	// Search in inbox.csv
	ifstream inboxFile("inbox.csv");
	if (inboxFile.is_open()) {
		string sender, recipient, subject, body, priority;
		while (getline(inboxFile, sender, ',') &&
			getline(inboxFile, recipient, ',') &&
			getline(inboxFile, subject, ',') &&
			getline(inboxFile, body, ',') &&
			getline(inboxFile, priority, '\n')) {
			if (subject.find(searchTerm) != string::npos) {
				addNode(matches, sender, recipient, subject, body);
			}
		}
		inboxFile.close();
	}
	else {
		cout << "Error opening inbox.csv file.\n";
	}

	// Search in outbox.csv
	ifstream outboxFile("C:outbox.csv");
	if (outboxFile.is_open()) {
		string sender, recipient, subject, body, priority;
		while (getline(outboxFile, sender, ',') &&
			getline(outboxFile, recipient, ',') &&
			getline(outboxFile, subject, ',') &&
			getline(outboxFile, body, ',') &&
			getline(outboxFile, priority, '\n')) {
			if (subject.find(searchTerm) != string::npos) {
				addNode(matches, sender, recipient, subject, body);
			}
		}
		outboxFile.close();
	}
	else {
		cout << "Error opening outbox.csv file.\n";
	}

	// Display matched subjects and let the user choose one to view
	displayAndChoose(matches);

	// Clean up linked list memory
	while (matches) {
		Node* temp = matches;
		matches = matches->next;
		delete temp;
	}
}

const string spamWords[] = {
	"Free", "Guarantee", "Urgent", "Winner", "Congratulations", "Act now",
	"Click here", "Exclusive deal", "Limited time", "No cost", "Risk-free",
	"Offer expires", "100% free", "Apply now", "Earn cash", "Save big",
	"Win big", "Double your income", "Cash bonus", "Cheap", "Promise you",
	"Fast cash", "Increase sales", "Lowest price", "No obligation",
	"Money back", "Million dollars", "Best price", "Satisfaction guaranteed",
	"Credit card required", "Miracle cure", "Get paid", "Earn extra income",
	"Work from home", "Urgent response", "Act immediately", "Be your own boss",
	"Join free", "Call now", "Eliminate debt", "Extra income", "Limited availability",
	"Special promotion", "YouÅfre a winner", "Lowest rates", "Fast approval",
	"Apply now", "As seen on", "Get it now", "No hidden fees"
};

string toLowerCase(const string& str) {
	string lowerStr = str;  // Copy original string
	for (size_t i = 0; i < lowerStr.length(); ++i) {
		if (lowerStr[i] >= 'A' && lowerStr[i] <= 'Z') {
			lowerStr[i] = lowerStr[i] + ('a' - 'A');  // Convert to lowercase
		}
	}
	return lowerStr;
}

// Function to check if an email contains spam words
bool containsSpam(const Email& email, const string spamWords[], int numWords) {
	// Convert subject and body to lowercase
	string lowerSubject = toLowerCase(email.subject);
	string lowerBody = toLowerCase(email.body);

	// Check if any spam word is found in the subject or body (case-insensitive)
	for (int i = 0; i < numWords; ++i) {
		string lowerSpamWord = toLowerCase(spamWords[i]);
		if (lowerSubject.find(lowerSpamWord) != string::npos ||
			lowerBody.find(lowerSpamWord) != string::npos) {
			return true;
		}
	}
	return false;
}

// Function to display and prompt for spam email handling
void handleSpamEmails(Stack& spamStack) {
	Email* current = spamStack.peek();  // Start with the top email
	while (current != nullptr) {
		cout << "Spam Email Found:\n";
		cout << "Sender: " << current->sender << "\n";
		cout << "Recipient: " << current->recipient << "\n";
		cout << "Subject: " << current->subject << "\n";
		cout << "Body: " << current->body << "\n";
		cout << "Priority: " << current->priority << "\n";

		// Prompt the user to keep or delete the spam email
		char choice;
		cout << "Do you want to delete this email? (y/n): ";
		cin >> choice;
		if (choice == 'y' || choice == 'Y') {
			spamStack.pop();  // Remove the email from the stack if the user chooses to delete
		}
		else {
			spamStack.pop();  // Keep the email in the stack, but pop it so we can move to the next email
		}

		// Check if there are more emails left in the stack
		current = spamStack.peek();  // Move to the next email (after pop)
	}
}
