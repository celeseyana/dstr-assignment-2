#include "Header.hpp";

int main()
{
    Admin admin;
    string role;
    Stack inbox;
    Queue outbox;
    Stack poppedEmails;
    Queue dequeuedEmails;

    // Load emails from CSV file
    inbox.loadFromFile("Inbox.csv");
    outbox.loadFromFile("Outbox.csv");
    admin.loadUsers("login.txt");

    bool loginSuccess = login("login.txt", role);

    if (!loginSuccess)
    {
        cout << "Login failed!" << endl;
        return 0;
    }

    int choice;
    bool exit = false;

    while (!exit) {
        // Basic menu for the user
        std::cout << "Email System\n";
        std::cout << "1. View Most Recent Email in Inbox\n";
        std::cout << "2. Compose and Send a New Email\n";
        std::cout << "3. View and Send Email from Outbox\n";

        if (role == "admin")
        {
            cout << "4. Add User" << endl;
            cout << "5. Delete User" << endl;
            cout << "6. Modify User" << endl;
            cout << "7. View users" << endl;
        }

        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(); // To handle the newline character after entering a choice

        switch (choice) {
        case 1: 
        {
            if (!inbox.isEmpty())
            {
                Email* recentEmail = inbox.peek();
                if (recentEmail != nullptr)
                {
                    displayEmail(recentEmail);
                }
            }
            else
            {
                cout << "Inbox is empty!" << endl;
            }
            
            break;
        }
        case 2: {
            // Compose and enqueue an email to the outbox
            writeEmail(outbox);

            Email* newEmail = outbox.getRear();
            appendToFile("Outbox.csv", newEmail);
            outbox.saveToFile("Outbox.csv");
            break;
        }
        case 3: {
            // Display emails with indexes
            cout << "Emails in Outbox:\n";
            outbox.displayOutboxWithIndex();  // Now part of Queue class

            int emailIndex;
            cout << "Enter the number of the email you want to send: ";
            cin >> emailIndex;

            // Dequeue the email at the specified index
            if (emailIndex > 0) {
                Queue tempQueue;
                Email* current = outbox.getFront();
                int currentIndex = 1;

                while (current != nullptr) {
                    if (currentIndex != emailIndex) {
                        // Re-enqueue emails that are not the selected one
                        tempQueue.enqueue(current->sender, current->recipient, current->subject, current->body);
                    }
                    else {
                        // Display and "send" the selected email
                        displayEmail(current);
                        cout << "Sending email...\n";
                        
                    }
                    current = current->next;
                    currentIndex++;
                }

                // Overwrite outbox with the updated queue (without the selected email)
                outbox = tempQueue;  // Replace the old queue with the new one
                outbox.saveToFile("Outbox.csv");  // Save updated queue to file
            }
            else {
                cout << "Invalid email selection.\n";
            }
            break;
        }
        case 4:
        {
            if (role == "admin")
            {
                admin.addUser();
                break;
            }
            else
            {
                cout << "You do not have permission to add users." << endl;
                break;
            }
        }
        break;

        case 5:
        {
            if (role == "admin")
            {
                admin.deleteUser();
            }
            else
            {
                cout << "You do not have permission to delete users." << endl;
            }
        }
        case 6:
        {
            if (role == "admin")
            {
                admin.modifyUser();
                break;
            }
            else
            {
                cout << "You do not have permission to modify users." << endl;
                break;
            }
        }
        case 7:
        {
            if (role == "admin")
            {
                admin.displayUsers();
                break;
            }
            else
            {
                cout << "You do not have permission to view all users" << endl;
                break;
            }
        }
        case 0: {
            while (!poppedEmails.isEmpty())
            {
                Email* poppedEmail = poppedEmails.pop();
                removeEmailFromFile("Inbox.csv", poppedEmail);
                delete poppedEmail;
            }
            // Save emails to CSV files before exiting
            inbox.saveToFile("Inbox.csv");
            std::cout << "Exiting and saving emails...\n";
            exit = true;
            break;
        }
        default: {
            std::cout << "Invalid choice. Please try again.\n";
        }
        }
    }

    return 0;
}