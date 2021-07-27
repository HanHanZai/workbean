#include"RBTree.h"
template<class T>
inline RBTree<T>::RBTree():pRoot(NULL)
{
}

template<class T>
inline RBTree<T>::~RBTree()
{
    destroy();
}

template<class T>
inline void RBTree<T>::preOrder()
{
    preOrder(pRoot);
}

template<class T>
inline void RBTree<T>::inOrder()
{
    preOrder(pRoot);
}

template<class T>
inline void RBTree<T>::postOrder()
{
    postOrder(pRoot);
}

template<class T>
inline RBNode<T>* RBTree<T>::search(T key)
{
    if (!pRoot){
        return NULL;
    }
    return search(pRoot,key);
}

template<class T>
inline RBNode<T>* RBTree<T>::iterativeSearch(T key)
{
    if (!pRoot){
        return NULL;
    }
    return iterativeSearch(pRoot,key);
}

template<class T>
inline T RBTree<T>::minimum()
{
    if(!pRoot){
        return NULL;
    }
    return minimum(pRoot);
}

template<class T>
inline T RBTree<T>::maximum()
{
    if (!pRoot){
        return NULL;
    }
    return maximum(pRoot);
}

/* 返回第一个后继节点 */
/* 最右边节点返回root的parent */
template<class T>
inline RBNode<T>* RBTree<T>::successor(RBNode<T>* x)
{
    /* 该节点有右节点，返回最小的右节点 */
    if(x->right){
        return minimum(x->right);
    }

    RBNode<T>* y = x->parent;
    /* 该节点无右节点，只有左节点，如果当前节点是左子节点，那么父节点为后继节点 */
    /* 如果当前节点为右子节点，那么就一直回溯到最近的祖父节点 */
    while(y &&(y->right == x)){
        x = y;
        y =y->parent;
    }
    return y;
}

/* 返回前继节点 */
/* 最左边节点返回root的parent */
template<class T>
inline RBNode<T>* RBTree<T>::predecessor(RBNode<T>* x)
{
    if (x->left){
        return maximum(x->left);
    }

    RBNode<T>* y = x->parent;
    while(y && (y->left == x)){
        x = y;
        y = y->parent;
    }
    return y;
}

template<class T>
inline void RBTree<T>::insert(T key)
{
    RBTree<T>* node = NULL;
    /* 新插入的节点都应该是红色节点 */
    if (node = new RBNode<T>(key, RED, NULL, NULL, NULL) == NULL) {
        return;
    }
    insert(pRoot,node);
}

template<class T>
inline void RBTree<T>::remove(T key)
{
    RBNode<T>* node;
    if ((node = search(pRoot,key)) != NULL) {
        remove(pRoot, node);
    }
}

template<class T>
inline void RBTree<T>::destroy()
{
    destroy(pRoot);
}

template<class T>
inline void RBTree<T>::print()
{
    print(pRoot);
}

template<class T>
inline void RBTree<T>::preOrder(RBNode<T>* x) const
{
    if (x!=NULL){
        cout<<x->key<<endl;
        preOrder(x->left);
        preOrder(x->right);
    }
}

template<class T>
inline void RBTree<T>::inOrder(RBNode<T>* x) const
{
    if(x!=NULL){
        inOrder(x->left);
        cout<<x->key<<endl;
        inOrder(x->right);
    }
}

template<class T>
inline void RBTree<T>::postOrder(RBNode<T>* x) const
{
    if(x!=NULL){
        postOrder(x->left);
        postOrder(x->right);
        cout<<x->key<<endl;
    }
}

template<class T>
inline RBNode<T>* RBTree<T>::search(RBNode<T>* x, T key) const
{
    if (!x){
        return NULL;
    }
    if(x->key < key){
        return search(x->right,key);
    }
    else if(x->key == key){
        return x;
    }
    else{
        return search(x->left,key);
    }
}

template<class T>
inline RBNode<T>* RBTree<T>::iterativeSearch(RBNode<T>* x, T key) const
{
    if (!x){
        return NULL;
    }
    while(x && x->key!=key){
        if (key > x->key){
            x = x->right;
        }
        else{
            x = x->left;
        }
    }
    return x;
}

template<class T>
inline RBNode<T>* RBTree<T>::minimum(RBNode<T>* x)
{
    if (x == NULL){
        return NULL;
    }
    while(x->left!=NULL){
        x = x->left;
    }
    return x;
}

template<class T>
inline RBNode<T>* RBTree<T>::maximum(RBNode<T>* x)
{
    if (x == NULL){
        return NULL;
    }
    while(x->right!=NULL){
        x = x->right;
    }
    return x;
}

template<class T>
inline void RBTree<T>::leftRotate(RBNode<T>*& root, RBNode<T>* x)
{
    /* 设置x的右子节点为y */
    RBNode<T>* y = x->right;

    /* 将y的左子节点当作x右子节点 */
    x->right = y->left;
    if (y->left) {
        y->left->parent = x;
    }

    /* 将x的父亲设置为y的父亲，x的父亲设置为y */
    y->parent = x->parent;
    if (x->parent == NULL) {
        root = y;
    }
    else {
        if (x->parent->left == x) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }
    }

    /* 将x设置为y的孩子 */
    y->left = x;
    /* 将y的父节点设置为y */
    x->parent = y;
}

template<class T>
inline void RBTree<T>::rightRotate(RBNode<T>*& root, RBNode<T>* y)
{
    /* 以y为中心进行旋转，那么 */
    RBNode<T>* x = y->left;
    /* 将当前节点的子节点置为旋转节点的右节点 */
    y->left = x->right;

    if (x->right) {
        /* 更新右子节点的父节点为y */
        x->right->parent = y;
    }

    /* 设置旋转后的节点为之前的父节点 */
    x->parent = y->parent;
    if (y->parent == NULL) {
        root = y;
    }
    else {
        if (y->parent->left == y) {
            y->parent->left = x;
        }
        else {
            y->parent->right = x;
        }
    }

    /* 设置旋转后的右子节点为原来的节点 */
    x->right = y;
    /* 设置原来的节点的父节点为当前节点 */
    y->parent = x;
}

template<class T>
inline void RBTree<T>::insert(RBNode<T>*& root, RBNode<T>* node)
{
    /* y是前置节点 */
    RBNode<T>* y = NULL;
    RBNode<T>* x = root;

    /* 1.将红黑树当作一颗二叉树，将节点添加到二叉查找树中 */
    while (x != NULL) {
        y = x;
        if (node->key < x->key) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }
    node->parent = y;

    if (y != NULL) {
        if (node->key < y->key) {
            y->left = node;
        }
        else {
            y->right = node;
        }
    }
    else {
        root = node;
    }

    /* 进行旋转修正 */
    insertFixUp(root, node);
}

/* 在向红黑树插入节点后，就破坏了红黑树的平衡性，通过该函数来重新塑造一颗红黑树 */
/* root 红黑树的根，node是插入的节点 */
template<class T>
inline void RBTree<T>::insertFixUp(RBNode<T>*& root, RBNode<T>* node)
{
    RBNode<T>* parent, *gParent;

    /* 若父节点存在，且父节点的颜色是红色 */
    /* 插入节点是红色，所以，一旦父节点为黑色，可以不用操作 */
    while ((parent = rb_parent(node)) && rb_is_red(parent)) {
        /* 获取祖父节点 */
        gParent = rb_parent(parent);

        /* 若父节点是祖父节点的左孩子 */
        if (gParent->left == parent) {
            /* Case1条件：父亲，叔叔节点是红色，通过调整颜色即可 */
            /* 将叔，父的颜色设为黑色，祖父设为红色,将当前操作节点修改成祖父节点 */
            {
                RBNode<T>* uncle = gParent->right;
                if (uncle && rb_is_red(uncle)) {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gParent);
                    node = gParent;
                    continue;
                }
            }

            /* Case2条件：叔叔节点是黑色，且当前孩子是右子节点 */
            if (parent->right == node) {
                RBNode<T>* pTmp;
                leftRotate(root,parent);
                pTmp = parent;
                parent = node;
                node = pTmp;
            }

            /* Case3条件：叔叔节点是黑色，当前孩子是左子节点 */
            /* 左左旋转一次，左右旋转两次 */
            rb_set_black(parent);
            rb_set_red(gParent);
            /* 以祖父节点为中心进行旋转 */
            rightRotate(root,gParent);
        }
        else /* 父节点是祖父节点的右节点 */ 
        {
            /* Case1条件：叔叔节点是红色 */
            {
                RBNode<T>* uncle = gParent->left;
                if (uncle && rb_is_red(uncle)) {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gParent);
                    node = gParent;
                    continue;
                }
            }

            /* Case2条件：叔叔是黑色，且当前节点是左子节点 */
            if (parent->left == node) {
                RBNode<T>* pTmp;
                rightRotate(root, parent);
                /* 修改当前节点为旋转前的父节点，也就是当前节点的子节点 */
                pTmp = parent;
                parent = node;
                node = pTmp;
            }

            /* Case3条件：叔叔是黑色，且当前节点是右子节点 */
            rb_set_black(parent);
            rb_set_red(gParent);
            leftRotate(root, gParent);
        }
    }

    /* 最后将根节点颜色修改成黑色 */
    rb_set_black(root);
}

template<class T>
inline void RBTree<T>::remove(RBNode<T>*& root, RBNode<T>* node)
{
    RBNode<T>* child, *parent;
    RBColor color;

    /* 被删除节点的左右孩子都不为空的情况 */
    /* 双子节点情况 */
    if ((node->left != NULL) && (node->right != NULL)) {
        /* 被删节点的后续节点，称为"取代节点" */
        /* 用来取代"被删节点",然后再将"被删节点"去掉 */

        /* 获取删除节点的后继节点 */
        RBNode<T>* replace = node;
        replace = replace->right;
        /* 获取右子节点中的最大节点 */
        while (replace->left != NULL) {
            replace = replace->left;
        }

        /* node节点不是根节点,只有根节点才没有父节点 */
        if (rb_parent(node)) {
            if (rb_parent(node)->left == node) {
                node->parent->left = replace;
            }
            else {
                node->parent->right = replace;
            }
        }
        else {
            /* 删除的是根节点，直接更新根节点 */
            root = replace;
        }

        /* child是"取代节点"的右孩子，也是需要调整的节点 */
        child = replace->right;
        /* "取代节点"的父节点 child的祖父节点 */
        parent = rb_parent(replace); 
        /* 保存"取代节点"的颜色 */
        color = rb_color(replace);

        /* "被删除节点"是"它的后继节点的父节点" */
        if (parent == node) {
            parent = replace;
        }
        else {
            if (child) {
                /* 直接升级为父节点 */
                rb_set_parent(child, parent);
            }
            parent->right = node->right;
            rb_set_parent(node->right, replace);
        }

        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;

        if (color == BLACK) {
            /* 剩下一个单节点 */
            removeFixUp(root, child, parent);
        }

        delete node;
        return;
    }

    /* 单节点处理 */
    if (node->left != NULL) {
        child = node->left;
    }
    else {
        child = node->right;
    }

    parent = node->parent;
    /* 保存"取代节点"的颜色 */
    color = node->color;

    if (child) {
        child->parent = parent;
    }

    /* node节点 不是根节点 */
    if (parent) {
        if (parent->left == node) {
            parent->left = child;
        }
        else {
            parent->right = child;
        }
    }
    else {
        root = child;
    }

    /* 如果是黑色节点，需要删除将父节点设置为黑色 */
    if (color == BLACK) {
        removeFixUp(root, child, parent);
    }

    /* 直接删除节点 */
    delete node;
}

/* 红黑树删除修正函数，目的是在删除节点之后，红黑树失去平衡，再调用该函数 */
/* 目的是重新调整称为一颗合格的红黑树 */
template<class T>
inline void RBTree<T>::removeFixUp(RBNode<T>*& root, RBNode<T>* node, RBNode<T>* parent)
{
    RBNode<T>* other;
    /* 当前节点不是根节点，节点不存在或者节点是红色 */
    while ((!node || rb_is_black(node)) && node != root) {
        /* 如果是左子节点 */
        if (parent->left == node) {
            /* 判断兄弟节点 */
            other = parent->right;
            if (rb_is_red(other)) {
                /* Case 1: x的兄弟w是红色节点 */
                rb_set_black(other);
                rb_set_red(parent);
                /* 左旋转父节点 */
                leftRotate(root, parent);
                /* 将节点转向兄弟节点 */
                other = parent->right;
            }

            /* 如果存在2个黑色子节点  */
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right)))
            {
                /* Case 2:x的兄弟w是黑色，并且两个孩子也都是黑色的 */
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else {
                if (!other->right || rb_is_black(other->right)) {
                    /* Case 3:x的兄弟w是黑色，并且w的左孩子是红色，右孩子为黑色 */
                    rb_set_black(other->left);
                    rb_set_red(other);
                    rightRotate(root, other);
                    other = parent->right;
                }

                /* Case 4:x的兄弟w是黑色的，并且w的右孩子是红色的，左孩子任意颜色 */
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->right);
                leftRotate(root, parent);
                node = root;
                break;
            }
        }
        else {
            other = parent->left;
            if (rb_is_red(other)) {
                /* Case 1: x的兄弟w是红色的 */
                rb_set_black(other);
                rb_set_parent(parent);
                rightRotate(root, parent);
                other = parent->left;
            }

            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right))) {
                /* Case 2：x的兄弟w是黑色的，且w的两个孩子也是黑色的 */
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else {
                if (!other->left || rb_is_black(other->left)) {
                    /* Case 3:x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色 */
                    rb_set_black(other->right);
                    rb_set_red(other);
                    leftRotate(root, other);
                    other = other->left;
                }

                /* Case 4:x的兄弟w是黑色的，并且w的右孩子是红色的，左孩子任意颜色 */
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->left);
                rightRotate(root, parent);
                node = root;
                break;
            }
        }
    }

    /* 最后流转到根节点的话，直接将根节点设置为黑色 */
    if (node) {
        rb_set_black(node);
    }
}

template<class T>
inline void RBTree<T>::destroy(RBNode<T>*& tree)
{
    if (!tree)
        return;
    if (tree->left){
        destroy(tree->left);
    }
    else if(tree->right){
        destroy(tree->right);
    }
}

template<class T>
inline void RBTree<T>::print(RBNode<T>* tree, T key, int direction)
{
    if (!tree){
        return;
    }

    if (direction == 0){
        cout<<"(B)root key = "<< key <<endl;
    }else{
        cout<<rb_is_black(tree)?"(B)":"(R)"<<"node key = "<< key << endl;
    }

    print(tree->left,tree->left->key,1);
    print(tree->right,tree->right->key,-1);
}
