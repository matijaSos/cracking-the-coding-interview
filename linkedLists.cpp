#include <iostream>
#include <vector>

using namespace std;

struct Node{
    int value;
    Node *next;
};

void appendToEnd(Node *head, int value) {
    // TODO(matija): if head == NULL throw an error.

    Node *currNode = head;
    while (currNode->next != NULL) {
        currNode = currNode->next;
    }

    Node *newNode = new Node();
    newNode->value = value;
    newNode->next = NULL;

    currNode->next = newNode;
}

void printList(Node *head) {
    Node *currNode = head;
    while (currNode != NULL) {
        cout << currNode->value << " -> ";
        currNode = currNode->next;
    }
    cout << "NULL" << endl;
}

Node* createListFromVector(vector<int> &values) {
    Node *head = NULL;

    for (int const &value : values) {
        if (head == NULL) {
            head = new Node();
            head->value = value;
            head->next = NULL;
        } else {
            appendToEnd(head, value);
        }
    }

    return head;
}

/*
 * Problem 2.2
 * Find and return the k-th element from the end of a singly linked list.
 *
 * Solution
 * --------
 * Straightforward solution would be to determine the size of the list (first pass),
 * and then return the element in the second pass. That's O(n) complexity.
 *
 * Same complexity but slightly better would be to have two pointers that are
 * moved k places one from another. When "faster" pointer hits the end, the "slower"
 * pointer will point at k-th element from the end.
 *
 */

/* 
 * Here is the recursive solution they proposed but I'm not a big fan
 * of it - it doesn't improve complexity and is kinda "hacky" since it uses
 * a "global" counter - it's not "mathematically" sound and thus a bit hard to
 * understand.
 */
Node* getKthFromEndWrapper(Node *head, int k, int &posFromEnd) {
    if (head == NULL) {
        posFromEnd = 0;
        return NULL;
    }

    Node *node = getKthFromEndWrapper(head->next, k, posFromEnd);

    posFromEnd++;
    if (posFromEnd == k) {
        return head;
    }
    return node;
}

Node* getKthFromEnd(Node *head, int k) {
    int posFromEnd;
    return getKthFromEndWrapper(head, k, posFromEnd);
}

int main() {

    vector<int> listValues = {1, 2, 3, 4, 5};

    Node *head = createListFromVector(listValues);

    printList(head);

    Node *kthFromEnd = getKthFromEnd(head, 3);
    cout << "3rd from end: " << kthFromEnd->value << endl;

    return 0;
}
