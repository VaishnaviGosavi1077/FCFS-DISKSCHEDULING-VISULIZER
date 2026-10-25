// FCFS Disk Scheduling algorithm (C++ reference implementation)
// Compile: g++ -std=c++17 fcfs.cpp -o fcfs
// Run:     ./fcfs
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cmath>
using namespace std;

int main() {
    string line;
    int head, diskSize;
    cout << "=== FCFS Disk Scheduling ===\n";
    cout << "Number of cylinders (e.g. 200 means 0-199): ";
    if (!(cin >> diskSize) || diskSize < 1) {
        cerr << "Invalid disk size.\n"; return 1;
    }
    cout << "Initial head position: ";
    if (!(cin >> head) || head < 0 || head >= diskSize) {
        cerr << "Invalid initial head position.\n"; return 1;
    }
    cin.ignore(10000, '\n');
    cout << "Enter requests separated by spaces or commas: ";
    getline(cin, line);
    for (char &c : line) if (c == ',') c = ' ';
    stringstream ss(line);
    vector<int> requests;
    int value;
    while (ss >> value) {
        if (value < 0 || value >= diskSize) {
            cerr << "Request " << value << " is outside the valid range 0-" << diskSize - 1 << ".\n";
            return 1;
        }
        requests.push_back(value);
    }
    if (requests.empty()) { cerr << "Please enter at least one request.\n"; return 1; }

    long long totalMovement = 0;
    cout << "\nStep\tFrom\tTo\tSeek distance\n";
    cout << "Start\t-\t" << head << "\t0\n";
    int current = head;
    for (size_t i = 0; i < requests.size(); ++i) {
        int distance = abs(requests[i] - current);
        cout << i + 1 << "\t" << current << "\t" << requests[i] << "\t" << distance << '\n';
        totalMovement += distance;
        current = requests[i];
    }
    cout << "\nSeek sequence: " << head;
    for (int request : requests) cout << " -> " << request;
    cout << "\nTotal head movement: " << totalMovement << " cylinders\n";
    cout << "Average seek distance: " << static_cast<double>(totalMovement) / requests.size() << " cylinders/request\n";
    cout << "Requests served: " << requests.size() << '\n';
    return 0;
}