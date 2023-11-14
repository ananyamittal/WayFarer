#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <climits>

using namespace std;

class Graph {
    unordered_map<string, vector<pair<string, int>>> g;

public:
    void addEdge(const string& start, const string& end, int wt) {
        g[start].push_back({end, wt});
        g[end].push_back({start, wt});
    }

    void displayCities(char c) {
        for (const auto& x : g) {
            if (x.first[0] == c)
                cout << x.first << '\n';
        }
    }


    int shortestPath(const string& dep, const string& des, vector<string>& path) {
        unordered_map<string, int> dist;
        unordered_map<string, string> parent;
        for (const auto& x : g) {
            dist[x.first] = INT_MAX;
            parent[x.first] = "";
        }

        priority_queue<pair<int, string>> q;
        q.push({0, dep});
        dist[dep] = 0;

        while (!q.empty()) {
            string current = q.top().second;
            q.pop();

            if (current == des)
                break;

            for (const auto& neighbor : g[current]) {
                string next = neighbor.first;
                int weight = neighbor.second;
                int newDist = dist[current] + weight;
                if (newDist < dist[next]) {
                    dist[next] = newDist;
                    parent[next] = current;
                    q.push({-newDist, next});
                }
            }
        }

        if (dist[des] == INT_MAX) {
            cout << "No path found from " << dep << " to " << des << endl;
            return -1;
        }

        string current = des;
        while (current != "") {
            path.push_back(current);
            current = parent[current];
        }
        reverse(path.begin(), path.end());

        return dist[des];
    }

    int calculatePrice(int sp, int choice) {
        if (choice == 1)
            return sp * 7;
        if (choice == 2)
            return sp * 8;
        if (choice == 3)
            return sp * 3;
        if (choice == 4)
            return sp * 10;
        return 0;
    }
    void printAlternatePaths(const string& dep, const string& des) {
        vector<vector<string>> allPaths = this->allPaths(dep, des);

        // sort paths by length, in ascending order
        sort(allPaths.begin(), allPaths.end(), [](const vector<string>& p1, const vector<string>& p2) {
            return p1.size() < p2.size();
        });

        // print the top 10 alternate paths
        int numPaths = min((int)allPaths.size() - 1, 10);
        cout << "\n--------------------------------\n";
        cout << " Top " << numPaths << " Alternate Paths:\n";
        cout << "--------------------------------\n";
        for (int i = 1; i <= numPaths; i++) {
            vector<string> path = allPaths[i];
            int length = 0;
            for (int j = 0; j < path.size() - 1; j++) {
                string u = path[j];
                string v = path[j + 1];
                for (const auto& neighbor : g[u]) {
                    if (neighbor.first == v) {
                        length += neighbor.second;
                        break;
                    }
                }
            }
            int price = calculatePrice(length, 2);
            cout << i << ". ";
            for (int j = 0; j < path.size() - 1; j++) {
                cout << path[j] << " -> ";
            }
            cout << path.back() << " (" << length << " km, Rs. " << price << ")" << endl;
        }
        cout << "--------------------------------\n";
    }
    vector<vector<string>> allPaths(const string& start, const string& end) {
        vector<vector<string>> result;
        vector<string> path;
        unordered_map<string, bool> visited;
        allPathsHelper(start, end, visited, path, result);
        return result;
    }

private:
    void allPathsHelper(const string& start, const string& end, unordered_map<string, bool>& visited, vector<string>& path, vector<vector<string>>& result) {
        visited[start] = true;
        path.push_back(start);

        if (start == end) {
            result.push_back(path);
        } else {
            for (const auto& neighbor : g[start]) {
                const string& next = neighbor.first;
                if (!visited[next]) {
                    allPathsHelper(next, end, visited, path, result);
                }
            }
        }

        visited[start] = false;
        path.pop_back();
    }
};

int main() {
    Graph g;
    string a, b;
    int w;

    ifstream infile("data_traveller.txt");
    if (!infile) {
        cout << "Failed to open the file." << endl;
        return 1;
    }

    string line;
    while (getline(infile, line)) {
        istringstream iss(line);
        if (!(iss >> a >> b >> w)) {
            cout << "Error parsing the line: " << line << endl;
            continue;
        }
        g.addEdge(a, b, w);
    }
    infile.close();

    system("cls");
    cout<<endl;
    cout << "\t\t__________________________________________________________________\n";
    cout << "\t\t\t\t\t WELCOME TO Wayfarer\n";
    cout << "\t\t__________________________________________________________________\n\n";
    cout << "\t\tWayfarer is a travel planning and route recommendation platform\n \t\tthat helps users discover and plan trips to various destinations\n \t\t\t\t\t across India.";
    cout<<endl;
    cout<<endl;
    cout<<endl;
    cout << "Press any key to continue...";
    cin.ignore();
    system("cls");

    cout << "\n-----------------------------------------------\n";
    cout << " Enter the First letter of the Departure City: ";
    cout << "\n-----------------------------------------------\n";
    char fde;
    cin >> fde;

    cout << "\n----------------------------------------\n";
    cout << " List of cities starting with letter " << fde << ":\n";
    cout << "\n----------------------------------------\n";
    g.displayCities(fde);
    cout << "--------------------------------\n\n";

    cout << "\n--------------------------------\n";
    cout << " Enter the Departure City: ";
    cout << "\n--------------------------------\n";
    string dep;
    cin >> dep;

    cout << "\n----------------------------------------------------\n";
    cout << " Enter the First letter of the Destination City: ";
    cout << "\n----------------------------------------------------\n";
    char dest;
    cin >> dest;

    cout << "\n--------------------------------\n";
    cout << "\tList of cities starting with letter " << dest << ":\n";
    cout << "\n--------------------------------\n";
    g.displayCities(dest);
    cout << "\n--------------------------------\n\n";

    cout << "\n--------------------------------\n";
    cout << " Enter the Destination: ";
    cout << "\n--------------------------------\n";
    string des;
    cin >> des;

    vector<string> path;
    int sp = g.shortestPath(dep, des, path);

    if (sp == -1) {
        cout << "No path found from " << dep << " to " << des << endl;
        return 0;
    }

    cout << "\n------------------------------------------\n";
    cout << "Select a choice of transport:\n";
    cout << "------------------------------------------\n";
    cout << "1. Bike\n";
    cout << "2. Cab\n";
    cout << "3. Train\n";
    cout << "4. Aeroplane\n";
    cout << "\n----------------------------------------\n";

    int choice;
    cin >> choice;
    system("cls");

    cout << "\t\t-----------------YOUR Wayfarer TICKET----------------\n";
    cout << "\t===============================================================\n";
    //cout << "\t\t\t Booking Date: 23-05-2022\n";
    time_t now = time(0);

    char* dt = ctime(&now);

    cout << "\t   The local date and time is: " << dt << endl;
    cout << "\t\t\t Best Route: " << sp << " Km" << endl;
    cout << "\t\t\t Total Fare: Rs." << g.calculatePrice(sp, choice) << endl;
    cout << "\t===============================================================\n";

    if (choice==3){
    cout << "\t\t\t\t Connectivity: \n";
    cout << "\t===============================================================\n";
    for (size_t i = 0; i < path.size() - 1; i++) {
        cout << "\t" << i + 1 << ". " << path[i] << " --> " << path[i + 1] << endl;
    }
    }

    return 0;
}
