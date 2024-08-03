#include "StackFrame.h"
#include <iostream>
#include <fstream>
#include "errors.h"
#include "constants.h"

using namespace std;
template<class T>
int AVLTree<T>::getHeightRec(Node *node) {
    if (node == NULL) return 0;
    int lh = this->getHeightRec(node->pLeft);
    int rh = this->getHeightRec(node->pRight);
    return (lh > rh ? lh : rh) + 1;
}
template<class T>
void AVLTree<T>::deleteTree(Node* node) {
    if (node == nullptr) return;
    deleteTree(node->pLeft);
    deleteTree(node->pRight);
    delete node;
}
template<class T>
typename AVLTree<T>::Node* AVLTree<T>::rotateRight(typename AVLTree<T>::Node* root) {
    Node* temp = root->pLeft;
    root->pLeft = temp->pRight;
    temp->pRight = root;
    return temp;
}
template<class T>
typename AVLTree<T>::Node* AVLTree<T>::rotateLeft(typename AVLTree<T>::Node* root) {
    Node* temp = root->pRight;
    root->pRight = temp->pLeft;
    temp->pLeft = root;
    return temp;
}
template<class T>
typename AVLTree<T>::Node* AVLTree<T>::insertRec(typename AVLTree<T>::Node* node, const T &value, const string key) {
    if (node == nullptr) return new Node(value, key);
    if (key < node->key) node->pLeft = insertRec(node->pLeft, value, key);
    else if (key > node->key) node->pRight = insertRec(node->pRight, value, key);
    else node->data = value;
    int balance = getBalance(node);
    if (balance > RH && key < node->pLeft->key) return rotateRight(node);
    if (balance < LH && key > node->pRight->key) return rotateLeft(node);
    if (balance > RH && key > node->pLeft->key) {
        node->pLeft = rotateLeft(node->pLeft);
        return rotateRight(node);
    }
    if (balance < LH && key < node->pRight->key) {
        node->pRight = rotateRight(node->pRight);
        return rotateLeft(node);
    }
    return node;
}
template<class T>
typename AVLTree<T>::Node* AVLTree<T>::deletion(typename AVLTree<T>::Node* root, string key) {
    if (!root) return nullptr;
    if (key > root->key) root->pRight = deletion(root->pRight, key);
    else if (key < root->key) root->pLeft = deletion(root->pLeft, key);
    else {// node with only one child or no child
        if ((root->pLeft == NULL) || (root->pRight == NULL)) {
            Node *temp = root->pLeft ? root->pLeft : root->pRight;
            if (temp == NULL) {// No child case
                temp = root;
                root = NULL;
            }
            else *root = *temp;
            delete temp;
        }
        else {
            Node *cur = root->pLeft; // travel node to find the greatest node of left subtree
            while (cur->pRight != nullptr) cur = cur->pRight;
            root->key = cur->key;
            root->data = cur->data;
            root->pLeft = deletion(root->pLeft, cur->key);
        }
    }
    if (!root) return root;
    int balance = getBalance(root);
    if(balance > RH && getBalance(root->pLeft) >= EH) return rotateRight(root);// LL rotation
    if(balance < LH && getBalance(root->pRight) <= EH) return rotateLeft(root);// RR rotation
    if(balance > RH && getBalance(root->pLeft) < EH) {// LR rotation
        root->pLeft = rotateLeft(root->pLeft);
        return rotateRight(root);
    }
    if(balance < LH && getBalance(root->pRight) > EH) {// RL rotation
        root->pRight = rotateRight(root->pRight);
        return rotateLeft(root);
    }
    return root;
}
template<class T>
typename AVLTree<T>::Node* AVLTree<T>::searchRec(typename AVLTree<T>::Node* node, const string value) {
    if (node == NULL) return NULL;
    if (value == node->key) return node;
    if (value < node->key) return searchRec(node->pLeft, value);
    return searchRec(node->pRight, value);
}
template<class T>
std::string AVLTree<T>::findParentNode(const std::string value) {
    typename AVLTree<T>::Node* current = root;
    typename AVLTree<T>::Node* parent = NULL;
    while(current != NULL && current->key != value) {
        parent = current;
        if (value < current->key) current = current->pLeft;
        else current = current->pRight;
    }
    if (!parent) return "null";
    if (!current) return "undefine";
    return parent->key;
}
void DoublyLinkedList::add(operation e) {
    Node* newNode = new Node(e);
    if (count == 0) {
        head = newNode;
        tail = newNode;
    }
    else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    count++;
}
operation DoublyLinkedList::removeAt(int index) {
    // if (index < 0 || index >= count) throw out_of_range("Index is out of range");
    operation removeValue = operation(0, OTHER);
    Node* temp;
    if (index == 0) {
        removeValue = head->value;
        temp = head;
        head = temp->next;
        if (temp == tail) tail = NULL;
    }
    else {
        Node* current;
        if (index < count/2) {
            current = head;
            for (int i = 0; i < index - 1; ++i) current = current->next;
            temp = current->next;
            removeValue = temp->value;
            current->next = temp->next;
            temp->next->prev = current;
        }
        else if (index == count - 1) {
            removeValue = tail->value;
            temp = tail;
            tail = tail->prev;
            tail->next = NULL;
        }
        else {
            current = tail;
            for (int i = count - 1; i > index + 1; --i) current = current->prev;
            temp = current->prev;
            removeValue = temp->value;
            current->prev = temp->prev;
            temp->prev->next = current;
        }
    }
    delete temp;
    count--;
    return removeValue;
}
operation DoublyLinkedList::get(int index) {
    // if (index < 0 || index >= count) throw out_of_range("Index is out of range");
    Node* current;
    if (index < count/2) {
        current = head;
        for (int i = 0; i < index; ++i) current = current->next;
    }
    else {
        current = tail;
        for (int i = count - 1; i > index; --i) current = current->prev;
    }
    return current->value;
}
void DoublyLinkedList::clear() {
    while(count) removeAt(0);
    head = NULL;
    tail = NULL;
}
StackFrame::StackFrame() : opStackMaxSize(OPERAND_STACK_MAX_SIZE), 
    localVarSpaceSize(LOCAL_VARIABLE_SPACE_SIZE), line(1), count(0) {
    list = new DoublyLinkedList();
    avlTree = new AVLTree<operation>();
}
StackFrame::~StackFrame() {
    delete list;
    delete avlTree;
}
void StackFrame::push(operation e) {
    if (size() == opStackMaxSize / 2) throw StackFull(line);
    list->add(e);
}
void StackFrame::pop() {
    if (empty()) throw StackEmpty(line);
    list->removeAt(size() - 1);
}
operation StackFrame::top() {
    if (empty()) throw StackEmpty(line);
    return list->get(size() - 1);
}
bool StackFrame::checkChar(std::string str) {
    int size = str.size();
    for (int i = 0; i < size; ++i) 
        if (!isalpha(str[i])) return false;
    return true;
}
void StackFrame::calculateBasicAndLogic(std::string str) {
    operation topFirst = this->top();
    this->pop();
    operation topSecond = this->top();
    this->pop();
    if (str[0] == 'i') {
        // cout << str << endl;
        if (topFirst.getStatus() == FLOAT || topSecond.getStatus() == FLOAT)
            throw TypeMisMatch(line);
        topFirst.setOper(int(topFirst.getOper()));
        topSecond.setOper(int(topSecond.getOper()));
        if (str == "iadd") this->push(topFirst + topSecond);
        else if (str == "isub") this->push(topSecond - topFirst);
        else if (str == "imul") this->push(topFirst * topSecond);
        else if (str == "idiv") {
            if (topFirst.getOper() == 0) throw DivideByZero(line);
            this->push(topSecond / topFirst);
        }
        else if (str == "irem") {
            if (topFirst.getOper() == 0) throw DivideByZero(line);                
            this->push(topSecond % topFirst);
        }
        else if (str == "iand") this->push(topFirst & topSecond);
        else if (str == "ior") this->push(topFirst | topSecond);            
        else if (str == "ieq") this->push(operation(topFirst == topSecond, topFirst.getStatus()));
        else if (str == "ineq") this->push(operation(!(topFirst == topSecond), topFirst.getStatus()));
        else if (str == "ilt") this->push(operation(topSecond < topFirst, topFirst.getStatus()));
        else this->push(operation(topSecond > topFirst, topFirst.getStatus())); // igt
    }
    else { // str[0] = 'f'
        if (topFirst.getStatus() == INT) topFirst.setStatus(FLOAT);
        if (topSecond.getStatus() == INT) topSecond.setStatus(FLOAT);
        if (str == "fadd") this->push(topFirst + topSecond);
        else if (str == "fsub") this->push(topSecond - topFirst);
        else if (str == "fmul") this->push(topFirst * topSecond);
        else if (str == "fdiv") {
            if (topFirst.getOper() == 0) throw DivideByZero(line);
            this->push(topSecond / topFirst);
        }
        else if (str == "feq") this->push(operation(topFirst == topSecond, topFirst.getStatus()));
        else if (str == "fneq") this->push(operation(!(topFirst == topSecond), topFirst.getStatus()));
        else if (str == "flt") this->push(operation(topSecond < topFirst, topFirst.getStatus()));
        else this->push(operation(topSecond > topFirst, topFirst.getStatus())); // fgt
    }
}
void StackFrame::checkTypeAndConvert(std::string str) {
    if (str == "top") {
        cout << this->top().getOper() << "\n";
        return;
    }
    operation topFirst = this->top();
    this->pop();
    if (str[0] == 'i') {
        if (topFirst.getStatus() == FLOAT) throw TypeMisMatch(line);
        if (str == "ineg") {
            if (topFirst.getOper() == 0) 
                this->push(operation((int)topFirst.getOper(), topFirst.getStatus()));
            else this->push(operation(-1*topFirst.getOper(), topFirst.getStatus()));
        }
        else if (str == "i2f") this->push(operation(topFirst.getOper(), FLOAT));
        else if (str == "ibnot") 
        this->push(operation(topFirst.getOper() == 0? 1 : 0, topFirst.getStatus()));
    }
    else if (str[0] == 'f') {
        if (topFirst.getStatus() == INT) topFirst.setStatus(FLOAT);
        if (str == "fneg") this->push(operation(-1*topFirst.getOper(), topFirst.getStatus()));
        else if (str == "f2i") this->push(operation((int)topFirst.getOper(), INT));
    }
}
void StackFrame::loadAndStore(std::string str, std::string value) {
    if (!checkChar(value)) return;
    if (str == "iload" || str == "fload") {
        operation temp = avlTree->getData(avlTree->search(value));
        if (temp == operation(-1, INT)) throw UndefinedVariable(line);
        if (str[0] == 'i' && temp.getStatus() == FLOAT) throw TypeMisMatch(line);
        else if (str[0] == 'f' && temp.getStatus() == INT) throw TypeMisMatch(line);
        this->push(temp);
    }
    else if (str == "istore" || str == "fstore") {
        operation temp = this->top();
        this->pop();
        if (str[0] == 'i' && temp.getStatus() == FLOAT) throw TypeMisMatch(line);
        else if (str[0] == 'f' && temp.getStatus() == INT) throw TypeMisMatch(line);
        if (avlTree->search(value)) avlTree->insert(temp, value);
        else {
            if (count >= localVarSpaceSize / 2) throw LocalSpaceFull(line);
            avlTree->insert(temp, value);
            count++;
        }
    }
    else if (str == "val") {
        operation temp = avlTree->getData(avlTree->search(value));
        if (temp == operation(-1, OTHER)) throw UndefinedVariable(line);
        cout << temp.getOper() << "\n"; 
    }  
    else { // par
        string res = avlTree->findParentNode(value);
        if (res == "undefine") throw UndefinedVariable(line);
        cout << res << "\n";
    }
}
void StackFrame::run(string filename) {
    ifstream ifs(filename);
    if (!ifs.is_open()) return;
    string str;
    while(getline(ifs, str)) {
        if (str == "") continue;
        int size = str.size();
        const size_t index = str.find(" ");
        string instruction = str;
        if (index != std::string::npos) instruction = str.substr(0, index);
        if (instruction == "iadd" || instruction == "fadd" || instruction == "isub" 
            || instruction == "fsub" || instruction == "imul" || instruction == "fmul" 
            || instruction == "idiv" || instruction == "fdiv" || instruction == "irem" 
            || instruction == "iand" || instruction == "ior" || instruction == "ieq" 
            || instruction == "feq" || instruction == "ineq" || instruction == "fneq" 
            || instruction == "ilt" || instruction == "flt" || instruction == "igt" 
            || instruction == "fgt") calculateBasicAndLogic(instruction);
        else if (instruction == "ineg" || instruction == "fneg" || instruction == "ibnot" 
            || instruction == "i2f" || instruction == "f2i" || instruction == "top"
            ) checkTypeAndConvert(instruction);
        else {
            string strValue = str.substr(index + 1, size - index);
            if (instruction == "iconst" || instruction == "fconst") {
                float value;
                if (str.find(".") != string::npos) value = stof(strValue);
                else value = stoi(strValue);
                if (instruction == "iconst") {
                    this->push(operation(int(value), INT));
                }
                else this->push(operation(value, FLOAT));
            }
            else loadAndStore(instruction, strValue);
        }
        line++;
    }
    ifs.close();
}