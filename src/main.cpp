#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <fstream>
#include <chrono>

using namespace std;
#include <numeric>
#include <random>

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
        h.preferences[x] = j;
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
        s.preferences[x] = j;
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
    const Hospital* ht;
    vector<set<int>> sPreferences(s.size(), set<int>());
    for (int i = 0; i < pairing.pairs.size(); i++) {
        // Check for Duplicates
        if (students.count(pairing.pairs[i].second) > 0 || hospitals.count(pairing.pairs[i].first) > 0) {
            cout << "INVALID because there are duplicate students and/or hospitals." << endl;
            return false;
        }

        hospitals.insert(pairing.pairs[i].first);
        students.insert(pairing.pairs[i].second);
        // Create sets of the hospitals students prefer to their current matching
        const Student* student = &s[pairing.pairs[i].second-1];
        for (int j = 0; j < student->preferenceList.size(); j++) {
            // Exit when current hopsital is reached
            if (student->preferenceList[j] == pairing.pairs[i].first) {
                break;
            }
            sPreferences[student->id-1].insert(student->preferenceList[j]);
        }
    }
    
    // Ensure that it is a stable matching
    for (int i = 0; i < h.size(); i++) {
        for (int j = 0; j < h[i].preferenceList.size(); j++) {
            // Exit when student being checked is the student matched with the hospital
            if (h[i].preferenceList[j] == pairing.pairs[i].second) {
                break;
            }
            // Check if the student the hospital would prefer also prefers this hopsital to their current matching
            if (sPreferences[h[i].preferenceList[j]-1].count(i+1) > 0) {
                cout << "UNSTABLE matching because hospital " << i + 1 << " wants student " << h[i].preferenceList[j] << " and that student prefers this matching." << endl;
                return false;
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
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        // os << "Timer " << name << " finished in " << duration.count() << " nanoseconds\n";
        os << duration.count() << "\n";
    }
};

int main(int argc, char *argv[]) {
    if (argc >= 2) {
        if (std::string{argv[1]} == "-m") {
            int N;
            std::cin >> N;
            std::vector<Hospital> hospitals;
            std::vector<Student> students;
            for (int i = 1; i <= N; i++) {
                hospitals.push_back(readHospital(i, N, std::cin));
            }
            for (int i = 1; i <= N; i++) {
                students.push_back(readStudent(i, N, std::cin));
            }
            Matching m;
            m = createMatching(hospitals, students);
            std::sort(m.pairs.begin(), m.pairs.end());
            for (auto &[h, s] : m.pairs) {
                std::cout << h << " " << s << "\n";
            }
        }
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<int> order(12);
    std::iota(order.begin(), order.end(), 0);
    std::shuffle(order.begin(), order.end(), gen);
    for (int j = 0; j < 12; j++) {
        std::ifstream f{"../inputs/" + std::to_string(1 << order[j]) + ".txt"};
        std::ofstream o{"../outputs/" + std::to_string(1 << order[j]) + "time.txt"};
        std::ofstream of{"../outputs/" + std::to_string(1 << order[j]) + ".txt"};
        // auto &o = std::cout;
        Timer total{"Whole Program", o};
        int N;
        f >> N;
        std::vector<Hospital> hospitals;
        std::vector<Student> students;
        for (int i = 1; i <= N; i++) {
            hospitals.push_back(readHospital(i, N, f));
        }
        for (int i = 1; i <= N; i++) {
            students.push_back(readStudent(i, N, f));
        }
        Matching m;
        m = createMatching(hospitals, students);
        std::sort(m.pairs.begin(), m.pairs.end());
        for (auto &[h, s] : m.pairs) {
            of << h << " " << s << "\n";
        }
    }
  
    bool works = Verifier(m, hospitals, students);

    if (works) {
        o << "This is a valid stable matching" << endl;
    } else {
        o << "This is not a valid stable matching" << endl;
    }
}