#include "Header.hpp"

int main() {
    Admin admin;
    string role;
    Stack inbox;
    Queue outbox;
    Stack poppedEmails;
    Queue dequeuedEmails;
    Queue tempOutboxQueue;
    string userEmail;

    // Load emails from CSV files
    inbox.loadFromFile("Inbox.csv");
    outbox.loadFromFile("Outbox.csv");
    admin.loadUsers("login.txt");

    bool isRunning = true;

    while (isRunning) {
        cout << "Log In" << endl;
        cout << "--------------" << endl;
        bool loginSuccess = login("login.txt", role, userEmail);  // Authenticate user

        if (!loginSuccess) {
            cout << "Login failed!" << endl;
            continue;  // Return to login prompt without exiting
        }

        bool isLoggedIn = true;

        while (isLoggedIn) {
            // Display the menu based on the user role
            cout << "===== Welcome " << userEmail << "! =====" << endl;
            cout << "Email System\n";
            cout << "1. View all your emails\n";
            cout << "2. View Most Recent Email in Inbox\n";
            cout << "3. Compose and Send a New Email\n";
            cout << "4. View and Send Email from Outbox\n";
            cout << "5. Spam Inbox\n";

            if (role == "admin") {
                cout << "6. Add User\n";
                cout << "7. Delete User\n";
                cout << "8. Modify User\n";
                cout << "9. View users\n";
            }

            cout << "10. Log out\n";
            cout << "0. Exit\n";
            cout << "Enter your choice: ";

            int choice;
            cin >> choice;
            cin.ignore();  // Handle newline character after input
            
            cout << string(23, '=') << endl;
            if (choice < 0 || choice >(role == "admin" ? 10 : 10)) {
                cout << "Invalid choice. Please try again.\n";
                continue;  // Return to the menu without proceeding
            }
            switch (choice) {
            case 1: {
                inbox.viewReceivedEmails(userEmail);  // Display emails for logged-in user
                break;
            }
            case 2: {
                inbox.displayRecentEmails(inbox, userEmail);  // View most recent email
                break;
            }
            case 3: {
                writeEmail(outbox, userEmail);  // Compose and send an email
                break;
            }
            case 4: {
                // Display only the emails in the outbox that belong to the logged-in user
                outbox.displayOutboxWithIndex(userEmail);  // Pass the user's email to filter the display

                int emailIndex;
                cout << "Enter the number of the email you want to send, enter 0 to exit: ";
                cin >> emailIndex;

                if (emailIndex > 0) {
                    Queue tempQueue; // Temporary queue for emails not being sent
                    Queue senderQueue; // queue for emails of the sender's outbox
                    Email* current = outbox.getFront(); // Get the front of the queue

                    while (current != nullptr) {
                        if (current->sender == userEmail) {
                            senderQueue.enqueue(current->sender, current->recipient, current->subject, current->body, current->priority);
                        }
                        else {
                            // If the email is not from the sender, re-enqueue it in tempQueue
                            tempQueue.enqueue(current->sender, current->recipient, current->subject, current->body, current->priority);
                        }
                        current = current->next; // Move to the next email in outbox
                    }

                    Email* senderEmail = senderQueue.getFront();
                    int currentIndex = 1;
                    bool emailFound = false;

                    while (senderEmail != nullptr) {
                        if (currentIndex == emailIndex) {
                            // Display and send the selected email
                            cout << emailIndex << ". ";
                            displayEmail(senderEmail);
                            cout << "Sending email to " << senderEmail->recipient << "..." << endl;
                            inbox.push(senderEmail->sender, senderEmail->recipient, senderEmail->subject, senderEmail->body, senderEmail->priority);
                            emailFound = true;
                        }
                        else {
                            tempQueue.enqueue(senderEmail->sender, senderEmail->recipient, senderEmail->subject, senderEmail->body, senderEmail->priority);
                        }
                        senderEmail = senderEmail->next; // Move to the next email
                        currentIndex++;
                    }

                    if (!emailFound) {
                        cout << "Invalid Email Selection!" << endl;
                    }
                    else {
                        outbox = tempQueue;
                    }
                }
                else if (emailIndex == 0) {
                    // exiting
                    break;
                }
                else {
                    cout << "Invalid email selection!" << endl;
                }
                break;
            }

            case 5: {
                cout << "why is it coming to hereeeeeeeee\n\n";
                checkForDuplicates(inbox);
                inbox.saveToFile("Inbox.csv");
                break;
            }
            case 6: {
                if (role == "admin") {
                    admin.addUser();  // Admin adds a new user
                }
                else {
                    cout << "You do not have permission to add users." << endl;
                }
                break;
            }
            case 7: {
                if (role == "admin") {
                    admin.deleteUser();  // Admin deletes a user
                }
                else {
                    cout << "You do not have permission to this function." << endl;
                }
                break;
            }
            case 8: {
                if (role == "admin") {
                    admin.modifyUser();  // Admin modifies a user
                }
                else {
                    cout << "You do not have permission to this function." << endl;
                }
                break;
            }
            case 9: {
                if (role == "admin") {
                    admin.displayUsers();  // Admin views all users
                }
                else {
                    cout << "You do not have permission to this function." << endl;
                }
                break;
            }
            case 10: {
                // Log out and return to login screen
                isLoggedIn = false;
                break;
            }
            case 0: {
                // Exit the program
                isRunning = false;
                isLoggedIn = false;

                // Clean up: remove popped emails from memory, not from file
                while (!poppedEmails.isEmpty()) {
                    Email* poppedEmail = poppedEmails.pop();
                    delete poppedEmail;  // Free allocated memory for emails
                }

                cout << "Exiting without saving emails...\n";
                break;
            }
            default: {
                cout << "Invalid choice. Please try again.\n";
                break;
            }
            }

            if (!isLoggedIn && isRunning) {
                cout << "Logging out...\n";  // Acknowledge logout action
            }
        }
    }

    return 0;
}