#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

enum class Priority {
    // Higher numeric values represent more urgent alerts and match the menu.
    Low = 1,
    Medium,
    High,
    Critical
};

struct Update {
    // Each alert keeps every detail update with the time it was entered.
    std::string timestamp;
    std::string details;
};

struct Alert {
    // An alert stays in the collection after withdrawal so its history is preserved.
    int id;
    std::string headline;
    Priority priority;
    bool active;
    std::vector<Update> updates;
};

std::string currentTimestamp() {
    const std::time_t now = std::time(nullptr);
    const std::tm* localTime = std::localtime(&now);
    if (localTime == nullptr) {
        return "Unknown time";
    }

    std::ostringstream output;
    output << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    return output.str();
}

std::string priorityName(Priority priority) {
    switch (priority) {
        case Priority::Critical: return "Critical";
        case Priority::High: return "High";
        case Priority::Medium: return "Medium";
        case Priority::Low: return "Low";
    }
    return "Unknown";
}

std::string readNonEmpty(const std::string& prompt) {
    // Reject blank input while allowing spaces within headlines and details.
    while (true) {
        std::cout << prompt;
        std::string input;
        if (!std::getline(std::cin, input)) {
            return {};
        }
        if (input.find_first_not_of(" \t\r\n") != std::string::npos) {
            return input;
        }
        std::cout << "Please enter a value.\n";
    }
}

int readNumber(const std::string& prompt, int minimum, int maximum) {
    // Parse the entire line and enforce the requested range before accepting it.
    while (true) {
        std::cout << prompt;
        std::string input;
        if (!std::getline(std::cin, input)) {
            return -1;
        }

        std::istringstream parser(input);
        int value;
        char extra;
        if ((parser >> value) && !(parser >> extra) &&
            value >= minimum && value <= maximum) {
            return value;
        }
        std::cout << "Enter a number from " << minimum << " to " << maximum << ".\n";
    }
}

Alert* findAlert(std::vector<Alert>& alerts, int id) {
    // Return a pointer to the matching alert, or nullptr when the ID is unknown.
    const auto found = std::find_if(alerts.begin(), alerts.end(),
                                    [id](const Alert& alert) { return alert.id == id; });
    return found == alerts.end() ? nullptr : &*found;
}

void issueAlert(std::vector<Alert>& alerts, int& nextId) {
    // A new alert starts active and records its initial details as the first update.
    const std::string headline = readNonEmpty("Headline: ");
    if (headline.empty()) return;

    std::cout << "Priority: 1) Low  2) Medium  3) High  4) Critical\n";
    const int priority = readNumber("Choose priority: ", 1, 4);
    if (priority == -1) return;

    const std::string details = readNonEmpty("Details: ");
    if (details.empty()) return;

    alerts.push_back({nextId++, headline, static_cast<Priority>(priority), true,
                      {{currentTimestamp(), details}}});
    std::cout << "Alert #" << alerts.back().id << " issued.\n";
}

void updateAlert(std::vector<Alert>& alerts) {
    const int id = readNumber("Alert ID to update: ", 1, std::numeric_limits<int>::max());
    if (id == -1) return;

    Alert* alert = findAlert(alerts, id);
    if (alert == nullptr) {
        std::cout << "No alert found with that ID.\n";
        return;
    }
    if (!alert->active) {
        std::cout << "This alert has been withdrawn and cannot be updated.\n";
        return;
    }

    const std::string details = readNonEmpty("New details: ");
    if (details.empty()) return;
    // Append instead of replacing details so readers can follow the story's history.
    alert->updates.push_back({currentTimestamp(), details});
    std::cout << "Alert #" << id << " updated.\n";
}

void withdrawAlert(std::vector<Alert>& alerts) {
    const int id = readNumber("Alert ID to withdraw: ", 1, std::numeric_limits<int>::max());
    if (id == -1) return;

    Alert* alert = findAlert(alerts, id);
    if (alert == nullptr) {
        std::cout << "No alert found with that ID.\n";
    } else if (!alert->active) {
        std::cout << "Alert #" << id << " is already withdrawn.\n";
    } else {
        alert->active = false;
        std::cout << "Alert #" << id << " withdrawn (resolved).\n";
    }
}

void listAlerts(const std::vector<Alert>& alerts, bool activeOnly) {
    // Build a view of the requested alerts without changing the stored collection.
    std::vector<const Alert*> displayed;
    for (const Alert& alert : alerts) {
        if (!activeOnly || alert.active) displayed.push_back(&alert);
    }

    if (displayed.empty()) {
        std::cout << (activeOnly ? "There are no active alerts.\n" : "No alerts have been issued.\n");
        return;
    }

    if (activeOnly) {
        // Stable sorting keeps alerts with equal priority in their original issue order.
        std::stable_sort(displayed.begin(), displayed.end(),
                         [](const Alert* left, const Alert* right) {
                             return static_cast<int>(left->priority) >
                                    static_cast<int>(right->priority);
                         });
    }

    for (const Alert* alert : displayed) {
        std::cout << "\n#" << alert->id << " [" << priorityName(alert->priority) << "] "
                  << (alert->active ? "ACTIVE" : "WITHDRAWN / RESOLVED") << '\n'
                  << alert->headline << '\n';
        for (const Update& update : alert->updates) {
            std::cout << "  " << update.timestamp << " - " << update.details << '\n';
        }
    }
}

int main() {
    // Alerts are held in memory for this run; IDs increase and are not reused.
    std::vector<Alert> alerts;
    int nextId = 1;

    while (true) {
        std::cout << "\n=== Breaking News Alerts ===\n"
                  << "1) Issue an alert\n"
                  << "2) Update an alert\n"
                  << "3) Withdraw a resolved alert\n"
                  << "4) Show active alerts (by priority)\n"
                  << "5) Show all alerts and update history\n"
                  << "0) Exit\n";
        const int choice = readNumber("Choose an option: ", 0, 5);
        if (choice == -1 || choice == 0) {
            std::cout << "Goodbye.\n";
            return 0;
        }

        switch (choice) {
            case 1: issueAlert(alerts, nextId); break;
            case 2: updateAlert(alerts); break;
            case 3: withdrawAlert(alerts); break;
            case 4: listAlerts(alerts, true); break;
            case 5: listAlerts(alerts, false); break;
        }
    }
}
