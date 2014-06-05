/** @file */
#ifndef __LINKEDLIST_H
#define __LINKEDLIST_H

#include "IndexOutOfBound.h"
#include "ElementNotExist.h"

using namespace std;

/**
 * A linked list.
 *
 * The iterator iterates in the order of the elements being loaded into this list.
 */
template <class T>
class LinkedList
{
	struct node {
		T data;
		node *pre,*next;
		node(T data, node *pre=NULL, node *next=NULL) : data(data), pre(pre), next(next) {}
	} *front, *rear;
	int Size;

public:
    class Iterator
    {
		LinkedList *a;
		node *pos, *last;
    public:
		Iterator(LinkedList<T> *x) {
			a=x;
			pos=NULL;
			last=NULL;
		}
        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {
			if (pos!=NULL&&pos->next!=NULL) {
			   return true;
			} else if (pos==NULL&&a->front!=NULL) {
				return true;
			} else {
				return false;
			}
		}

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const T &next() {
			if (!hasNext())	throw ElementNotExist("\nElement Not Exist\n");
			if (pos!=NULL) {
				pos=pos->next;
			} else {
				pos=a->front;
			}
			last=pos;
			return pos->data;
		}

        /**
         * TODO Removes from the underlying collection the last element
         * returned by the iterator
         * The behavior of an iterator is unspecified if the underlying
         * collection is modified while the iteration is in progress in
         * any way other than by calling this method.
         * @throw ElementNotExist
         */
        void remove() {
			if (last==NULL) throw ElementNotExist("\nElement Not Exist\n");
			if (last==a->front) {
				a->front=last->next;
			} else {
				last->pre->next=last->next;
			}
			if (last==a->rear) {
				a->rear=last->pre;
			} else {
				last->next->pre=last->pre;
			}
			pos=last->pre;
			delete last;
			last=NULL;
			--a->Size;
		}
    };

    /**
     * TODO Constructs an empty linked list
     */
    LinkedList () {
		front=rear=NULL;
		Size=0;
	}

    /**
     * TODO Copy constructor
     */
    LinkedList(const LinkedList &c) {
		front=NULL;
		rear=NULL;
		node *tmp=NULL;
		for (node *cur=c.front; cur!=NULL; cur=cur->next) {
			rear=new node(cur->data,tmp,NULL);
			if (tmp!=NULL) {
				tmp->next=rear;
			} else {
				front=rear;
			}
			tmp=rear;
		}
		Size=c.Size;
	}

    /**
     * TODO Assignment operator
     */
    LinkedList& operator=(const LinkedList &c) {
		if (this!=&c) {
			node *cur=front;
			while (cur!=NULL) {
				node *tmp=cur->next;
				delete cur;
				cur=tmp;
			}
			front=NULL;
			rear=NULL;
			node *tmp=NULL;
			for (node *cur=c.front; cur!=NULL; cur=cur->next) {
				rear=new node(cur->data,tmp,NULL);
				if (tmp!=NULL) {
					tmp->next=rear;
				} else {
					front=rear;
				}
				tmp=rear;
			}
			Size=c.Size;
		}
		return *this;
	}

    /**
     * TODO Desturctor
     */
    ~LinkedList() {
		node *cur=front;
		while (cur!=NULL) {
			node *tmp=cur->next;
			delete cur;
			cur=tmp;
		}
	}

    /**
     * TODO Appends the specified element to the end of this list.
     * Always returns true.
     */
    bool add(const T& e) {
		if (front==NULL) {
			front=rear=new node(e,NULL,NULL);
		} else {
			rear->next=new node(e,rear,NULL);
			rear=rear->next;
		}
		++Size;
		return true;
	}

    /**
     * TODO Inserts the specified element to the beginning of this list.
     */
    void addFirst(const T& elem) {
		if (rear==NULL) {
			front=rear=new node(elem,NULL,NULL);
		} else {
			front->pre=new node(elem,NULL,front);
			front=front->pre;
		}
		++Size;
	}

    /**
     * TODO Insert the specified element to the end of this list.
     * Equivalent to add.
     */
    void addLast(const T &elem) {
		if (front==NULL) {
			front=rear=new node(elem,NULL,NULL);
		} else {
			rear->next=new node(elem,rear,NULL);
			rear=rear->next;
		}
		++Size;
	}

    /**
     * TODO Inserts the specified element to the specified position in this list.
     * The range of index parameter is [0, size], where index=0 means inserting to the head,
     * and index=size means appending to the end.
     * @throw IndexOutOfBound
     */
    void add(int index, const T& element) {
		if (index<0||index>Size) {
			throw IndexOutOfBound("\nIndex Out Of Bound\n");
		} else if (Size==0) {
			front=rear=new node(element,NULL,NULL);
			++Size;
		} else if (index==0) {
			addFirst(element);
		} else if (index==Size) {
			addLast(element);
		} else {
			node *tmp=front;
			for (int i=1; i<index; ++i) {
				tmp=tmp->next;
			}
			node *cur=new node(element,tmp,tmp->next);
			tmp->next->pre=cur;
			tmp->next=cur;
			++Size;
		}
	}

    /**
     * TODO Removes all of the elements from this list.
     */
    void clear() {
		node *cur=front;
		while (cur!=NULL) {
			node *tmp=cur->next;
			delete cur;
			cur=tmp;
		}
		Size=0;
		front=rear=NULL;
	}

    /**
     * TODO Returns true if this list contains the specified element.
     */
    bool contains(const T& e) const {
		for (node *tmp=front; tmp!=NULL; tmp=tmp->next) {
			if (tmp->data==e) return true;
		}
		return false;
	}

    /**
     * TODO Returns a const reference to the element at the specified position in this list.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    const T& get(int index) const {
		if (index<0||index>=Size) throw IndexOutOfBound("\nIndex Out Of Bound\n");
		node *tmp=front;
		for (int i=0; i<index; ++i) tmp=tmp->next;
		return tmp->data;
	}

    /**
     * TODO Returns a const reference to the first element.
     * @throw ElementNotExist
     */
    const T& getFirst() const {
		if (front==NULL) throw ElementNotExist("\nElement Not Exist\n");
		return front->data;
	}

    /**
     * TODO Returns a const reference to the last element.
     * @throw ElementNotExist
     */
    const T& getLast() const {
		if (rear==NULL) throw ElementNotExist("\nElement Not Exist\n");
		return rear->data;
	}

    /**
     * TODO Returns true if this list contains no elements.
     */
    bool isEmpty() const {
		if (front==NULL) {
			return true;
		} else {
			return false;
		}
	}

    /**
     * TODO Removes the element at the specified position in this list.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    void removeIndex(int index) {
		if (index<0||index>=Size) throw IndexOutOfBound("\nIndex Out Of Bound\n");
		node *tmp=front;
		for (int i=0; i<index; ++i) tmp=tmp->next;
		if (tmp->pre!=NULL) {
			tmp->pre->next=tmp->next;
		} else {
			front=tmp->next;
		}
		if (tmp->next!=NULL) {
			tmp->next->pre=tmp->pre;
		} else {
			rear=tmp->pre;
		}
		delete tmp;
		--Size;
	}

    /**
     * TODO Removes the first occurrence of the specified element from this list, if it is present.
     * Returns true if it was present in the list, otherwise false.
     */
    bool remove(const T &e) {
		for (node *tmp=front; tmp!=NULL; tmp=tmp->next) {
			if (tmp->data==e) {
				if (tmp->pre!=NULL) {
					tmp->pre->next=tmp->next;
				} else {
					front=tmp->next;
				}
				if (tmp->next!=NULL) {
					tmp->next->pre=tmp->pre;
				} else {
					rear=tmp->pre;
				}
				delete tmp;
				--Size;
				return true;
			}
		}
		return false;
	}

    /**
     * TODO Removes the first element from this list.
     * @throw ElementNotExist
     */
    void removeFirst() {
		if (front==NULL) throw ElementNotExist("\nElement Not Exist\n");
		node *tmp=front;
		front=front->next;
		if (front!=NULL) {
			front->pre=NULL;
		} else {
			rear=NULL;
		}
		delete tmp;
		--Size;
	}

    /**
     * TODO Removes the last element from this list.
     * @throw ElementNotExist
     */
    void removeLast() {
		if (rear==NULL) throw ElementNotExist("\nElement Not Exist\n");
		node *tmp=rear;
		rear=rear->pre;
		if (rear!=NULL) {
			rear->next=NULL;
		} else {
			front=NULL;
		}
		delete tmp;
		--Size;
	}

    /**
     * TODO Replaces the element at the specified position in this list with the specified element.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    void set(int index, const T &element) {
		if (index<0||index>=Size) throw IndexOutOfBound("\nIndex Out Of Bound\n");
		node *tmp=front;
		for (int i=0; i<index; ++i) tmp=tmp->next;
		tmp->data=element;
	}

    /**
     * TODO Returns the number of elements in this list.
     */
    int size() const {
		return Size;
	}

    /**
     * TODO Returns an iterator over the elements in this list.
     */
    Iterator iterator() {
		return Iterator(this);
	}
};

#endif
