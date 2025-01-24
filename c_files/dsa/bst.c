#include <stdio.h>
#include <stdlib.h>


typedef struct Node* node;
struct Node { 
    int val;
    node left;
    node right;
};
extern node get_new_node(int i);
extern void fix_node(node n, node rt);
node root = NULL;
void add_bst(int i) { 
    node n = get_new_node(i);
    if(root == NULL) {
        root = n;
        return;
    }
    fix_node(n, root);
}
void fix_node(node n, node rt){
    if(n->val <= rt->val) { 
        if(rt->left == NULL) { 
            rt->left = n;
            return;
        }
        fix_node(n, rt->left);
    }else {
        if(rt->right == NULL) { 
            rt->right = n;
            return;
        }
        fix_node(n, rt->right);
    }
}

int exists (int i, node rt) { 
    if(rt == NULL){
        return -1;
    }
    if(rt->val == i) { 
        return 0;
    }

    if( i <= rt->val){ 
        return exists(i, rt->left);
    }else { 
        return exists(i, rt->right);
    }
} 
int contains(int i){ 
    return exists(i, root);
}
node get_new_node(int i) { 
    node n = malloc(sizeof(struct Node));
    n->val = i;
    n->left = NULL;
    n->right = NULL;
}



int main (void) { 
    add_bst(34);
    add_bst(20);
    add_bst(82);
    add_bst(91);
    add_bst(56);
    add_bst(55);
    add_bst(20);
    add_bst(88);
    printf("exists: %d\n", contains(20));
    printf("exists: %d\n", contains(91));
    printf("exists: %d\n", contains(88));
    printf("exists: %d\n", contains(22));
    return 0;
}