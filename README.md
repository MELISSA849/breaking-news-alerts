# Breaking News Alert System

A beginner-friendly C++ console project for creating and managing breaking-news
alerts. Each alert has a unique ID, headline, priority, time, and status.

## Features

- Create alerts with Low, Medium, High, or Critical priority.
- View alerts ordered from Critical to Low.
- Search for alerts by ID.
- Withdraw or resolve an alert.
- Manage up to 100 alerts during one program run.

## Requirements

- A C++ compiler that supports C++11 or later.

## Build and run

From the project directory:

```sh
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp -o breaking-news-alerts
./breaking-news-alerts
```

The program stores alerts in memory only. Alerts are cleared when the program
exits.
