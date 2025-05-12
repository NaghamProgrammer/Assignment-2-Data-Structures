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
    Node(const int &id,const Contact &value): height(1) {
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
        updateHeight(x);

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

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current && current->getLeft() != nullptr)
            current = current->getLeft();
        return current;
    }

public:
    AVLTree() {
        root = nullptr;
    }

    Node* insertNode(Node* parent, Node* NewNode) {
        if (parent == nullptr)
            return NewNode;

        if (NewNode->getKey() < parent->getKey())
            parent->setLeft(insertNode(parent->getLeft(), NewNode));
        else if (NewNode->getKey() > parent->getKey())
            parent->setRight(insertNode(parent->getRight(), NewNode));
        else
            throw runtime_error("Duplicate key");

        updateHeight(parent);
        int balance = balanceFactor(parent);

        if (balance > 1) {
            if (NewNode->getKey() < parent->getLeft()->getKey())
                return rightRotate(parent);
            parent->setLeft(leftRotate(parent->getLeft()));
            return rightRotate(parent);
        }

        // Right Heavy
        if (balance < -1) {
            if (NewNode->getKey() > parent->getRight()->getKey())
                return leftRotate(parent);
            parent->setRight(rightRotate(parent->getRight()));
            return leftRotate(parent);
        }

        return parent;
    }

    Node* deleteNode(const int &id, Node* node) {
        if (node == nullptr)
            throw runtime_error("Empty Tree");

        if (node->getKey() > id)
            node->setLeft(deleteNode( id, node->getLeft()));
        else if (node->getKey() < id)
            node->setRight(deleteNode( id, node->getRight()));
        else {
            if (node->getLeft() == nullptr || node->getRight() == nullptr) {
                Node* temp = node->getLeft() ? node->getLeft() : node->getRight();
                if (temp == nullptr) {
                    temp = node;
                    node = nullptr;
                } else {
                    delete node;
                    node = temp;
                }

                delete temp;
            } else {
                Node* temp = minValueNode(node->getRight());
                node->setKey(temp->getKey());
                node->setValue(temp->getValue());
                node->setRight(deleteNode(temp->getKey(),node->getRight()));
            }
        }

        updateHeight(node);
        int balance = balanceFactor(node);

        if (balance > 1 && balanceFactor(node->getLeft()) >= 0)
            return rightRotate(node);
        if (balance > 1 && balanceFactor(node->getLeft()) < 0) {
            node->setLeft(leftRotate(node->getLeft()));
            return rightRotate(node);
        }
        if (balance < -1 && balanceFactor(node->getRight()) <= 0)
            return leftRotate(node);
        if (balance < -1 && balanceFactor(node->getRight()) > 0) {
            node->setRight(rightRotate(node->getRight()));
            return leftRotate(node);
        }

        return node;
    }

    Node* searchNode(const int &id, Node* node){
        if (node == nullptr)
            throw runtime_error("Empty Tree");

        if (node->getKey() > id)
            return searchNode(id, node->getLeft());
        if (node->getKey() < id)
            return searchNode(id, node->getRight());
        if (node->getKey() == id)
            return node;
        throw runtime_error("No such node");
    }

    void ListIDs(Node* node) {
        if (root == nullptr)
            throw runtime_error("Address Book is empty");
        if (node == nullptr)
            return;

        ListIDs(node->getLeft());
        Contact contact = node->getValue();

        cout << "ID:" << node->getKey()
             << ", Name:" << contact.name
             << ", Phone:" << contact.phone
             << ", Email:" << contact.email << endl;

        ListIDs(node->getRight());
    }

    void displayTree(Node* node, int space = 0, int height = 10) {
        if (node == nullptr)
            return;

        space += height;
        displayTree(node->getRight(), space);
        cout << endl;
        for (int i = height; i < space; i++) cout << " ";
        cout << node->getKey() << "\n";
        displayTree(node->getLeft(), space);
    }

    Node* getRoot() {
        return root;
    }

    void setRoot(Node* root) {
        this->root = root;
    }

    ~AVLTree() {
        delete root;
    }

};



class AddressLibrary {
private:
    AVLTree* tree;

    static int getID(ifstream &in){
        int value;
        try {
            in >> value;
            in.ignore();
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
        const regex pattern("^01\\d-\\d{3}-\\d{4}$");
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
        contact.name = getUserName(input);
        contact.phone = getPhoneNumber(input);
        contact.email = getEmail(input);

        try {
            Node *newNode = new Node(id, contact);
            tree->setRoot(tree->insertNode(tree->getRoot(),newNode));

            cout << "This Contact is added succefully" << endl;
        }
        catch (exception& e) {
            cout << "Error: Contact with ID " << id << " already exists.\n";
        }

    }

    void searchContact(ifstream &input) {
        cout << "Which ID do you want to search about?\n";
        int id = getID(input);

        try {
            Node *node = tree->searchNode(id, tree->getRoot());
            Contact contact = node->getValue();

            cout << "ID: " << node->getKey() << endl;
            cout << "Name: " << contact.name << endl;
            cout << "Phone: " << contact.phone << endl;
            cout << "Email: " << contact.email << endl;
        }
        catch (exception& e) {
            cout << "Contact Not Found\n";
        }
    }

    void deleteContact(ifstream &input) {
        cout << "Which ID do you want to delete about?\n";
        int id = getID(input);
        try {
            tree->setRoot(tree->deleteNode(id, tree->getRoot()));
            cout << "This contact is deleted\n";
        }
        catch (exception& e) {
            cout << "Contact not Found\n";
        }
    }

    void listContact() {
        try {
            tree->ListIDs(tree->getRoot());
        }
        catch (exception& e) {
            cout << e.what() << endl;
        }
    }

    void DisplayContact() {
        tree->displayTree(tree->getRoot());
    }

    ~AddressLibrary() {
        delete tree;
    }
};


int main() {
    ifstream inputFile;
    string input, fileName;
    AddressLibrary addressLibrary;

    while (true) {
        cout << "# === Welcome To Our Address Book Tree === #\n";
        cout << "1. Add New Contact"
        << "\n2. Search for Contact"
        << "\n3. Delete Contact"
        << "\n4. List All Contact"
        << "\n5. Display Contact tree structure\n";
        cout << "Enter Your File Path(Enter 0 here to close program): ";
        getline(cin, fileName);

        if (fileName == "0") {
            cout << "Thank you for using our address library.\n";
            return 0;
        }

        try {
            inputFile.open(fileName);
            while (getline(inputFile, input)) {
                if (input == "1")
                    addressLibrary.addContact(inputFile);
                else if (input == "2")
                    addressLibrary.searchContact(inputFile);
                else if (input == "3")
                    addressLibrary.deleteContact(inputFile);
                else if (input == "4")
                    addressLibrary.listContact();
                else if (input == "5")
                    addressLibrary.DisplayContact();
            }
            inputFile.close();
            cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
        }
        catch (exception& e) {
            cout << "Wrong FileName" << endl;
        }
    }
    return 0;
}
