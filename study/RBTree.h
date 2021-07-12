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
    /* ��ɫ */
    RBColor color;
    /* �����ֵ */
    T key;
    /* ���ӽڵ� */
    RBNode* left;
    /* ���ӽڵ� */
    RBNode* right;
    /* ���ڵ� */
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

    /* ǰ����� */
    void preOrder();

    /* ������� */
    void midOrder();

    /* ������� */
    void postOrder();

    /* �ݹ�ʵ�ֲ���"�����"�м�ֵΪkey�Ľڵ� */
    RBNode<T>* search(T key);

    /* �ǵݹ�ʵ�ֲ���"�����"�м�ֵΪkey�Ľڵ� */
    RBNode<T>* iterativeSearch(T key);

    /* ��ȡ��С�Ľڵ㣬������С�ڵ�ļ�ֵ */
    T minimum();

    /* ��ȡ���Ľڵ㣬�������Ľڵ�ļ�ֵ */
    T maximum();

    /* �ҽڵ�(x)�ĺ����ڵ㣬�����ڵ��ڸýڵ�ĵ�һ���ڵ� */
    RBNode<T>* successor(RBNode<T>* x);

    /* �ҽڵ�(x)��ǰ���ڵ㣬��С�ڵ��ڸýڵ�ĵ�һ���ڵ� */
    RBNode<T>* predecessor(RBNode<T>* x);

    /* ���ڵ�(KeyΪ��ֵ)���뵽������� */
    void insert(T key);

    /* ɾ��ָ����key��ֵ */
    void remove(T key);

    /* ���ٺ���� */
    void destroy();

    /* ��ӡ����� */
    void print();

private:
    /* ǰ���������� */
    void preOrder(RBNode<T>* x)const;

    /* ������������ */
    void inOrder(RBNode<T>* x)const;

    /* ������������ */
    void postOrder(RBNode<T>* x)const;

    /* �ݹ�ʵ�� ���Һ����x�м�ֵΪkey�Ľڵ� */
    RBNode<T>* search(RBNode<T>* x, T key)const;

    /* �ǵݹ�ʵ�֣����Һ����x�м�ֵΪkey�Ľڵ� */
    RBNode<T>* iterativeSearch(RBNode<T>* x, T key)const;

    /* ������С�Ľڵ� */
    RBNode<T>* minimum(RBNode<T>* x);

    /* �������Ľڵ� */
    RBNode<T>* maximum(RBNode<T>* x);

    /* ����ת */
    void leftRotate(RBNode<T>* &root, RBNode<T>* x);

    /* ����ת */
    void rightRotate(RBNode<T>* &root, RBNode<T>* y);

    /* ���뺯�� */
    void insert(RBNode<T>* &root, RBNode<T>* node);

    /* ������������ */
    void insertFixUp(RBNode<T>* &root, RBNode<T>* node);

    /* ɾ������ */
    void remove(RBNode<T>* &root, RBNode<T>* node);

    /* ɾ���������� */
    void removeFixUp(RBNode<T>* &root, RBNode<T>* node,RBNode<T>* parent);

    /* ���ٺ���� */
    void destroy(RBNode<T>* &tree);

    /* ��ӡ����� */
    void print(RBNode<T>* tree, T key, int direction);
};