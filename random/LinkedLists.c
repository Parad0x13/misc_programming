#include <iostream>
#include <cstdlib>
using namespace std;

class Node {
public:
    Node(int _value, Node *_link = NULL) {
        value = _value;
        link = _link;
    }

    void render() {
        cout << "(" << this << ": " << value << ")";
    }

    int value;
    Node *link;
};

class LinkedList {
public:
    void render() {
        cout << "Linked List:" << endl;
        Node *ref = head;
        while(ref != NULL) {
            ref->render();
            cout << " -> ";
            ref = ref->link;
        }
        cout << endl;
    }

    // Edge case: If you try to add a Node that already
    // has children it will remove that Node's children
    // [BUG] The index doesn't properly work. Entering an index
    // greater than node count and you'll get odd results
    void add(Node *node, int index = -1) {
        Node *prev, *entry;
        bool enoughNodes = true;
        if(index == -1) {
            entry = this->end();
            if(entry == NULL) {
                head = node;
                return;
            }
        }
        else {
            if(index == 0) {
                entry = NULL;
            }
            else {
                entry = this->atIndex(index-1);
            }
        }

        if(enoughNodes) {
            if(entry == NULL){
                node->link = head;
                head = node;
            }
            else {
                Node *tmp = entry->link;
                entry->link = node;
                node->link = tmp;
            }
        }
    }

    void remove(int index) {
        if(head == NULL) {
            return;
        }

        Node *ref = head, *prev;
        bool enoughNodes = true;
        for(int d=0;d<index;d++) {
            if(ref->link == NULL) {
                enoughNodes = false;
                break;
            }
            prev = ref;
            ref = ref->link;
        }

        if(enoughNodes) {
            if(ref == head) {
                head = ref->link;
            }
            else {
                prev->link = ref->link;
            }
        }
    }

    // Index -1 should be the end of the list
    Node *atIndex(int index) {
        if(index == -1) {
            Node *ref = head;
            if(ref == NULL) {
                return NULL;
            }

            while(ref->link != NULL) {
                ref = ref->link;
            }
            return ref;
        }

        Node *ref = head;
        for(int d=0;d<index;d++) {
            if(ref->link == NULL) {
                return NULL;
            }
            ref = ref->link;
        }

        return ref;
    }

    Node *end() {
        return this->atIndex(-1);
    }

    Node *head;
};

int main() {
    LinkedList *ll = new LinkedList();
    ll->add(new Node(0));
    ll->add(new Node(1));
    ll->add(new Node(2));
    ll->render();
    //ll->remove(0);
    //ll->render();

        return 0;
}