#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;

// Node structure for the stack
struct Node {
    char* url;
    Node* next;

    Node(const char* url) {
        this->url = new char[strlen(url) + 1];
        strcpy(this->url, url);
        next = nullptr;
    }

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

    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    void push(const char* url) {
        Node* newNode = new Node(url);
        newNode->next = top;
        top = newNode;
        size++;
    }

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

    char* peek() const {
        if (isEmpty()) {
            return nullptr;
        }
        return top->url;
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    int getSize() const {
        return size;
    }

    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }

    void print() const {
        if (isEmpty()) {
            cout << "[]";
            return;
        }

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
            delete[] urls[i];
        }
        cout << "]";
        delete[] urls;
    }
};

// Browser history system
class BrowserHistory {
private:
    Stack backStack;
    Stack forwardStack;

public:
    void visit(const char* url) {
        forwardStack.clear();
        backStack.push(url);
    }

    const char* goBack() {
        if (backStack.getSize() <= 1) return nullptr;

        forwardStack.push(backStack.pop());
        return backStack.peek();
    }

    const char* goForward() {
        if (forwardStack.isEmpty()) return nullptr;

        backStack.push(forwardStack.pop());
        return backStack.peek();
    }

    const char* getCurrentUrl() const {
        return backStack.peek();
    }

    const Stack& getBackStack() const {
        return backStack;
    }

    const Stack& getForwardStack() const {
        return forwardStack;
    }
};

// Extract URL from command
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

// Print full state
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

bool isCommand(const char* input, const char* command) {
    int i = 0;
    while (input[i] && command[i]) {
        if (tolower(input[i]) != tolower(command[i])) return false;
        i++;
    }
    return input[i] == '\0' && command[i] == '\0';
}

int main() {
    BrowserHistory browser;

    cout << "Available Commands:\n";
    cout << "1) goBack()\n";
    cout << "2) goForward()\n";
    cout << "3) visit(URL)\n";
    cout << "4) Exit\n\n";

    char command[256];
    while (true) {
        cout << "Enter Command: ";
        cin.getline(command, sizeof(command));

        if (isCommand(command, "Exit")) break;

        else if (isCommand(command, "goBack()")) {
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
                delete[] url;
            } else {
                cout << "Invalid URL format. Use: visit(URL)\n";
            }
            printState(browser);
        }

        else {
            cout << "Invalid command. Available commands:\n";
            cout << "goBack(), goForward(), visit(URL), Exit\n";
            printState(browser);
        }
    }

    return 0;
}

