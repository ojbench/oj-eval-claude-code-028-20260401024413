#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

struct Student {
    string name;
    char gender;
    int classNum;
    int scores[9];
    int avgScore;
    int rank;

    void calculateAvg() {
        int sum = 0;
        for (int i = 0; i < 9; i++) {
            sum += scores[i];
        }
        avgScore = sum / 9;
    }

    bool operator<(const Student& other) const {
        // Higher average score comes first
        if (avgScore != other.avgScore) {
            return avgScore > other.avgScore;
        }
        // Compare individual scores from 0 to 8
        for (int i = 0; i < 9; i++) {
            if (scores[i] != other.scores[i]) {
                return scores[i] > other.scores[i];
            }
        }
        // Compare name lexicographically
        return name < other.name;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    unordered_map<string, Student*> students;
    vector<Student*> ranking; // Use vector to maintain order until FLUSH
    bool started = false;

    string command;
    while (cin >> command) {
        if (command == "ADD") {
            string name;
            char gender;
            int classNum;
            cin >> name >> gender >> classNum;

            if (started) {
                cout << "[Error]Cannot add student now.\n";
                int dummy;
                for (int i = 0; i < 9; i++) cin >> dummy;
                continue;
            }

            if (students.count(name)) {
                cout << "[Error]Add failed.\n";
                int dummy;
                for (int i = 0; i < 9; i++) cin >> dummy;
                continue;
            }

            Student* s = new Student();
            s->name = name;
            s->gender = gender;
            s->classNum = classNum;
            for (int i = 0; i < 9; i++) {
                cin >> s->scores[i];
            }
            s->calculateAvg();
            students[name] = s;

        } else if (command == "START") {
            started = true;
            // Build initial ranking by sorting
            for (auto& p : students) {
                ranking.push_back(p.second);
            }
            sort(ranking.begin(), ranking.end(), [](Student* a, Student* b) {
                return *a < *b;
            });
            // Update ranks
            for (int i = 0; i < (int)ranking.size(); i++) {
                ranking[i]->rank = i + 1;
            }

        } else if (command == "UPDATE") {
            string name;
            int code, score;
            cin >> name >> code >> score;

            if (!students.count(name)) {
                cout << "[Error]Update failed.\n";
                continue;
            }

            // Only update the score, don't re-sort
            Student* s = students[name];
            s->scores[code] = score;
            s->calculateAvg();

        } else if (command == "FLUSH") {
            // Re-sort and update all ranks
            sort(ranking.begin(), ranking.end(), [](Student* a, Student* b) {
                return *a < *b;
            });
            for (int i = 0; i < (int)ranking.size(); i++) {
                ranking[i]->rank = i + 1;
            }

        } else if (command == "PRINTLIST") {
            for (int i = 0; i < (int)ranking.size(); i++) {
                Student* s = ranking[i];
                string genderStr = (s->gender == 'M') ? "male" : "female";
                cout << s->rank << " " << s->name << " " << genderStr << " "
                     << s->classNum << " " << s->avgScore << "\n";
            }

        } else if (command == "QUERY") {
            string name;
            cin >> name;

            if (!students.count(name)) {
                cout << "[Error]Query failed.\n";
                continue;
            }

            cout << "STUDENT " << name << " NOW AT RANKING "
                 << students[name]->rank << "\n";

        } else if (command == "END") {
            break;
        }
    }

    // Cleanup
    for (auto& p : students) {
        delete p.second;
    }

    return 0;
}
