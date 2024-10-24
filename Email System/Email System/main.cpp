#include "Header.hpp";

int main() {
    Admin admin;
    string role;
    Stack inbox;
    Queue outbox;
    Stack poppedEmails;
    Queue dequeuedEmails;
    string email;

    // Load emails from CSV file
    inbox.loadFromFile("Inbox.csv");
    outbox.loadFromFile("Outbox.csv");
    admin.loadUsers("login.txt");

    bool loginSuccess = login("login.txt", role, email);

    if (!loginSuccess) {
        cout << "Login failed!" << endl;
        return 0;
    }

    int choice;
    bool exit = false;

    while (!exit) {
        // Basic menu for the user
        cout << "Welcome " << email << "!" << endl;
        std::cout << "Email System\n";
        cout << "1. View all your emails" << endl;
        std::cout << "2. View Most Recent Email in Inbox\n";
        std::cout << "3. Compose and Send a New Email\n";
        std::cout << "4. View and Send Email from Outbox\n";

        if (role == "admin") {
            cout << "5. Add User" << endl;
            cout << "6. Delete User" << endl;
            cout << "7. Modify User" << endl;
            cout << "8. View users" << endl;
        }

        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(); // To handle the newline character after entering a choice

        switch (choice) {
        case 1:
        {
            inbox.viewReceivedEmails(email);
            break;
        }
        case 2: {
            /*if (!inbox.isEmpty()) {
                Email* recentEmail = inbox.peek();
                if (recentEmail != nullptr) {
                    displayEmail(recentEmail);
                }
            }
            else {
                cout << "Inbox is empty!" << endl;
            }
            break;*/
            inbox.displayRecentEmails(inbox, email);
            break;
        }
        case 3: {
            // Compose and enqueue an email to the outbox
            writeEmail(outbox, email);
            Email* newEmail = outbox.getRear();
            break;
        }
        case 4: {
            // Display emails with indexes
            cout << "Emails in Outbox:\n";
            outbox.displayOutboxWithIndex();

            int emailIndex;
            cout << "Enter the number of the email you want to send: ";
            cin >> emailIndex;

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
                        cout << endl;
                    }
                    current = current->next;
                    currentIndex++;
                }

                // Overwrite outbox with the updated queue (without the selected email)
                outbox = tempQueue;  // Replace the old queue with the new one
            }
            else {
                cout << "Invalid email selection.\n";
            }
            break;
        }
        case 5: {
            if (role == "admin") {
                admin.addUser();
            }
            else {
                cout << "You do not have permission to add users." << endl;
            }
            break;
        }
        case 6: {
            if (role == "admin") {
                admin.deleteUser();
            }
            else {
                cout << "You do not have permission to delete users." << endl;
            }
            break;
        }
        case 7: {
            if (role == "admin") {
                admin.modifyUser();
            }
            else {
                cout << "You do not have permission to modify users." << endl;
            }
            break;
        }
        case 8: {
            if (role == "admin") {
                admin.displayUsers();
            }
            else {
                cout << "You do not have permission to view all users" << endl;
            }
            break;
        }
        case 0: {
            // Only remove popped emails from memory, not from the file
            while (!poppedEmails.isEmpty()) {
                Email* poppedEmail = poppedEmails.pop();
                // removeEmailFromFile("Inbox.csv", poppedEmail);  // Comment this out if changes should not persist
                delete poppedEmail;
            }
            std::cout << "Exiting without saving emails...\n";
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
