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
			getline(file, sender, ',');
			getline(file, recipient, ',');
			getline(file, subject, ',');
			getline(file, body, '\n');
			push(sender, recipient, subject, body);
		}
		file.close();
	}

	void saveToFile(const string& filename)
	{
		ofstream file(filename);
		Email* current = top;
		while (current != nullptr)
		{
			file << current->sender << "," << current->recipient << "," << current->subject << "," << current->body << "\n";
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
	Email* dequeue()
	{
		if (isEmpty())
		{
			cout << "Outbox is empty!\n";
			return nullptr;
		}
		Email* emailToReturn = front;
		front = front->next;
		if (front == nullptr)
		{
			rear == nullptr;
		}
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
			getline(file, sender, ',');
			getline(file, recipient, ',');
			getline(file, subject, ',');
			getline(file, body, '\n');
			enqueue(sender, recipient, subject, body);
		}
		file.close();
	}
	void saveToFile(const string& filename)
	{
		ofstream file(filename, ios::app);

		Email* current = front;
		while (current != nullptr)
		{
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
		std::cout << "No email to display.\n";
		return;
	}
	std::cout << "\n----- Email -----\n";
	std::cout << "Sender: " << email->sender << "\n";
	std::cout << "Recipient: " << email->recipient << "\n";
	std::cout << "Subject: " << email->subject << "\n";
	std::cout << "Body: " << email->body << "\n";
	std::cout << "-----------------\n\n";
}