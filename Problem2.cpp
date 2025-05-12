#include <optional>

#include "iostream"
#include "regex"
#include "fstream"
using namespace std;

struct Contact {
    string name;
    string phone;
    string email;
};

class Node {
private:
    int key;
    int height;
    Contact Value;
    Node* left;
    Node* right;
public:
    Node(const int &id,const Contact &value) {
        int height=1;
        this->key = id;
        this->Value = value;
        this->left = nullptr;
        this->right = nullptr;
    }
    int getKey() const {
        return key;
    }

    void setKey(int key) {
        this->key = key;
    }

    Contact getValue() const {
        return Value;
    }

    void setValue(const Contact &value) {
        Value = value;
    }

    Node *getLeft() const {
        return left;
    }

    void setLeft(Node *left) {
        this->left = left;
    }

    Node *getRight() const {
        return right;
    }

    void setRight(Node *right) {
        this->right = right;
    }
    int getHeight() const {
        return height;
    }
    void setHeight(const int & height) {
        this->height = height;
    }

    ~Node() {
        delete left;
        delete right;
    }
};
class AVLTree {
private:
    Node* root;
    int height(Node* node) {
        if (node == nullptr) return 0;
        return node->getHeight();
    }

    int balanceFactor(Node* node) {
        if (node == nullptr) return 0;
        return height(node->getLeft()) - height(node->getRight());
    }

    void updateHeight(Node* node) {
        if (node == nullptr) return;
        node->setHeight( 1 + max(height(node->getLeft()), height(node->getRight())));
    }

    Node* rightRotate(Node* node) {
        Node* x = node->getLeft();
        Node* y = x->getRight();

        x->setRight(node);
        node->setLeft(y);

        updateHeight(node);
        updateHeight(y);

        return x;
    }
    Node* leftRotate(Node* node) {
        Node* y = node->getRight();
        Node* x = y->getLeft();

        y->setLeft(node);
        node->setRight(x);

        updateHeight(node);
        updateHeight(y);

        return y;
    }

public:
    AVLTree() {
        root = nullptr;
    }

    void insertNode(Node *node){}
    void deleteNode(const int &id){}
    Node* searchNode(const int &id){}
    void ListIDs(){}
    void displayTree(){}

};



class AddressLibrary {
private:
    AVLTree* tree;

    static int getID(ifstream &in){
        int value;
        try {
            cout << "Enter ID:";
            in >> value;
            return value;
        }catch (exception& e) {
            cout << "Please Enter a correct number: ";
        }
        return 0;
    }

    static string getEmail(ifstream &in) {
        const regex pattern(R"(^[a-zA-Z0-9.]+@[a-zA-Z]+\.[a-zA-Z]{2,}$)");
        string email;
        getline(in, email);
        if (regex_match(email, pattern))
            return email;
        cout << "Please Enter a correct Email\n";
        return "";
    }

    static string getPhoneNumber(ifstream &in) {
        const regex pattern("^01\d-\d{3}-\d{4}$");
        string number;
        getline(in, number);
        if (regex_match(number, pattern))
            return number;
        cout << "Please Enter a correct Phone\n";
        return "";
    }
    static string getUserName(ifstream &in) {
        string userName;
        getline(in, userName);
        return userName;
    }

public:
    AddressLibrary() {
        tree = new AVLTree();
    }
    void addContact(ifstream &input) const {
        Contact contact;
        int id = getID(input);
        contact.email = getEmail(input);
        contact.phone = getPhoneNumber(input);
        contact.name = getUserName(input);

        try {
            Node *newNode = new Node(id, contact);
            tree->insertNode(newNode);

            cout << "This Contact is added succefully" << endl;
        }
        catch (exception& e) {
            cout << "This id is added before\n";
        }

    }
    void searchContact(ifstream &input) {
        cout << "Which ID do you want to search about?";
        int id = getID(input);

        try {
            Node *node = tree->searchNode(id);
            Contact contact = node->getValue();

            cout << "ID" << node->getKey() << endl;
            cout << "Name" << contact.name << endl;
            cout << "Phone" << contact.phone << endl;
            cout << "Email" << contact.email << endl;
        }
        catch (exception& e) {
            cout << "This ID is not found in the tree\n";
        }
    }
    void deleteContact(ifstream &input) {
        cout << "Which ID do you want to delete about?";
        int id = getID(input);
        try {
            tree->deleteNode(id);
            cout << "This contact is deleted\n";
        }
        catch (exception& e) {
            cout << "This id is not found in the tree\n";
        }
    }
    void listContact(ifstream &input) {
        tree->ListIDs();
    }
    void DisplayContact(ifstream &input) {
        tree->displayTree();
    }
    ~AddressLibrary() {
        delete tree;
    }
};


int main() {
    ifstream inputFile;
    inputFile.open("Problem2 TestCases/input.txt");
    string input;
    AddressLibrary addressLibrary;

    while (getline(inputFile, input)) {
        if (input == "1")
            addressLibrary.addContact(inputFile);
        else if (input == "2")
            addressLibrary.searchContact(inputFile);
        else if (input == "3")
            addressLibrary.deleteContact(inputFile);
        else if (input == "4")
            addressLibrary.listContact(inputFile);
        else if (input == "5")
            addressLibrary.DisplayContact(inputFile);
        else
            cout << "Please enter a valid input\n";
    }
    inputFile.close();
    cout << "Thanks for using My program" << endl;
    return 0;
}