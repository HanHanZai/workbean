#include"RBTree.h"
template<class T>
inline RBTree<T>::RBTree()
{
}

template<class T>
inline RBTree<T>::~RBTree()
{
}

template<class T>
inline void RBTree<T>::preOrder()
{
}

template<class T>
inline void RBTree<T>::midOrder()
{
}

template<class T>
inline void RBTree<T>::postOrder()
{
}

template<class T>
inline RBNode<T>* RBTree<T>::search(T key)
{
    return NULL;
}

template<class T>
inline RBNode<T>* RBTree<T>::iterativeSearch(T key)
{
    return NULL;
}

template<class T>
inline T RBTree<T>::minimum()
{
    return T();
}

template<class T>
inline T RBTree<T>::maximum()
{
    return T();
}

template<class T>
inline RBNode<T>* RBTree<T>::successor(RBNode<T>* x)
{
    return NULL;
}

template<class T>
inline RBNode<T>* RBTree<T>::predecessor(RBNode<T>* x)
{
    return NULL;
}

template<class T>
inline void RBTree<T>::insert(T key)
{
    RBTree<T>* node = NULL;
    /* �²���Ľڵ㶼Ӧ���Ǻ�ɫ�ڵ� */
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
}

template<class T>
inline void RBTree<T>::print()
{
}

template<class T>
inline void RBTree<T>::preOrder(RBNode<T>* x) const
{
}

template<class T>
inline void RBTree<T>::inOrder(RBNode<T>* x) const
{
}

template<class T>
inline void RBTree<T>::postOrder(RBNode<T>* x) const
{
}

template<class T>
inline RBNode<T>* RBTree<T>::search(RBNode<T>* x, T key) const
{
    return NULL;
}

template<class T>
inline RBNode<T>* RBTree<T>::iterativeSearch(RBNode<T>* x, T key) const
{
    return NULL;
}

template<class T>
inline RBNode<T>* RBTree<T>::minimum(RBNode<T>* x)
{
    return NULL;
}

template<class T>
inline RBNode<T>* RBTree<T>::maximum(RBNode<T>* x)
{
    return NULL;
}

template<class T>
inline void RBTree<T>::leftRotate(RBNode<T>*& root, RBNode<T>* x)
{
    /* ����x�����ӽڵ�Ϊy */
    RBNode<T>* y = x->right;

    /* ��y�����ӽڵ㵱��x���ӽڵ� */
    x->right = y->left;
    if (y->left) {
        y->left->parent = x;
    }

/* ��x�ĸ�������Ϊy�ĸ��ף�x�ĸ�������Ϊy */
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

/* ��x����Ϊy�ĺ��� */
y->left = x;
/* ��y�ĸ��ڵ�����Ϊy */
x->parent = y;
}

template<class T>
inline void RBTree<T>::rightRotate(RBNode<T>*& root, RBNode<T>* y)
{
    /* ��yΪ���Ľ�����ת����ô */
    RBNode<T>* x = y->left;
    /* ����ǰ�ڵ���ӽڵ���Ϊ��ת�ڵ���ҽڵ� */
    y->left = x->right;

    if (x->right) {
        /* �������ӽڵ�ĸ��ڵ�Ϊy */
        x->right->parent = y;
    }

    /* ������ת��Ľڵ�Ϊ֮ǰ�ĸ��ڵ� */
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

    /* ������ת������ӽڵ�Ϊԭ���Ľڵ� */
    x->right = y;
    /* ����ԭ���Ľڵ�ĸ��ڵ�Ϊ��ǰ�ڵ� */
    y->parent = x;
}

template<class T>
inline void RBTree<T>::insert(RBNode<T>*& root, RBNode<T>* node)
{
    /* y��ǰ�ýڵ� */
    RBNode<T>* y = NULL;
    RBNode<T>* x = root;

    /* 1.�����������һ�Ŷ����������ڵ���ӵ������������ */
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

    /* ������ת���� */
    insertFixUp(root, node);
}

/* ������������ڵ�󣬾��ƻ��˺������ƽ���ԣ�ͨ���ú�������������һ�ź���� */
/* root ������ĸ���node�ǲ���Ľڵ� */
template<class T>
inline void RBTree<T>::insertFixUp(RBNode<T>*& root, RBNode<T>* node)
{
    RBNode<T>* parent, *gParent;
    /* �����ڵ���ڣ��Ҹ��ڵ����ɫ�Ǻ�ɫ */
    while ((parent = rb_parent(node)) && rb_is_red(parent)) {
        /* ��ȡ�游�ڵ� */
        gParent = rb_parent(parent);

        /* �����ڵ����游�ڵ������ */
        if (gParent->left == parent) {
            /* Case1���������ף�����ڵ��Ǻ�ɫ */
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

            /* Case2����������ڵ��Ǻ�ɫ���ҵ�ǰ���������ӽڵ� */
            if (parent->right == node) {
                RBNode<T>* pTmp;
                leftRotate(root.parent);
                pTmp = parent;
                parent = node;
                node = pTmp;
            }

            /* Case3����������ڵ��Ǻ�ɫ����ǰ���������ӽڵ� */
            rb_set_black(parent);
            rb_set_red(gParent);
            rightRotate(root,gParent);
        }
        else /* ���ڵ����游�ڵ���ҽڵ� */ 
        {
            /* Case1����������ڵ��Ǻ�ɫ */
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

            /* Case2�����������Ǻ�ɫ���ҵ�ǰ�ڵ������ӽڵ� */
            if (parent->left == node) {
                RBNode<T>* pTmp;
                rightRotate(root, parent);
                pTmp = parent;
                parent = node;
                node = pTmp;
            }

            /* Case3�����������Ǻ�ɫ���ҵ�ǰ�ڵ������ӽڵ� */
            rb_set_black(parent);
            rb_set_red(gParent);
            leftRotate(root, gParent);
        }
    }

    /* ��󽫸��ڵ���ɫ�޸ĳɺ�ɫ */
    rb_set_black(root);
}

template<class T>
inline void RBTree<T>::remove(RBNode<T>*& root, RBNode<T>* node)
{
    RBNode<T>* child, *parent;
    RBColor color;

    /* ��ɾ���ڵ�����Һ��Ӷ���Ϊ�յ���� */
    if ((node->left != NULL) && (node->right != NULL)) {
        /* ��ɾ�ڵ�ĺ����ڵ㣬��Ϊ"ȡ���ڵ�" */
        /* ����ȡ��"��ɾ�ڵ�",Ȼ���ٽ�"��ɾ�ڵ�"ȥ�� */
        RBNode<T>* replace = node;
        replace = replace->right;
        while (replace->left != NULL) {
            replace = replace->left;
        }

        /* node�ڵ㲻�Ǹ��ڵ�,ֻ�и��ڵ��û�и��ڵ� */
        if (rb_parent(node)) {
            if (rb_parent(node)->left == node) {
                node->parent->left = replace;
            }
            else {
                node->parent->right = replace;
            }
        }
        else {
            /* ɾ�����Ǹ��ڵ㣬ֱ�Ӹ��¸��ڵ� */
            root = replace;
        }

        /* child��"ȡ���ڵ�"���Һ��ӣ�Ҳ����Ҫ�����Ľڵ� */
        child = replace->right;
        parent = rb_parent(replace);
        /* ����"ȡ���ڵ�"����ɫ */
        color = rb_color(replace);

        /* "��ɾ���ڵ�"��"���ĺ�̽ڵ�ĸ��ڵ�" */
        if (parent == node) {
            parent = replace;
        }
        else {
            if (child) {
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
            removeFixUp(root, child, parent);
        }

        delete node;
        return;
    }

    if (node->left != NULL) {
        child = node->left;
    }
    else {
        child = node->right;
    }

    parent = node->parent;
    /* ����"ȡ���ڵ�"����ɫ */
    color = node->color;

    if (child) {
        child->parent = parent;
    }

    /* node�ڵ� ���Ǹ��ڵ� */
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

    if (color == BLACK) {
        removeFixUp(root, child, parent);
    }

    delete node;
}

/* �����ɾ������������Ŀ������ɾ���ڵ�֮�󣬺����ʧȥƽ�⣬�ٵ��øú��� */
/* Ŀ�������µ�����Ϊһ�źϸ�ĺ���� */
template<class T>
inline void RBTree<T>::removeFixUp(RBNode<T>*& root, RBNode<T>* node, RBNode<T>* parent)
{
    RBNode<T>* other;
    /* ��ǰ�ڵ㲻�Ǹ��ڵ㣬�ڵ㲻���ڻ��߽ڵ��Ǻ�ɫ */
    while ((!node || rb_is_black(node)) && node != root) {
        /* ��������ӽڵ� */
        if (parent->left == node) {
            /* �ж��ֵܽڵ� */
            other = parent->right;
            if (rb_is_red(other)) {
                /* Case 1: x���ֵ�w�Ǻ�ɫ�ڵ� */
                rb_set_black(other);
                rb_set_red(parent);
                /* ����ת���ڵ� */
                leftRotate(root, parent);
                /* ���ڵ�ת���ֵܽڵ� */
                other = parent->right;
            }

            /* �������2����ɫ�ӽڵ�  */
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right)))
            {
                /* Case 2:x���ֵ�w�Ǻ�ɫ��������������Ҳ���Ǻ�ɫ�� */
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else {
                if (!other->right || rb_is_black(other->right)) {
                    /* Case 3:x���ֵ�w�Ǻ�ɫ������w�������Ǻ�ɫ���Һ���Ϊ��ɫ */
                    rb_set_black(other->left);
                    rb_set_red(other);
                    rightRotate(root, other);
                    other = parent->right;
                }

                /* Case 4:x���ֵ�w�Ǻ�ɫ�ģ�����w���Һ����Ǻ�ɫ�ģ�����������ɫ */
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
                /* Case 1: x���ֵ�w�Ǻ�ɫ�� */
                rb_set_black(other);
                rb_set_parent(parent);
                rightRotate(root, parent);
                other = parent->left;
            }

            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right))) {
                /* Case 2��x���ֵ�w�Ǻ�ɫ�ģ���w����������Ҳ�Ǻ�ɫ�� */
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else {
                if (!other->left || rb_is_black(other->left)) {
                    /* Case 3:x���ֵ�w�Ǻ�ɫ�ģ�����w�������Ǻ�ɫ���Һ���Ϊ��ɫ */
                    rb_set_black(other->right);
                    rb_set_red(other);
                    leftRotate(root, other);
                    other = other->left;
                }

                /* Case 4:x���ֵ�w�Ǻ�ɫ�ģ�����w���Һ����Ǻ�ɫ�ģ�����������ɫ */
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->left);
                rightRotate(root, parent);
                node = root;
                break;
            }
        }
    }
    if (node) {
        rb_set_black(node);
    }
}

template<class T>
inline void RBTree<T>::destroy(RBNode<T>*& tree)
{
}

template<class T>
inline void RBTree<T>::print(RBNode<T>* tree, T key, int direction)
{
}
