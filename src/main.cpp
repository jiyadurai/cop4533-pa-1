#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>

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

Hospital readHospital(int i, int N) {
    Hospital h{i};
    h.preferenceList.reserve(N);
    for (int j = 1; j <= N; j++) {
        int x;
        std::cin >> x;
        h.preferences[j] = x;
        h.preferenceList.emplace_back(x);
    }
    return h;
}

Student readStudent(int i, int N) {
    Student s{i};
    s.preferenceList.reserve(N);
    for (int j = 1; j <= N; j++) {
        int x;
        std::cin >> x;
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
            unmatchedHospitals.insert(currentAssignments[student.id]);
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

int main() {
    int N;
    std::cin >> N;
    std::vector<Hospital> hospitals;
    std::vector<Student> students;
    for (int i = 1; i <= N; i++) {
        hospitals.push_back(readHospital(i, N));
    }
    for (int i = 1; i <= N; i++) {
        students.push_back(readStudent(i, N));
    }

    Matching m = createMatching(hospitals, students);
    for (auto &[h, s] : m.pairs) {
        std::cout << "Hospital " << h << " is matched with student " << s << "\n";
    }
}