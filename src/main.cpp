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

}