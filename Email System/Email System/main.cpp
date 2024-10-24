#include "Header.hpp"

int main() {
    Admin admin;
    string role;
    Stack inbox;
    Queue outbox;
    Stack poppedEmails;
    Queue dequeuedEmails;
    string email;

    // Load emails from CSV files
    inbox.loadFromFile("Inbox.csv");
    outbox.loadFromFile("Outbox.csv");
    admin.loadUsers("login.txt");

    bool isRunning = true;

    while (isRunning) {
        cout << "Log In" << endl;
        cout << "--------------" << endl;
        bool loginSuccess = login("login.txt", role, email);  // Authenticate user

        if (!loginSuccess) {
            cout << "Login failed!" << endl;
            continue;  // Return to login prompt without exiting
        }

        bool isLoggedIn = true;

        while (isLoggedIn) {
            // Display the menu based on the user role
            cout << "Welcome " << email << "!" << endl;
            cout << "Email System\n";
            cout << "1. View all your emails\n";
            cout << "2. View Most Recent Email in Inbox\n";
            cout << "3. Compose and Send a New Email\n";
            cout << "4. View and Send Email from Outbox\n";

            if (role == "admin") {
                cout << "5. Add User\n";
                cout << "6. Delete User\n";
                cout << "7. Modify User\n";
                cout << "8. View users\n";
            }

            cout << "9. Log out\n";
            cout << "0. Exit\n";
            cout << "Enter your choice: ";

            int choice;
            cin >> choice;
            cin.ignore();  // Handle newline character after input
            if (choice < 0 || choice >(role == "admin" ? 8 : 8)) {
                cout << "Invalid choice. Please try again.\n";
                continue;  // Return to the menu without proceeding
            }
            switch (choice) {
            case 1: {
                inbox.viewReceivedEmails(email);  // Display emails for logged-in user
                break;
            }
            case 2: {
                inbox.displayRecentEmails(inbox, email);  // View most recent email
                break;
            }
            case 3: {
                writeEmail(outbox, email);  // Compose and send an email
                break;
            }
            case 4: {
                // Display only the emails in the outbox that belong to the logged-in user
                outbox.displayOutboxWithIndex(email);  // Pass the user's email to filter the display

                int emailIndex;
                cout << "Enter the number of the email you want to send, enter 0 to exit: ";
                cin >> emailIndex;

                if (emailIndex > 0) {
                    Queue tempQueue;
                    Email* current = outbox.getFront();
                    int currentIndex = 1;

                    while (current != nullptr) {
                        // Only process the email if it belongs to the logged-in user
                        if (currentIndex == emailIndex) {
                            // Send the selected email
                            displayEmail(current);
                            cout << "Sending email...\n";
                            // Here you would handle the actual sending logic
                        }
                        else {
                            // Re-enqueue emails that aren't being sent
                            tempQueue.enqueue(current->sender, current->recipient, current->subject, current->body);
                        }
                        current = current->next;
                        currentIndex++;
                    }

                    outbox = tempQueue;  // Replace old queue with updated one
                }
                else if(emailIndex = 0) {
                    break;
                }
                else
                {
                    cout << "Invalid email selection.\n";
                }
                break;
            }

            case 5: {
                if (role == "admin") {
                    admin.addUser();  // Admin adds a new user
                }
                else {
                    cout << "You do not have permission to add users." << endl;
                }
                break;
            }
            case 6: {
                if (role == "admin") {
                    admin.deleteUser();  // Admin deletes a user
                }
                else {
                    cout << "You do not have permission to delete users." << endl;
                }
                break;
            }
            case 7: {
                if (role == "admin") {
                    admin.modifyUser();  // Admin modifies a user
                }
                else {
                    cout << "You do not have permission to modify users." << endl;
                }
                break;
            }
            case 8: {
                if (role == "admin") {
                    admin.displayUsers();  // Admin views all users
                }
                else {
                    cout << "You do not have permission to view users." << endl;
                }
                break;
            }
            case 9: {
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