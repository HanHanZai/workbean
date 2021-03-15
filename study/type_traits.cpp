struct __true_type{};
struct __false_type{};

/* __false_type 是需要自己进行实现的 */
template<class type>
struct __type_traits{
    typedef __true_type this_dummy_member_must_be_first;
    typedef __false_type has_trivial_default_constructor; /* 是否有默认构造函数 */ 
    typedef __false_type has_trivial_constructor; /* 是否有构造函数 */
    typedef __false_type has_trivial_assignment_operator; /* 是否有赋值符号重载 */
    typedef __false_type has_trivial_destrctor; /* 是否存在析构函数 */
    typedef __false_type is_POD_type; /* 是否属于POD类型 */
};
