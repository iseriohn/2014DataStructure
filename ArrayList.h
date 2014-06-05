/** @file */
#ifndef __ARRAYLIST_H
#define __ARRAYLIST_H

#include "IndexOutOfBound.h"
#include "ElementNotExist.h"

/**
 * The ArrayList is just like vector in C++.
 * You should know that "capacity" here doesn't mean how many elements are now in this list, where it means
 * the length of the array of your internal implemention
 *
 * The iterator iterates in the order of the elements being loaded into this list
 */
template <class T>
class ArrayList
{
public:
	T *elem;
	int Size,capa;
	
	void doubleCapacity() {
		T *tmp=elem;
		elem=new T[capa<<1];
		for (int i=0; i<Size; ++i) elem[i]=tmp[i];
		capa<<=1;
		delete []tmp;
	}
		
    class Iterator
    {
		ArrayList<T> *a;
		int pos,last;

		public:
		Iterator(ArrayList<T> *x) {
			a=x;
			pos=-1;
			last=-1;
		}
        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {
			return pos+1<a->Size;
		}

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const T &next() {
			if (hasNext()) {
				++pos;
				last=pos;
				return a->elem[pos];
			} else {
				throw ElementNotExist("\nElement Not Exist\n");
			}
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
			if (last>=a->Size||last<0) throw ElementNotExist("\nElement Not Exist\n");
			a->removeIndex(last);
			--pos;
			last=-1;
		}
    };

    /**
     * TODO Constructs an empty array list.
     */
    ArrayList() {
		Size=0;
		capa=1;
		elem=new T[capa];
	}

    /**
     * TODO Destructor
     */
    ~ArrayList() {
		delete []elem;
	}

    /**
     * TODO Assignment operator
     */
    ArrayList& operator=(const ArrayList<T> &x) {
		if (this!=&x) {
			delete []elem;
			Size=x.Size;
			capa=x.capa;
			elem=new T[capa];
			for (int i=0; i<Size; ++i) elem[i]=x.elem[i];
		}
		return *this;
	}

    /**
     * TODO Copy-constructor
     */
    ArrayList(const ArrayList<T> &x) {
		Size=x.Size;
		capa=x.capa;
		elem=new T[capa];
		for (int i=0; i<Size; ++i) elem[i]=x.elem[i];
	}

    /**
     * TODO Appends the specified element to the end of this list.
     * Always returns true.
     */
    bool add(const T& e) {
		if (Size==capa) doubleCapacity();
		elem[Size++]=e;
		return true;
	}

    /**
     * TODO Inserts the specified element to the specified position in this list.
     * The range of index parameter is [0, size], where index=0 means inserting to the head,
     * and index=size means appending to the end.
     * @throw IndexOutOfBound
     */
    void add(int index, const T& element) {
		if (index>Size||index<0) throw IndexOutOfBound("\nIndex Out Of Bound\n");
		if (Size==capa) doubleCapacity();
		for (int i=Size; i>index; --i) elem[i]=elem[i-1];
		elem[index]=element;
		++Size;
	}

    /**
     * TODO Removes all of the elements from this list.
     */
    void clear() {
		delete []elem;
		Size=0;
		capa=1;
		elem=new T[capa];
	}

    /**
     * TODO Returns true if this list contains the specified element.
     */
    bool contains(const T& e) const {
		for (int i=0; i<Size; ++i) if (elem[i]==e) return true;
		return false;
	}

    /**
     * TODO Returns a const reference to the element at the specified position in this list.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    const T& get(int index) const {
		if (index>=Size||index<0) throw IndexOutOfBound("\nIndex Out Of Bound\n");
		return elem[index];
	}

    /**
     * TODO Returns true if this list contains no elements.
     */
    bool isEmpty() const {
		return Size==0;
	}

    /**
     * TODO Removes the element at the specified position in this list.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    void removeIndex(int index) {
		if (index>=Size||index<0) throw IndexOutOfBound("\nIndex Out Of Bound\n");
		--Size;
		for (int i=index; i<Size; ++i) elem[i]=elem[i+1];
	}

    /**
     * TODO Removes the first occurrence of the specified element from this list, if it is present.
     * Returns true if it was present in the list, otherwise false.
     */
    bool remove(const T &e) {
		for (int i=0; i<Size; ++i) {
			if (elem[i]==e) {
				removeIndex(i);
				return true;
			}
		}
		return false;
	}

    /**
     * TODO Replaces the element at the specified position in this list with the specified element.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    void set(int index, const T &element) {
		if (index>=Size||index<0) throw IndexOutOfBound("\nIndex Out Of Bound\n");
		elem[index]=element;
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
