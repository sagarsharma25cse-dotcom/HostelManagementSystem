

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <stdexcept>
using namespace std;

//  BASE CLASS 
class Person {
protected:
    int id;
    string name;
    string phone;

public:
    Person() : id(0), name(""), phone("") {}

    Person(int i, string n, string p)
        : id(i), name(n), phone(p) {}

    virtual void display() const {
        cout << "ID: " << id << "\nName: " << name
             << "\nPhone: " << phone << '\n';
    }

    virtual ~Person() {}

    int getId() const { return id; }
    string getName() const { return name; }
    string getPhone() const { return phone; }

    void setName(string n) { name = n; }
    void setPhone(string p) { phone = p; }
};

//  STUDENT CLASS
class Student : public Person {
public:
    Student() : Person() {}

    Student(int i, string n, string p)
        : Person(i, n, p) {}

    void display() const override {
        cout << "Student ID: " << id
             << "\nName: " << name
             << "\nPhone: " << phone << '\n';
    }
};

//  WARDEN CLASS
class Warden : public Person {
public:
    Warden() : Person() {}

    Warden(int i, string n, string p)
        : Person(i, n, p) {}

    void display() const override {
        cout << "Warden ID: " << id
             << "\nName: " << name
             << "\nPhone: " << phone << '\n';
    }
};

//  ROOM CLASS
class Room {
private:
    int roomNo;
    int capacity;
    int occupied;

public:
    Room() : roomNo(0), capacity(1), occupied(0) {}

    Room(int r, int c) : roomNo(r), capacity(c), occupied(0) {}

    int getRoomNo() const { return roomNo; }
    int getCapacity() const { return capacity; }
    int getOccupied() const { return occupied; }

    bool isAvailable() const {
        return occupied < capacity;
    }

    bool addOccupant() {
        if (!isAvailable()) return false;
        occupied++;
        return true;
    }

    bool removeOccupant() {
        if (occupied <= 0) return false;
        occupied--;
        return true;
    }

    void setCapacity(int c) {
        if (c >= occupied && c > 0) capacity = c;
    }

    void display() const {
        cout << "Room: " << roomNo
             << " | Capacity: " << capacity
             << " | Occupied: " << occupied
             << " | Available beds: " << capacity - occupied
             << '\n';
    }
};

//  ALLOCATION CLASS 
class Allocation {
private:
    int allocationId;
    int studentId;
    int roomNo;
    string date;
    bool active;

public:
    Allocation()
        : allocationId(0), studentId(0), roomNo(0),
          date(""), active(false) {}

    Allocation(int a, int s, int r, string d)
        : allocationId(a), studentId(s), roomNo(r),
          date(d), active(true) {}

    int getId() const { return allocationId; }
    int getStudentId() const { return studentId; }
    int getRoomNo() const { return roomNo; }
    bool isActive() const { return active; }
    void deactivate() { active = false; }

    void display() const {
        cout << "Allocation ID: " << allocationId
             << " | Student ID: " << studentId
             << " | Room: " << roomNo
             << " | Date: " << date
             << " | Status: " << (active ? "Active" : "Vacated")
             << '\n';
    }
};

//  FILE MANAGER CLASS 
class FileManager {
public:
    static void saveStudents(const Student* students, int count) {
        ofstream file("students.txt");
        if (!file) throw runtime_error("Cannot save students.");

        for (int i = 0; i < count; i++) {
            file << students[i].getId() << '\n'
                 << students[i].getName() << '\n'
                 << students[i].getPhone() << '\n';
        }
    }

    static void saveRooms(const Room* rooms, int count) {
        ofstream file("rooms.txt");
        if (!file) throw runtime_error("Cannot save rooms.");

        for (int i = 0; i < count; i++) {
            file << rooms[i].getRoomNo() << ' '
                 << rooms[i].getCapacity() << ' '
                 << rooms[i].getOccupied() << '\n';
        }
    }

    static void saveAllocations(const Allocation* items, int count) {
        ofstream file("allocations.txt");
        if (!file) throw runtime_error("Cannot save allocations.");

        for (int i = 0; i < count; i++) {
            file << items[i].getId() << ' '
                 << items[i].getStudentId() << ' '
                 << items[i].getRoomNo() << ' '
                 << items[i].isActive() << '\n';
        }
    }
};

// ==================== HOSTEL CLASS ====================
class Hostel {
private:
    Student* students;
    Room* rooms;
    Allocation* allocations;

    int studentCount, roomCount, allocationCount;
    int studentCapacity, roomCapacity, allocationCapacity;

    void growStudents() {
        int newCapacity = studentCapacity * 2;
        Student* temp = new Student[newCapacity];

        for (int i = 0; i < studentCount; i++)
            temp[i] = students[i];

        delete[] students;
        students = temp;
        studentCapacity = newCapacity;
    }

    void growRooms() {
        int newCapacity = roomCapacity * 2;
        Room* temp = new Room[newCapacity];

        for (int i = 0; i < roomCount; i++)
            temp[i] = rooms[i];

        delete[] rooms;
        rooms = temp;
        roomCapacity = newCapacity;
    }

    void growAllocations() {
        int newCapacity = allocationCapacity * 2;
        Allocation* temp = new Allocation[newCapacity];

        for (int i = 0; i < allocationCount; i++)
            temp[i] = allocations[i];

        delete[] allocations;
        allocations = temp;
        allocationCapacity = newCapacity;
    }

    int findStudent(int id) const {
        for (int i = 0; i < studentCount; i++)
            if (students[i].getId() == id) return i;
        return -1;
    }

    int findRoom(int roomNo) const {
        for (int i = 0; i < roomCount; i++)
            if (rooms[i].getRoomNo() == roomNo) return i;
        return -1;
    }

    int findAllocation(int id) const {
        for (int i = 0; i < allocationCount; i++)
            if (allocations[i].getId() == id &&
                allocations[i].isActive()) return i;
        return -1;
    }

public:
    Hostel()
        : studentCount(0), roomCount(0), allocationCount(0),
          studentCapacity(5), roomCapacity(5), allocationCapacity(5) {
        students = new Student[studentCapacity];
        rooms = new Room[roomCapacity];
        allocations = new Allocation[allocationCapacity];

        load();
    }

    ~Hostel() {
        delete[] students;
        delete[] rooms;
        delete[] allocations;
    }

    // Add a student
    void addStudent() {
        int id;
        string name, phone;

        cout << "Enter student ID: ";
        cin >> id;

        if (findStudent(id) != -1) {
            cout << "Student ID already exists.\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter student name: ";
        getline(cin, name);
        cout << "Enter phone: ";
        getline(cin, phone);

        if (studentCount == studentCapacity) growStudents();
        students[studentCount++] = Student(id, name, phone);

        cout << "Student added successfully.\n";
    }

    // Add a room
    void addRoom() {
        int roomNo, capacity;

        cout << "Enter room number: ";
        cin >> roomNo;
        cout << "Enter room capacity: ";
        cin >> capacity;

        if (findRoom(roomNo) != -1 || capacity <= 0) {
            cout << "Invalid room number or capacity.\n";
            return;
        }

        if (roomCount == roomCapacity) growRooms();
        rooms[roomCount++] = Room(roomNo, capacity);

        cout << "Room added successfully.\n";
    }

    void displayRecords() const {
        cout << "\n--- STUDENTS ---\n";
        if (studentCount == 0) cout << "No students recorded.\n";
        for (int i = 0; i < studentCount; i++) {
            students[i].display();
            cout << "----------------\n";
        }

        cout << "\n--- ROOMS ---\n";
        if (roomCount == 0) cout << "No rooms recorded.\n";
        for (int i = 0; i < roomCount; i++)
            rooms[i].display();

        cout << "\n--- ALLOCATIONS ---\n";
        if (allocationCount == 0) cout << "No allocations recorded.\n";
        for (int i = 0; i < allocationCount; i++)
            allocations[i].display();
    }

    void searchRecord() const {
        int choice;
        cout << "1. Search student\n2. Search room\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            int id;
            cout << "Enter student ID: ";
            cin >> id;
            int i = findStudent(id);

            if (i == -1) cout << "Student not found.\n";
            else students[i].display();
        } else if (choice == 2) {
            int no;
            cout << "Enter room number: ";
            cin >> no;
            int i = findRoom(no);

            if (i == -1) cout << "Room not found.\n";
            else rooms[i].display();
        } else {
            cout << "Invalid choice.\n";
        }
    }

    void updateStudent() {
        int id;
        cout << "Enter student ID to update: ";
        cin >> id;

        int i = findStudent(id);
        if (i == -1) {
            cout << "Student not found.\n";
            return;
        }

        string name, phone;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter new name: ";
        getline(cin, name);
        cout << "Enter new phone: ";
        getline(cin, phone);

        students[i].setName(name);
        students[i].setPhone(phone);
        cout << "Student updated successfully.\n";
    }

    void deleteStudent() {
        int id;
        cout << "Enter student ID to delete: ";
        cin >> id;

        int i = findStudent(id);
        if (i == -1) {
            cout << "Student not found.\n";
            return;
        }

        for (int j = 0; j < allocationCount; j++) {
            if (allocations[j].getStudentId() == id &&
                allocations[j].isActive()) {
                cout << "Vacate the student's room before deleting.\n";
                return;
            }
        }

        for (int j = i; j < studentCount - 1; j++)
            students[j] = students[j + 1];

        studentCount--;
        cout << "Student deleted successfully.\n";
    }

    // Main transaction: allocate a room
    void allocateRoom() {
        int studentId, roomNo;
        string date;

        cout << "Enter student ID: ";
        cin >> studentId;

        if (findStudent(studentId) == -1) {
            cout << "Student does not exist. Add the student first.\n";
            return;
        }

        for (int i = 0; i < allocationCount; i++) {
            if (allocations[i].getStudentId() == studentId &&
                allocations[i].isActive()) {
                cout << "Student already has an active allocation.\n";
                return;
            }
        }

        cout << "Enter room number: ";
        cin >> roomNo;

        int r = findRoom(roomNo);
        if (r == -1) {
            cout << "Room not found.\n";
            return;
        }

        if (!rooms[r].isAvailable()) {
            cout << "Room is full.\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter allocation date (DD-MM-YYYY): ";
        getline(cin, date);

        if (allocationCount == allocationCapacity) growAllocations();

        int allocationId = allocationCount + 1;
        allocations[allocationCount++] =
            Allocation(allocationId, studentId, roomNo, date);

        rooms[r].addOccupant();
        cout << "Room allocated successfully. Allocation ID: "
             << allocationId << '\n';
    }

    // Vacate a room
    void vacateRoom() {
        int id;
        cout << "Enter active allocation ID: ";
        cin >> id;

        int a = findAllocation(id);
        if (a == -1) {
            cout << "Active allocation not found.\n";
            return;
        }

        int r = findRoom(allocations[a].getRoomNo());
        if (r != -1) rooms[r].removeOccupant();

        allocations[a].deactivate();
        cout << "Room vacated successfully.\n";
    }

    void report() const {
        int availableBeds = 0, occupiedBeds = 0;

        cout << "\n========== HOSTEL REPORT ==========\n";
        cout << "Total students: " << studentCount << '\n';
        cout << "Total rooms: " << roomCount << '\n';

        for (int i = 0; i < roomCount; i++) {
            availableBeds += rooms[i].getCapacity()
                           - rooms[i].getOccupied();
            occupiedBeds += rooms[i].getOccupied();
        }

        cout << "Occupied beds: " << occupiedBeds << '\n';
        cout << "Available beds: " << availableBeds << '\n';
        cout << "Total allocations: " << allocationCount << '\n';
        cout << "====================================\n";
    }

    void save() const {
        try {
            FileManager::saveStudents(students, studentCount);
            FileManager::saveRooms(rooms, roomCount);
            FileManager::saveAllocations(allocations, allocationCount);
            cout << "Records saved successfully.\n";
        } catch (const exception& e) {
            cout << "File error: " << e.what() << '\n';
        }
    }

    void load() {
        // Load students
        ifstream sf("students.txt");
        if (sf) {
            int id;
            string name, phone;

            while (sf >> id) {
                sf.ignore(numeric_limits<streamsize>::max(), '\n');
                if (!getline(sf, name) || !getline(sf, phone)) break;

                if (studentCount == studentCapacity) growStudents();
                students[studentCount++] = Student(id, name, phone);
            }
        }

        // Load rooms
        ifstream rf("rooms.txt");
        if (rf) {
            int no, cap, occupied;

            while (rf >> no >> cap >> occupied) {
                if (cap <= 0 || occupied < 0 || occupied > cap ||
                    findRoom(no) != -1) continue;

                if (roomCount == roomCapacity) growRooms();
                rooms[roomCount] = Room(no, cap);

                for (int j = 0; j < occupied; j++)
                    rooms[roomCount].addOccupant();

                roomCount++;
            }
        }

        // Load allocations
        ifstream af("allocations.txt");
        if (af) {
            int aid, sid, roomNo, active;

            while (af >> aid >> sid >> roomNo >> active) {
                if (allocationCount == allocationCapacity)
                    growAllocations();

                allocations[allocationCount] =
                    Allocation(aid, sid, roomNo, "Loaded from file");

                if (!active) allocations[allocationCount].deactivate();
                allocationCount++;
            }
        }
    }
};

// MAIN MENU 
int main() {
    Hostel hostel;
    int choice;

    do {
        cout << "\n====================================\n";
        cout << "      HOSTEL MANAGEMENT SYSTEM\n";
        cout << "====================================\n";
        cout << "1. Add Record (Student/Room)\n";
        cout << "2. Display Records\n";
        cout << "3. Search Record\n";
        cout << "4. Update Student Record\n";
        cout << "5. Delete Student Record\n";
        cout << "6. Main Transaction (Allocate/Vacate)\n";
        cout << "7. Report\n";
        cout << "8. Save / Load Records\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a valid number.\n";
            continue;
        }

        switch (choice) {
        case 1: {
            int type;
            cout << "1. Add Student\n2. Add Room\nChoice: ";
            if (!(cin >> type)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input.\n";
                break;
            }

            if (type == 1) hostel.addStudent();
            else if (type == 2) hostel.addRoom();
            else cout << "Invalid choice.\n";
            break;
        }
        case 2:
            hostel.displayRecords();
            break;
        case 3:
            hostel.searchRecord();
            break;
        case 4:
            hostel.updateStudent();
            break;
        case 5:
            hostel.deleteStudent();
            break;
        case 6: {
            int action;
            cout << "1. Allocate Room\n2. Vacate Room\nChoice: ";
            if (!(cin >> action)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input.\n";
                break;
            }

            if (action == 1) hostel.allocateRoom();
            else if (action == 2) hostel.vacateRoom();
            else cout << "Invalid choice.\n";
            break;
        }
        case 7:
            hostel.report();
            break;
        case 8: {
            int action;
            cout << "1. Save Records\n2. Reload Records\nChoice: ";
            if (!(cin >> action)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input.\n";
                break;
            }

            if (action == 1) {
                hostel.save();
            } else if (action == 2) {
                cout << "Restart the program to load saved records safely.\n";
            } else {
                cout << "Invalid choice.\n";
            }
            break;
        }
        case 9:
            hostel.save();
            cout << "Thank you for using Hostel Management System!\n";
            break;
        default:
            cout << "Invalid choice. Select 1 to 9.\n";
        }
    } while (choice != 9);

    return 0;
}
