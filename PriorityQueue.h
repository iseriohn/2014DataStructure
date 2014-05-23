/** @file */
#ifndef __PRIORITYQUEUE_H
#define __PRIORITYQUEUE_H

#include "ArrayList.h"
#include "ElementNotExist.h"
#include <algorithm>

/**
* This is a priority queue based on a priority priority queue. The
* elements of the priority queue are ordered according to their
* natual ordering (operator<), or by a Comparator provided as the
* second template parameter.
* The head of this queue is the least element with respect to the
* specified ordering (different from C++ STL).
* The iterator does not return the elements in any particular order.
* But it is required that the iterator will eventually return every
* element in this queue (even if removals are performed).
*/

/*----------------------------------------------------------------------*/
/**
* Default Comparator with respect to natural order (operator<).
*/
template <class V>
class Less
{
public:
    bool operator()(const V& a, const V& b) { return a < b; }
};

/**
* To use this priority queue, users need to either use the
* default Comparator or provide their own Comparator of this
* kind.
* The Comparator should be a class with a public function
* public: bool operator()(const V&, const V&);
* overriding the "()" operator.
* The following code may help you understand how to use a
* Comparator and, especially, why we override operator().
*/

// #include <iostream>
// using namespace std;
//
// template <class T, class C = Less<T> >
// class Example
// {
// private:
// C cmp;
// public:
// bool compare(const T& a, const T& b)
// {
// return cmp(a, b);
// }
// };
//
// int main()
// {
// Example<int, Less<int> > example; // Less<int> can be omitted.
// cout << example.compare(1, 2) << endl;
// cout << example.compare(2, 1) << endl;
// }

/*----------------------------------------------------------------------*/

template <class V, class C = Less<V> >
class PriorityQueue
{
	ArrayList<V> data;
	C cmp;

public:
    class Iterator
    {
		PriorityQueue<V,C> *a;
		int pos, last;
    public:
		Iterator(PriorityQueue<V,C> *x) {
		   a=x;
		   pos=-1;
		   last=-1;
		}

        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {
			return pos+1<a->data.Size;
		}

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const V &next() {
			if (!hasNext()) throw ElementNotExist("\nElement Not Exist\n");
			++pos;
			last=pos;
			return a->data.elem[pos];
		}

		/**
		 * TODO Removes from the underlying collection the last element
		 * returned by the iterator.
		 * The behavior of an iterator is unspecified if the underlying
		 * collection is modified while the iteration is in progress in
		 * any way other than by calling this method.
		 * @throw ElementNotExist
		 */
		void remove() {
			if (last==-1) throw ElementNotExist("\nElement Not Exist\n");
			int i=last;
			while (i*2+1<a->data.Size) {
				if (i*2+2>=a->data.Size||a->cmp(a->data.elem[i*2+1],a->data.elem[i*2+2])) {
					swap(a->data.elem[i],a->data.elem[i*2+1]);
					i=i*2+1;
				} else {
					swap(a->data.elem[i],a->data.elem[i*2+2]);
					i=i*2+2;
				}
			}
			swap(a->data.elem[i],a->data.elem[a->data.Size-1]);
			if (i!=a->data.Size-1) {
				while (i) {
					if (a->cmp(a->data.elem[i],a->data.elem[(i-1)>>1])) {
						swap(a->data.elem[i],a->data.elem[(i-1)>>1]);
						i=(i-1)/2;
					} else break;
				}
			}
			--a->data.Size;
			last=-1;
			--pos;
		}
    };

    /**
     * TODO Constructs an empty priority queue.
     */
    PriorityQueue() {
		data=ArrayList<V>();
	}

    /**
     * TODO Destructor
     */
    ~PriorityQueue() {
		data.clear();
	}

    /**
     * TODO Assignment operator
     */
    PriorityQueue &operator=(const PriorityQueue &x) {
		data.clear();
		data=x.data;
		return *this;
	}

    /**
     * TODO Copy-constructor
     */
    PriorityQueue(const PriorityQueue &x) {
		data=x.data;
	}

	/**
	 * TODO Initializer_list-constructor
	 * Constructs a priority queue over the elements in this Array List.
     * Requires to finish in O(n) time.
	 */
	PriorityQueue(const ArrayList<V> &x) {
		data=x;
		for (int i=data.Size/2; i>=0; --i) {
			int j=i;
			while (j*2+1<data.Size) {
				if (j*2+2<data.Size&&cmp(data.elem[j*2+2],data.elem[j])&&cmp(data.elem[j*2+2],data.elem[j*2+1])) {
					swap(data.elem[j],data.elem[j*2+2]);
					j=j*2+2;
				} else if (cmp(data.elem[j*2+1],data.elem[j])) {
					swap(data.elem[j],data.elem[j*2+1]);
					j=j*2+1;
				} else break;
			}
		}
	}

    /**
     * TODO Returns an iterator over the elements in this priority queue.
     */
    Iterator iterator() {
		return Iterator(this);
	}

    /**
     * TODO Removes all of the elements from this priority queue.
     */
    void clear() {
		data.clear();
	}

    /**
     * TODO Returns a const reference to the front of the priority queue.
     * If there are no elements, this function should throw ElementNotExist exception.
     * @throw ElementNotExist
     */
    const V &front() const {
		if (data.Size==0) throw ElementNotExist("\nElement Not Exist\n");
		return data.elem[0];
	}

    /**
     * TODO Returns true if this PriorityQueue contains no elements.
     */
    bool empty() const {
		return data.Size==0;
	}

    /**
     * TODO Add an element to the priority queue.
     */
    void push(const V &value) {
		data.add(data.Size,value);
		int i=data.Size-1;
		while (i) {
			if (cmp(data.elem[i],data.elem[(i-1)>>1])) {
				swap(data.elem[i],data.elem[(i-1)>>1]);
			} else break;
		}
	}

    /**
     * TODO Removes the top element of this priority queue if present.
     * If there is no element, throws ElementNotExist exception.
     * @throw ElementNotExist
     */
    void pop() {
		if (data.Size==0) throw ElementNotExist("\nElement Not Exist\n");
		int i=0;
		while (i*2+1<data.Size) {
			if (i*2+2>=data.Size||cmp(data.elem[i*2+1],data.elem[i*2+2])) {
				swap(data.elem[i],data.elem[i*2+1]);
				i=i*2+1;
			} else {
				swap(data.elem[i],data.elem[i*2+2]);
				i=i*2+2;
			}
		}
		swap(data.elem[i],data.elem[data.Size-1]);
		if (i!=data.Size-1) {
			while (i) {
				if (cmp(data.elem[i],data.elem[(i-1)>>1])) {
					swap(data.elem[i],data.elem[(i-1)>>1]);
					i=(i-1)/2;
				} else break;
			}
		}
		--data.Size;
	}

    /**
     * TODO Returns the number of key-value mappings in this map.
     */
    int size() const {
		return data.Size;
	}
};

#endif
