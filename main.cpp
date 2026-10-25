#include <iostream>
#include <string>
using namespace std;

class Alert {
    int AlertID;
    string headline, priority, time, status;
    static int nextID;

public:
    Alert() {
        AlertID = 0;
        status = "Empty";
    }

    Alert(string h, string p, string t) {
        AlertID = nextID++;
        headline = h;
        priority = p;
        time = t;
        status = "Active";
    }

    inline int getID() { return AlertID; }
    inline string getHeadline() { return headline; }
    inline string getPriority() { return priority; }
    inline string getTime() { return time; }
    inline string getStatus() { return status; }
    inline void withdraw() { status = "Withdrawn/Resolved"; }

    friend void displayAlert(Alert alert);
};

int Alert::nextID = 1;

void displayAlert(Alert alert) {
    cout << "\nAlert ID: " << alert.AlertID
         << "\nHeadline: " << alert.headline
         << "\nPriority: " << alert.priority
         << "\nTime: " << alert.time
         << "\nStatus: " << alert.status << "\n";
}

Alert* findAlert(Alert alerts[], int count, int id) {
    for (int i = 0; i < count; i++)
        if (alerts[i].getID() == id) return &alerts[i];
    return nullptr;
}

int priorityRank(string priority) {
    if (priority == "Critical") return 4;
    if (priority == "High") return 3;
    if (priority == "Medium") return 2;
    return 1;
}

int main() {
    Alert alerts[100];
    int count = 0, choice = 0;

    do {
        cout << "\n===== BREAKING NEWS ALERT SYSTEM =====\n"
             << "1. Create Alert\n2. View All Alerts\n"
             << "3. Search Alert\n4. Withdraw Alert\n5. Exit\n"
             << "Enter choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Enter a number from 1 to 5.\n";
            continue;
        }
        cin.ignore(10000, '\n');

        switch (choice) {
        case 1:
            if (count == 100) {
                cout << "Alert list is full.\n";
                break;
            }
            {
                string headline, time;
                int level;
                cout << "Headline: ";
                getline(cin, headline);
                cout << "Priority (1 Low, 2 Medium, 3 High, 4 Critical): ";
                if (!(cin >> level) || level < 1 || level > 4) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid priority.\n";
                    break;
                }
                cin.ignore(10000, '\n');
                cout << "Time (example: 10:30 AM): ";
                getline(cin, time);
                string priorities[] = {"Low", "Medium", "High", "Critical"};
                alerts[count] = Alert(headline, priorities[level - 1], time);
                cout << "Alert created. ID: " << alerts[count].getID() << "\n";
                count++;
            }
            break;

        case 2:
            if (count == 0) cout << "No alerts available.\n";
            for (int rank = 4; rank >= 1; rank--)
                for (int i = 0; i < count; i++)
                    if (priorityRank(alerts[i].getPriority()) == rank)
                        displayAlert(alerts[i]);
            break;

        case 3:
        case 4: {
            int id;
            cout << "Enter Alert ID: ";
            if (!(cin >> id)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid ID.\n";
                break;
            }
            cin.ignore(10000, '\n');
            Alert* found = findAlert(alerts, count, id);
            if (found == nullptr) cout << "Alert not found.\n";
            else if (choice == 3) displayAlert(*found);
            else if (found->getStatus() != "Active")
                cout << "Alert is already resolved.\n";
            else {
                found->withdraw();
                cout << "Alert withdrawn/resolved.\n";
            }
            break;
        }

        case 5:
            cout << "Goodbye!\n";
            break;
        default:
            cout << "Choose a number from 1 to 5.\n";
        }
    } while (choice != 5);

    return 0;
}
