#include <stdio.h>
#include <stdlib.h>

typedef enum {
    APPLE = 1,
    BANANA,
    CHERRY,
    DATE,
    ELDERBERRY,
    FIG,
    GRAPE,
    HONEYDEW
} Fruit;

typedef struct Node {
    Fruit value;
    struct Node* first_child;
    struct Node* next_sibling;
} Node;

typedef struct {
    Node* root;
} Tree;

const char* FruitToString(Fruit fruit) {
    static const char* names[] = {"", "apple", "banana", "cherry", "date", 
                                   "elderberry", "fig", "grape", "honeydew"};
    return names[fruit];
}

Node* CreateNode(Fruit value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        printf("memory allocation failed\n");
        exit(1);
    }
    new_node->value = value;
    new_node->first_child = NULL;
    new_node->next_sibling = NULL;
    return new_node;
}

Node* FindNode(Node* root, Fruit value) {
    if (!root) return NULL;
    if (root->value == value) return root;
    
    Node* child = root->first_child;
    while (child) {
        Node* found = FindNode(child, value);
        if (found) return found;
        child = child->next_sibling;
    }
    return NULL;
}

void FreeSubtree(Node* node) {
    if (!node) return;
    Node* child = node->first_child;
    while (child) {
        Node* next = child->next_sibling;
        FreeSubtree(child);
        child = next;
    }
    free(node);
}

void AddNode(Tree* tree, Fruit parent_value, Fruit new_value) {
    if (!tree->root) {
        tree->root = CreateNode(new_value);
        printf("root '%s' created\n", FruitToString(new_value));
        return;
    }
    
    Node* parent = FindNode(tree->root, parent_value);
    if (!parent) {
        printf("parent '%s' not found\n", FruitToString(parent_value));
        return;
    }
    
    Node* new_node = CreateNode(new_value);
    if (!parent->first_child) {
        parent->first_child = new_node;
    } else {
        Node* sibling = parent->first_child;
        while (sibling->next_sibling) sibling = sibling->next_sibling;
        sibling->next_sibling = new_node;
    }
    printf("added '%s' under '%s'\n", FruitToString(new_value), FruitToString(parent_value));
}

void PrintTree(Node* node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; i++) printf("  ");
    printf("%s\n", FruitToString(node->value));
    
    Node* child = node->first_child;
    while (child) {
        PrintTree(child, depth + 1);
        child = child->next_sibling;
    }
}

void DeleteNode(Tree* tree, Fruit value) {
    if (!tree->root) return;
    
    if (tree->root->value == value) {
        FreeSubtree(tree->root);
        tree->root = NULL;
        printf("deleted '%s'\n", FruitToString(value));
        return;
    }
    
    // поиск родителя
    Node* parent = NULL;
    Node* curr = tree->root;
    
    // функция поиска родителя
    Node* FindParent(Node* node, Fruit target) {
        if (!node) return NULL;
        Node* child = node->first_child;
        while (child) {
            if (child->value == target) return node;
            Node* res = FindParent(child, target);
            if (res) return res;
            child = child->next_sibling;
        }
        return NULL;
    }
    
    parent = FindParent(tree->root, value);
    if (!parent) {
        printf("'%s' not found\n", FruitToString(value));
        return;
    }
    
    Node* prev = NULL;
    Node* curr_child = parent->first_child;
    while (curr_child) {
        if (curr_child->value == value) {
            if (prev) prev->next_sibling = curr_child->next_sibling;
            else parent->first_child = curr_child->next_sibling;
            FreeSubtree(curr_child);
            printf("deleted '%s'\n", FruitToString(value));
            return;
        }
        prev = curr_child;
        curr_child = curr_child->next_sibling;
    }
}

int GetHeight(Node* node) {
    if (!node) return 0;
    int max_h = 0;
    for (Node* child = node->first_child; child; child = child->next_sibling) {
        int h = GetHeight(child);
        if (h > max_h) max_h = h;
    }
    return max_h + 1;
}

void GetWidths(Node* node, int depth, int* widths) {
    if (!node) return;
    widths[depth]++;
    for (Node* child = node->first_child; child; child = child->next_sibling) {
        GetWidths(child, depth + 1, widths);
    }
}

int IsWidthDecreasing(Tree* tree) {
    if (!tree->root) return 0;
    
    int h = GetHeight(tree->root);
    int* widths = (int*)calloc(h, sizeof(int));
    GetWidths(tree->root, 0, widths);
    
    printf("widths: ");
    for (int i = 0; i < h; i++) printf("%d ", widths[i]);
    printf("\n");
    
    int decreasing = 1;
    for (int i = 1; i < h; i++) {
        if (widths[i] >= widths[i-1]) {
            decreasing = 0;
            break;
        }
    }
    free(widths);
    return decreasing;
}

int main() {
    Tree tree = {NULL};
    int choice;
    int temp_val, temp_parent;  // используем int для ввода
    Fruit val, parent_val;
    
    printf("\n=== tree operations ===\n");
    printf("values: 1-apple,2-banana,3-cherry,4-date,5-elderberry,6-fig,7-grape,8-honeydew\n");
    
    do {
        printf("\n1-add 2-print 3-delete 4-check 5-exit: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                if (!tree.root) {
                    printf("root value (1-8): ");
                    scanf("%d", &temp_val);
                    val = (Fruit)temp_val;  // явное приведение типа
                    AddNode(&tree, 0, val);
                } else {
                    printf("parent value (1-8): ");
                    scanf("%d", &temp_parent);
                    parent_val = (Fruit)temp_parent;
                    printf("new value (1-8): ");
                    scanf("%d", &temp_val);
                    val = (Fruit)temp_val;
                    AddNode(&tree, parent_val, val);
                }
                break;
            case 2:
                if (!tree.root) printf("empty\n");
                else PrintTree(tree.root, 0);
                break;
            case 3:
                if (!tree.root) printf("empty\n");
                else {
                    printf("value to delete (1-8): ");
                    scanf("%d", &temp_val);
                    val = (Fruit)temp_val;
                    DeleteNode(&tree, val);
                }
                break;
            case 4:
                if (!tree.root) printf("empty\n");
                else {
                    printf("%s\n", IsWidthDecreasing(&tree) ? 
                           "width strictly decreasing" : "width NOT strictly decreasing");
                }
                break;
        }
    } while (choice != 5);
    
    if (tree.root) FreeSubtree(tree.root);
    return 0;
}
