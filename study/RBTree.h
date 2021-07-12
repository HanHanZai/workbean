#define rb_parent(r) ((r)->parent)
#define rb_color(r) ((r)->color)
#define rb_is_red(r) ((r)->color == RED)
#define rb_is_black(r) ((r)->color == BLACK)
#define rb_set_black(r) do{((r)->color = BLACK);}while(0)
#define rb_set_red(r) do{((r)->color = RED);}while(0)
#define rb_set_parent(r,p) do{ ((r)->parent = p); }while(0)
#define rb_set_color(r,c) do{((r)->color = c);}while(0)

enum RBColor {RED,BLACK};
template<class T>
class RBNode {
public:
    /* 颜色 */
    RBColor color;
    /* 具体的值 */
    T key;
    /* 左子节点 */
    RBNode* left;
    /* 右子节点 */
    RBNode* right;
    /* 父节点 */
    RBNode* parent;

    RBNode(T value, RBColor c, RBNode* p, RBNode* l, RBNode* r) :color(c), key(value), parent(p), left(l), right(r) {};
};

template<class T>
class RBTree {
private:
    RBNode<T>* pRoot;
public:
    RBTree();
    ~RBTree();

    /* 前序遍历 */
    void preOrder();

    /* 中序遍历 */
    void midOrder();

    /* 后序遍历 */
    void postOrder();

    /* 递归实现查找"红黑树"中键值为key的节点 */
    RBNode<T>* search(T key);

    /* 非递归实现查找"红黑树"中键值为key的节点 */
    RBNode<T>* iterativeSearch(T key);

    /* 获取最小的节点，返回最小节点的键值 */
    T minimum();

    /* 获取最大的节点，返回最大的节点的键值 */
    T maximum();

    /* 找节点(x)的后续节点，即大于等于该节点的第一个节点 */
    RBNode<T>* successor(RBNode<T>* x);

    /* 找节点(x)的前导节点，即小于等于该节点的第一个节点 */
    RBNode<T>* predecessor(RBNode<T>* x);

    /* 将节点(Key为键值)插入到红黑树中 */
    void insert(T key);

    /* 删除指定键key的值 */
    void remove(T key);

    /* 销毁红黑树 */
    void destroy();

    /* 打印红黑树 */
    void print();

private:
    /* 前序遍历红黑树 */
    void preOrder(RBNode<T>* x)const;

    /* 中序遍历红黑树 */
    void inOrder(RBNode<T>* x)const;

    /* 后序遍历红黑树 */
    void postOrder(RBNode<T>* x)const;

    /* 递归实现 查找红黑树x中键值为key的节点 */
    RBNode<T>* search(RBNode<T>* x, T key)const;

    /* 非递归实现，查找红黑树x中键值为key的节点 */
    RBNode<T>* iterativeSearch(RBNode<T>* x, T key)const;

    /* 查找最小的节点 */
    RBNode<T>* minimum(RBNode<T>* x);

    /* 查找最大的节点 */
    RBNode<T>* maximum(RBNode<T>* x);

    /* 左旋转 */
    void leftRotate(RBNode<T>* &root, RBNode<T>* x);

    /* 右旋转 */
    void rightRotate(RBNode<T>* &root, RBNode<T>* y);

    /* 插入函数 */
    void insert(RBNode<T>* &root, RBNode<T>* node);

    /* 插入修正函数 */
    void insertFixUp(RBNode<T>* &root, RBNode<T>* node);

    /* 删除函数 */
    void remove(RBNode<T>* &root, RBNode<T>* node);

    /* 删除修正函数 */
    void removeFixUp(RBNode<T>* &root, RBNode<T>* node,RBNode<T>* parent);

    /* 销毁红黑树 */
    void destroy(RBNode<T>* &tree);

    /* 打印红黑树 */
    void print(RBNode<T>* tree, T key, int direction);
};