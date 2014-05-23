/** @file */
#ifndef __DEQUE_H
#define __DEQUE_H

#include "ElementNotExist.h"
#include "IndexOutOfBound.h"

/**
 * An deque is a linear collection that supports element insertion and removal at both ends.
 * The name deque is short for "double ended queue" and is usually pronounced "deck".
 * Remember: all functions but "contains" and "clear" should be finished in O(1) time.
 *
 * You need to implement both iterators in proper sequential order and ones in reverse sequential order. 
 */
template <class T>
class Deque
{
	T *elem;
	int Size,capa,head,tail;

	int mod(const int &a, const int &b) const {
		return (a%b+b)%b;
	}

	void doubleCapacity() {
		T *tmp=elem;
		elem=new T[capa<<1];
		for (int i=head; i<=tail; ++i) elem[mod(i,capa<<1)]=tmp[mod(i,capa)];
		capa<<=1;
		delete []tmp;
	}

public:
    class Iterator
    {
		Deque<T> *a;
		int pos,last,idx;

		int mod(const int &a, const int &b) const {
			return (a%b+b)%b;
		}

    public:
		Iterator(Deque<T> *x, int k) {
			a=x;
			idx=k;
			last=-1;
			if (k==1) {
				pos=a->head-1;
			} else {
				pos=a->tail+1;
			}
		}
        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {
			if (idx==1) {
				return pos<a->tail;
			} else {
				return pos>a->head;
			}
		}

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const T &next() {
			if (!hasNext()) throw ElementNotExist("\nElement Not Exist\n");
			pos+=idx;
			last=pos;
			return a->elem[mod(pos,a->capa)];
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
			if (last==-1) throw ElementNotExist("\nElement Not Exist\n");
			if (idx==1) {
				for (int i=last; i<a->tail; ++i) {
					a->elem[mod(i,a->capa)]=a->elem[mod(i+1,a->capa)];
				}
				--a->tail;
			} else {
				for (int i=last; i>a->head; --i) {
					a->elem[mod(i,a->capa)]=a->elem[mod(i-1,a->capa)];
				}
				++a->head;
			}
			--a->Size;
			pos-=idx;
			last=-1;
		}
    };

    /**
     * TODO Constructs an empty deque.
     */
    Deque() {
		Size=0;
		capa=1;
		head=0;
		tail=-1;
		elem=new T[capa];
	}

    /**
     * TODO Destructor
     */
    ~Deque () {
		delete []elem;
	}

    /**
     * TODO Assignment operator
     */
    Deque& operator=(const Deque<T> & x) {
		delete []elem;
		Size=x.Size;
		capa=x.capa;
		head=x.head;
		tail=x.tail;
		elem=new T[capa];
		for (int i=head; i<=tail; ++i) elem[mod(i,capa)]=x.elem[mod(i,capa)];
		return *this;
	}

    /**
     * TODO Copy-constructor
     */
    Deque(const Deque<T>& x) {
		Size=x.Size;
		capa=x.capa;
		head=x.head;
		tail=x.tail;
		elem=new T[capa];
		for (int i=head; i<=tail; ++i) elem[mod(i,capa)]=x.elem[mod(i,capa)];
	}
	
	/**
	 * TODO Inserts the specified element at the front of this deque. 
	 */
	void addFirst(const T& e) {
		if (Size==capa) doubleCapacity();
		--head;
		++Size;
		elem[mod(head,capa)]=e;
	}

	/**
	 * TODO Inserts the specified element at the end of this deque.
	 */
	void addLast(const T& e) {
		if (Size==capa) doubleCapacity();
		++tail;
		++Size;
		elem[mod(tail,capa)]=e;
	}

	/**
	 * TODO Returns true if this deque contains the specified element.
	 */
	bool contains(const T& e) const {
		for (int i=head; i<=tail; ++i) {
			if (elem[mod(i,capa)]==e) return true;
		}
		return false;
	}

	/**
	 * TODO Removes all of the elements from this deque.
	 */
	 void clear() {
		delete []elem;
		Size=0;
		capa=1;
		head=0;
		tail=-1;
		elem=new T[capa];
	 }

	 /**
	  * TODO Returns true if this deque contains no elements.
	  */
	bool isEmpty() const {
		return Size==0;
	}

	/**
	 * TODO Retrieves, but does not remove, the first element of this deque.
	 * @throw ElementNotExist
	 */
	 const T& getFirst() {
		if (head>tail) throw ElementNotExist("\nElement Not Exist\n");
		return elem[mod(head,capa)];
	 }

	 /**
	  * TODO Retrieves, but does not remove, the last element of this deque.
	  * @throw ElementNotExist
	  */
	 const T& getLast() {
		if (head>tail) throw ElementNotExist("\nElement Not Exist\n");
		return elem[mod(tail,capa)];
	 }

	 /**
	  * TODO Removes the first element of this deque.
	  * @throw ElementNotExist
	  */
	void removeFirst() {
		if (head>tail) throw ElementNotExist("\nElement Not Exist\n");
		++head;
		--Size;
	}

	/**
	 * TODO Removes the last element of this deque.
	 * @throw ElementNotExist
	 */
	void removeLast() {
		if (head>tail) throw ElementNotExist("\nElement Not Exist\n");
		--tail;
		--Size;
	}

	/**
	 * TODO Returns a const reference to the element at the specified position in this deque.
	 * The index is zero-based, with range [0, size).
	 * @throw IndexOutOfBound
	 */
	const T& get(int index) const {
		if (index<0||index>=Size) throw IndexOutOfBound("\nIndex Out Of Bound\n");
		return elem[mod(index+head,capa)];
	}
	
	/**
	 * TODO Replaces the element at the specified position in this deque with the specified element.
	 * The index is zero-based, with range [0, size).
	 * @throw IndexOutOfBound
	 */
	void set(int index, const T& e) {
		if (index<0||index>=Size) throw IndexOutOfBound("\nIndex Out Of Bound\n");
		elem[mod(index+head,capa)]=e;
	}

	/**
	 * TODO Returns the number of elements in this deque.
	 */
	 int size() const {
		return Size;
	 }

	 /**
	  * TODO Returns an iterator over the elements in this deque in proper sequence.
	  */
	 Iterator iterator() {
		return Iterator(this,1);
	 }

	 /**
	  * TODO Returns an iterator over the elements in this deque in reverse sequential order.
	  */
	 Iterator descendingIterator() {
		return Iterator(this,-1);
	 }
};

#endif
