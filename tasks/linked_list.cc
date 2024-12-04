#include <memory>
#include "../vendor/io.h"
using namespace std;

template <typename T>
struct Linked_List {

    void insert(T value) {
        unique_ptr<Node>* node = &root;
        while (node->get()) {
            node = &(*node)->next;
        }
        *node = make_unique<Node>(value);
    }

    void remove(T value) {
        // Handle the case where the root node needs to be removed
        while (root && root->data == value) {
            root = std::move(root->next);
        }

        // Traverse the list and find the node to remove
        Node* current = root.get();
        while (current && current->next) {
            if (current->next->data == value) {
                current->next = std::move(current->next->next);
            } else {
                current = current->next.get();
            }
        }
    }

    void display() {
        Node* node = root.get();
        print("[");
        while (node) {
            print(node->data);
            if (node->next.get())
                print(", ");
            node = node->next.get();
        }
        print("]");
    }

private:
    struct Node {
        T data;
        unique_ptr<Node> next;

        Node(T value): data(value) {}
        ~Node() {
            println("Node destroyed!");
        }
    };

    std::unique_ptr<Node> root;
};

int main() {
    {
        Linked_List<int> list;

        list.insert(4);
        list.insert(7);
        list.insert(8);

        list.display();
        print('\n');

        list.remove(7);

        list.display();
        print('\n');
    }
    
    return 0;
}
