#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <fstream>
#include <numeric>
#include <random>

struct Hospital {
    std::vector<int> preferenceList;
    std::unordered_map<int, int> preferences;
    int id;
    explicit Hospital(int id) {
        this->id = id;
    }
};

struct Student {
    std::vector<int> preferenceList;
    std::unordered_map<int, int> preferences;
    int id;
    explicit Student(int id) {
        this->id = id;
    }
};

struct Matching {
    std::vector<std::pair<int, int>> pairs;
};

Hospital readHospital(int i, int N, std::istream& in = std::cin) {
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

Student readStudent(int i, int N, std::istream& in = std::cin) {
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

Matching createMatching(const std::vector<Hospital> &hospitals, const std::vector<Student> &students) {
    // do the gale shapley algorithm
    const auto N = hospitals.size();
    if (N != students.size() || N == 0) {
        return {};
    }
    std::unordered_map<int, int> hospitalById;
    std::unordered_map<int, int> studentById;
    // holds position numbers of hospitals which are currently unmatched
    std::set<int> unmatchedHospitals;
    for (int i = 0; i < N; i++) {
        unmatchedHospitals.insert(i);
        hospitalById[hospitals[i].id] = i;
        studentById[students[i].id] = i;
    }
    // holds the place of what should be the next match checked for each hospital (not by id, by place in list)
    std::vector<int> matchedUpTo(N, 0);
    // holds the matches that have already been made as <student id, hospital id> pairs
    std::unordered_map<int, int> currentAssignments;
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

struct Timer {
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::string name;
    std::ostream& os;
    explicit Timer(const std::string_view _name, std::ostream &o = std::cout) : name(_name), os(o) {
        start = std::chrono::high_resolution_clock::now();
    }
    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        // os << "Timer " << name << " finished in " << duration.count() << " nanoseconds\n";
        os << duration.count() << "\n";
    }
};

int main() {
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
}