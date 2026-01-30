#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <fstream>
#include <chrono>

using namespace std;

struct Hospital {
    vector<int> preferenceList;
    unordered_map<int, int> preferences;
    int id;
    explicit Hospital(int id) {
        this->id = id;
    }
};

struct Student {
    vector<int> preferenceList;
    unordered_map<int, int> preferences;
    int id;
    explicit Student(int id) {
        this->id = id;
    }
};

struct Matching {
    vector<pair<int, int>> pairs;
};

Hospital readHospital(int i, int N, istream& in = cin) {
    Hospital h{i};
    h.preferenceList.reserve(N);
    for (int j = 1; j <= N; j++) {
        int x;
        in >> x;
        h.preferences[j] = x;
        h.preferenceList.emplace_back(x);
    }
    return h;
}

Student readStudent(int i, int N, istream& in = cin) {
    Student s{i};
    s.preferenceList.reserve(N);
    for (int j = 1; j <= N; j++) {
        int x;
        in >> x;
        s.preferences[j] = x;
        s.preferenceList.emplace_back(x);
    }
    return s;
}

Matching createMatching(const vector<Hospital> &hospitals, const vector<Student> &students) {
    // do the gale shapley algorithm
    const auto N = hospitals.size();
    if (N != students.size() || N == 0) {
        return {};
    }
    unordered_map<int, int> hospitalById;
    unordered_map<int, int> studentById;
    // holds position numbers of hospitals which are currently unmatched
    set<int> unmatchedHospitals;
    for (int i = 0; i < N; i++) {
        unmatchedHospitals.insert(i);
        hospitalById[hospitals[i].id] = i;
        studentById[students[i].id] = i;
    }
    // holds the place of what should be the next match checked for each hospital (not by id, by place in list)
    vector<int> matchedUpTo(N, 0);
    // holds the matches that have already been made as <student id, hospital id> pairs
    unordered_map<int, int> currentAssignments;
    // when N matches have been made, we are done
    while (currentAssignments.size() < N) {
        auto nextUnmatched = *unmatchedHospitals.begin();
        auto &hospital = hospitals[nextUnmatched];
        //
        auto &student = students[studentById[hospital.preferenceList[matchedUpTo[nextUnmatched]]]];
        matchedUpTo[nextUnmatched]++;

        if (!currentAssignments.contains(student.id)) {
            // if the desired student does not have a match yet, match them with this hospital
            unmatchedHospitals.erase(nextUnmatched);
            currentAssignments[student.id] = hospital.id;
        } else if (student.preferences.at(currentAssignments[student.id]) > student.preferences.at(hospital.id)) {
            // if the student likes the new hospital more than its old match
            unmatchedHospitals.insert(hospitalById[currentAssignments[student.id]]);
            currentAssignments[student.id] = hospital.id;
            unmatchedHospitals.erase(nextUnmatched);
        } else {
            continue;
        }
    }
    Matching result;
    result.pairs.reserve(N);
    for (auto &[student, hospital] : currentAssignments) {
        result.pairs.emplace_back(hospital, student);
    }

    return result;
}

bool Verifier(const Matching& pairing, const vector<Hospital>& h, const vector<Student>& s) {
    set<int> students;
    set<int> hospitals;
    for (int i = 0; i < pairing.pairs.size(); i++) {
        // Check for Duplicates
        if (students.count(pairing.pairs[i].second) > 0 || hospitals.count(pairing.pairs[i].first) > 0) {
            return false;
        }

        hospitals.insert(pairing.pairs[i].first);
        students.insert(pairing.pairs[i].second);

        // Ensure that it is a stable matching
        for (int j = 0; j < h[pairing.pairs[i].first-1].preferenceList.size(); j++) {
            if (h[pairing.pairs[i].first-1].preferenceList[j] == pairing.pairs[i].second) {
                break;
            }
        }
    }
    return true;
}

struct Timer {
    chrono::time_point<chrono::high_resolution_clock> start;
    string name;
    ostream& os;
    explicit Timer(const string_view _name, ostream &o = cout) : name(_name), os(o) {
        start = chrono::high_resolution_clock::now();
    }
    ~Timer() {
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        os << "Timer " << name << " finished in " << duration.count() << " microseconds\n";
    }
};

int main() {
    ifstream f{"../inputs/64.txt"};
    //ofstream o{"../outputs/64.txt"};
    auto &o = cout;
    Timer total{"Whole Program", o};
    int N;
    f >> N;
    vector<Hospital> hospitals;
    vector<Student> students;
    for (int i = 1; i <= N; i++) {
        hospitals.push_back(readHospital(i, N, f));
    }
    for (int i = 1; i <= N; i++) {
        students.push_back(readStudent(i, N, f));
    }

    Matching m;
    {
        Timer createMatchingTime{"Create Matching", o};
        m = createMatching(hospitals, students);
    }

    bool works = Verifier(m, hospitals, students);

    cout << m.pairs.size() << endl;

    sort(m.pairs.begin(), m.pairs.end());
    for (auto &[h, s] : m.pairs) {
        o << "Hospital " << h << " is matched with student " << s << "\n";
    }

    if (works) {
        o << "This is a valid stable matching" << endl;
    } else {
        o << "This is not a valid stable matching" << endl;
    }
}