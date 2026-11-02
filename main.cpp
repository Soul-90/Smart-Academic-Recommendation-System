#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <iomanip>
#include <algorithm>

using namespace std;

// ==========================================
// DATA STRUCTURES
// ==========================================
struct Course {
    string name;
    int credits;
    double suitabilityScore;
};

// ==========================================
// RED-BLACK TREE (Course Catalog)
// ==========================================
enum Color { RED, BLACK };
struct RBNode {
    Course data;
    Color color;
    RBNode *left, *right, *parent;
    RBNode(Course c) : data(c), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class CourseCatalog {
private:
    RBNode* root;
    RBNode* insertBST(RBNode* root, RBNode* pt) {
        if (root == nullptr) return pt;
        if (pt->data.name < root->data.name) {
            root->left = insertBST(root->left, pt);
            root->left->parent = root;
        } else if (pt->data.name > root->data.name) {
            root->right = insertBST(root->right, pt);
            root->right->parent = root;
        }
        return root;
    }
public:
    CourseCatalog() : root(nullptr) {}
    void addCourse(string name, int credits) {
        Course c = {name, credits, 0.0};
        RBNode* node = new RBNode(c);
        root = insertBST(root, node);
        if(root != nullptr) root->color = BLACK; 
    }
};

// ==========================================
// GRAPH & DAG (Prerequisites)
// ==========================================
class PrerequisiteGraph {
public:
    unordered_map<string, vector<string>> adjList;
    
    void addDependency(string course, string prereq) {
        adjList[course].push_back(prereq);
    }
    
    bool isEligible(string course, unordered_set<string>& completedCourses) {
        for (const string& prereq : adjList[course]) {
            if (completedCourses.find(prereq) == completedCourses.end()) {
                return false;
            }
        }
        return true;
    }
};

// ==========================================
// SORTING (Quick Sort Ranking)
// ==========================================
int partition(vector<Course>& arr, int low, int high) {
    double pivot = arr[high].suitabilityScore;
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j].suitabilityScore > pivot) { 
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(vector<Course>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// ==========================================
// DYNAMIC PROGRAMMING (Knapsack Credit Optimization)
// ==========================================
vector<Course> optimizeCreditsDP(vector<Course>& courses, int maxCredits) {
    int n = courses.size();
    vector<vector<double>> dp(n + 1, vector<double>(maxCredits + 1, 0.0));

    for (int i = 1; i <= n; i++) {
        for (int w = 1; w <= maxCredits; w++) {
            if (courses[i - 1].credits <= w) {
                dp[i][w] = max(courses[i - 1].suitabilityScore + dp[i - 1][w - courses[i - 1].credits], dp[i - 1][w]);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    vector<Course> selected;
    int w = maxCredits;
    for (int i = n; i > 0 && dp[i][w] > 0; i--) {
        if (dp[i][w] != dp[i - 1][w]) {
            selected.push_back(courses[i - 1]);
            w -= courses[i - 1].credits;
        }
    }
    return selected;
}

// ==========================================
// BACKTRACKING (Schedule Generator)
// ==========================================
bool solveSchedule(vector<Course>& courses, vector<string>& schedule, int index, int maxSlots) {
    if (index == courses.size()) return true; 

    for (int slot = 0; slot < maxSlots; slot++) {
        if (schedule[slot] == "") {
            schedule[slot] = courses[index].name; 
            
            if (solveSchedule(courses, schedule, index + 1, maxSlots)) {
                return true;
            }
            schedule[slot] = ""; 
        }
    }
    return false;
}

// ==========================================
// SCORING ENGINE
// ==========================================
double calculateScore(double academicMatch, double skillMatch, double interestMatch, double prereqMatch) {
    return (0.30 * academicMatch) + (0.30 * skillMatch) + (0.20 * interestMatch) + (0.20 * prereqMatch);
}

// ==========================================
// MAIN DRIVER
// ==========================================
int main() {
    cout << "--- Smart Academic Recommendation System ---" << endl;

    // 1. Setup Catalog
    CourseCatalog catalog;
    catalog.addCourse("Machine Learning", 4);
    catalog.addCourse("Cyber Security", 3);
    catalog.addCourse("Cloud Computing", 4);
    
    // 2. Interactive Student Input
    string studentName, studentInterest;
    cout << "\nEnter Student Name: ";
    getline(cin, studentName);
    cout << "Enter Primary Field of Interest (e.g., AI, Security, Cloud, Data): ";
    getline(cin, studentInterest);
    
    string interestLower = studentInterest;
    transform(interestLower.begin(), interestLower.end(), interestLower.begin(), ::tolower);

    cout << "\n--- Enter Raw Academic Marks (0-100) ---" << endl;
    double mark_maths, mark_oops, mark_dsa, mark_cn, mark_dbms;
    cout << "Maths: "; cin >> mark_maths;
    cout << "OOPS: "; cin >> mark_oops;
    cout << "DSA: "; cin >> mark_dsa;
    cout << "Computer Networks: "; cin >> mark_cn;
    cout << "DBMS: "; cin >> mark_dbms;

    // 3. System Calculates Domain Suitability Scores
    double ml_acad = (mark_maths + mark_oops + mark_dsa) / 3.0;
    double ml_int = (interestLower.find("ai") != string::npos || interestLower.find("machine") != string::npos) ? 100.0 : 0.0;
    double ml_score = calculateScore(ml_acad, 80.0, ml_int, 100.0);

    double cs_acad = (mark_cn + mark_oops + mark_dsa) / 3.0;
    double cs_int = (interestLower.find("secur") != string::npos) ? 100.0 : 0.0;
    double cs_score = calculateScore(cs_acad, 75.0, cs_int, 100.0); 

    double cc_acad = (mark_cn + mark_dbms + mark_oops) / 3.0;
    double cc_int = (interestLower.find("cloud") != string::npos) ? 100.0 : 0.0;
    double cc_score = calculateScore(cc_acad, 85.0, cc_int, 100.0);

    // 4. Compile Course Options
    vector<Course> eligibleCourses = {
        {"Machine Learning", 4, ml_score},
        {"Cyber Security", 3, cs_score},
        {"Cloud Computing", 4, cc_score}
    };

    // 5. Execution Pipeline
    quickSort(eligibleCourses, 0, eligibleCourses.size() - 1);
    
    cout << "\n[SYSTEM] Scoring Complete. Calculated Suitability Scores for " << studentName << ":" << endl;
    for (auto c : eligibleCourses) {
        cout << "- " << c.name << " (Score: " << fixed << setprecision(2) << c.suitabilityScore << "%)" << endl;
    }

    int SEMESTER_CREDIT_LIMIT = 7; 
    vector<Course> dpSelection = optimizeCreditsDP(eligibleCourses, SEMESTER_CREDIT_LIMIT);
    
    // Output 1: Dynamic Programming course combination line
    cout << "\nOptimizing course selection to maximize suitability score within credit limit (" << SEMESTER_CREDIT_LIMIT << " Credits Max):" << endl;
    int totalCredits = 0; double totalScore = 0;
    for (auto c : dpSelection) {
        cout << "- " << c.name << " (Credits: " << c.credits << ")" << endl;
        totalCredits += c.credits;
        totalScore += c.suitabilityScore;
    }

    // Output 2: Backtracking scheduling line
    cout << "\nGenerating conflict-free timetable slots for selected courses:" << endl;
    vector<string> timetable(5, ""); 
    if (solveSchedule(dpSelection, timetable, 0, 5)) {
        for(int i=0; i<5; i++) {
            if(timetable[i] != "") cout << "Slot " << i+1 << ": " << timetable[i] << endl;
        }
    } else {
        cout << "Conflict detected. Could not schedule all courses." << endl;
    }

    return 0;
}