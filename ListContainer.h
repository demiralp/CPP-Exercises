/** @file       ListContainer.h
 *  @details    A template container class with an underlying
 *              doubly linked list structure.
 *  @author     Caglayan DOKME, caglayandokme@gmail.com
 *  @date       February 25, 2021 -> First release
 *              February 26, 2021 -> File documented with doxygen.
 *                                -> Conditional remove functions added.
 *                                -> Swap function added.
 *              February 27, 2021 -> Resize function added.
 *                                -> Remove function with lambda predicate added.
 *                                -> Duplicate remover function added.
 *                                -> Sort method added.
 *              February 28, 2021 -> Emplace append and emplace prepend methods added.
 *                                -> Iterator class and related functions added.
 *              March 2, 2021     -> Merge and concatenate methods added.
 *                                   Splice method added.
 *              March 5, 2021     -> Recursive inclusion blocker added.
 *                                   Fill constructor added with two versions.
 *              March 6, 2021     -> Range constructor added.
 *                                   Copy constructor added.
 *                                   Move constructor added.
 *                                   Initializer list constructor added.
 *                                   Equality and inequality operator overloaded for iterator class.
 *
 *  @note       Feel free to contact for questions, bugs or any other thing.
 *  @copyright  No copyright. Code is open source.
 */

#ifndef LIST_CONTAINER_H
#define LIST_CONTAINER_H

#include <iostream>

// Forward declaration
template<class T> class ListNode;

template<class T>
class List{
public:
    class iterator; // Forward declaration

    /*** Constructors and Destructors ***/
    List();                     // Default constructor
    List (const size_t n);      // Construct with n nodes initally

    template<class... Args>
    List(const size_t n, Args&&... args);   // Construct with n nodes initially using the arguments

    template<class AnotherIteratorType>
    List(AnotherIteratorType begin, AnotherIteratorType end);   // Range constructor

    List(List<T>& anotherList);                     // Copy constructor
    List(List<T>&& anotherList);                    // Move constructor
    List(std::initializer_list<T> initializerList); // Initializer list constructor

    virtual ~List();            // Destructor

    /*** Element Access ***/
    const T& First() const; // Get the first data as an rValue
    const T& Last() const;  // Get the last data as an rValue
    T& First();             // Get the first data as an lValue
    T& Last();              // Get the last data as an lValue

    /*** Modifiers ***/
    List<T>& Append(const T& data);     // Add after the last node
    List<T>& Prepend(const T& data);    // Add before the first node

    template <class... Args>
    List<T>& EmplaceAppend(Args&&... args);     // Constructs the node element inplace
    template <class... Args>
    List<T>& EmplacePrepend(Args&&... args);    // Constructs the node element inplace

    template<class RuleT>
    List<T>& RemoveIf(RuleT Predicate);         // Remove all fulfilling the condition of predicate

    List<T>& RemoveFirst();                     // Remove the first node
    List<T>& RemoveLast();                      // Remove the last node
    List<T>& RemoveIf(const T& data);           // Remove all samples of a specific data
    List<T>& RemoveFirstOf(const T& data);      // Remove the first sample of a specific data
    List<T>& RemoveLastOf(const T& data);       // Remove the last sample of a specific data
    List<T>& RemoveIfNot(const T& data);        // Remove all samples which are not of a specific data
    List<T>& RemoveFirstNotOf(const T& data);   // Remove the first sample that is not the given data
    List<T>& RemoveLastNotOf(const T& data);    // Remove the last sample that is not the given data
    List<T>& EraseAll();                        // Remove all elements
    void ReplaceAllWith(const T& oldData, const T& newData);
    void ReplaceFirstWith(const T& oldData, const T& newData);
    void ReplaceLastWith(const T& oldData, const T& newData);

    /*** Operations ***/
    void Swap(List<T>& anotherList);                            // Exchanges the content of the list by the content of another list
    void Resize(const size_t newSize, const T& data = 0);       // Resizes the list so that it contains newSize of elements
    void Unique();                                              // Remove duplicate values
    void Sort();                                                // Sorts in ascending order
    void PrintAll(std::ostream& stream) const;                  // Prints all elements by inserting to the given stream
    void Merge(List<T>& anotherList);                           // Merges two sorted list
    void Concatenate(List<T>& anotherList);                     // Concatenates two lists
    void Splice(const iterator& destination, List<T>& anotherList);

    /*** Status Checkers ***/
    bool isEmpty() const        { return (numberOfNodes == 0);                  }
    size_t GetNodeCount() const { return numberOfNodes;                         }
    bool isSorted() const       { return (!isEmpty() && firstPtr->isSorted());  }   // Recursively checks the status of each node

    /*** Operator Overloadings ***/
    bool operator==(const List<T>& anotherList) const    // Compare two lists by equality
    { return (firstPtr == anotherList.firstPtr); }
    bool operator!=(const List<T>& anotherList) const    // Compare two lists by inequality
    { return !operator==(anotherList); }

    /* Declaring a function as a friend inside of a template class
       corrupts the template usage. You may want to check the holy StackOverflow :)
       stackoverflow.com/questions/4660123 */
    template<class _T>
    friend std::ostream& operator<<(std::ostream& stream, List<_T>& list);

    /*** Iterators ***/
    class iterator{
        friend class List;
    public:
        iterator() = delete;    // There must be a node address to reach the list
        iterator(ListNode<T>* node) : node(node)
        { if(node == nullptr) throw std::logic_error("Iterator construction failed!"); }

        void operator++()       { if(node->nextPtr != nullptr) node = node->nextPtr; }  // Prefix increment
        void operator++(int)    { if(node->nextPtr != nullptr) node = node->nextPtr; }  // Postfix increment
        void operator--()       { if(node->prevPtr != nullptr) node = node->prevPtr; }  // Prefix decrement
        void operator--(int)    { if(node->prevPtr != nullptr) node = node->prevPtr; }  // Postfix decrement

        T& operator*() { return node->data; }   // Dereference operator
        bool operator==(const iterator& anotherIt) { return (node == anotherIt.node);   }   // Equality operator
        bool operator!=(const iterator& anotherIt) { return !operator==(anotherIt);     }   // Inequality operator

    private:
        ListNode<T>* node = nullptr;
    };

    iterator begin()    // Returns an iterator pointing to the first element
    {
        if(isEmpty() == true)
            throw std::logic_error("Cannot iterate in an empty list!");

        return iterator(firstPtr);
    }

    iterator end()      // Returns an iterator pointing to the last element
    {
        if(isEmpty() == true)
            throw std::logic_error("Cannot iterate in an empty list!");

        /* IMPORTANT NOTE:  Unlike the STL's end, this end function returns an iterator
                            starting from the last node of the list. So, be careful :) */
        return iterator(lastPtr);
    }

private:
    /*** Searching ***/
    ListNode<T>* Find(const T& data, ListNode<T>* beginByNode);
    ListNode<T>* FindNotOf(const T& data, ListNode<T>* beginByNode);
    ListNode<T>* FindReversed(const T& data, ListNode<T>* beginByNode);
    ListNode<T>* FindNotOfReversed(const T& data, ListNode<T>* beginByNode);
    ListNode<T>* FindMinimum(ListNode<T>* beginByNode);

    /*** Operations **/
    void DetachNode(ListNode<T>* removingNode);                                     // Detaching a node from a list by not destroying the content
    void RemoveNode(ListNode<T>* removingNode);                                     // Remove a specific node
    List<T>& RemoveIf(const T& data, ListNode<T>* beginByNode);                     // Remove all samples of a specific data
    void SwapNodes(ListNode<T>* firstNode, ListNode<T>* secondNode);                // Swap different nodes
    void SwapSuccessiveNodes(ListNode<T>* firstNode, ListNode<T>* secondNode);      // Swap directly linked nodes
    void SwapNonSuccessiveNodes(ListNode<T>* firstNode, ListNode<T>* secondNode);   // Swap indirectly linked nodes
    void Append(ListNode<T>* baseNode, ListNode<T>* newNode);       // Appending a node to a certain node
    void Prepend(ListNode<T>* baseNode, ListNode<T>* newNode);      // Prepending a node to a certain node
    void Append(ListNode<T>* baseNode, List<T>& anotherList);       // Appending a list to a certain node7

    /*** Members ***/
    ListNode<T>* firstPtr   = nullptr;  // First node of the list
    ListNode<T>* lastPtr    = nullptr;  // Last node of the list
    size_t numberOfNodes    = 0;        // Node count
};

template<class T>
class ListNode{
    friend class List<T>;

public:
    ListNode(const T& data) : data(data), prevPtr(nullptr), nextPtr(nullptr)
    { /* Empty constructor */ }

    template<class... Args>
    ListNode(Args&&... args): data(args...), prevPtr(nullptr), nextPtr(nullptr)
    { /* Empty constructor */ }

    // Recursively checks the status of each node
    bool isSorted() const
    {
        if(nextPtr == nullptr)
            return true;

        if(nextPtr->data < data)
            return false;

        return nextPtr->isSorted();
    }

private:
    T data;
    ListNode* prevPtr = nullptr;
    ListNode* nextPtr = nullptr;
};

/**
 * @brief Default constructor
 */
template<class T>
List<T>::List()
: firstPtr(nullptr), lastPtr(nullptr), numberOfNodes(0)
{ /* Empty constructor */ }

/**
 * @brief   Constructs a container with n elements initially.
 * @param   n   Size of initial construction nodes.
 */
template<class T>
List<T>::List (const size_t n)
: firstPtr(nullptr), lastPtr(nullptr), numberOfNodes(0)
{
    // Append n nodes to empty list by in place construction
    while (GetNodeCount() < n)
        EmplaceAppend();
}

/**
 * @brief   Constructs a container with n elements initially using the given arguments.
 * @param   n       Size of initial construction nodes.
 * @param   args    Construction arguments for initial nodes.
 */
template<class T>
template<class... Args>
List<T>::List(const size_t n, Args&&... args)
: firstPtr(nullptr), lastPtr(nullptr), numberOfNodes(0)
{
    // Append n nodes to empty list by in place construction
    while (GetNodeCount() < n)
        EmplaceAppend(args...);
}

/**
 * @brief   Constructs a container with as many elements as the range [begin, end),
 *          with each element emplace-constructed from its corresponding element in that range, in the same order.
 * @param   begin   Input iterator to the initial position in a range.
 * @param   end     Input iterator to the final position in a range.
 * @note    The begin iterator must placed prior to the end iterator.
 * @note    Template used for iterator type because the user may want to copy the items of a different type of container.
 *          Here is where the idea comes from : stackoverflow.com/questions/30121228
 */
template<class T>
template<class AnotherIteratorType>
List<T>::List(AnotherIteratorType begin, AnotherIteratorType end)
: firstPtr(nullptr), lastPtr(nullptr), numberOfNodes(0)
{
    AnotherIteratorType tempIt = begin;

    // Append all nodes in the range by copying its data members
    while (true)
    {
        EmplaceAppend(*tempIt);     // Append by inplace construction

        if(tempIt != end)   // Continue until the last one is appended
            tempIt++;
        else
            break;
    }
}

/**
 * @brief   Constructs a list with a copy of each of the elements from another list, in the same order.
 * @param   anotherList List to be copied from.
 * @note    If you want to copy another type of list, you shall use the range constructor.
 */
template<class T>
List<T>::List(List<T>& anotherList)
: firstPtr(nullptr), lastPtr(nullptr), numberOfNodes(0)
{
    if(anotherList.isEmpty() == true)
        return;

    List<T>::iterator it = anotherList.begin();

    // Copy all elements one by one
    while(numberOfNodes != anotherList.GetNodeCount())
    {
        Append(*it);
        it++;
    }
}

/**
 * @brief   Move constructor, steals the resources of the given list.
 * @param   anotherList Locally created constant source list.
 * @note    It is recommendded to use the std::mode for the input list.
 */
template<class T>
List<T>::List(List<T>&& anotherList)
: firstPtr(anotherList.firstPtr), lastPtr(anotherList.lastPtr), numberOfNodes(anotherList.GetNodeCount())
{
    /* No need to make an element wised copy as the source is
       a locally created list container. Assigning nullptr
       to the source container prevents destroying its content as
       we used its resources to construct the new one. */
    anotherList.firstPtr        = nullptr;
    anotherList.lastPtr         = nullptr;
    anotherList.numberOfNodes   = 0;
}

/**
 * @brief   Construction with initializer list
 * @param   initializerList   Initializer list
 */
template<class T>
List<T>::List(std::initializer_list<T> initializerList)
: firstPtr(nullptr), lastPtr(nullptr), numberOfNodes(0)
{
    // Append each element by using a range-for
    for(const T& element : initializerList)
        Append(element);
}

/**
 * @brief Destroys all nodes one by one
 */
template<class T>
List<T>::~List()
{
    /* Destroy all nodes until there is no node left. */
    while(isEmpty() == false)
        RemoveFirst();
}


/**
 * @brief   Appends the given data next to the last node
 * @param   data      Data to be appended
 * @return  lValue reference to the current list to support cascades
 */
template<class T>
List<T>& List<T>::Append(const T& data)
{
    if(isEmpty() == true)  // If it is the first node
    {
        firstPtr    = new ListNode<T>(data);    // Create the first node
        lastPtr     = firstPtr; // The last and the first points the same node
    }
    else
    {
        lastPtr->nextPtr = new ListNode<T>(data);   // Create and append the node
        lastPtr->nextPtr->prevPtr = lastPtr;        // Adjust prevNode connection
        lastPtr = lastPtr->nextPtr;                 // Update the lastPtr
    }

    numberOfNodes++;    // Increase the number of nodes

    return *this;   // Support cascaded appends
}


/**
 * @brief   Prepends the data before the first node
 * @param   data      Data to be prepended
 * @return  lValue reference to the current list to support cascades
 */
template<class T>
List<T>& List<T>::Prepend(const T& data)
{
    if(isEmpty() == true)   // If it is the first node
    {
        firstPtr    = new ListNode<T>(data);    // Create the first node
        lastPtr     = firstPtr; // The last and the first points the same node
    }
    else
    {
        firstPtr->prevPtr = new ListNode<T>(data);  // Create and prepend the node
        firstPtr->prevPtr->nextPtr = firstPtr;      // Adjust nextNode connection
        firstPtr = firstPtr->prevPtr;               // Update the firstPtr
    }

    numberOfNodes++;    // Increase the number of nodes

    return *this;
}

/**
 * @brief   Constructs the node's data member inplace by appending it to the list
 * @param   args    Arguments forwarded to construct the new element.
 * @return  lValue reference to the current list to support cascades
 */
template<class T>
template<class... Args>
List<T>& List<T>::EmplaceAppend(Args&&... args)
{
    if(isEmpty() == true)  // If it is the first node
    {
        firstPtr    = new ListNode<T>(args...);    // Create the first node
        lastPtr     = firstPtr; // The last and the first points the same node
    }
    else
    {
        lastPtr->nextPtr = new ListNode<T>(args...);    // Create and append the node
        lastPtr->nextPtr->prevPtr = lastPtr;            // Adjust prevNode connection
        lastPtr = lastPtr->nextPtr;                     // Update the lastPtr
    }

    numberOfNodes++;    // Increase the number of nodes

    return *this;   // Support cascaded appends
}
/**
 * @brief   Constructs the node's data member inplace by prepending it to the list
 * @param   args    Arguments forwarded to construct the new element.
 * @return  lValue reference to the current list to support cascades
 */
template<class T>
template<class... Args>
List<T>& List<T>::EmplacePrepend(Args&&... args)
{
    if(isEmpty() == true)   // If it is the first node
    {
        firstPtr    = new ListNode<T>(args...);    // Create the first node
        lastPtr     = firstPtr; // The last and the first points the same node
    }
    else
    {
        firstPtr->prevPtr = new ListNode<T>(args...);   // Create and prepend the node
        firstPtr->prevPtr->nextPtr = firstPtr;          // Adjust nextNode connection
        firstPtr = firstPtr->prevPtr;                   // Update the firstPtr
    }

    numberOfNodes++;    // Increase the number of nodes

    return *this;
}

/**
 * @brief   Iterator to reach to the first element.
 * @return  rValue reference to the data of first node.
 * @throws  std::logic_error If the list is empty
 */
template<class T>
const T& List<T>::First() const
{
    if(isEmpty() == true)
        throw std::logic_error("List is empty!");

    return firstPtr->data;
}

/**
 * @brief   Iterator to reach to the last element.
 * @return  rValue reference to the data of last node.
 * @throws  std::logic_error If the list is empty
 */
template<class T>
const T& List<T>::Last() const
{
    if(isEmpty() == true)
        throw std::logic_error("List is empty!");

    return lastPtr->data;
}

/**
 * @brief   Iterator to reach to the first element.
 * @return  lValue reference to the data of first node.
 * @throws  std::logic_error If the list is empty
 */
template<class T>
T& List<T>::First()
{
    if(isEmpty() == true)
        throw std::logic_error("List is empty!");

    return firstPtr->data;
}

/**
 * @brief   Iterator to reach to the last element.
 * @return  lValue reference to the data of last node.
 * @throws  std::logic_error If the list is empty
 */
template<class T>
T& List<T>::Last()
{
    if(isEmpty() == true)
        throw std::logic_error("List is empty!");

    return lastPtr->data;
}

/**
 * @brief   Removes from the list all the elements for which Predicate returns true.
 * @param   Predicate   Unary predicate that, taking a value of the same type as those
 *                      contained in the list nodes, returns true for those values to be
 *                      removed from the list, and false for those remaining.
 * @return  lValue reference to the current list to support cascaded calls
 * @note    An example call would look like this
 *          userList.RemoveIf([](T value) {return value > XXX;});
 *
 * @note    For more examples, refer to:
 *          github.com/CaglayanDokme/CPP-Exercises/blob/main/FuncWithLambdaArg.cpp
 */
template<class T>
template<class RuleT>
List<T>& List<T>::RemoveIf(RuleT Predicate)
{
    ListNode<T> *currentNode = firstPtr, *tempNode;

    while(currentNode != nullptr)
    {
        tempNode = currentNode->nextPtr;

        if(Predicate(currentNode->data) == true)
            RemoveNode(currentNode);

        currentNode = tempNode;
    }

    return *this; // Support cascaded calls
}

/**
 * @brief   Removes the first node
 * @return  lValue reference to the current list to support cascaded calls
 */
template<class T>
List<T>& List<T>::RemoveFirst()
{
    if(isEmpty() == false)
    {
        ListNode<T>* tempPtr = firstPtr;    // Save removing node addresss
        firstPtr = firstPtr->nextPtr;       // Update firstPtr
        delete tempPtr;                     // Delete saved firstPtr
        numberOfNodes--;                    // Decrement node count

        if(firstPtr != nullptr)
            firstPtr->prevPtr = nullptr;    // Remove prevNode connection
    }

    return *this;   // Support cascaded remove calls
}

/**
 * @brief   Removes the last node
 * @return  lValue reference to the current list to support cascaded calls
 */
template<class T>
List<T>& List<T>::RemoveLast()
{
    if(isEmpty() == false)
    {
        ListNode<T>* tempPtr = lastPtr;     // Save removing node addresss
        lastPtr = lastPtr->prevPtr;         // Update lastPtr
        delete tempPtr;                     // Delete saved lastPtr
        numberOfNodes--;                    // Decrement node count

        if(lastPtr != nullptr)
            lastPtr->nextPtr = nullptr;    // Remove nextNode connection
    }

    return *this;   // Support cascaded remove calls
}

/**
 * @brief   Removes all samples of a specific kind of data
 * @param   data    Value to be removed
 * @return  lValue reference to the list to support cascaded calls
 */
template<class T>
List<T>& List<T>::RemoveIf(const T& data)
{
    // Remove by starting from the first node
    return RemoveIf(data, firstPtr);
}

/**
 * @brief   Removes the first sample of given data.
 * @param   data Search key
 * @return  lValue reference to the list to support cascaded calls
 */
template<class T>
List<T>& List<T>::RemoveFirstOf(const T& data)
{
    RemoveNode(Find(data, firstPtr));   // Find and remove the first sample

    return *this;
}

/**
 * @brief   Removes the last sample of given data.
 * @param   data Search key
 * @return  lValue reference to the list to support cascaded calls
 */
template<class T>
List<T>& List<T>::RemoveLastOf(const T& data)
{
    RemoveNode(FindReversed(data, lastPtr));   // Find and remove the last sample

    return *this;
}

/**
 * @brief   Removes all samples not of a specific kind of data
 * @param   data    Value to be removed
 * @return  lValue reference to the list to support cascaded calls
 */
template<class T>
List<T>& List<T>::RemoveIfNot(const T& data)
{
    ListNode<T>* removingNode;      // Node to be removed
    ListNode<T>* searchStartPoint;  // Node where the search will start

    /* Find and remove all inequal nodes until
     * we hit the last of the list */
    removingNode = FindNotOf(data, firstPtr);    // Find the first inequal sample
    while(removingNode != nullptr)
    {
        searchStartPoint = removingNode->nextPtr;           // Save the next node
        RemoveNode(removingNode);                           // Remove the node found
        removingNode = FindNotOf(data, searchStartPoint);   // Find the next removing node
    }

    return *this;
}

/**
 * @brief   Removes the first sample not of given data.
 * @param   data Comparison key
 * @return  lValue reference to the list to support cascaded calls
 */
template<class T>
List<T>& List<T>::RemoveFirstNotOf(const T& data)
{
    // Find and remove the first sample not of given data
    RemoveNode(FindNotOf(data, firstPtr));

    return *this;
}

/**
 * @brief   Removes the last sample not of given data.
 * @param   data Comparison key
 * @return  lValue reference to the list to support cascaded calls
 */
template<class T>
List<T>& List<T>::RemoveLastNotOf(const T& data)
{
    // Find and remove the last sample not of given data
    RemoveNode(FindNotOfReversed(data, lastPtr));

    return *this;
}

/**
 * @brief   Removes all nodes
 * @return  lValue reference to the empty list to support cascaded calls
 */
template<class T>
List<T>& List<T>::EraseAll()
{
    /* Remove all until the list is empty */
    while(isEmpty() == false)
        RemoveFirst();
}

/**
 * @brief   Replaces all nodes containing the oldData with the newData
 * @param   oldData Data key to be replaced
 * @param   newData Replace value
 */
template<class T>
void List<T>::ReplaceAllWith(const T& oldData, const T& newData)
{
    ListNode<T>* currentNode = firstPtr;

    while(currentNode != nullptr)
    {
        currentNode = Find(oldData, currentNode);   // Find the key

        if(currentNode != nullptr)
        {
            currentNode->data   = newData;              // Replace data
            currentNode         = currentNode->nextPtr; // Pass to next node
        }
    }
}

/**
 * @brief   Replaces the first node containing the oldData with the newData
 * @param   oldData Data key to be replaced
 * @param   newData Replace value
 */
template<class T>
void List<T>::ReplaceFirstWith(const T& oldData, const T& newData)
{
    ListNode<T>* currentNode = Find(oldData, firstPtr);

    if(currentNode != nullptr)
        currentNode->data   = newData;              // Replace data
}

/**
 * @brief   Replaces the last node containing the oldData with the newData
 * @param   oldData Data key to be replaced
 * @param   newData Replace value
 */
template<class T>
void List<T>::ReplaceLastWith(const T& oldData, const T& newData)
{
    ListNode<T>* currentNode = FindReversed(oldData, lastPtr);

    if(currentNode != nullptr)
        currentNode->data   = newData;              // Replace data
}

/**
 * @brief   Swaps two different lists' contents.
 * @param   anotherList     List to be swapped with this.
 * @note    There is no need to make a complete swap.
 *          Interchancing the first and last nodes does the same.
 */
template<class T>
void List<T>::Swap(List<T>& anotherList)
{
    if(*this == anotherList)
        return;     // Self swap is not required

    ListNode<T>* tempPtr;
    size_t tempSize;

    // Swap the first nodes of each list
    tempPtr                 = firstPtr;             // Save the firstPtr of this
    firstPtr                = anotherList.firstPtr; // Replace the firstPtr of this
    anotherList.firstPtr    = tempPtr;              // Replace the firstPtr of the other list

    tempPtr                 = lastPtr;             // Save the lastPtr of this
    lastPtr                 = anotherList.lastPtr; // Replace the lastPtr of this
    anotherList.lastPtr     = tempPtr;             // Replace the lastPtr of the other list

    // Swap the size values
    tempSize                    = numberOfNodes;                // Save the size of this list
    numberOfNodes               = anotherList.numberOfNodes;    // Replace the size of this
    anotherList.numberOfNodes   = tempSize;                     // Replace the size of the other list
}

/**
 * @brief Resizes the list so that it contains n elements
 * @param newSize   New list size, expressed in number of elements
 * @param data      Object whose content is copied to the appended nodes
 */
template<class T>
void List<T>::Resize(const size_t newSize, const T& data)
{
    // Remove excessive nodes if exists
    while(newSize < GetNodeCount())
        RemoveLast();

    // Append new nodes if needed
    while(newSize > GetNodeCount())
        Append(data);
}

/**
 * @brief Removes all but the first element from every consecutive group of equal elements in the container.
 */
template<class T>
void List<T>::Unique()
{
    ListNode<T>* currentNode = firstPtr;

    while(currentNode != nullptr)
    {
        // Remove all duplicates of current node
        RemoveIf(currentNode->data, currentNode->nextPtr);

        // Do it for all nodes, each time the current node will become unique
        currentNode = currentNode->nextPtr;
    }
}

/**
 * @brief Sorts the elements with insertion sort.
 */
template<class T>
void List<T>::Sort()
{
    // At least two nodes required for sorting
    if((isEmpty() == true) || (firstPtr == lastPtr))
        return;

    ListNode<T> *minNode, *swapNode = firstPtr;
    while (swapNode != nullptr)
    {
        minNode = FindMinimum(swapNode);    // Find the minimum node of the list after (including)swap node
        SwapNodes(minNode, swapNode);       // Swap the minimum node and swapNode
        swapNode = minNode->nextPtr;        // Continue from the next node
    }
}

/**
 * @brief Prints the data of all nodes
 * @param stream    Output stream where the list will be inserted to.
 */
template<class T>
void List<T>::PrintAll(std::ostream& stream) const
{
    for(ListNode<T>* currentNode = firstPtr; currentNode != nullptr; currentNode = currentNode->nextPtr)
    {
        stream << currentNode->data << " ";
    }
}

/**
 * @brief   Merges two lists into a single list.
 * @param   anotherList List to be merged
 * @note    The second list will be completely flushed after this operation.
 */
template<class T>
void List<T>::Merge(List<T>& anotherList)
{
    // Both of the lists must be sorted before merging
    if(isSorted() == false)
        Sort(); // Sort first

    if(anotherList.isSorted() == false)
        anotherList.Sort(); // Sort first

    ListNode<T> *currentNodeL1 = firstPtr, *currentNodeL2 = anotherList.firstPtr;

    while(currentNodeL1 != nullptr)
    {
        currentNodeL2 = anotherList.firstPtr;
        if(currentNodeL1->data > currentNodeL2->data)
        {
            anotherList.DetachNode(currentNodeL2);
            Prepend(currentNodeL1, currentNodeL2);
        }
        else
            currentNodeL1 = currentNodeL1->nextPtr;
    }

    // Concatenate remaining elements of other list as they are already sorted
    if(anotherList.isEmpty() == false)
        Concatenate(anotherList);
}

/**
 * @brief   Concatenates another list to this one.
 * @param   anotherList List to be concatenated.
 */
template<class T>
void List<T>::Concatenate(List<T>& anotherList)
{
    if(anotherList.isEmpty() == true)
        return;

    if(isEmpty() == true)
        firstPtr = anotherList.firstPtr;

    // Link the first and last nodes
    anotherList.firstPtr->prevPtr = lastPtr;
    lastPtr->nextPtr = anotherList.firstPtr;

    // Update the lastPtr of current list
    lastPtr = anotherList.lastPtr;

    // Update the node counters
    numberOfNodes += anotherList.GetNodeCount();

    // Unlink the other list without destroying the content of it
    anotherList.firstPtr        = nullptr;
    anotherList.lastPtr         = nullptr;
    anotherList.numberOfNodes   = 0;
}

/**
 * @brief   Transfers elements from other list into this list by appending them at position.
 * @param   destination Position the append will occur.
 * @param   anotherList Source list. It will be completely flushed.
 */
template<class T>
void List<T>::Splice(const iterator& destination, List<T>& anotherList)
{
    if(destination.node == nullptr)
        throw std::logic_error("Iterator had been corrupted!");

    Append(destination.node, anotherList);
}

/**
 * @brief   Output insertion overloaded to be used with a list
 * @param   stream  Output stream where the list will be inserted to.
 * @param   list    List to be inserted.
 * @return  lValue reference to stream to support cascaded calls.
 */
template<class T>
std::ostream& operator<<(std::ostream& stream, List<T>& list)
{
    if((list.isEmpty() == true) || (list.firstPtr == nullptr))
        stream << "-- empty list --";
    else
        list.PrintAll(stream);

    return stream; // Support cascaded streams
}

/**
 * @brief   Finds the address of the first node where the specified data is contained at.
 * @param   data          Search key
 * @param   beginByNode   Search start by the given node
 * @return  Address of the first sample of given search key
 *          Returns nullptr if the data couldn't found.
 * @note    The algorithm used is the linear search as there are no value-based relation between nodes.
 */
template<class T>
ListNode<T>* List<T>::Find(const T& data, ListNode<T>* beginByNode)
{
    // Search begins by the given node
    ListNode<T>* currentNode = beginByNode;

    /* Break the search if the element is found
     * or the last element is hit */
    while(currentNode != nullptr)
    {
        if(currentNode->data == data)
            break;
        else
            currentNode = currentNode->nextPtr;
    }

    return currentNode;
}

/**
 * @brief   Finds the address of the first node where the data is not the specified one.
 * @param   data          Comparison key
 * @param   beginByNode   Search start by the given node
 * @return  Address of the first sample that is not of given search key
 *          Returns nullptr if the data couldn't found.
 * @note    The algorithm used is the linear search as there are no value-based relation between nodes.
 */
template<class T>
ListNode<T>* List<T>::FindNotOf(const T& data, ListNode<T>* beginByNode)
{
    // Search begins by the given node
    ListNode<T>* currentNode = beginByNode;

    /* Break the search if the element is found
     * or the last element is hit */
    while(currentNode != nullptr)
    {
        if(currentNode->data != data)   // Careful! It is the inequality operator
            break;
        else
            currentNode = currentNode->nextPtr;
    }

    return currentNode;
}

/**
 * @brief   Finds the address of the last node where the specified data is contained at.
 * @param   data          Search key
 * @param   beginByNode   Search start by the given node
 * @return  Address of the last sample of given search key
 *          Returns nullptr if the data couldn't found.
 * @note    The algorithm used is the reversed linear search as there are no value-based relation between nodes.
 */
template<class T>
ListNode<T>* List<T>::FindReversed(const T& data, ListNode<T>* beginByNode)
{
    // Search begins by the given node
    ListNode<T>* currentNode = beginByNode;

    /* Break the search if the element is found
     * or the first element is hit */
    while(currentNode != nullptr)
    {
        if(currentNode->data == data)
            break;
        else
            currentNode = currentNode->prevPtr; // Search advances reversely
    }

    return currentNode;
}

/**
 * @brief   Finds the address of the last node where the data is not the specified one.
 * @param   data          Comparison key
 * @param   beginByNode   Search start by the given node
 * @return  Address of the last sample that is not of given search key
 *          Returns nullptr if the data couldn't found.
 * @note    The algorithm used is the reversed linear search as there are no value-based relation between nodes.
 */
template<class T>
ListNode<T>* List<T>::FindNotOfReversed(const T& data, ListNode<T>* beginByNode)
{
    // Search begins by the given node
    ListNode<T>* currentNode = beginByNode;

    /* Break the search if the element is found
     * or the first element is hit */
    while(currentNode != nullptr)
    {
        if(currentNode->data != data)   // Careful! It is the inequality operator
            break;
        else
            currentNode = currentNode->prevPtr; // Search advances reversely
    }

    return currentNode;
}

/**
 * @brief   Finds the node with minimum data starting from a given node
 * @param   beginByNode Search starts from this node.s
 * @return  Address of the node with minimum data.
 * @throws  std::logic_error If the list is empty or the start node is undefined.
 */
template<class T>
ListNode<T>* List<T>::FindMinimum(ListNode<T>* beginByNode)
{
    // Check for exceptional situations
    if(beginByNode == nullptr)
        throw std::logic_error("Start node cannot be a nullptr!");
    else if(isEmpty() == true)
        throw std::logic_error("List is empty!");
    else{}

    ListNode<T>* currentNode = beginByNode->nextPtr, *minNode = beginByNode;

    while(currentNode != nullptr)
    {
        if(currentNode->data < minNode->data)
            minNode = currentNode;

        currentNode = currentNode->nextPtr;
    }

    return minNode;
}

/**
 * @brief   Removes a certain node from the list by not destroying the content of the node.s
 * @param   removingNode Address of the node to be removed.
 * @throw   std::logic_error If the list was empty.
 */
template<class T>
void List<T>::DetachNode(ListNode<T>* removingNode)
{
    if(isEmpty() == true)
        throw std::logic_error("Empty list cannot have any nodes!");

    // Re-link the neighbour nodes
    if(removingNode == firstPtr)
        firstPtr = removingNode->nextPtr;
    else
        removingNode->prevPtr->nextPtr = removingNode->nextPtr;

    if(removingNode == lastPtr)
        lastPtr = removingNode->prevPtr;
    else
        removingNode->nextPtr->prevPtr = removingNode->prevPtr;

    // Detach completely by removing links
    removingNode->nextPtr = nullptr;
    removingNode->prevPtr = nullptr;

    // Decrease node count of the list
    numberOfNodes--;
}

/**
 * @brief   Removes the given node.
 * @param   removingNode Address of the node to be removed.
 */
template<class T>
void List<T>::RemoveNode(ListNode<T>* removingNode)
{
    if(removingNode == nullptr)         // Return if the node is not valids
        return;
    else if(removingNode == firstPtr)   // First and last pointers are special
        RemoveFirst();                  // as there is no previous or next
    else if(removingNode == lastPtr)    // pointers for nodes. This affects
        RemoveLast();                   // the reconnection strategy.
    else    // If it is a random node
    {
        // Connect previous and next nodes
        removingNode->nextPtr->prevPtr = removingNode->prevPtr;
        removingNode->prevPtr->nextPtr = removingNode->nextPtr;

        delete removingNode;    // Delete the node
        numberOfNodes--;        // Decrement node counter
    }
}

/**
 * @brief   Removes all samples of given key.
 * @param   data        Search key
 * @param   beginByNode Node where the search will start from
 * @return  lValue reference to the list to support cascaded calls
 */
template<class T>
List<T>& List<T>::RemoveIf(const T& data, ListNode<T>* beginByNode)
{
    ListNode<T>* removingNode;      // Node to be removed
    ListNode<T>* searchStartPoint;  // Node where the search will start again from

    /* Find and remove all specified nodes until
     * we hit the last of the list */
    removingNode = Find(data, beginByNode);    // Find the first sample
    while(removingNode != nullptr)
    {
        searchStartPoint = removingNode->nextPtr;       // Save the next node
        RemoveNode(removingNode);                       // Remove the node found
        removingNode = Find(data, searchStartPoint);    // Find the next removing node
    }

    return *this;
}

/**
 * @brief   Swaps any kind of nodes.
 * @param   firstNode   Node to be swapped.
 * @param   secondNode  Node to be swapped.
 * @throws  std::logic_error If the nodes are undefined.
 */
template<class T>
void List<T>::SwapNodes(ListNode<T>* firstNode, ListNode<T>* secondNode)
{
    // Check for exceptional situations
    if((firstNode == nullptr) || (secondNode == nullptr))
        throw std::logic_error("Cannot swap undefined nodes!");
    else if (firstNode == secondNode)
        return; // No need to swap
    else{}

    // Successive nodes should be handled in a slightly different way
    if(firstNode->nextPtr == secondNode)
        SwapSuccessiveNodes(firstNode, secondNode);
    else if (firstNode->prevPtr == secondNode)
        SwapSuccessiveNodes(secondNode, firstNode);
    else
        SwapNonSuccessiveNodes(firstNode, secondNode);
}

/**
 * @brief   Swaps successive nodes.
 * @param   firstNode   Node to be swapped.
 * @param   secondNode  Node to be swapped.
 * @throws  std::logic_error If the nodes are not successively bounded.
 */
template<class T>
void List<T>::SwapSuccessiveNodes(ListNode<T>* firstNode, ListNode<T>* secondNode)
{
    // Check for exceptional situations
    if((firstNode->nextPtr != secondNode) || (secondNode->prevPtr != firstNode))
        throw std::logic_error("Nodes are not successive!");

    // Rebuild links between successive nodes
    secondNode->prevPtr = firstNode->prevPtr;
    firstNode->nextPtr  = secondNode->nextPtr;

    // Adjust the previous and next nodes
    // firstPtr and lastPtr may be modified
    if(firstNode != firstPtr)
        firstNode->prevPtr->nextPtr = secondNode;
    else
        firstPtr = secondNode;

    if(secondNode != lastPtr)
        secondNode->nextPtr->prevPtr = firstNode;
    else
        lastPtr = firstNode;

    firstNode->prevPtr  = secondNode;
    secondNode->nextPtr = firstNode;
}

/**
 * @brief   Swaps the nonsuccessive nodes between each other.
 * @param   firstNode   Node to be swapped.
 * @param   secondNode  Node to be swapped.
 * @throws  std::logic_error If any node is undefined.
 * @throws  std::logic_error If nodes are the same.
 */
template<class T>
void List<T>::SwapNonSuccessiveNodes(ListNode<T>* firstNode, ListNode<T>* secondNode)
{
    // Check for exceptional situations
    if((firstNode == nullptr) || (secondNode == nullptr))
        throw std::logic_error("Cannot swap undefined nodes!");
    else if (firstNode == secondNode)
        throw std::logic_error("Nodes must be different!");
    else if(firstNode->nextPtr == secondNode)
        return SwapSuccessiveNodes(firstNode, secondNode);
    else if(firstNode->prevPtr == secondNode)
        return SwapSuccessiveNodes(secondNode, firstNode);
    else{}

    ListNode<T>* tempPtr = nullptr;

    /** Update previos pointers **/
    if(firstNode == firstPtr)	// First one is the firstPtr
    {
        secondNode->prevPtr->nextPtr 	= firstNode;
        firstNode->prevPtr 				= secondNode->prevPtr;
        secondNode->prevPtr 			= nullptr;
        firstPtr                        = secondNode;	// Change global firstPtr
    }
    else
    {
        if(secondNode == firstPtr)	// Second one is the firstPtr
        {
            firstNode->prevPtr->nextPtr	= secondNode;
            secondNode->prevPtr			= firstNode->prevPtr;
            firstNode->prevPtr 			= nullptr;
            firstPtr                    = firstNode;	// Change global firstPtr
        }
        else	// Both of the nodes are not the edges of the list
        {
            firstNode->prevPtr->nextPtr 	= secondNode;
            secondNode->prevPtr->nextPtr 	= firstNode;
            tempPtr                         = secondNode->prevPtr;
            secondNode->prevPtr 			= firstNode->prevPtr;
            firstNode->prevPtr 				= tempPtr;
        }
    }

    /** Update next pointers **/
    if(firstNode == lastPtr)	// First one is the lastPtr
    {
        firstNode->nextPtr 				= secondNode->nextPtr;
        secondNode->nextPtr->prevPtr 	= firstNode;
        secondNode->nextPtr 			= nullptr;
        lastPtr                         = secondNode;	// Change global lastPtr
    }
    else
    {
        if(secondNode == lastPtr)	// Second one is the lastPtr
        {
            secondNode->nextPtr 		= firstNode->nextPtr;
            firstNode->nextPtr->prevPtr = secondNode;
            firstNode->nextPtr 			= nullptr;
            lastPtr                     = firstNode;	// Change the global lastPtr
        }
        else	// Both of the nodes are not the edges of the list
        {
            firstNode->nextPtr->prevPtr 	= secondNode;
            secondNode->nextPtr->prevPtr 	= firstNode;
            tempPtr                         = firstNode->nextPtr;
            firstNode->nextPtr 				= secondNode->nextPtr;
            secondNode->nextPtr 			= tempPtr;
        }
    }
}

/**
 * @brief   Appends a node to a given node of the list.
 * @param   baseNode    Node from the current list.
 * @param   newNode     Any node to be appended.
 * @throws  std::logic_error If any of the given nodes is NULL.
 */
template<class T>
void List<T>::Append(ListNode<T>* baseNode, ListNode<T>* newNode)
{
    if((baseNode == nullptr) || (newNode == nullptr))
        throw std::logic_error("Base node cannot be NULL while appending!");

    // Update lastPtr if needed
    if(baseNode == lastPtr)
        lastPtr = newNode;
    else
        baseNode->nextPtr->prevPtr = newNode;

    newNode->prevPtr    = baseNode;
    newNode->nextPtr    = baseNode->nextPtr;
    baseNode->nextPtr   = newNode;

    numberOfNodes++;    // Increment node count
}

/**
 * @brief   Prepends a node to a given node of the list.
 * @param   baseNode    Node from the current list.
 * @param   newNode     Any node to be prepended.
 * @throws  std::logic_error If any of the given nodes is NULL.
 */
template<class T>
void List<T>::Prepend(ListNode<T>* baseNode, ListNode<T>* newNode)
{
    if((baseNode == nullptr) || (newNode == nullptr))
        throw std::logic_error("Base node cannot be NULL while appending!");

    // Update lastPtr if needed
    if(baseNode == firstPtr)
        firstPtr = newNode;
    else
        baseNode->prevPtr->nextPtr = newNode;

    newNode->nextPtr    = baseNode;
    newNode->prevPtr    = baseNode->prevPtr;
    baseNode->prevPtr   = newNode;

    numberOfNodes++;    // Increment node count
}

/**
 * @brief   Appends a list to a node of the current list
 * @param   baseNode    Destination node
 * @param   anotherList List to appended to
 * @throw   std::logic_error If the destination node is NULL.
 */
template<class T>
void List<T>::Append(ListNode<T>* baseNode, List<T>& anotherList)
{
    if(baseNode == nullptr)
        throw std::logic_error("Base node cannot be NULL while appending!");

    if(baseNode == lastPtr)
        return Concatenate(anotherList);

    if(anotherList.isEmpty() == true)
        return;

    if(isEmpty() == true)
        return Swap(anotherList);

    // Adjust new links
    baseNode->nextPtr->prevPtr      = anotherList.lastPtr;
    anotherList.lastPtr->nextPtr    = baseNode->nextPtr;

    anotherList.firstPtr->prevPtr   = baseNode;
    baseNode->nextPtr               = anotherList.firstPtr;

    // Incremenet node counter
    numberOfNodes += anotherList.GetNodeCount();

    // Destroy old links of the other list
    anotherList.firstPtr    = nullptr;
    anotherList.lastPtr     = nullptr;
    anotherList.numberOfNodes = 0;

}

#endif  // Prevent recursive inclusion
