#ifndef __STACK_FRAME_H__
#define __STACK_FRAME_H__
#include <queue>
#include <math.h>
#include <iostream>
using namespace std;
#include <string>
#include <stack>
/*
StackFrame declaration
*/
enum typeStatus {INT = 0, FLOAT, OTHER};
enum BalanceValue {LH = -1, EH, RH};
class operation {
    float oper;
    typeStatus status;
public:  
    operation() {}
    operation(float oper, typeStatus status) : oper(oper), status(status) {}
    void setOper(float oper) {this->oper = oper;}
    void setStatus(typeStatus status) {this->status = status;}
    float getOper() {return oper;}
    typeStatus getStatus() {return status;}
    bool operator==(const operation& other) const {
        return oper == other.oper && status == other.status;
    }
    bool operator<(const operation& other) const {
        return oper < other.oper && status == other.status;
    }
    bool operator>(const operation& other) const {
        return oper > other.oper && status == other.status;
    }
    operation operator+(const operation& other) const {
        return operation(this->oper + other.oper, this->status);
    }
    operation operator-(const operation& other) const {
        return operation(this->oper - other.oper, this->status);
    }
    operation operator*(const operation& other) const {
        return operation(this->oper * other.oper, this->status);
    }
    operation operator/(const operation& other) const {
        operation res =  operation(this->oper / other.oper, this->status);
        if (this->status == INT) res.setOper(int(res.getOper()));
        return res;
    }
    operation operator%(const operation& other) const {
        return operation(this->oper - int(this->oper / other.oper) * other.oper, this->status);
    }
    operation operator&(const operation& other) const {
        return operation((int)this->oper & (int)other.oper, this->status);
    }
    operation operator|(const operation& other) const {
        return operation((int)this->oper | (int)other.oper, this->status);
    }
    // void operator<<(const operation& other)
    // {
    //     cout << other.oper;
    // }
};

template<class T>
class AVLTree {
public: class Node;
private: Node *root;
private:
    int getHeightRec(Node *node);
    void deleteTree(Node* node);
    Node* rotateRight(Node* root);
    Node* rotateLeft(Node* root);
    Node* insertRec(Node* node, const T &value, const std::string key);
    Node* deletion(Node*root, std::string key);
    Node* searchRec(Node* node, const std::string value);
public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() {deleteTree(root);}
    void printNSpace(int n)
{
    for (int i = 0; i < n - 1; i++)
        cout << " ";
}

void printInteger(int &n)
{
    cout << n << " ";
}
void printTreeStructure() {
    int height = this->getHeight();
    if (this->root == nullptr) {
        std::cout << "NULL\n";
        return;
    }

    queue<Node *> q;
    q.push(root);
    int level = 0;
    int maxNodes = 1;
    int space = pow(2, height + 1);

    printNSpace(space / 2);
    while (!q.empty()) {
        int nodesAtLevel = 0;
        for (int i = 0; i < maxNodes; ++i) {
            Node* currentNode = q.front();
            q.pop();

            if (currentNode) {
                cout << currentNode->key << ":" << currentNode->data.getOper();
                q.push(currentNode->pLeft);
                q.push(currentNode->pRight);
            } else {
                cout << " ";
                q.push(nullptr);
                q.push(nullptr);
            }

            printNSpace(space);
            nodesAtLevel++;
        }

        cout << endl;
        level++;
        maxNodes *= 2;
        space /= 2;
        printNSpace(space / 2);

        if (level == height) {
            break;
        }
    }
}

    int getHeight() {return this->getHeightRec(this->root);}
    void insert(const T &value, const std::string key) {root = insertRec(root, value, key);}
    void remove(const std::string key) {root = deletion(root, key);}
    int getBalance(Node*subroot) {
        return !subroot? 0 : getHeightRec(subroot->pLeft)- getHeightRec(subroot->pRight);
    }
    Node* search(const std::string value) {return searchRec(root, value);}
    T getData(Node* node) {return !node? T(-1, OTHER) : node->data;}
    std::string findParentNode(const std::string value);
public:
    class Node {
        std::string key;
        T data;
        Node *pLeft, *pRight;
        BalanceValue balance;
        friend class AVLTree<T>;
    public:
        Node(T value, std::string key) : key(key), data(value), pLeft(NULL), pRight(NULL), balance(EH) {}
        ~Node() {}
    };
};
class DoublyLinkedList {
protected: class Node;
    Node* head;
    Node* tail;
    int count;
public:
    DoublyLinkedList() : head(NULL), tail(NULL), count(0) {}
    ~DoublyLinkedList() {clear();}
    void add(operation e);
    operation removeAt(int index);
    operation get(int index);
    bool empty() {return count == 0;}
    int size() {return count;}
    void clear();
protected:
    class Node {
    public:
        operation value;
        Node* next;
        Node* prev;
        Node(operation value) : value(value), next(NULL), prev(NULL) {}
    };
};
class StackFrame {
    int opStackMaxSize; // max size of operand stack
    int localVarSpaceSize; // size of local variable array
    int line;
    int count; // size of current avlTree
    DoublyLinkedList* list;
    AVLTree<operation>* avlTree;
public:
    /*
    Constructor of StackFrame
    */
    StackFrame();
    ~StackFrame();
    void push(operation e);
    void pop();
    operation top();
    bool checkChar(std::string str);
    bool empty() {return list->empty();}
    int size() {return list->size();}
    void calculateBasicAndLogic(std::string str);
    void checkTypeAndConvert(std::string str);
    void loadAndStore(std::string str, std::string value);
    /*
    Run the method written in the testcase
    @param filename name of the file
    */
    void run(std::string filename);
};

#endif // !__STACK_FRAME_H__