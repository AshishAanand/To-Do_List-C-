#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype> 

using namespace std;

struct Task {
    int id;
    string description;
    bool completed;
};

class ToDoList {
    private:
        vector<Task> taskList;

        void saveToFile() {
            ofstream file("tasks.txt");
            if (!file) {
                cerr << "Error: Could not save tasks to file." << endl;
                return;
            }

            for (const auto& task : taskList) {
                file << task.id << " | " << task.description << " | " << task.completed << "\n";
            }

            file.close();
        }

        string trim(const string& str) {
            size_t first = str.find_first_not_of(' ');
            if (first == string::npos) return "";
            size_t last = str.find_last_not_of(' ');
            return str.substr(first, last - first + 1);
        }

    public:
        ToDoList() {
            loadFromFile();
        }

        void loadFromFile() {
            ifstream file("tasks.txt");
            if (!file) {
                return; // No error if file doesn't exist
            }

            taskList.clear();
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                Task task;
                string completedStatus;
                try {
                    task.id = stoi(trim(line));
                } catch (const invalid_argument& e) {
                    cerr << "Error: Invalid task ID in file." << endl;
                    getline(ss, completedStatus);
                    try {
                        task.completed = (stoi(trim(completedStatus)) == 1);
                    } catch (const invalid_argument& e) {
                        cerr << "Error: Invalid status value. Skipping line: " << line << endl;
                        continue;
                    } catch (const out_of_range& e) {
                        cerr << "Error: Status value out of range. Skipping line: " << line << endl;
                        continue;
                    }
                    cerr << "Error: Task ID out of range in file." << endl;
                    continue;
                }
                try{
                    task.id = stoi(trim(line));
                    getline(ss, task.description, '|');
                    task.description = trim(task.description);
                    getline(ss, completedStatus);
                    task.completed = (stoi(trim(completedStatus)) == 1);
                    taskList.push_back(task);
                } catch (const exception& e) {
                    cerr << "Error: Corrupted file format. Skipping line: " << line << endl;
                    continue;
                }                                                                                                   
            }
            file.close();
        }

        void addTask(const string& description) {
            int id = taskList.empty() ? 1 : taskList.back().id + 1;
            Task newTask = {id, description, false};
            taskList.push_back(newTask);
            saveToFile();
            cout << "Task added successfully\n" << endl;
        }

        void viewTasks() const {
            if (taskList.empty()) {
                cout << "No tasks available\n" << endl;
                return;
            }

            for (const auto& task : taskList) {
                cout << task.id << ". " << task.description << " " << (task.completed ? "[Completed]" : "[Pending]") << endl;
            }
        }

        void markTaskCompleted(int taskID) {
            for (auto& task : taskList) {
                if (task.id == taskID) {
                    task.completed = true;
                    saveToFile();
                    cout << "Task marked as completed\n" << endl;
                    return;
                }
            }
            cout << "Task not found\n" << endl;
        }

        void viewCompletedTasks(){
            for(auto& task : taskList){
                if(task.completed){
                    cout << task.id << ". " << task.description << " " << (task.completed ? "[Completed]" : "[Pending]") << "\n" << endl;
                }else{
                    cout << "No tasks completed yet\n" << endl;
                }
            }
        }

        void viewPendingTasks(){
            for(auto& task : taskList){
                if(!task.completed){
                    cout << task.id << ". " << task.description << " " << (task.completed ? "[Completed]" : "[Pending]") << "\n" << endl;
                }else{
                    cout << "All tasks are completed\n" << endl;
                }
            }
        }

        void deleteTask(int taskID) {
            auto it = remove_if(taskList.begin(), taskList.end(), [taskID](const Task& task) {
                return task.id == taskID;
            });

            if (it != taskList.end()) {
                taskList.erase(it, taskList.end()); // Properly erase the removed tasks
                saveToFile();
                cout << "Task deleted successfully\n" << endl;
            } else {
                cout << "Task not found\n" << endl;
            }
        }
};

int main() {
    ToDoList taskList;
    string userInput;
    string userTask;
    int taskID;

    cout << "=====================================" << endl;
    cout << "         To-Do List       " << endl;
    cout << "=====================================\n" << endl;
    cout << "User Manual\n" << endl;
    cout << "1. View your tasks" << endl;
    cout << "2. Add tasks" << endl;
    cout << "3. Delete task" << endl;
    cout << "4. Mark as completed" << endl;
    cout << "5. View completed tasks" << endl;
    cout << "6. View pending tasks" << endl;
    cout << "7. Exit" << endl;
    cout << "=====================================\n" << endl;
    cout << "Use the sequence number id of tasks to do." << endl;

    while (true) {
        cout << "What do you want to do: ";
        getline(cin, userInput);

        if (userInput == "1") {
            cout << "=====================================" << endl;
            cout << "         Your Tasks       " << endl;
            cout << "=====================================\n\n" << endl;
            taskList.viewTasks();
        } else if (userInput == "2") {
            cout << "=====================================" << endl;
            cout << "         Add task       " << endl;
            cout << "=====================================\n" << endl;
            cout << "Enter your task: \n\n";
            getline(cin, userTask);
            taskList.addTask(userTask);
        } else if (userInput == "3") {
            cout << "=====================================" << endl;
            cout << "         Delete task       " << endl;
            cout << "=====================================\n" << endl;
            cout << "Enter task ID to delete: \n\n";
            cin >> taskID;
            cin.ignore();
            taskList.deleteTask(taskID);
        } else if (userInput == "4") {
            cout << "=====================================" << endl;
            cout << "         Mark task as completed       " << endl;
            cout << "=====================================\n" << endl;
            cout << "Enter task ID to mark as completed: \n\n";
            cin >> taskID;
            cin.ignore();
            taskList.markTaskCompleted(taskID);
        }else if(userInput == "5"){
            cout << "=====================================" << endl;
            cout << "         Your completed tasks       " << endl;
            cout << "=====================================\n\n" << endl;
            taskList.viewCompletedTasks();
        }else if(userInput == "6"){
            cout << "=====================================" << endl;
            cout << "         Your pending tasks       " << endl;
            cout << "=====================================\n\n" << endl;
            taskList.viewPendingTasks();
        } else if (userInput == "7") {
            cout << "=====================================" << endl;
            cout << "      Thanks for using this app       " << endl;
            cout << "=====================================" << endl;
            break;
        } else {
            cout << "Invalid option. Please enter a number between 1 and 7." << endl;
        }
    }

    return 0;
}
