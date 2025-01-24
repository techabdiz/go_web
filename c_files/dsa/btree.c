#include <stdio.h>
#include <stdlib.h>

#define B_MAX 4
typedef struct Node* node;
struct Node { 
    /**
     * The reason for having a list of values is, 
     * we can multiple values from the disk, 
     * reducing transfer time as compare to a BST
     */
    int keys[B_MAX];
    node children[B_MAX+1];
    // ... can have another array with the actual page values 
    // same index as keys
    int len;
    int is_leaf; //  > 0 true
};
extern node get_new_node(int i);


node root;
void add(int i) { 
    if(root == NULL) { 
        root = get_new_node();
    }
    fix_node(i, root);
}

void fix_node(int v, node rt){ 
    if(rt->len < B_MAX-1) {  // node still has room
        sort_and_fix_key(i, rt);
        return;
    }
    for(int i = 0; i < rt->len; i++) { 
        if (rt->keys[i+1] > v) { 
            sort_and_fix_key(v, rt->children[i]); // if next element is greater than v, then look in current's children
        }
    }
    
}
void sort_and_fix_key(int v, node rt){ 
    for (int i = 0; i < rt->len; i++) {  // searching in the middle
        if (rt->keys[i] < v && rt->keys[i+1] > v) { 
            int swap = rt->keys[i+1];
            rt->keys[i+1] = v; // apply split rules here 
            for (int j = i+2; j < B_MAX; j++) {
                rt->keys[j] = swap;
                swap = rt->keys[j+1];
            } 
            rt->len++;
            return;
        }
    }
    rt->keys[rt->len] = v; // exhausted.... pushing at the emd
    rt->len++;
    return;
}

node get_new_node(int is_leaf) { 
    node n = malloc(sizeof(struct Node));
    n->len = 0;
    n->is_leaf = is_leaf;
    for(int i =0; i < B_MAX-1; i++) { 
        n->children[i] = NULL;
    }
}
