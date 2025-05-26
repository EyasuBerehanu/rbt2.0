//Eyasu Berehanu
//3/30/2025
//This is a binary search tree but now with red black tree insert and properties which allows you to insert values, read values from a file, delte values, and search up values in the bianry search tree 
//Sources: I mainly used the red black tree explained in 5 minutes video to help me build this but also stuff i learned from the first attempt https://www.youtube.com/watch?v=A3JZinzkMpk&ab_channel=MichaelSambol
//Mr Galbrith assistances on print from past heap project
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

using namespace std;
enum Color { //node colors for rbt
  BLACK, RED
};
struct Node {
    int data;
    Node* left; //left child pointer
    Node* right; //right child pointer
  Node* parent; //parent pointer
  Color color; //this is for red or black
  
    Node(int value) {
        color = RED;
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
void rotateRight(Node*& root, Node* current);
void rotateLeft(Node*& root, Node* current);
void fixInsert(Node*& root, Node* z);
Node* transplant(Node* root, Node* u, Node* v);
void fixRemove(Node*& root, Node*& x, Node* xParent);

void rotateRight(Node*& root, Node* current) { //this does the right roation for a given node
    Node* newRoot = current->left;
    current->left = newRoot->right;
    if (newRoot->right != nullptr)
        newRoot->right->parent = current;

    newRoot->parent = current->parent; //connects the newroot to currents parents
    if (current->parent == nullptr) {
        root = newRoot; 
    } else if (current->parent->right == current) { //if current is right child change parents right to new root
        current->parent->right = newRoot;
    } else { //else if left now it changes parents left to new root
        current->parent->left = newRoot;
    }

    newRoot->right = current;
    current->parent = newRoot;
}

void rotateLeft(Node*& root, Node* current) { //this does left roaion for a given node
    Node* newRoot = current->right;
    current->right = newRoot->left;
    if (newRoot->left != nullptr)
        newRoot->left->parent = current;

    newRoot->parent = current->parent;
    if (current->parent == nullptr) {
        root = newRoot;
    } else if (current->parent->left == current) { //if current is left child change parents left to new root
        current->parent->left = newRoot;
    } else { //else if right change parents right to new root
        current->parent->right = newRoot;
    }

    newRoot->left = current;
    current->parent = newRoot;
} 



void fixInsert(Node*& root, Node* z){
    while (z->parent && z->parent->color == RED) {
      if (z->parent == z->parent->parent->left) { //parent is left child
            Node* y = z->parent->parent->right;
	   
            if (y && y->color == RED) { //for case 1 when uncle is red
                z->parent->color = BLACK;         
                y->color = BLACK;                 
                z->parent->parent->color = RED;
                z = z->parent->parent;             
            }else {
	      if (z == z->parent->right) { //for case 2 when z is a right child
		z = z->parent;
		rotateLeft(root, z);
	      } 
	      z->parent->color = BLACK; //case 3 for when z is a left child
	      z->parent->parent->color = RED;
	      rotateRight(root, z->parent->parent); 
            }
      }else{ //mirroring the previous code for when parent is right
	  Node* y = z->parent->parent->left;
	  
	  if (y && y->color == RED) { //case 1 unc is red
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
	    }else{
	    if (z == z->parent->left) { //case 2 when z is left
                    z = z->parent;
                    rotateRight(root, z);
                }
	    z->parent->color = BLACK; //case 3 when z is right then we end by making sure the root is black
                z->parent->parent->color = RED;
                rotateLeft(root, z->parent->parent);
	    }
	}
    }
	root->color = BLACK;
    
}

Node* insert(Node* root, int value) { //insterts value into tree using the logic for Binary search tree
  Node* x = root;
  Node* y = nullptr;
  Node* newNode = new Node(value);
  //basically the binary search tree from before but had to change the way it worked so it would work with the fix insert
  while (x != nullptr){
    y = x;
    if(newNode->data < x->data){
      x = x->left;
    }else{
      x = x-> right;
    }
  }
  
  newNode->parent = y;
  if (y == nullptr) {
    root = newNode;
  }else if (newNode->data < y->data) { //uses recurtion to find the right postion for the value wanted for insert
        y->left = newNode;
    } else {
        y->right = newNode;
    }
  
  fixInsert(root, newNode); //here is where the tree fixes insert per rbt rules
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
Node* transplant(Node* root, Node* u, Node* v) { //this switchs the subtree root at u with the one rooted at v to help move subtrees for rbt
  if(u->parent == nullptr){ //u is the root
      root = v;
  }else if(u == u->parent->left){ //u is the left child
      u->parent->left = v;
  }else{ //u is the right child
      u->parent->right = v;
    }
  if(v!=nullptr){ //connects v parents 
      v->parent = u->parent;
    }
    return root;
}
void fixRemove(Node*& root, Node*& x, Node* xParent){
  while (x != root && (x == nullptr || x->color == BLACK)){
    if(x == xParent->left){ //if x is the left child
      Node* w = xParent->right;

      if (w != nullptr && w->color == RED){ //case 1 for when siblling is red which will roate it left the change color to get black
	w->color = BLACK;
	xParent->color = RED;
	rotateLeft(root, xParent);
	w = xParent->right;
      }
      if ((w == nullptr) || ((w->left == nullptr ||  w->left->color  == BLACK) &&(w->right == nullptr || w->right->color == BLACK))) { //case 2 for when all are black so we color sibling red
	if(w != nullptr){
	  w->color = RED;
	}
	x = xParent;
	xParent = xParent->parent;
      }else {
	if(w->right == nullptr || w->right->color == BLACK) { //case 3 for when right child is red and left is black which then we rotate left
           if (w->left != nullptr) {
	     w->left->color = BLACK;
	   }
	   
	   w->color = RED;
	   rotateRight(root, w);
	   w = xParent->right;
	}
	if (w != nullptr){ //case for when left child is red 
	  w->color = xParent->color; //colors w to match xparent color
	}
	xParent->color = BLACK; //colors xparent to black

	if (w != nullptr && w->left != nullptr){
	  w->right->color = BLACK; //changes to black
	}
	rotateLeft(root, xParent); //rotate xparent to the right to fix the double black
	x = root;
	break;
      }
    }else{ //mirror case so everything thing we did for when x was a right child but now left
      Node* w = xParent->left;

      if (w != nullptr && w->color == RED){//case 1 for when siblling is red which will roat\
e it left the change color to get black
	w->color = BLACK;
	xParent->color = RED;
	rotateRight(root, xParent);
	w = xParent->left;
      }
      if ((w == nullptr) || ((w->left == nullptr ||  w->left->color  == BLACK) &&(w->right == nullptr || w->right->color == BLACK))) { //case 2 for when all are black so we color sibling\red
	if(w != nullptr){
	  w->color = RED;
	}
	x = xParent;
	xParent = xParent->parent;
      }else {
	if(w->left == nullptr || w->left->color == BLACK) {//case 3 for when right child i\
s red and left is black which then we rotate left
           if (w->right != nullptr) {
	     w->right->color = BLACK;
	   }
	   
	   w->color = RED;
	   rotateLeft(root, w);
	   w = xParent->left;
	}
	if (w != nullptr){//case 4 for when left child is red
	  w->color = xParent->color;
	}
	xParent->color = BLACK;

	if (w != nullptr && w->left != nullptr){
	  w->left->color = BLACK;
	}
	rotateRight(root, xParent);
	x = root;
	break;
      }

    }
}

  if(x != nullptr){ //to make sure the node is black
    x->color = BLACK;
  }
}



Node* remove(Node* root, int value) { //removes node that was ask to be removed in the bianry search tree
  Node* z = root; //da root
  Node* y = z; //node that will end up being moved
  Node* x = nullptr; //replaces y
  bool firstColor = y->color;//keeps y starting color
  Node* xParent = nullptr; //for fixremove
  
  while (z != nullptr && z->data != value){ //gets the node to delete
    if(value < z->data){
      z = z->left;
    }else{
      z = z->right;
    }
  }
    if (root == nullptr) {
        return root;
    }

    if (z->right == nullptr) { //no right child 
      x = z->left;
      xParent = z->parent;
      root = transplant(root, z, z->left); //uses recurtion to remove the node from left if smaller value
    } else if (z->left == nullptr) { //no left child
      x = z->right;
      xParent = z->parent;
      root = transplant(root, z, z->right); //uses recurtion to remove the node from right if larger value
	
    } else { //node has 2 childs
      y = getSuccessor(z); //this is the smallest node in right subtree the succsessor
      firstColor = y->color;
      x = y->right;

      if (y->parent != z) { //moves right child up tree
	root = transplant(root, y, y->right);
	y->right = z->right;
	
	if(y->right != nullptr){
	  y->right->parent = y;
	}
	
        xParent = y->parent;
      }else{
	xParent = y;
      }
      
      root = transplant(root, z, y); //replaces root with y
      y->left = z->left;
      if (y->left != nullptr){
	y->left->parent = y;
      }
      
      y->color = z->color; //keps orginal coloer

    }
    delete z;

    if(firstColor == BLACK){ //fix if black node was delete uses fixRemove
      fixRemove(root, x, xParent);
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
    cout << root->data;
    if (root->color == RED) { //add the symoble of if the value is red or black for visualzation
        cout << " [R]";
    } else {
        cout << " [B]";
    }
    cout<< endl;
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
