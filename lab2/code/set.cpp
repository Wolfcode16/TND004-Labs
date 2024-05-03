#include "set.h"
#include "node.h"

int Set::Node::count_nodes = 0;

/*****************************************************
 * Implementation of the member functions             *
 ******************************************************/

/*
 *  Default constructor :create an empty Set
 */
int Set::get_count_nodes() {
    return Set::Node::count_nodes;
}

/*
 *  Default constructor :create an empty Set
 */
Set::Set() : counter{0} {
    // IMPLEMENT before Lab2 HA
    
    head = new Node();          // O(1)
    tail = new Node();          // O(1)

    head->next = tail;          // O(1)
    tail->prev = head;          // O(1)
}

/*
 *  Conversion constructor: convert val into a singleton {val}
 */
Set::Set(int val) : Set{} {  // create an empty list
    // IMPLEMENT before Lab2 HA
    
    insert_node(tail, val);             // O(1)
}

/*
 * Constructor to create a Set from a sorted vector of unique ints
 * Create a Set with all ints in sorted vector list_of_values
 */
Set::Set(const std::vector<int>& list_of_values) : Set{} {  // create an empty list
    // IMPLEMENT before Lab2 HA
    
    for (size_t e : list_of_values)     // O(n)
    {
        insert_node(tail, e);
    }
}

/*
 * Copy constructor: create a new Set as a copy of Set S
 * \param S Set to copied
 * Function does not modify Set S in any way
 */
Set::Set(const Set& S) : Set{} {  // create an empty list
    // IMPLEMENT before Lab2 HA
    
    Node* ptr = S.head->next; // First Node after dummy
    while (ptr != S.tail)               // O(n)
    {
        insert_node(tail, ptr->value);
        ptr = ptr->next;
    }
}

/*
 * Transform the Set into an empty set
 * Remove all nodes from the list, except the dummy nodes
 */
void Set::make_empty() {
    // IMPLEMENT before Lab2 HA

    Node* ptr = head->next;
    while (ptr != tail)             // O(n)
    {
        ptr = ptr->next;
        remove_node(ptr->prev);
    }

    head->next = tail;  // or ptr?
    tail->prev = head;
}

/*
 * Destructor: deallocate all memory (Nodes) allocated for the list
 */
Set::~Set() {
    // IMPLEMENT before Lab2 HA

    make_empty();       // O(n)
    remove_node(head);  // O(1)
    remove_node(tail);  // O(1)
}

/*
 * Assignment operator: assign new contents to the *this Set, replacing its current content
 * \param S Set to be copied into Set *this
 * Call by valued is used
 */
Set& Set::operator=(Set S) {
    // IMPLEMENT before Lab2 HA

    counter = S.counter;        // O(1)
    std::swap(head, S.head);    // O(1)
    std::swap(tail, S.tail);    // O(1)
    return *this;
}

/*
 * Test whether val belongs to the Set
 * Return true if val belongs to the set, otherwise false
 * This function does not modify the Set in any way
 */
bool Set::is_member(int val) const {
    // IMPLEMENT before Lab2 HA

    Node* ptr = head->next;
    while (ptr != tail)
    {
        if (ptr->value == val)
        {
            return true;
        }
        ptr = ptr->next;
    }

    return false;  // remove this line
}

/*
 * Test whether Set *this and S represent the same set
 * Return true, if *this has same elemnts as set S
 * Return false, otherwise
 */
bool Set::operator==(const Set& S) const {
    // IMPLEMENT before Lab2 HA

    if (counter != S.counter)
    {
        return false;
    }

    Node* ptr = head->next;
    Node* ptr_s = S.head->next;


    while (ptr != tail && ptr_s != S.tail)
    {
        if (ptr->value != ptr_s->value)
        {
            return false;
        }

        ptr = ptr->next;
        ptr_s = ptr_s->next;
    }

    return true;
}

/*
 * Three-way comparison operator: to test whether *this == S, *this < S, *this > S
 * Return std::partial_ordering::equivalent, if *this == S
 * Return std::partial_ordering::less, if *this < S
 * Return std::partial_ordering::greater, if *this > S
 * Return std::partial_ordering::unordered, otherwise
 */
std::partial_ordering Set::operator<=>(const Set& S) const {
    // IMPLEMENT before Lab2 HA

    Node* ptr = head->next;
    Node* ptr_s = S.head->next;

    // If all elements in S are found in *this      [greater]
    if (this->counter > S.counter)
    {
        while (ptr_s != S.tail)
        {
            if (ptr->value != ptr_s->value)
            {
                ptr = ptr->next;
            }
            else
            {
                ptr = ptr->next;
                ptr_s = ptr_s->next;    // no duplicates so its ok.
            }

            if (ptr == tail)
            {
                return std::partial_ordering::unordered;
            }

            if (ptr_s == S.tail)
            {
                return std::partial_ordering::greater;
            }
        }
    }

    // If no elements in S are found in *this       [less]
    if (this->counter < S.counter)
    {
        while (ptr != tail)
        {
            if (ptr->value != ptr_s->value)
            {
                ptr_s = ptr_s->next;
            }
            else
            {
                ptr = ptr->next;
                ptr_s = ptr_s->next;    // no duplicates so its ok.
            }

            if (ptr_s == S.tail)
            {
                return std::partial_ordering::unordered;
            }

            if (ptr == tail)
            {
                return std::partial_ordering::less;
            }
        }
    }

    // If elements in S are the same as in *this    [equivalent]
    if (*this == S)
    {
        return std::partial_ordering::equivalent;
    }

    // Otherwise [unordered]
    return std::partial_ordering::unordered;
}

/*
 * Modify Set *this such that it becomes the union of *this with Set S
 * Set *this is modified and then returned
 */
Set& Set::operator+=(const Set& S) {
    // IMPLEMENT
    Node* ptr = head->next;
    Node* ptr_s = S.head->next;
    
    while (ptr != tail && ptr_s != S.tail)
    {
        if (ptr->value < ptr_s->value)
        {
            ptr = ptr->next;
        }

        if (ptr->value > ptr_s->value)
        {
            insert_node(ptr, ptr_s->value);
            ptr_s = ptr_s->next;
        }

        if (ptr->value == ptr_s->value)
        {
            ptr = ptr->next;
            ptr_s = ptr_s->next;
        }
    }

    while (ptr_s != S.tail)
    {
        insert_node(ptr, ptr_s->value);
        ptr_s = ptr_s->next;
    }

    return *this;
}

/*
 * Modify Set *this such that it becomes the intersection of *this with Set S
 * Set *this is modified and then returned
 */
Set& Set::operator*=(const Set& S) {
    // IMPLEMENT

    Node* ptr = head->next;
    Node* ptr_s = S.head->next;

    while (ptr != tail && ptr_s != S.tail)
    {
        if (ptr->value < ptr_s->value)
        {
            ptr = ptr->next;
            remove_node(ptr->prev);
        }

        if (ptr->value > ptr_s->value)
        {
            ptr_s = ptr_s->next;
        }

        if (ptr->value == ptr_s->value)
        {
            ptr = ptr->next;
            ptr_s = ptr_s->next;
        }
    }

    while (ptr != tail)
    {
        ptr = ptr->next;
        remove_node(ptr->prev);
    }

    return *this;
}

/*
 * Modify Set *this such that it becomes the Set difference between Set *this and Set S
 * Set *this is modified and then returned
 */
Set& Set::operator-=(const Set& S) {
    // IMPLEMENT

    Node* ptr = head->next;
    Node* ptr_s = S.head->next;

    while (ptr != tail && ptr_s != S.tail)
    {
        if (ptr->value < ptr_s->value)
        {
            ptr = ptr->next;
        }

        if (ptr->value > ptr_s->value)
        {
            ptr_s = ptr_s->next;
        }

        if (ptr->value == ptr_s->value)
        {
            ptr = ptr->next;
            ptr_s = ptr_s->next;
            remove_node(ptr->prev);
        }
    }

    return *this;
}


/* ******************************************** *
 * Private Member Functions -- Implementation   *
 * ******************************************** */

/*
 * Insert a new Node storing val after the Node pointed by p
 * \param p pointer to a Node
 * \param val value to be inserted  after position p
 */
void Set::insert_node(Node* p, int val) {
    // IMPLEMENT before Lab2 HA
    // Lecture 4, slide 6
    Node* newNode = new Node(val, p, p->prev);
    p->prev = p->prev->next = newNode;
    ++counter;
}

/*
 * Remove the Node pointed by p
 * \param p pointer to a Node
 */
void Set::remove_node(Node* p) {
    // IMPLEMENT before Lab2 HA
    if (p == nullptr) { return; }

    if (head == p) { head = p->next; }

    if (p->next != nullptr) { p->next->prev = p->prev; }

    if (p->prev != nullptr) { p->prev->next = p->next; }

    delete p;
    counter--;
}

/*
 * Write Set *this to stream os
 */
void Set::write_to_stream(std::ostream& os) const {
    if (is_empty()) {
        os << "Set is empty!";
    } else {
        Set::Node* ptr{head->next};

        os << "{ ";
        while (ptr != tail) {
            os << ptr->value << " ";
            ptr = ptr->next;
        }
        os << "}";
    }
}
