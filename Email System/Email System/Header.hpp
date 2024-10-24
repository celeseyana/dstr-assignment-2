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

class Stack
{
	Email* top;
public:
	Stack() : top(nullptr) {}

	//push email onto stack
	void push(string sender, string recipient, string subject, string body)
	{
		Email* newEmail = new Email{sender, recipient, subject, body, top};
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

};

class Queue
{
	Email* front;
	Email* rear;
public:
	Queue() : front (nullptr), rear(nullptr) {}

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

	void displayOutboxWithIndex() {
		Email* current = front;
		int index = 1;

		while (current != nullptr) {
			cout << index << ". Sender: " << current->sender
				<< ", Recipient: " << current->recipient
				<< ", Subject: " << current->subject << "\n";
			current = current->next;
			index++;
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

	// Load users from file into linked list
	void loadUsers(const std::string& filename) {
		std::ifstream file(filename);
		std::string email, password, role;

		while (file >> email >> password >> role) {
			addUserToList(email, password, role);
		}
		file.close();
	}

	// Add user to linked list (temporary)
	void addUserToList(const std::string& email, const std::string& password, const std::string& role) {
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
	}

	// Add new user
	void addUser() {
		std::string email, password, role;
		std::cout << "Enter email: ";
		std::cin >> email;
		std::cout << "Enter password: ";
		std::cin >> password;
		std::cout << "Enter role (admin/user): ";
		std::cin >> role;

		addUserToList(email, password, role);
		std::cout << "User added temporarily.\n";
	}

	// Delete user
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
				std::cout << "User deleted temporarily.\n";
				return;
			}
			prev = current;
			current = current->next;
		}
		std::cout << "User not found.\n";
	}

	// Modify user
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
				std::cout << "User modified temporarily.\n";
				return;
			}
			current = current->next;
		}
		std::cout << "User not found.\n";
	}

	// Display users (temporary list)
	void displayUsers() {
		User* current = head;
		std::cout << "Temporary Users List:\n";
		while (current != nullptr) {
			std::cout << "Email: " << current->email << ", Role: " << current->role << "\n";
			current = current->next;
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

void writeEmail(Queue& outbox) {
	string sender, recipient, subject, body;

	// Collect user input for the email
	cout << "Compose a New Email\n";
	cout << "Sender: ";
	getline(cin, sender);
	cout << "Recipient: ";
	getline(cin, recipient);
	cout << "Subject: ";
	getline(cin, subject);
	cout << "Body: ";
	getline(cin, body);

	// Enqueue the email to the outbox
	outbox.enqueue(sender, recipient, subject, body);

	cout << "Email composed and added to the outbox.\n";
}

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

bool login(const string& filename, string& role) {
	ifstream file(filename);
	string email, password, storedEmail, storedPassword, storedRole;

	if (!file.is_open()) {
		cout << "Error opening login file.\n";
		return false;
	}

	cout << "Enter email: ";
	cin >> email;
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
			return true;
		}
	}

	file.close();
	cout << "Invalid email or password.\n";
	return false;
}
