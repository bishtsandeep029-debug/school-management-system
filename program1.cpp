#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <limits>
using namespace std;

class school {
public:
    string name;
    int rollno;

    virtual void getdata() {
        cout << "Enter the name of student: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, name);

        cout << "Enter the roll number: ";
        cin >> rollno;
    }

    virtual void display() {
        cout << left << setw(25) << name
             << setw(12) << rollno;
    }
};

class student : public school {
public:
    int n;
    vector<int> marks;

    void getdata() override {
        school::getdata();

        cout << "Enter number of subjects: ";
        cin >> n;

        marks.resize(n);
        for (int i = 0; i < n; i++) {
            cout << "Enter marks of subject " << i + 1 << ": ";
            cin >> marks[i];
        }
    }

    void display() override {
        school::display();
        for (int i = 0; i < n; i++) {
            cout << setw(12) << marks[i];
        }
    }
};

class result : public student {
public:
    float percentage;
    int total;
    bool ispass;

    void getdata() override {
        student::getdata();

        total = 0;
        for (int i = 0; i < n; i++) {
            total += marks[i];
        }

        percentage = (total / (n * 100.0)) * 100;
        ispass = (percentage >= 40);
    }

    void display() override {
        student::display();

        cout << setw(13) << total
             << setw(13) << fixed << setprecision(2) << percentage
             << setw(13) << (ispass ? "Pass" : "Fail")
             << endl;
    }

    void writetofile(ofstream &file){ 
    file << name << " "
         << rollno << " "
         << n << " ";

    for(int i = 0; i < n; i++){
        file << marks[i] << " ";
    }

    file << total << " "
         << percentage << " "
         << ispass << endl;   // 1 or 0
    }

    bool readfromfile(ifstream &file){
    if(!(file >> name >> rollno >> n)) 
        return false;

    marks.resize(n);

    for(int i = 0; i < n; i++){
        file >> marks[i];
    }

    file >> total >> percentage >> ispass;

    return true;
}
};

void printHeader(int subjects){
    cout << left << setw(25) << "Name"
         << setw(12) << "Roll No";

    for (int i = 0; i < subjects; i++) {
        cout << setw(13) << ("Sub" + to_string(i+1));
    }

    cout << setw(12) << "Total"
         << setw(12) << "Percent"
         << setw(12) << "Result" << endl;

    cout << string(100, '-') << endl;
}

int main(){
    vector<result> students;
    int choice;

    // LOAD DATA ONCE AT START

    ifstream fin("students.txt");

    if(!fin){
        cout<< "No saved data found. Starting new database..." << endl;
    }
    else{
      result r;
      while(r.readfromfile(fin)){
        students.push_back(r);
        }    
      fin.close();
    }

    do {
        cout << "\n--- Student Management System ---\n";
        cout << "1. Add Students\n";
        cout << "2. Display Students\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;


    switch (choice) {

    case 1: {
        int size;
        cout << "Enter number of students: ";
        cin >> size;

        ofstream file("students.txt", ios::app | ios::in | ios::out);
        if (!file) {
            cout << "Error opening file!" << endl;
            break;
        }

        for (int i = 0; i < size; i++) {
            result r;
            cout << "\nEnter details of student " << i + 1 << ":\n";
            r.getdata();
            r.writetofile(file);
            students.push_back(r);
        }

        file.close();
        cout << "\nData saved successfully in students.txt !\n";
        break;
    }

    case 2: {
        
    if (students.empty()) {
        cout << "No student data available!\n";
    } else {

        printHeader(students[0].n);

        for (auto &s : students) {
            s.display();
        }

        cout << "\nTotal number of students: " << students.size() << endl;
    }
    break;
}

    case 3:
        cout << "Exiting program...\n";
        break;

    default:
        cout << "Invalid choice. Try again.\n";
    }

   } while (choice != 3);

    return 0;
 }
