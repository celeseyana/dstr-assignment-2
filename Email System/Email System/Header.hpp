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
	Email* next; // for linking emails in the stack or queue
};

struct User {
	std::string email;
	std::string password;
	std::string role;
	User* next;
};
void displayEmail(Email* email) {
	if (email == nullptr) {
		cout << "No email to display.\n";
		return;
	}
	cout << "\n----- Email -----\n";
	cout << "Sender: " << email->sender << "\n";
	cout << "Recipient: " << email->recipient << "\n";
	cout << "Subject: " << email->subject << "\n";
	cout << "Body: " << email->body << "\n";
	cout << "-----------------\n\n";
}
class Stack
{
	Email* top;
public:
	Stack() : top(nullptr) {}

	//push email onto stack
	void push(string sender, string recipient, string subject, string body)
	{
		Email* newEmail = new Email{ sender, recipient, subject, body, top };
		top = newEmail;
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
		string sender, recipient, subject, body;

		while (file.good())
		{
			// Read the sender, if the line is empty, skip it
			if (!getline(file, sender, ',')) continue;

			// Ensure that the other fields are read properly, or skip the line
			if (!getline(file, recipient, ',')) continue;
			if (!getline(file, subject, ',')) continue;
			if (!getline(file, body, '\n')) continue;

			// Push valid data into the stack
			push(sender, recipient, subject, body);
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
				<< current->subject << "," << current->body << "\n";
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
		while (current != nullptr) {
			if (current->recipient == loggedInEmail) {
				cout << index << ". Sender: " << current->sender << endl;
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
};

class Queue
{
	Email* front;
	Email* rear;
public:
	Queue() : front(nullptr), rear(nullptr) {}

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
	void enqueue(string sender, string recipient, string subject, string body)
	{
		Email* newEmail = new Email{ sender, recipient, subject, body, nullptr };
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
		string sender, recipient, subject, body;

		while (file.good())
		{
			// Read the sender, if the line is empty, skip it
			if (!getline(file, sender, ',')) continue;

			// Ensure that the other fields are read properly, or skip the line
			if (!getline(file, recipient, ',')) continue;
			if (!getline(file, subject, ',')) continue;
			if (!getline(file, body, '\n')) continue;

			// Push valid data into the stack
			enqueue(sender, recipient, subject, body);
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
		Email* current = front;  // Assuming you have a front pointer for your queue
		int index = 1;

		cout << "Outbox Emails:\n";
		while (current != nullptr) {
			// Only display emails that belong to the logged-in user
			if (current->sender == userEmail) {
				cout << index << ". Sender: " << current->sender
					<< ", Recipient: " << current->recipient
					<< ", Subject: " << current->subject << endl;
				index++;
			}
			current = current->next;  // Move to the next email
		}

		if (index == 1) {
			cout << "No emails in outbox for " << userEmail << ".\n";
		}
	}
};

//class Admin {
//public:
//	// Function to add a new user
//	void addUser(const string& filename) {
//		string email, password, role;
//		cout << "Enter new user's email: ";
//		getline(cin, email);
//		cout << "Enter new user's password: ";
//		getline(cin, password);
//		cout << "Enter new user's role (user/admin): ";
//		getline(cin, role);
//
//		ofstream file(filename, ios::app); // Open in append mode
//		if (!file.is_open()) {
//			cout << "Error: Unable to open the file.\n";
//			return;
//		}
//
//		file << email << "," << password << "," << role << "\n";
//		file.close();
//
//		cout << "User added successfully.\n";
//	}
//
//	// Function to delete a user
//	void deleteUser(const string& filename) {
//		string emailToDelete;
//		cout << "Enter the email of the user to delete: ";
//		getline(cin, emailToDelete);
//
//		ifstream file(filename);
//		ofstream tempFile("temp_login.txt");
//		if (!file.is_open() || !tempFile.is_open()) {
//			cout << "Error: Unable to open the file.\n";
//			return;
//		}
//
//		string line;
//		bool userFound = false;
//		while (getline(file, line)) {
//			size_t pos = line.find(',');
//			string email = line.substr(0, pos);
//
//			if (email != emailToDelete) {
//				tempFile << line << "\n";
//			}
//			else {
//				userFound = true;
//			}
//		}
//
//		file.close();
//		tempFile.close();
//
//		remove(filename.c_str());
//		rename("temp_login.txt", filename.c_str());
//
//		if (userFound) {
//			cout << "User deleted successfully.\n";
//		}
//		else {
//			cout << "User not found.\n";
//		}
//	}
//
//	// Function to update user password or role
//	void modifyUser(const string& filename) {
//		string emailToModify;
//		cout << "Enter the email of the user to modify: ";
//		getline(cin, emailToModify);
//
//		ifstream file(filename);
//		ofstream tempFile("temp_login.txt");
//		if (!file.is_open() || !tempFile.is_open()) {
//			cout << "Error: Unable to open the file.\n";
//			return;
//		}
//
//		string line;
//		bool userFound = false;
//		while (getline(file, line)) {
//			size_t pos = line.find(',');
//			string email = line.substr(0, pos);
//
//			if (email == emailToModify) {
//				userFound = true;
//				string newPassword, newRole;
//				cout << "Enter new password: ";
//				getline(cin, newPassword);
//				cout << "Enter new role (user/admin): ";
//				getline(cin, newRole);
//				tempFile << email << " " << newPassword << " " << newRole << "\n";
//			}
//			else {
//				tempFile << line << "\n";
//			}
//		}
//
//		file.close();
//		tempFile.close();
//
//		remove(filename.c_str());
//		rename("temp_login.txt", filename.c_str());
//
//		if (userFound) {
//			cout << "User modified successfully.\n";
//		}
//		else {
//			cout << "User not found.\n";
//		}
//	}
//};

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

void writeEmail(Queue& outbox, const string& email) {
	string recipient, subject, body;

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

	// Assuming Email has a constructor that takes these parameters
	outbox.enqueue(email, recipient, subject, body);
	cout << "Email composed successfully!\n";
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

bool login(const string& filename, string& role, string& email) {
	ifstream file(filename);
	string password, storedEmail, storedPassword, storedRole;

	if (!file.is_open()) {
		cout << "Error opening login file.\n";
		return false;
	}

	cout << "Enter email: ";
	cin >> email;;
	cout << "Enter password: ";
	cin >> password;

	while (file.good())
	{
		getline(file, storedEmail, ',');
		getline(file, storedPassword, ',');
		getline(file, storedRole, '\n');

		if (storedEmail == email && storedPassword == password)
		{
			role = storedRole;
			email = storedEmail;
			return true;
		}
	}

	file.close();
	cout << "Invalid email or password.\n";
	return false;
}