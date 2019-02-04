/********************************/
/*   Aryeh Nigri	338017866	*/
/*   Zeev Noiman	937574		*/
/********************************/

#ifndef PQUEUE_H
#define PQUEUE_H

#include <stdio.h>

// ==================== Classes Declaration ====================
#pragma region Declaration
struct Node
{
    int data;
    Node *next;
    Node();
    ~Node();
};

class PriorityQueue
{
  private:
    Node *_front;
    int _size;

  public:
    PriorityQueue();
    ~PriorityQueue();
    int empty(); //Returns whether the queue is empty.
    int size();   //Returns the size of the queue.
    int front();
    void push(int element); //push() function adds the element ‘g’ at the end of the queue
    int pop();              // pop() function deletes and returns the first element of the queue.
    void print();
};
#pragma endregion
// ==================== End Declaration ====================

// ==================== Classes Implementation ====================
#pragma region Implementation
Node::Node(){
    data = 0;
    next = NULL;
}

Node::~Node(){
    delete next;
}

PriorityQueue::PriorityQueue()
{
    _size = 0;
    _front = NULL;
}
PriorityQueue::~PriorityQueue()
{
    delete _front;
}
int PriorityQueue::empty()
{
    return (_size == 0);
}
int PriorityQueue::size()
{
    return _size;
}
int PriorityQueue::front()
{
    if (!empty())
    {
        return _front->data;
    }
    return -1;
}
void PriorityQueue::push(int element)
{
    if (empty())
    {
        _front = new Node();
        _front->data = element;
        _front->next = NULL;
    }
    else
    {
        Node *previous = _front;
        while (previous->data > element)
        {
            if (previous->next && previous->next->data > element)
                previous = previous->next;
            else
                break;
        }
        Node *newVal = new Node();
        newVal->data = element;
        if (previous->data > element)
        {
            if (previous->next)
            {
                newVal->next = previous->next;
                previous->next = newVal;
            }
            else // last one
            {
                newVal->next = NULL;
                previous->next = newVal;
            }
        }
        else // element higher than _front
        {
            newVal->next = previous; // newVal->next = _front;
            _front = newVal;
        }
    }
    _size++;
}
int PriorityQueue::pop()
{
    if (!empty())
    {
        Node *tmp = _front;
        _front = _front->next;
        _size--;
        return tmp->data;
    }
    return -1;
}
void PriorityQueue::print()
{
    printf("Priority Queue: ");
    Node *it = _front;
    while (it)
    {
        printf("%i\t", it->data);
        it = it->next;
    }
    printf("\n\r");
}

#pragma endregion
// ==================== End Implementation ====================


#endif // PQUEUE_H