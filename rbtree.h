#ifndef RB_TREE_H_
#define RB_TREE_H_

#include <functional>
#include <iterator>
#include <assert.h>


enum {	RED, BLACK	} ;

template <typename T>
struct node {
    static  node*          NIL;
    typedef typename T	   value_type;
    typedef unsigned long  size_type;
    T     key;
    node* parent;
    node* left;
    node* right;
    bool  color;
    node(value_type x, node* in_parent, bool in_color ): 
    key  (x), 
        parent (in_parent),  
        color  (in_color), 
        left   (NIL), 
        right  (NIL)
    {}
    node(value_type x, node* in_parent ): key  (x), 
        parent (in_parent),  
        color  (BLACK),   
        left   (NIL), 
        right  (NIL)	
    {}
    explicit node(const value_type& x ): node   (x), 
        color  (BLACK),   
        parent (NIL), 
        left   (NIL), 
        right  (NIL) 
    {}
    node():  color (BLACK),  
        parent (NIL), 
        left   (NIL), 
        right  (NIL) 
    {}
    node(node const& rhs )	{	
        (*this ) = rhs ;	
    }
    node* sibling(void )	{	
        return (is_left_son() ) ? parent->right : parent->left ;	
    }
    value_type& operator =(const value_type& x ) {
        key = x; 
    }
    bool operator == (const value_type& x ) const {
        return key == x ;	
    }
    bool operator != (const value_type& x ) const {	
        return key != x ;	
    }
    bool operator > (const value_type& x )  const {	
        return key > x ;	
    }
    bool operator<(const value_type& x )	  const {	
        return key < x ;	
    }
    bool operator >= (const value_type& x ) const {	
        return key >= x ;	
    }
    bool operator <= (const value_type& x ) const {	
        return key <= x ;	
    }
    bool operator == (const node& rhs ) const {	
        return (key == rhs.key ) && (parent == rhs.parent ) ;	
    }
    bool operator!= (const node& rhs ) const {	
        return (key != rhs.key ) || (parent != rhs.parent ) ;	
    }
    bool operator > (const node& rhs ) const {	
        return key > rhs.key ;	
    }
    bool operator < (const node& rhs ) const {	
        return key < rhs.key ;	
    }
    bool operator >=(const node& rhs ) const	{	
        return key >= rhs.key ;	
    }
    bool operator <= (const node& rhs ) const	{	
        return key <= other.key ;	
    }
    const bool is_left_son(void ) const	{	
        return (parent != NIL ) && (parent->left == this ) ;		
    }
    const bool is_right_son(void ) const	{	
        return (parent != NIL ) && (parent->right == this ) ;	
    }
    node* maximum()	{	
        return (right == NIL ) ? this : right->maximum() ;	
    }
    node* minimum()	{	
        return (left == NIL ) ? this : left->minimum() ;	
    }
    node* get_successor() {
        if (right != NIL )
            return right->minimum() ;
        if (is_left_son() )
            return parent ;
        node* succ = this ;
        do { 
            succ = succ->parent ; 
        }
        while ((succ != NIL ) && succ->is_right_son() ) ;
        if (succ != NIL )
            return succ->parent ;
        else
            return NIL ;
    }
    node* get_predecessor() {
        if (left != NIL )
            return left->maximum() ;
        if (is_right_son() )
            return parent ;
        node* pred = this ;
        do { 
            pred = pred->parent ; 
        }
        while ((pred != NIL ) && pred->is_left_son() ) ;
        if (pred != NIL )
            return pred->parent ;
        else
            return NIL ;
    }
private:
    static node SENTINEL_OBJECT ;
};
template< typename T >
node<T> node<T>::SENTINEL_OBJECT =  node<T>() ;

template< typename T >
node<T> * node<T>::NIL = &node<T>::SENTINEL_OBJECT ;

template< typename T >
class __iterator: public std::iterator< std::bidirectional_iterator_tag,
    typename T::value_type,
    typename T::size_type	> {
private:
    T *m_iterator;
public:
    typedef typename T::value_type*         pointer ;
    typedef typename T::value_type const*   const_pointer ;
    typedef typename T::value_type&         reference ;
    typedef typename T::value_type const&   const_reference ;
    explicit __iterator(): std::iterator< std::bidirectional_iterator_tag,
        typename T::value_type,
        typename T::size_type	>(), 
        m_iterator(T::NIL) {} ;
    __iterator(T* ptr ): m_iterator(ptr ) { }
    __iterator(const __iterator& rhs ): m_iterator(rhs.m_iterator ) { }

    __iterator operator++(void ) {
        m_iterator = m_iterator->get_successor() ;
        return* this ;
    }
    __iterator operator++(int ) {
        T* temp = m_iterator ;
        operator++() ;
        return __iterator(temp ) ;
    }
    __iterator operator--(void ) {
        m_iterator = m_iterator->get_predecessor() ;
        return (*this ) ;
    }
    __iterator operator--(int ) {
        T* temp = m_iterator ;
        operator--() ;
        return m_iterator(temp ) ;
    }
    __iterator& operator = (const_reference rhs ) {
        m_iterator->x = rhs ;
        return (*this );
    }
    bool operator == (const __iterator& rhs ) const {	
        return (m_iterator == rhs.m_iterator ) ;	
    }
    bool operator != (const __iterator& rhs ) const {	
        return (m_iterator != rhs.m_iterator ) ;	
    }
    operator T&(){	
        return *m_iterator ;	
    }
    operator const T&() const	{	
        return *m_iterator ;	
    }
    reference operator* ()	{	
        return m_iterator->key ;		
    }
    T* operator->(){	
        return m_iterator ;	
    }
};

template< typename T, typename CMP = std::less<T> >
class tree {
public:
    typedef typename node<T>                node_type;
    typedef typename T                      value_type;
    typedef value_type&                     reference;
    typedef value_type const&               const_reference;
    typedef typename node_type::size_type   size_type;
    typedef typename __iterator<node_type>  iterator ;
    typedef typename iterator const         const_iterator ;

    static node_type*   NIL;

    tree(): m_root(NIL),  m_size(0)	{}

    iterator  begin()const {   
        return minimum();
    }
    iterator end() const {   
        return iterator();	       
    }
    iterator root() const {   
        return iterator(m_root );  
    }
    bool empty() const {   
        return (m_root == NIL );   
    }
    size_type size() const {   
        return m_size;
    }

    iterator maximum() const {	
        return (empty()) ? end() : iterator(m_root->maximum() ) ;	
    }
    iterator minimum() const {	
        return (empty()) ? end() : iterator(m_root->minimum() ) ;	
    }

    iterator find(value_type x ) const {
        node_type* pos = m_root ;
        while (pos != NIL ) {
            if (pos->key == x )
                return iterator(pos) ;
            if (CMP()(x, pos->key))
                pos = pos->left ;
            else
                pos = pos->right ;
        }
        return end() ;
    }
    iterator insert(value_type x ) {
        return iterator(insert(new node_type(x, NIL, RED ) ) ) ;
    }
    iterator insert(iterator& it, const_reference x ) {
        return insert(x ) ;
    }
    template< typename InputIter >
    void insert(InputIter in_beg, InputIter in_end ) {
        for (InputIter idx = in_beg ; idx != in_end ; ++idx )
            insert(idx->x ) ;
    }
    void remove(value_type x ) {
        iterator pos = find(x ) ;
        if (pos != end())
            remove(pos) ;
    }
    ~tree() {
        node_type *z = m_root;
        while (z != NIL) {
            if (z->left != NIL)
                z = z->left;
            else if (z->right != NIL)
                z = z->right;
            else {
                if (z->parent) {
                    z = z->parent;
                    if (z->left != NIL){
                        delete (z->left);
                        z->left = NIL;
                    } else if (z->right != NIL){
                        delete ( z->right );
                        z->right = NIL;
                    }
                } else {
                    delete ( z );
                    z = NIL;
                }
            }
        }
    }
private:
    node_type*	m_root ;
    size_type	m_size ;

    void _left_rotate(node_type* x ) {
        node_type *y;
        y = x->right; // set y
        x->right = y->left; // turn y 's left subree into x 's right subtree
        if (y->left != NIL)
            y->left->parent = x; // Link x 's parent to y
        if (y != NIL)
            y->parent = x->parent;
        if (x->parent){
            if (x == x->parent->left)
                x->parent->left = y;
            else
                x->parent->right = y;
        } else {
            m_root = y;
        }
        y->left = x;
        if (x != NIL)
            x->parent = y;
    }
    void _right_rotate(node_type* x ) {
        if (x->left == NIL ) {
            return;
        }
        node_type *y = x->left;
        x->left = y->right;
        if (y->right != NIL)
            y->right->parent = x;
        if (y != NIL)
            y->parent = x->parent;
        if (x->parent) {
            if (x == x->parent->right)
                x->parent->right = y;
            else
                x->parent->left = y;
        } else {
            m_root = y;
        }
        y->right = x;
        if (x != NIL)
            x->parent = y;
    }
    node_type *insert(node_type* z ) {
        node_type *y = NIL;
        node_type *x = m_root;
        while ( x != NIL ) {
            y = x;
            if ( CMP()(z->key, x->key)) {
                x = x->left;
            } else {
                x = x->right;
            }
        }        
        z->parent = y;
        if (y == NIL) {
            // There is no node in the tree, so update the root
            m_root = z;
        } else {
            if( CMP()(z->key,y->key)) {
                y->left = z;
            } else {
                y->right = z;
            }
        }
        // Always Insert new node as RED.
        z->color = RED;
        _insert_fixup(z);
        _verify();
        return x;
    }
    void _insert_fixup( node_type* z ) {
        while (z != m_root && z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                node_type* y = z->parent->parent->right;
                if (y->color == RED) {
                    z->parent->color         = BLACK;
                    y->color                 = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right){
                        z = z->parent;
                        _left_rotate (z);
                    }
                    z->parent->color         = BLACK;
                    z->parent->parent->color = RED;
                    _right_rotate (z->parent->parent);
                }
            } else {
                node_type* y = z->parent->parent->left;
                if (y->color == RED) {
                    z->parent->color         = BLACK;
                    y->color                 = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        _right_rotate (z);
                    }
                    z->parent->color         = BLACK;
                    z->parent->parent->color = RED;
                    _left_rotate (z->parent->parent);
                }
            }
        }
        m_root->color = BLACK;
    }   

    iterator remove(iterator& rhs ) {    
        iterator next(rhs ) ;
        ++next ;
        node_type* x;
        node_type* y;
        node_type* z = rhs.operator->();

        if (z->left == NIL || z->right == NIL)
            y = z;
        else {
            y = z->right;
            while (y->left != NIL)
                y = y->left;
        }
        if (y->left != NIL)
            x = y->left;
        else
            x = y->right;

        x->parent = y->parent;
        if (y->parent)
        {
            if (y == y->parent->left)
                y->parent->left = x;
            else
                y->parent->right = x;
        }
        else
            m_root = x;
        if (y != z) {
            z = y;
        }
        if (y->color == BLACK)
            _remove_fixup (x);
        _verify();
        return next ;
    }
    void _remove_fixup( node_type *x ) {
        while (x != m_root && x->color == BLACK) {
            if (x == x->parent->left) {
                node_type* w = x->parent->right;
                if (w->color == RED) {
                    w->color         = BLACK;
                    x->parent->color = RED;
                    _left_rotate (x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right->color == BLACK)  {
                        w->left->color = BLACK;
                        w->color       = RED;
                        _right_rotate (w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    _left_rotate (x->parent);
                    x = m_root;
                }
            } else {
                node_type *w = x->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    _right_rotate (x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        _left_rotate (w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    _right_rotate (x->parent);
                    x = m_root;
                }
            }
        }
        x->color = BLACK;
    }

    bool _node_color(node_type* n) {
        return n == NIL ? BLACK : n->color;
    }

    void _verify() {
        assert ( _node_color(m_root) == BLACK );
        _verify_colors_for_each_node(m_root);
        _verifiy_colors_for_relationaship(m_root);
        _verify_path_count(m_root);
    }
    void _verify_colors_for_each_node(node_type *n) {
        assert( _node_color(n) == RED || 
            _node_color(n) == BLACK);
        if ( n == NIL) return;
        _verify_colors_for_each_node(n->left);
        _verify_colors_for_each_node(n->right);
    }
    void _verifiy_colors_for_relationaship(node_type *n) {
        if ( _node_color(n) == RED ) {
            assert (_node_color(n->left)   == BLACK);
            assert (_node_color(n->right)  == BLACK);
            assert (_node_color(n->parent) == BLACK);
        }
        if ( n == NIL) return;
        _verifiy_colors_for_relationaship(n->left);
        _verifiy_colors_for_relationaship(n->right);
    }
    void __verify_path_count(node_type *n,int black_count, 
        int* path_black_count) {
            if (_node_color(n) == BLACK) {
                black_count++;
            }
            if (n == NIL) {
                if (*path_black_count == -1) {
                    *path_black_count = black_count;
                } else {
                    assert (black_count == *path_black_count);
                }
                return;
            }
            __verify_path_count(n->left, black_count, path_black_count);
            __verify_path_count(n->right, black_count, path_black_count);

    }
    void _verify_path_count(node_type *n) {
        int black_count_path = -1;
        __verify_path_count(m_root, 0, &black_count_path);
    }
};

template< typename T, typename CMP>
typename tree< T, CMP>::node_type* tree< T, CMP>::NIL = 
tree< T,CMP>::node_type::NIL ;

#endif
