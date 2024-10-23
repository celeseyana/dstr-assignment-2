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

