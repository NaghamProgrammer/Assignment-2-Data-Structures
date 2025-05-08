#include <iostream>
#include <cstring>
#include <cctype>
#include <fstream>
using namespace std;

// Node structure for the stack
struct Node {
    char* url;
    Node* next;

    // Constructor: Allocate memory and copy URL
    Node(const char* url) {
        this->url = new char[strlen(url) + 1];
        strcpy(this->url, url);
        next = nullptr;
    }

    // Destructor: Free allocated memory
    ~Node() {
        delete[] url;
    }
};

// Stack implementation
class Stack {
private:
    Node* top;
    int size;

public:
    Stack() : top(nullptr), size(0) {}

    // Destructor: Clear the stack
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    // Push a new URL onto the stack
    void push(const char* url) {
        Node* newNode = new Node(url);
        newNode->next = top;
        top = newNode;
        size++;
    }

    // Pop the top URL from the stack and return a copy
    char* pop() {
        if (isEmpty()) {
            return nullptr;
        }

        Node* temp = top;
        char* url = new char[strlen(temp->url) + 1];
        strcpy(url, temp->url);

        top = top->next;
        delete temp;
        size--;

        return url;
    }

    // Return the top URL without removing it
    char* peek() const {
        if (isEmpty()) {
            return nullptr;
        }
        return top->url;
    }

    // Check if the stack is empty
    bool isEmpty() const {
        return top == nullptr;
    }

    // Get the current size of the stack
    int getSize() const {
        return size;
    }

    // Clear all elements from the stack
    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }

    // Print the contents of the stack from bottom to top
    void print() const {
        if (isEmpty()) {
            cout << "[]";
            return;
        }

        // Store URLs temporarily to print in bottom-to-top order
        char** urls = new char*[size];
        Node* current = top;
        for (int i = size - 1; i >= 0; i--) {
            urls[i] = new char[strlen(current->url) + 1];
            strcpy(urls[i], current->url);
            current = current->next;
        }

        cout << "[";
        for (int i = 0; i < size; i++) {
            cout << urls[i];
            if (i < size - 1) cout << ", ";
            delete[] urls[i];  // Clean up temporary memory
        }
        cout << "]";
        delete[] urls;
    }
};

// Browser history system using two stacks
class BrowserHistory {
private:
    Stack backStack;
    Stack forwardStack;

public:
    // Visit a new URL (clears forward history)
    void visit(const char* url) {
        forwardStack.clear();
        backStack.push(url);
    }

    // Go back to the previous URL if possible
    const char* goBack() {
        if (backStack.getSize() <= 1) return nullptr;

        forwardStack.push(backStack.pop());
        return backStack.peek();
    }

    // Go forward to the next URL if possible
    const char* goForward() {
        if (forwardStack.isEmpty()) return nullptr;

        backStack.push(forwardStack.pop());
        return backStack.peek();
    }

    // Get the current URL
    const char* getCurrentUrl() const {
        return backStack.peek();
    }

    // Access back stack (for printing)
    const Stack& getBackStack() const {
        return backStack;
    }

    // Access forward stack (for printing)
    const Stack& getForwardStack() const {
        return forwardStack;
    }
};

// Extract URL from command string (e.g., "visit(abc.com)")
const char* extractUrl(const char* command) {
    const char* openParen = strchr(command, '(');
    if (!openParen) return nullptr;

    const char* closeParen = strchr(openParen, ')');
    if (!closeParen) return nullptr;

    int urlLength = closeParen - openParen - 1;
    if (urlLength <= 0) return nullptr;

    char* url = new char[urlLength + 1];
    strncpy(url, openParen + 1, urlLength);
    url[urlLength] = '\0';

    return url;
}

// Print current state of browser history
void printState(const BrowserHistory& browser) {
    cout << "backStack: ";
    browser.getBackStack().print();
    cout << "\nforwardStack: ";
    browser.getForwardStack().print();
    cout << "\ncurrentUrl: ";
    const char* current = browser.getCurrentUrl();
    if (current) {
        cout << current;
    } else {
        cout << "None";
    }
    cout << "\n" << endl;
}

// Case-insensitive command comparison
bool isCommand(const char* input, const char* command) {
    int i = 0;
    while (input[i] && command[i]) {
        if (tolower(input[i]) != tolower(command[i])) return false;
        i++;
    }
    return input[i] == '\0' && command[i] == '\0';
}

// Handle a single command input
void processCommand(BrowserHistory& browser, const char* command) {
    if (isCommand(command, "goBack()")) {
        const char* result = browser.goBack();
        if (result) cout << "Success. Went back.\n";
        else cout << "Cannot go back. No more history.\n";
        printState(browser);
    }
    else if (isCommand(command, "goForward()")) {
        const char* result = browser.goForward();
        if (result) cout << "Success. Went forward.\n";
        else cout << "Cannot go forward. No more history.\n";
        printState(browser);
    }
    else if (strncasecmp(command, "visit(", 6) == 0) {
        const char* url = extractUrl(command);
        if (url) {
            browser.visit(url);
            cout << "Visited: " << url << "\n";
            delete[] url;  // Clean up allocated memory for extracted URL
        } else {
            cout << "Invalid URL format. Use: visit(URL)\n";
        }
        printState(browser);
    }
    else {
        cout << "Invalid command: " << command << "\nAvailable commands:\n";
        cout << "goBack(), goForward(), visit(URL)\n";
        printState(browser);
    }
}

int main() {
    BrowserHistory browser;

    cout << "Available Commands:\n";
    cout << "1) goBack()\n";
    cout << "2) goForward()\n";
    cout << "3) visit(URL)\n\n";

    cout << "Enter File Name: ";
    char filename[256];
    cin.getline(filename, sizeof(filename));

    ifstream inputFile(filename);
    if (!inputFile) {
        cout << "Error: Could not open file " << filename << endl;
        return 1;
    }

    char command[256];
    while (inputFile.getline(command, sizeof(command))) {
        // Skip empty lines
        if (strlen(command) == 0) continue;

        cout << "Processing command: " << command << endl;
        processCommand(browser, command);
    }

    inputFile.close();
    return 0;
}
