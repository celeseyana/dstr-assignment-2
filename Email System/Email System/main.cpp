#include "Header.hpp";

int main()
{
    Stack inbox;
    Queue outbox;
    Stack poppedEmails;
    Queue dequeuedEmails;

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
            int option;
            // Peek at the next email to be sent from the outbox
            Email* outgoingEmail = outbox.peek();
            displayEmail(outgoingEmail);
            while (true) {
                cout << "Send email? 0 = No, 1 = Yes: ";
                cin >> option;

                if (cin.fail() || option < 0 || option > 1) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter 0 or 1.\n";
                }
                else {
                    if (option == 0) {
                        cout << "Email is not sent." << endl;
                        break;
                    }
                    else {
                        cout << "Sending..." << endl;
                        outgoingEmail = outbox.dequeue();  // Dequeue the email
                        std::cout << "Dequeued Email: Sender: " << outgoingEmail->sender
                            << ", Recipient: " << outgoingEmail->recipient
                            << ", Subject: " << outgoingEmail->subject
                            << ", Body: " << outgoingEmail->body << "\n";
                        outbox.saveToFile("Outbox.csv");   // Update Outbox.

                        delete outgoingEmail;  // Free memory after use
                        break;
                    }
                }
            }
            break;
        }
        case 4: {
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