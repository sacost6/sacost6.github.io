#ifndef _BST_H
#define _BST_H

#include <iostream>
#include <vector>
#include <algorithm>



template <typename T>
class bst {
    
private:
    struct bst_node {
        T      val;
        bst_node *left;
        bst_node *right;
        int size;
        
        bst_node ( const T & _val = T{}, bst_node * l = nullptr, bst_node *r = nullptr, int s = 0)
        : val { _val },  left { l }, right {r}, size {s}
        { }
    };
    
    
    
public:
    // constructor:  initializes an empty tree
    bst(){
        root = nullptr;
    }
    
private:
    // helper function which recursively deallocates nodes
    //   in a tree.
    static void delete_nodes(bst_node *r){
        if(r==nullptr) return;
        delete_nodes(r->left);
        delete_nodes(r->right);
        delete r;
    }
    
public:
    // destructor
    ~bst() {
        delete_nodes(root);
    }
    
private:
    
    /**
     * function:  insert()
     * desc:      recursive helper function inserting x into
     *            binary search tree rooted  at r.
     *
     * returns:   pointer to root of tree after insertion.
     *
     * notes:     if x is already in tree, no modifications are made.
     */
    static bst_node * _insert(bst_node *r, T & x, bool &success){
        int high;
        int low;
  
        
        if(r == nullptr){
            success = true;
            return new bst_node(x, nullptr, nullptr, 0);
        }
        
        r->size++;
        if(r->val == x){
            success = false;
            r->size--;
            return r;
        }

        // check if tree is balanced
        

        
        
        if(x < r->val){
            
            r->left = _insert(r->left, x, success);
            
            if (r->left == nullptr && r->right != nullptr) {
                high = std::max(0, r->right->size+1);
                low = std::min(0, r->right->size+1);
                if(high > (2 * low + 1)) {
                    std::vector<bst_node*> nodes;
                    storeTreeNodes(r,nodes);
                    r = balanceTree(r);
                }
            }
            
            else if(r->left != nullptr && r->right == nullptr) {
                high = std::max(r->left->size+1, 0);
                low = std::min(r->left->size+1, 0);
                if(high > (2 * low + 1)) {
                    std::vector<bst_node*> nodes;
                    storeTreeNodes(r,nodes);
                    r = balanceTree(r);
                }
            }
            else if(r->left != nullptr && r->right != nullptr) {
                high = std::max(r->left->size+1, r->right->size+1);
                low = std::min(r->left->size+1, r->right->size+1);
                if(high > (2 * low + 1)) {
                    std::vector<bst_node*> nodes;
                    storeTreeNodes(r,nodes);
                    r = balanceTree(r);
                }
            }
            
            
            
            
        
            if(!success) {
                r->size--;
            }
            return r;
        }
        else {
            
            r->right = _insert(r->right, x, success);
            if (r->left == nullptr && r->right != nullptr) {
                high = std::max(0, r->right->size+1);
                low = std::min(0, r->right->size+1);
                if(high > (2 * low + 1)) {
                    std::vector<bst_node*> nodes;
                    storeTreeNodes(r,nodes);
                    r = balanceTree(r);
                }
            }
            
            else if(r->left != nullptr && r->right == nullptr) {
                high = std::max(r->left->size+1, 0);
                low = std::min(r->left->size+1,0);
                if(high > (2* low + 1)) {
                    std::vector<bst_node*> nodes;
                    storeTreeNodes(r,nodes);
                    r = balanceTree(r);
                }
            }
            else if(r->left != nullptr && r->right != nullptr) {
                high = std::max(r->left->size+1, r->right->size+1);
                low = std::min(r->left->size+1, r->right->size+1);
                if(high > (2 * low + 1)) {
                    std::vector<bst_node*> nodes;
                    storeTreeNodes(r,nodes);
                    r = balanceTree(r);
                }
            }

            
            
            if(!success) {
                r->size--;
            }
            return r;
        }

    }
    
    
public:
    /**
     * function:  insert
     * desc:      inserts x into BST given by t.  Note that
     *            a BST stores a SET -- no duplicates.  Thus,
     *            if x is already in t when call made, no
     *            modifications to tree result.
     *
     * note:      helper function does most of the work.
     *
     */
    bool insert(T & x){
        
        bool success;
        root = _insert(root, x, success);
        return success;
    }
    
    /**
     * function:  contains()
     * desc:      returns true or false depending on whether x is an
     *            element of BST (calling object)
     *
     */
    bool contains(const T & x){
        bst_node *p = root;
        
        while(p != nullptr){
            
            if(p->val == x)
                return true;
            if(x < p->val){
                p = p->left;
            }
            else
                p = p->right;
        }
        return false;
    }
    
private:
    // returns pointer to node containing
    //   smallest value in tree rooted at r
    static bst_node * _min_node(bst_node *r ){
        if(r==nullptr)
            return nullptr; // should never happen!
        while(r->left != nullptr)
            r = r->left;
        return r;
    }
    
    // returns pointer to node containing
    //   smallest value in tree rooted at r
    static bst_node * _max_node(bst_node *r ){
        if(r==nullptr)
            return nullptr; // should never happen!
        while(r->right != nullptr)
            r = r->right;
        return r;
    }
    
    // recursive helper function for node removal
    //   returns root of resulting tree after removal.
    static bst_node * _remove(bst_node *r, T & x, bool &success){
        bst_node *tmp;
        bool sanity;
        
        if(r==nullptr){
            success = false;
            return nullptr;
        }
        if(r->val == x){
            success = true;
            
            if(r->left == nullptr){
                tmp = r->right;
                delete r;
                return tmp;
            }
            if(r->right == nullptr){
                tmp = r->left;
                delete r;
                return tmp;
            }
            // if we get here, r has two children
            r->val = _min_node(r->right)->val;
            r->right = _remove(r->right, r->val, sanity);
            
            if (r->left == nullptr && r->right != nullptr) {
                int high = std::max(0, r->right->size+1);
                int low = std::min(0, r->right->size+1);
                if(high > (2 * low + 1)) {
                    std::vector<bst_node*> nodes;
                    storeTreeNodes(r,nodes);
                    r = balanceTree(r);
                }
            }
            
            else if(r->left != nullptr && r->right == nullptr) {
                int high = std::max(r->left->size+1, 0);
                int low = std::min(r->left->size+1, 0);
                if(high > (2 * low + 1)) {
                    std::vector<bst_node*> nodes;
                    storeTreeNodes(r,nodes);
                    r = balanceTree(r);
                }
            }
            else if(r->left != nullptr && r->right != nullptr) {
                int high = std::max(r->left->size+1, r->right->size+1);
                int low = std::min(r->left->size+1, r->right->size+1);
                if(high > (2 * low + 1)) {
                    std::vector<bst_node*> nodes;
                    storeTreeNodes(r,nodes);
                    r = balanceTree(r);
                }
            }

            

            if(!sanity)
                std::cerr << "ERROR:  remove() failed to delete promoted value?\n";
            r->size--;
            return r;
        }
        if(x < r->val){
            r->left = _remove(r->left, x, success);
            
            if (r->left == nullptr && r->right != nullptr) {
                int high = std::max(0, r->right->size+1);
                int low = std::min(0, r->right->size+1);
                if(high > (2 * low + 1)) {
                    std::vector<bst_node*> nodes;
                    storeTreeNodes(r,nodes);
                    r = balanceTree(r);
                }
            }
            
            else if(r->left != nullptr && r->right == nullptr) {
                int high = std::max(r->left->size+1, 0);
                int low = std::min(r->left->size+1, 0);
                if(high > (2 * low + 1)) {
                    std::vector<bst_node*> nodes;
                    storeTreeNodes(r,nodes);
                    r = balanceTree(r);
                }
            }
            else if(r->left != nullptr && r->right != nullptr) {
                int high = std::max(r->left->size+1, r->right->size+1);
                int low = std::min(r->left->size+1, r->right->size+1);
                if(high > (2 * low + 1)) {
                    std::vector<bst_node*> nodes;
                    storeTreeNodes(r,nodes);
                    r = balanceTree(r);
                }
            }

            
            
        }
        else {
            r->right = _remove(r->right, x, success);
            
            if (r->left == nullptr && r->right != nullptr) {
                int high = std::max(0, r->right->size+1);
                int low = std::min(0, r->right->size+1);
                if(high > (2 * low + 1)) {
                    std::vector<bst_node*> nodes;
                    storeTreeNodes(r,nodes);
                    r = balanceTree(r);
                }
            }
            
            else if(r->left != nullptr && r->right == nullptr) {
                int high = std::max(r->left->size+1, 0);
                int low = std::min(r->left->size+1, 0);
                if(high > (2 * low + 1)) {
                    std::vector<bst_node*> nodes;
                    storeTreeNodes(r,nodes);
                    r = balanceTree(r);
                }
            }
            else if(r->left != nullptr && r->right != nullptr) {
                int high = std::max(r->left->size+1, r->right->size+1);
                int low = std::min(r->left->size+1, r->right->size+1);
                if(high > (2 * low + 1)) {
                    std::vector<bst_node*> nodes;
                    storeTreeNodes(r,nodes);
                    r = balanceTree(r);
                }
            }

        }
        
        if(success) {
            r->size--;
        }
        
        return r;
        
    }
    
public:
    
    
    
    
    
    
    
    
    bool remove(T & x){
        bool success;
        root = _remove(root, x, success);
        return success;
    }
    
    
private:
    // recursive helper function to compute size of
    //   tree rooted at r
    static int _size(bst_node *r){
        if(r==nullptr) return 0;
        return _size(r->left) + _size(r->right) + 1;
    }
    
public:
    int size() {
        return _size(root);
    }
    
private:
    
    static int _height(bst_node *r){
        int l_h, r_h;
        
        if(r==nullptr) return -1;
        l_h = _height(r->left);
        r_h = _height(r->right);
        return 1 + (l_h > r_h ? l_h : r_h);
    }
    
public:
    
    int height() {
        return _height(root);
    }
    
    bool min(T & answer) {
        if(root == nullptr){
            return false;
        }
        answer = _min_node(root)->val;
        return true;
    }
    
    T max() {
        return _max_node(root)->val;
    }
    
    /******************************************
     *
     * "stubs" for assigned TODO functions below
     *
     *****************************************/
    
    // TODO
    std::vector<T> * to_vector() {
        
        if(root == nullptr) {
            return nullptr;
        }
        int i = 0;
        std:: vector<T> * tVect = new std::vector<T>();
        std:: vector<bst_node*> nodes;
        
    
        storeTreeNodes(root, nodes);
        
        for(i=0;i<nodes.size();i++) {
            tVect->push_back(nodes[i]->val);
        }
        return tVect;
        
    }
    
    
    /* TODO
     * Function:  get_ith
     * Description:  determines the ith smallest element in t and
     *    "passes it back" to the caller via the reference parameter x.
     *    i ranges from 1..n where n is the number of elements in the
     *    tree.
     *
     *    If i is outside this range, false is returned.
     *    Otherwise, true is returned (indicating "success").
     *
     * Runtime:  O(h) where h is the tree height
     */
    bool get_ith(int i, T &x) {
        
        
        int n = size();
        
        
        if(i < 1 || i > n)
            return false;
        
        if(root == nullptr) {
            return false;
        }
        
        x = _get_ith_FAST(root, i);
        return true;

        return false;
    }
    
    
    bool get_ith_SLOW(int i, T &x) {
        int n = size();
        int sofar=0;
        
        if(i < 1 || i > n)
            return false;
        
        _get_ith_SLOW(root, i, x, sofar);
        return true;
    }
    
    
    /* TODO
     * Function:  num_geq
     * Description:  returns the number of elements in tree which are
     *       greater than or equal to x.
     *
     * Runtime:  O(h) where h is the tree height
     */
    int num_geq(const T & x) {
        int matches = 0;
        _num_geq(root, x, matches);

        return matches;
        
        
    }
    
    /*
     * function:     num_geq_SLOW
     * description:  same functionality as num_geq but sloooow (linear time)
     *
     */
    int num_geq_SLOW(const T & x) {
        return _num_geq_SLOW(root, x);
    }
    
    
    /* TODO
     * Function:  num_leq
     * Description:  returns the number of elements in tree which are less
     *      than or equal to x.
     *
     * Runtime:  O(h) where h is the tree height
     *
     **/
    int num_leq(const T &x) {
        
        int matches = 0;
        _num_leq(root, x, matches);
 
        return matches;
        
    
        
    }
    
    /*
     * function:     num_leq_SLOW
     * description:  same functionality as num_leq but sloooow (linear time)
     *
     */
    int num_leq_SLOW(const T & x) {
        return _num_leq_SLOW(root, x);
    }
    
    /* TODO
     * Function:  num_range
     * Description:  returns the number of elements in tree which are
     *       between min and max (inclusive).
     *
     * Runtime:  O(h) where h is the tree height
     *
     **/
    int num_range(const T & min, const T & max) {
        
        
        if( root == nullptr ) {
            return 0; }
        int total = 0;
        _num_range(root, min, max, total);
        return total;
        

        
    }
    
    /*
     * function:     num_range_SLOW
     * description:  same functionality as num_range but sloooow (linear time)
     *
     */
    int num_range_SLOW(const T & min, const T & max) {
        return _num_range_SLOW(root, min, max);
    }
    
private:
    
    
    static void _get_ith_SLOW(bst_node *t, int i, T &x, int &sofar) {
        if(t==nullptr)
            return;
        _get_ith_SLOW(t->left, i, x, sofar);
        
        if(sofar==i)
            return;
        sofar++;
        if(sofar==i) {
            x = t->val;
            return;
        }
        _get_ith_SLOW(t->right, i, x, sofar);
    }
    
    static int _num_geq_SLOW(bst_node * t, const T & x) {
        int total;
        
        if(t==nullptr) return 0;
        total =_num_geq_SLOW(t->left, x) + _num_geq_SLOW(t->right, x);
        
        if(t->val >= x)
            total++;
        return total;
    }
    
    static int _num_leq_SLOW(bst_node *t, const T &x) {
        int total;
        
        if(t==nullptr) return 0;
        total =_num_leq_SLOW(t->left, x) + _num_leq_SLOW(t->right, x);
        
        if(t->val <= x)
            total++;
        return total;
    }
    
    static int _num_range_SLOW(bst_node *t, const T &min, const T &max) {
        int total;
        
        if(t==nullptr) return 0;
        total =_num_range_SLOW(t->left, min, max) +
        _num_range_SLOW(t->right, min, max);
        
        if(t->val >= min && t->val <= max)
            total++;
        return total;
    }
    
    static int _get_ith_FAST(bst_node *t, int i) {
        
        if(t == nullptr) {
            return 0;
        }
        
        
        if(t->left != nullptr && t->left->size+1 >= i) {
            return _get_ith_FAST(t->left, i);
        }
        if(t->left != nullptr && t->left->size+1 < i-1 ) {
            return _get_ith_FAST(t->right, i-(t->left->size+2));
        }
        
        if(t->left != nullptr && t->left->size+1 == i - 1) {
            return t->val;
        }
        
        return 0;

    }
    
    static void _num_geq(bst_node *r, int i, int &matches) {
        
        
        if(r == nullptr) {
            return;
        }
        if(r->val >= i) {
            matches++;
            if(r->right != nullptr)
                
                matches = matches + 1 + r->right->size;
            _num_geq(r->left, i, matches);
        }
        else if( r->val < i) {
            _num_geq(r->right, i, matches);
        }
        
    }
    
    
    static void _num_leq(bst_node* r, int i, int &matches) {
        
    
    if(r == nullptr) {
        return;
    }
    if(r->val <= i) {
        matches++;
        if(r->left != nullptr)
    
        matches = matches + 1 + r->left->size;
        _num_leq(r->right, i, matches);
    }
    else if( r->val > i) {
        _num_leq(r->left, i, matches);
    }
    }
    
    static void _num_range(bst_node* r, int min, int max, int &total) {
        if( r == nullptr) {
            return;
        }
        
        if(r->val < min) {
            _num_range(r->right, min, max, total);
            }
        else if(r->val > max) {
            _num_range(r->left, min, max, total);
        }
        else {
            total++;
            _num_range(r->right, min, max, total);
            _num_range(r->left, min, max, total);
            
        }
        }
    
    
    
    
    
    
    
    
    
     // stores the node of the tree in a vector
    static void storeTreeNodes(bst_node *root, std::vector<bst_node*> &nodes) {
        if(root == nullptr) {
            return;
        }
        storeTreeNodes(root->left, nodes);
        nodes.push_back(root);
        storeTreeNodes(root->right, nodes);
    }
    
    // converts unbalanced bts to a balanced one
    
    
    
    static bst_node* balanceTree(bst_node* root) {
                
        
        std::vector<bst_node*> nodes;
        storeTreeNodes(root, nodes);
        
        int s = nodes.size();
        return buildTreeUtil(nodes, 0, s-1);
        
    }
    
    
    
    
    
private:
    static void indent(int m){
        int i;
        for(i=0; i<m; i++)
            std::cout << "-";
    }
    
    static void _inorder(bst_node *r){
        if(r==nullptr) return;
        _inorder(r->left);
        std::cout << "[ " << r->val << " ]\n";
        _inorder(r->right);
    }
    
    static void _preorder(bst_node *r, int margin){
        if(r==nullptr) {
            indent(margin);
            std::cout << " nullptr \n";
        }
        else {
            indent(margin);
            std::cout << "[ " << r->val << " ]\n";
            _preorder(r->left, margin+3);
            _preorder(r->right, margin+3);
        }
    }
    
    /*
     * TODO:
     * Complete the (recursive) helper function for the post-order traversal.
     * Remember: the indentation needs to be proportional to the height of the node!
     */
    static void _postorder(bst_node *r, int margin){
        
        std::cout << "\n   WARNING:  POSTORDER UNIMPLEMENTED...\n";
    }
    
public:
    void inorder() {
        std::cout << "\n======== BEGIN INORDER ============\n";
        _inorder(root);
        std::cout << "\n========  END INORDER  ============\n";
    }
    
    
    void preorder() {
        
        std::cout << "\n======== BEGIN PREORDER ============\n";
        _preorder(root, 0);
        std::cout << "\n========  END PREORDER  ============\n";
        
    }
    
    // indentation is proportional to depth of node being printed
    //   depth is #hops from root.
    void postorder() {
        
        std::cout << "\n======== BEGIN POSTORDER ============\n";
        _postorder(root, 0);
        std::cout << "\n========  END POSTORDER  ============\n";
        
    }
    
private:
    /*
     * Recursive  helper function _from_vec, used by
     * bst_from_sorted_arr(...). The function must return a sub-tree that is
     * perfectly balanced, given a sorted array of elements a.
     */
    static bst_node * _from_vec( std::vector<T> &a, int low, int hi){
        int m;
        bst_node *root;
        
        if(hi < low) return nullptr;
        m = (low+hi)/2;
        root = new bst_node(a[m]);
        root->left  = _from_vec(a, low, m-1);
        root->right = _from_vec(a, m+1, hi);
        return root;
        
    }
    
    static bst_node* buildTreeUtil(std::vector<bst_node*> &nodes, int start,
                        int end)
    {
        // base case
        if (start > end)
            return NULL;
        
        /* Get the middle element and make it root */
        int mid = (start + end)/2;
        bst_node *root = nodes[mid];

        
        /* Using index in Inorder traversal, construct
         left and right subtress */
        

        root->left  = buildTreeUtil(nodes, start, mid-1);
        
            if(root->left != nullptr && root->right == nullptr) {
                root->size = root->left->size + 1;
            }
        
        
        root->right = buildTreeUtil(nodes, mid+1, end);
        
        
            if(root->right != nullptr && root->left != nullptr) {
                root->size = root->left->size+1 + root->right->size+1;
            }
            if(root->left == nullptr && root->right != nullptr) {
                root->size = root->right->size+1;
            }
        
            if(root->left == nullptr && root->right == nullptr) {
                root->size = 0;
            }
        
        return root;
    }
    
    
    
    
    
    
public:
    static bst * from_sorted_vec(const std::vector<T> &a, int n){
        
        bst * t = new bst();
        t->root = _from_vec(a, 0, n-1);
        return t;
    }
    
    // TODO:  num_leaves
    //   Hint:  feel free to write a helper function!!
    int num_leaves() {
        
        std::cout << "\n     WARNING:  bst::num_leaves UNIMPLEMENTED...\n";
        return 0;
    }
    
    // TODO:  num_at_level
    // description:  returns the number of nodes at specified level
    //   in tree.
    // note:  the root is at level 0.
    // Hint:  recursive helper function?
    int num_at_level(int level) {
        
        std::cout << "\n     WARNING:  bst::num_leaves UNIMPLEMENTED...\n";
        return 0;
    }
    
    
private:
    bst_node *root;
    
    
}; // end class bst

#endif
