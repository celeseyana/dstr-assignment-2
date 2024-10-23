#include "Header.hpp";

int main()
{
    Stack inbox;
    Queue outbox;

    // Load emails from CSV file
    inbox.loadFromFile("Inbox.csv");
    outbox.loadFromFile("Outbox.csv");

    int choice;
    bool exit = false;

    while (!exit) {
        // Basic menu for the user
        std::cout << "Email System\n";
        std::cout << "1. View Most Recent Email in Inbox\n";
        std::cout << "2. Compose and Send a New Email\n";
        std::cout << "3. View and Send Email from Outbox\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(); // To handle the newline character after entering a choice

        switch (choice) {
        case 1: {
            // Pop and view the most recent email in the inbox
            Email* recentEmail = inbox.pop();
            if (recentEmail != nullptr) {
                displayEmail(recentEmail);
                // Optional: Free memory if dynamically allocated
                delete recentEmail;
            }
            break;
        }
        case 2: {
            // Compose and enqueue an email to the outbox
            writeEmail(outbox);
            break;
        }
        case 3: {
            // Dequeue and display the next email to be sent from the outbox
            Email* outgoingEmail = outbox.dequeue();
            if (outgoingEmail != nullptr) {
                std::cout << "Sending email...\n";
                displayEmail(outgoingEmail);
            }
            break;
        }
        case 4: {
            // Exit the system
            exit = true;
            // Save emails to CSV files before exiting
            inbox.saveToFile("Inbox.csv");
            outbox.saveToFile("Outbox.csv");
            std::cout << "Exiting and saving emails...\n";
            break;
        }
        default: {
            std::cout << "Invalid choice. Please try again.\n";
        }
        }
    }

    return 0;
}