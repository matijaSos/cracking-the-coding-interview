#include <iostream>
#include <map>

using namespace std;

class LinkedList {
    
    /**
     * Class that represents the single element of the list.
     */
    class Node {
        public:
            int value;
            Node *next;

            Node(int value) {
                this->value = value;
                this->next = NULL;
            }
    };

    private:
        /* Pointer to the first element of the linked list. */
        Node *head;

    public:
        /**
         * Initializes empty linked lists.
         */
        LinkedList() {
            this->head = NULL;
        }
        
        /**
         * Appends given element to the end of the list.
         */
        void appendToEnd(int value) {
            if (head == NULL) {
                head = new Node(value);
                head->next = NULL;

                return;
            }

            Node *currNode = head;

            // Get to the last node.
            while (currNode->next != NULL) {
                currNode = currNode->next;
            }

            // Create new node.
            Node *newNode = new Node(value);
            newNode->next = NULL;

            // Append it to the the last node.
            currNode->next = newNode;
        }

        /**
         * Removes all nodes with the given value from the list.
         */
        void removeElement(int valueToRemove) {
            // Remove consecutives at the beginning.
            while (head != NULL && head->value == valueToRemove) {
                Node *tmp = head;
                head = head->next;
                delete tmp;
            }

            // Here the list is either empty or head is pointing at the node 
            // different from valueToRemove.
            Node *currNode = head;
            while (currNode != NULL) {
                if (currNode->next != NULL && currNode->next->value == valueToRemove) {
                    Node *tmp = currNode->next;
                    currNode->next = currNode->next->next;
                    delete tmp;
                } else {
                    // Move forward only if the next node doesn't have to be deleted.
                    currNode = currNode->next;
                }
            }
        }

        /**
         * Acts like unique, removing all duplicate elements from the list.
         */
        void removeDuplicates() {
            if (head == NULL) return;

            map<int, bool> presentElements;
            presentElements[head->value] = true;

            Node *currNode = head;
            while (currNode->next != NULL) {
                if (presentElements.find(currNode->next->value) != presentElements.end()) {
                    // Encountered duplicate - delete it.
                    Node *tmp = currNode->next;
                    currNode->next = currNode->next->next;
                    delete tmp;
                } else {
                    // Not a duplicate, marked as present and move on.
                    presentElements[currNode->next->value] = true;
                    currNode = currNode->next;
                }
            }
        }

        /**
         * Prints elements of the list
         */
        void print() {
            Node *currNode = this->head;

            while (currNode != NULL) {
                cout << currNode->value << " -> ";
                currNode = currNode->next;
            }
            cout << "NULL" << endl;
        }
};

/*
 * Problem 2.1
 * Write code to remove duplicates from an unsorted linked list.
 *
 * Solution
 * --------
 *
 * Use a map to store elements that have already been encountered and just delete duplicates.
 *
 * Time complexity: O(n) - we traverse the list once and delete duplicates.
 * Space complexity: Depends on the number of different elements in the list.
 *
 * In case we weren't allowed to use additional memory, we'd do O(N^2) time complexity -
 * for each element move through the list and delete it's duplicates.
 *
 * Solved in LinkedList class method removeDuplicates().
 */

int main() {

    cout << "Hey, I am test output in linked lists chapter!" << endl;

    LinkedList list = LinkedList();

    list.appendToEnd(10);
    list.appendToEnd(20);
    list.appendToEnd(10);
    list.appendToEnd(10);
    list.appendToEnd(30);
    list.appendToEnd(10);
    list.appendToEnd(30);
    list.appendToEnd(17);

    list.print();

    //list.removeElement(10);
    list.removeDuplicates();
    list.print();

    return 0;
}
