
//Eyasu Berehanu
//3/30/2025
//This is a binary search tree which allows you to insert values, read values from a file, delte values, and search up values in the bianry search tree 
//Sources:
//Some help from my brother with getsuccessor, 
//Geeksforgeeks on helping how to learn how to do insert, search and delte 
//Mr Galbrith assistances on print from past heap project
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

using namespace std;
enum Color {
  BLACK, RED
};
struct Node {
    int data;
    Node* left; //left child pointer
    Node* right; //right child pointer
    Node* parent;
    Color color;
    Node(int value) {
        data = value;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
};

Node* insert(Node* root, int value);
void printTree(Node* root, int depth);
void filess(Node*& root, int& size);
Node* remove(Node* root, int value);
Node* getSuccessor(Node* cur); 
Node* search(Node* root, int value);

void rotateRight(Node*& root, Node* current) {
    Node* newRoot = current->left;
    current->left = newRoot->right;
    if (newRoot->right != nullptr)
        newRoot->right->parent = current;

    newRoot->parent = current->parent;
    if (current->parent == nullptr) {
        root = newRoot; 
    } else if (current->parent->right == current) {
        current->parent->right = newRoot;
    } else {
        current->parent->left = newRoot;
    }

    newRoot->right = current;
    current->parent = newRoot;
}

void rotateLeft(Node*& root, Node* current) {
    Node* newRoot = current->right;
    current->right = newRoot->left;
    if (newRoot->left != nullptr)
        newRoot->left->parent = current;

    newRoot->parent = current->parent;
    if (current->parent == nullptr) {
        root = newRoot;
    } else if (current->parent->left == current) {
        current->parent->left = newRoot;
    } else {
        current->parent->right = newRoot;
    }

    newRoot->left = current;
    current->parent = newRoot;
} 



void fixInsert(Node*& root, Node* z){
    while (z->parent && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;

            if (y && y->color == RED) {
                z->parent->color = BLACK;         
                y->color = BLACK;                 
                z->parent->parent->color = RED;
                z = z->parent->parent;             
            }else {
	      if (z == z->parent->right) {
		z = z->parent;
		//rotateLeft(root, z)
	      } 
	      z->parent->parent->color = BLACK; //come back to this
	      z->parent->color = RED;
	      //rotataeRight(root, z) 
            }
	}else{
	  Node* y = z->parent->parent->left;
	  
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
	    }else{
	      if (z == z->parent->left) {
                    z = z->parent;
                    //rotateRight(root, z);
                }
	        z->parent-parent->color = BLACK; //RETURN
                z->parent->color = RED;
                //rotateLeft(root, z);
	    }
	}
	root->color = BLACK;
    }
}
Node* insert(Node* root, int value) { //insterts value into tree using the logic for Binary search tree
    if (root == nullptr) {
        return new Node(value);
    }

    if (value < root->data) { //uses recurtion to find the right postion for the value wanted for insert
        root->left = insert(root->left, value);
    } else {
        root->right = insert(root->right, value);
    }

    return root;
}

Node* getSuccessor(Node* cur) { //finds the next large node or the successor of a node or number
    if (cur == nullptr || cur->right == nullptr){ //whyen the current node or right clid is a null then there is no succesor
       return nullptr; 
    } 
    cur = cur->right;
    while (cur->left != nullptr) {
        cur = cur->left;
    }
    return cur;
}

Node* remove(Node* root, int value) { //removes node that was ask to be removed in the bianry search tree
    if (root == nullptr) {
        return nullptr;
    }

    if (value < root->data) { 
        root->left = remove(root->left, value); //uses recurtion to remove the node from left if smaller value
    } else if (value > root->data) { 
        root->right = remove(root->right, value); //uses recurtion to remove the node from right if larger value
    } else {
        if (root->left == nullptr && root->right == nullptr) {
            delete root;
            return nullptr;
        }
        else if (root->left == nullptr) { //replaces root with right child if need to 
            Node* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == nullptr) { //replaces root with left child if need to 
            Node* temp = root->left;
            delete root;
            return temp;
        }
        else {
            Node* successor = getSuccessor(root); //replaces with the succersor if both exist inorder succersor
        if (successor != nullptr) {  
                root->data = successor->data;
                root->right = remove(root->right, successor->data);
            }
        }
    }
    return root;
}

Node* search(Node* root, int value) { //searchs for a value in binary search tree
    if (root == nullptr || root->data == value) {
        return root;
    }

    if (value > root->data) {
        return search(root->right, value); //uses recurtyion to seatch the right if value we are searching for is larger
    }

    return search(root->left, value); //uses recurtyion to seatch the left if value we are searching for is smaller
}

void printTree(Node* root, int depth = 0) { //prints tree similar to heap
    if (root == nullptr){
       return; 
    } 

    printTree(root->right, depth + 1); //uses recurtion to print right sideof a tree
    for (int i = 0; i < depth; i++) {
        cout << "   ";
    }
    cout << root->data << endl;
    printTree(root->left, depth + 1);  //uses recurtion to print left sideof a tree
}

void filess(Node*& root) {
    ifstream file("numbers.txt");
    int value;
    while (file >> value) {
        if (value >= 1 && value <= 999) {
            root = insert(root, value);
        }
    }
    file.close();
}

int main() {
    Node* root = nullptr;
    int choice;

    cout << "Choose:" << endl;
    cout << "1 for file input" << endl;
    cout << "2 for manual input" << endl;
    cin >> choice;

    if (choice == 1) { //if 1 is chosen then it goes to numbers.txt and prints those numbers
        filess(root);
    } else if (choice == 2) {  //if 2 is chosen then it lets u imput your own order of numbers
        cout << "Enter numbers and type done to finish it (only nums from 1-999 plz)" << endl;
        while (true) {
            char input[10];
            cin >> input;
            if (strcmp(input, "done") == 0){
               break; 
            } 
            int value = atoi(input);
            if (value >= 1 && value <= 999) {
                root = insert(root, value);
            }
        }
    }
    printTree(root);
    while (true) {
        cout << " " << endl;
        
        cout << " " << endl; 
        
        //prompts to seach insert remove and quit
        cout << "Type I to insert a number" << endl;
        cout << "Type R to remove a number" << endl;
        cout << "Type S to search for a number" << endl;
        cout << "Type Q to quit" << endl;

        int value;
        char input[10];
            cin >> input;
            
        if (strcmp(input, "I") == 0) { //if I is types it ask for a value to insert
            cout << "Enter number to insert ";
            cin >> value;
            if (value >= 1 && value <= 999) {
                root = insert(root, value);
                printTree(root);
            }
        } else if (strcmp(input, "R") == 0) { //if R is types it ask for a value to remove
            cout << "Enter number to remove: ";
            cin >> value;
            root = remove(root, value);
            printTree(root);
        } else if (strcmp(input, "S") == 0) { //if S is types it ask for a value to Search
            cout << "Enter number to search for ";
            cin >> value;
            
            Node* result = search(root, value);
            if (result != nullptr) {
                printTree(root);
                cout << value << " is in the tree" << endl;
            } else {
                printTree(root);
                cout << value << " is not in the tree" << endl;
            } 
        } else {
            break; //anything else it breaks
        }
    }

    return 0;
}
