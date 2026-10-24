# Breaking News Alerts

A C++17 console app for issuing breaking-news alerts, appending updates, and
withdrawing resolved alerts. Active alerts are shown from highest to lowest
priority; alerts with equal priority remain in issue order. Alert and update
history is kept in memory for the duration of the program.

Build and run:

```sh
clang++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o breaking-news-alerts
./breaking-news-alerts
```
