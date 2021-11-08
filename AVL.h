/****************
Template created by Kazumi Slott
CS311

Your name: Jorge Ezequiel Garcia Lopez
Your programmer number: 08
Any difficulties?: 
*****************/

#ifndef AVL_H
#define AVL_H
#include <iostream>
#include <queue>
#include <stack>
using namespace std;

//forward declaration of AVL class. We need this because AVL class is used as a friend in Node. 
//Check your stack_L_T.h if you don't know what to put here.
template<class T>
class AVL;

//Make Node and AVL template classes.
//You put the class definition and member function implementations in .h when you use template.  
//If you don't know how to make them template,
//Check page 3 of CS211 "Lec Notes on Exception Handling". I have a template stack class there.
//Also you made stack_L_T.h using template at the beginning of this semester.

template<class T>  //make it a template class
class Node
{
  friend AVL<T>; //AVL wants to access private members of Node
private:
  T el;
  Node* right;
  Node* left;
  Node* up;
  int bf; // [-2, -1, 0, +1, +2] possible balance factor values
public:
  Node() {right = left = up = NULL;bf = 0;}; //default constructor. set right and left to NULL
  Node(const T& e){el = e;}; 
  T getEl() const {return el;} //returns el
};

//Make it a template class
template<class T>
class AVL
{
private:
  Node<T>* root; //check stack_L_T.h to see how declared top
  void destroy(Node<T>* p); //private function. WHY?
  void inOrderPrint(Node<T>* p);

  /** NEW AVL Functions **/
  Node<T>* rightRotation(Node<T>* P);
  Node<T>* leftRotation(Node<T>* P);
  void balanceTree(Node<T>* newNode);
  Node<T>* updatebf(Node<T>* Q);

 public:
  AVL() {root = NULL;} //implement constructor here
  ~AVL(); 
  void insertNodeI(const T& e);
  void inOrderPrint();
  void BFTprint();
};

//implement the member functions in the header if you are making a template class

//destructor.
//If you don't know how to make the destructor, check the following
//CS211 Lec notes on AVL recursive functions - destroy and in order traversal
//CS211 Lecture recording - Lecture 4/28(Wed) - AVL:: inOrderPrint(), destroy()

/**
 * This AVL destructor utilizes the destroy function
 * to delete the nodes of the Binary Search Tree.
 */
template<class T>
AVL<T>::~AVL()
{
  destroy(root);
}

/**
 * This function deletes the nodes of a Binary Search Tree.
 * @p Node to be deleted
 */
template<class T>
void AVL<T>::destroy(Node<T>* p) //private function. WHY?
{
  //destroy all nodes
  if (p == NULL)
    return;
  destroy(p->left);
  destroy(p->right);
  delete p;
}

//This is an insert function using iteration.
//You will see insert function using iteration is unwieldy compared to that using recursion above.
template <class T>
void AVL<T>::insertNodeI(const T& e)
{
  Node<T>* newNode = new Node<T>;//make a new node.  Check stack_L_T.h to see how you made a new node.
  newNode->el = e;
  Node<T>* p  = root;
  Node<T>* parent = NULL; //points to the parent of p. 

  //move p to NULL
  while(p != NULL)
    {
      //parent points to the parent node while p points to the left or right child.
      parent = p; 
      if(e < p->el)
	      p = p->left;
      else //p->el <= e
	      p = p->right;
    }

  if(parent == NULL) //tree is empty
    root = newNode;
  else if(e < parent->el) //insert a child as the parent's left child
  {
    parent->left = newNode;
    newNode->up = parent;
    balanceTree(newNode);
  }
  else //parent->el <= e //insert a child as the parent's right child
  {
    parent->right = newNode;
    newNode->up = parent;
    balanceTree(newNode);
  }
}

//If you don't how to make this, check the following
//CS211 Lec notes on AVL recursive functions - destroy and in order traversal
//CS211 Lecture recording - Lecture 4/28(Wed) - AVL:: inOrderPrint(), destroy()

template <class T>
void AVL<T>::inOrderPrint()
{
  inOrderPrint(root);
}

template <class T> //private function. WHY?
void AVL<T>::inOrderPrint(Node<T>* p)
{
  if (p == NULL)
    return;
  inOrderPrint(p->left);
  cout << p->el << "-->";
  inOrderPrint(p->right);
}

template <class T>
void AVL<T>::BFTprint()  //Breadth First Traversal Print
{
    //Use the library's queue. https://www.google.com/url?client=internal-element-cse&cx=000747561938282180878:lrfgxurgbda&q=https://www.cplusplus.com/reference/queue/queue/&sa=U&ved=2ahUKEwimjMj9v-nzAhXDCjQIHXeYDm8QFnoECAUQAQ&usg=AOvVaw3vUfDpaPV6Q--9naQsl681

    //Make sure to include <queue> at the top
    //create a queue obj of pointers to Nodes
    
    //algorithm is discussed in lecture
    
    //NOTICE
    //front() gives you the front element but it doesn't remove it from the queue.
    //pop() removes the front element

  // Breadth First Traversal moves through 1st to nth level

  queue<Node<T>*> queue;
  Node<T>* p = root;

  if (p != NULL)
    queue.push(p);

  while(!queue.empty())
  {
    p = queue.front();

    if (p != NULL)
    {
      if (p->up == NULL)
        cout << p->el << " " << p->bf << " -->"; 
      else
        cout << p->el << " " << p->bf << " " << p->up->el << "-->" ; 

      queue.pop();

      if (p->left != NULL)
        queue.push(p->left);
      if (p->right != NULL)
        queue.push(p->right);
    }
  }
} // end BFT

template <class T>
void AVL<T>::balanceTree(Node<T>* newNode)
{
    //newNode points to the newly added node
    //call updatebf() //The algorithm is in my lecture notes. Use that.
    //If updatebf() returns NULL, no need to balance the tree
    //figure out if P(the root of the subtree that needs to be fixed) comes from left or
    //right of its parent or root.
    //There are 4 types of violations. LL, RR, LR, or RL. Check my lecture nodes for the
    //diagrams. For each type, call rotateRight(), rotateLeft() or both.
    //if(P->bf == 2 && P->left != NULL && P->left->bf == 1) à Which type of violation?
    //Don’t forget to attach the fixed subtree to the parent or root
    if (updatebf(newNode) != NULL)
    {
      //figure out if P(the root of the subtree that needs to be fixed) comes from left or
      //right of its parent or root.
      Node<T>* violatingNode = updatebf(newNode);

      if (violatingNode->bf = 2) // left heavy // must do 2 rotations
      {
        // check leftbf and rightbf for 1 or -1
        // while (leftbf != 0 and rightbf != 0)
        //  rotate
        ;
      }
      else  // right heavy // must do 2 rotations
      {
        // check leftbf and rightbf for 1 or -1
        ;
      }

    }

}

template <class T>
Node<T>* AVL<T>::updatebf(Node<T>* Q)
{
  Node<T>* P = NULL; // Q's parent node
  Node<T>* violatingNode = NULL;
  //STOPPED HERE
  if (Q->up != NULL)
    P = Q->up;

  if (Q->el < P->el && Q == P->left)
    P->bf++;
  else
    P->bf--;

  while (P != root && (P->bf > -2 && P->bf < 2) )
  {
    // Q = p
    Q = P;
    // P = p's parent
    P = P->up;

    if (Q->bf == 0)
      return violatingNode;

    if (Q->el < P->el && Q == P->left)
      P->bf++;
    else
      P->bf--;
  }

  if (P->bf == -2 || P->bf == 2)
    violatingNode = P;

    //return the pointer to the node with a violation.
    //OR
    //return NULL if there is no violation.
  return violatingNode;
}

template <class T>
Node<T>* AVL<T>::rightRotation(Node<T>* p)
{
}

template <class T>
Node<T>* AVL<T>::leftRotation(Node<T>* p)
{
  //Don't forget to change up and bf as well appropriate pointers.
  //This function returns the new root.
  //Check the YouTube video on AVL you watched. He gives you a simplified algorithm for
  //this. I have 10 lines of code in this function. 
}

#endif
