#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#define MAX_STUDENTS 100
#define MAX_TEACHERS 2


struct Course {
    string code;
    string title;
};

class Student;

class Person {
protected:
    string name;
    int id;

public:
    Person() : name("null"), id(0) {}
    Person(string n, int i) : name(n), id(i) {}

    string getName() const { return name; }
    int getId() const { return id; }

    void setName(string n) { name = n; }
    void setId(int i) { id = i; }

    void setId(string idStr) {
        stringstream ss(idStr);
        ss >> id;
    }

    virtual void displayPersonDetails() const {
        cout << "Name: " << name << endl;
        cout << "ID: " << id << endl;
    }
};

class Student : public Person {
private:
    string nationality;
    string course_name;
    int school_year;
    int midtermGrades[MAX_TEACHERS];
    int finalGrades[MAX_TEACHERS];
    char letterGrades[MAX_TEACHERS];

public:
    Student() : Person(), nationality("null"), course_name("null"), school_year(0) {
        for (int i = 0; i < MAX_TEACHERS; i++) {
            midtermGrades[i] = -1;
            finalGrades[i] = -1;
            letterGrades[i] = 'N';
        }
    }

    Student(string name, int id, string nation, string course, int year)
        : Person(name, id), nationality(nation), course_name(course), school_year(year) {
        for (int i = 0; i < MAX_TEACHERS; i++) {
            midtermGrades[i] = -1;
            finalGrades[i] = -1;
            letterGrades[i] = 'N';
        }
    }

    void setMidtermGrade(int teacherIndex, int grade) {
        if (teacherIndex >= 0 && teacherIndex < MAX_TEACHERS) {
            midtermGrades[teacherIndex] = grade;
            calculateLetterGrade(teacherIndex);
        }
    }

    void setFinalGrade(int teacherIndex, int grade) {
        if (teacherIndex >= 0 && teacherIndex < MAX_TEACHERS) {
            finalGrades[teacherIndex] = grade;
            calculateLetterGrade(teacherIndex);
        }
    }

    int getMidtermGrade(int teacherIndex) const {
        return midtermGrades[teacherIndex];
    }

    int getFinalGrade(int teacherIndex) const {
        return finalGrades[teacherIndex];
    }

    float getAverage(int teacherIndex) const {
        if (midtermGrades[teacherIndex] == -1 || finalGrades[teacherIndex] == -1)
            return -1;
        return 0.4 * midtermGrades[teacherIndex] + 0.6 * finalGrades[teacherIndex];
    }

    char getLetterGrade(int teacherIndex) const {
        return letterGrades[teacherIndex];
    }

    void calculateLetterGrade(int teacherIndex) {
        float avg = getAverage(teacherIndex);
        if (avg == -1) return;

        if (avg >= 90) letterGrades[teacherIndex] = 'A';
        else if (avg >= 80) letterGrades[teacherIndex] = 'B';
        else if (avg >= 70) letterGrades[teacherIndex] = 'C';
        else if (avg >= 60) letterGrades[teacherIndex] = 'D';
        else letterGrades[teacherIndex] = 'F';
    }

    void displayPersonDetails() const override {
        cout << "Student name: " << name << endl;
        cout << "Student ID: " << id << endl;
        cout << "Nationality: " << nationality << endl;
        cout << "Course: " << course_name << endl;
        cout << "School year: " << school_year << endl;
    }

    void greet(string message = "Welcome to the student portal!") const {
        cout << message << endl;
    }

    static int getMaxStudents() {
        return MAX_STUDENTS;
    }

    friend float calculateOverallAverage(const Student& s, int teacherIndex);
};

class Teacher : public Person {
private:
    string course_name;
    string password;

public:
    Teacher() : Person(), course_name("null"), password("null") {}
    Teacher(string name, int id, string course, string pass) : Person(name, id), course_name(course), password(pass) {}

    bool verifyPassword(string enteredPassword) {
        return enteredPassword == password;
    }

    string getCourseName() const { return course_name; }

    void displayPersonDetails() const override {
        cout << "Teacher name: " << name << endl;
        cout << "Teacher ID: " << id << endl;
        cout << "Course: " << course_name << endl;
    }

    void assignGradesToStudent(Student students[], int numStudents, int teacherIndex) {
        int studentChoice;
        cout << "\nSelect a student to assign grades:\n";
        for (int i = 0; i < numStudents; i++) {
            cout << i + 1 << ". " << students[i].getName() << endl;
        }
        cout << "Enter the number of the student: ";
        cin >> studentChoice;

        if (studentChoice >= 1 && studentChoice <= numStudents) {
            int midterm, final;
            cout << "Enter midterm grade: ";
            cin >> midterm;
            students[studentChoice - 1].setMidtermGrade(teacherIndex, midterm);

            cout << "Enter final grade: ";
            cin >> final;
            students[studentChoice - 1].setFinalGrade(teacherIndex, final);

            cout << "Grades assigned successfully!\n";
        } else {
            cout << "Invalid selection.\n";
        }
    }
};

float calculateOverallAverage(const Student& s, int teacherIndex) {
    return s.getAverage(teacherIndex);
}

int main() {
    Student students[MAX_STUDENTS];
    Teacher teachers[MAX_TEACHERS];

    students[0] = Student("Ali", 11, "Turkey", "Computer Science", 4);
    students[1] = Student("Veli", 12, "Turkey", "Mechanical Engineering", 4);
    students[2] = Student("Zeynep", 13, "Turkey", "Computer Science", 4);
    students[3] = Student("Azra", 14, "Turkey", "Mechanical Engineering", 4);
    students[4] = Student("Batuhan", 15, "Turkey", "Computer Science", 4);

    teachers[0] = Teacher("Kristin", 999, "Software Design", "password123");
    teachers[1] = Teacher("Salim", 1000, "OOP2", "password456");

    int userType;
    bool exitProgram = false;

    while (!exitProgram) {
        cout << "\n=== Select Login Type ===\n";
        cout << "1. Teacher Login\n";
        cout << "2. Student Login\n";
        cout << "3. Exit\n";
        cout << "Your choice: ";
        cin >> userType;

        switch (userType) {
        case 1: {
            string name, enteredPassword;
            bool authenticated = false;
            int attempts = 3;

            while (attempts > 0 && !authenticated) {
                cout << "\nTeacher Panel: Please enter your name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter your password: ";
                cin >> enteredPassword;

                for (int i = 0; i < MAX_TEACHERS; i++) {
                    if (teachers[i].getName() == name && teachers[i].verifyPassword(enteredPassword)) {
                        authenticated = true;
                        cout << "Login successful! Welcome, " << teachers[i].getName() << "!\n";

                        cout << "\n=== Student List ===\n";
                        for (int j = 0; j < 5; j++) {
                            cout << j + 1 << ". " << students[j].getName() << " (ID: " << students[j].getId() << ")\n";
                        }

                        char another;
                        do {
                            teachers[i].assignGradesToStudent(students, 5, i);
                            cout << "\nWould you like to assign another grade? (y/n): ";
                            cin >> another;
                        } while (another == 'y' || another == 'Y');

                        cout << "\nExiting Teacher Panel.\n";
                        break;
                    }
                }

                if (!authenticated) {
                    attempts--;
                    cout << "Incorrect name or password. You have " << attempts << " attempt(s) left.\n";
                }
            }

            break;
        }

        case 2: {
            int sid;
            cout << "\nStudent Panel\n";
            cout << "Please enter your student ID: ";
            cin >> sid;
            bool found = false;

            for (int i = 0; i < 5; i++) {
                if (students[i].getId() == sid) {
                    found = true;
                    cout << "\nStudent Details:\n";
                    students[i].displayPersonDetails();
                    students[i].greet();

                    cout << "\nWhich teacher's course grade would you like to see?\n";
                    for (int j = 0; j < MAX_TEACHERS; j++) {
                        cout << j + 1 << ". " << teachers[j].getName() << " (" << teachers[j].getCourseName() << ")\n";
                    }

                    int choice;
                    cout << "Enter number: ";
                    cin >> choice;

                    if (choice >= 1 && choice <= MAX_TEACHERS) {
                        int mid = students[i].getMidtermGrade(choice - 1);
                        int fin = students[i].getFinalGrade(choice - 1);
                        float avg = calculateOverallAverage(students[i], choice - 1);
                        char letter = students[i].getLetterGrade(choice - 1);

                        if (mid == -1 || fin == -1) {
                            cout << "Grades for this course are not fully assigned yet.\n";
                        } else {
                            cout << "\nMidterm: " << mid << endl;
                            cout << "Final: " << fin << endl;
                            cout << "Average: " << avg << endl;
                            cout << "Letter Grade: " << letter << endl;
                        }
                    } else {
                        cout << "Invalid selection.\n";
                    }

                    break;
                }
            }

            if (!found) {
                cout << "No student found with this ID.\n";
            }
            break;
        }

        case 3:
            cout << "\nExiting program. Goodbye!\n";
            exitProgram = true;
            break;

        default:
            cout << "Invalid choice. Program terminating.\n";
        }
    }

    return 0;
}

