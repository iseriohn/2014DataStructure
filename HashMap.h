/** @file */

#ifndef __HASHMAP_H
#define __HASHMAP_H

#include "ElementNotExist.h"

/**
 * HashMap is a map implemented by hashing. Also, the 'capacity' here means the
 * number of buckets in your internal implemention, not the current number of the
 * elements.
 *
 * Template argument H are used to specify the hash function.
 * H should be a class with a static function named ``hashCode'',
 * which takes a parameter of type K and returns a value of type int.
 * For example, the following class
 * @code
 *      class Hashint {
 *      public:
 *          static int hashCode(int obj) {
 *              return obj;
 *          }
 *      };
 * @endcode
 * specifies an hash function for integers. Then we can define:
 * @code
 *      HashMap<int, int, Hashint> hash;
 * @endcode
 *
 * Hash function passed to this class should observe the following rule: if two keys
 * are equal (which means key1 == key2), then the hash code of them should be the
 * same. However, it is not generally required that the hash function should work in
 * the other direction: if the hash code of two keys are equal, the two keys could be
 * different.
 *
 * Note that the correctness of HashMap should not rely on the choice of hash function.
 * This is to say that, even the given hash function always returns the same hash code
 * for all keys (thus causing a serious collision), methods of HashMap should still
 * function correctly, though the performance will be poor in this case.
 *
 * The order of iteration could be arbitary in HashMap. But it should be guaranteed
 * that each (key, value) pair be iterated exactly once.
 */
template <class K, class V, class H>
class HashMap
{
public:
    class Entry
    {
        K key;
        V value;
    public:
        Entry(K k, V v)
        {
            key = k;
            value = v;
        }

        K getKey() const
        {
            return key;
        }

        V getValue() const
        {
            return value;
        }

		V &GetValue() {
			return value;
		}
    };

	class node {
	public:
		Entry data;
		node *next;
		node() {}
		node(Entry data, node *next=NULL) : data(data), next(next) {}
	};
	const static int Omod=97;
	const static double frac=0.80;
	int Mod;
	int capa, Size;
	node **elem;

	int hash(const K &key) const {
		return (H::hashCode(key)%Mod+Mod)%Mod;
	}

	void rehash() {
		node **e=elem;
		int tmod=Mod;
		Mod*=2;
		elem=new node *[Mod];
		capa=Mod*frac;
		for (int i=0; i<Mod; ++i) elem[i]=NULL;
		for (int i=0; i<tmod; ++i) {
			for (node *tmp=e[i]; tmp!=NULL; tmp=tmp->next) {
				int index=hash(tmp->data.getKey());
				node *cur=new node(tmp->data,elem[index]);
				elem[index]=cur;
			}
		}
		for (int i=0; i<tmod; ++i) {
			node *cur=e[i];
			while (cur!=NULL) {
				node *tmp=cur->next;
				delete cur;
				cur=tmp;
			}
		}
		delete []e;
	}

    class Iterator
    {
		node **elem;
		node *cur;
		int idx,Mod;

    public:
		Iterator(node **elem, int Mod) : elem(elem), Mod(Mod) {
			cur=NULL;
			idx=-1;
		}


        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {
			if (cur==NULL||cur->next==NULL) {
				int i=idx+1;
				while (i<Mod) {
					if (elem[i]!=NULL) return 1;
					++i;
				}
			} else return 1;
			return 0;
		}

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const Entry &next() {
			if (!hasNext()) throw ElementNotExist("\nElement Not Exist\n");
			if (cur==NULL||cur->next==NULL) {
				++idx;
				while (idx<Mod) {
					if (elem[idx]!=NULL) {
						cur=elem[idx];
						return cur->data;
					}
					++idx;
				}
			} else {
				cur=cur->next;
			}
			return cur->data;
		}
    };

    /**
     * TODO Constructs an empty hash map.
     */
    HashMap() {
		Mod=Omod;
		capa=Mod*frac;
		Size=0;
		elem=new node *[Mod];
		for (int i=0; i<Mod; ++i) {
			elem[i]=NULL;
		}
	}

    /**
     * TODO Destructor
     */
    ~HashMap() {
		for (int i=0; i<Mod; ++i) {
			node *cur=elem[i];
			while (cur!=NULL) {
				node *tmp=cur->next;
				delete cur;
				cur=tmp;
			}
		}
		delete []elem;
	}

    /**
     * TODO Assignment operator
     */
    HashMap &operator=(const HashMap &x) {
		for (int i=0; i<Mod; ++i) {
			node *cur=elem[i];
			while (cur!=NULL) {
				node *tmp=cur->next;
				delete cur;
				cur=tmp;
			}
		}
		delete []elem;
		Mod=x.Mod;
		capa=x.capa;
		Size=x.Size;
		elem=new node *[Mod];
		for (int i=0; i<Mod; ++i) {
			elem[i]=NULL;
			node *cur=elem[i];
			for (node *tmp=x.elem[i]; tmp!=NULL; tmp=tmp->next) {
				cur=new node(tmp->data,elem[i]);
				elem[i]=cur;
			}
		}
		return *this;
	}

    /**
     * TODO Copy-constructor
     */
    HashMap(const HashMap &x) {
		Mod=x.Mod;
		capa=x.capa;
		Size=x.Size;
		elem=new node *[Mod];
		for (int i=0; i<Mod; ++i) {
			elem[i]=NULL;
			node *cur=elem[i];
			for (node *tmp=x.elem[i]; tmp!=NULL; tmp=tmp->next) {
				cur=new node(tmp->data,elem[i]);
				elem[i]=cur;
			}
		}
	}

    /**
     * TODO Returns an iterator over the elements in this map.
     */
    Iterator iterator() const {
		return Iterator(elem,Mod);
	}

    /**
     * TODO Removes all of the mappings from this map.
     */
    void clear() {
		for (int i=0; i<Mod; ++i) {
			node *cur=elem[i];
			while (cur!=NULL) {
				node *tmp=cur->next;
				delete cur;
				cur=tmp;
			}
		}
		delete []elem;
		Mod=Omod;
		capa=Mod*frac;
		Size=0;
		elem=new node *[Mod];
		for (int i=0; i<Mod; ++i) {
			elem[i]=NULL;
		}
	}

    /**
     * TODO Returns true if this map contains a mapping for the specified key.
     */
    bool containsKey(const K &key) const {
		int index=hash(key);
		for (node *tmp=elem[index]; tmp!=NULL; tmp=tmp->next) {
			if (tmp->data.getKey()==key) return true;
		}
		return false;
	}

    /**
     * TODO Returns true if this map maps one or more keys to the specified value.
     */
    bool containsValue(const V &value) const {
		for (int i=0; i<Mod; ++i) {
			for (node *tmp=elem[i]; tmp!=NULL; tmp=tmp->next) {
				if (tmp->data.getValue()==value) return true;
			}
		} 
		return false;
	}

    /**
     * TODO Returns a const reference to the value to which the specified key is mapped.
     * If the key is not present in this map, this function should throw ElementNotExist exception.
     * @throw ElementNotExist
     */
    const V &get(const K &key) const {
		if (!containsKey(key)) throw ElementNotExist("\nElement Not Exist\n");
		int index=hash(key);
		node *tmp=elem[index];
		for (; tmp!=NULL; tmp=tmp->next) {
			if (tmp->data.getKey()==key) return tmp->data.GetValue();
		}
		return tmp->data.GetValue();
	}

    /**
     * TODO Returns true if this map contains no key-value mappings.
     */
    bool isEmpty() const {
		if (Size==0) return 1;
		return 0;
	}

    /**
     * TODO Associates the specified value with the specified key in this map.
     */
    void put(const K &key, const V &value) {
		int index=hash(key);
		for (node *tmp=elem[index]; tmp!=NULL; tmp=tmp->next) {
			if (tmp->data.getKey()==key) {
				tmp->data=Entry(key,value);
				return;
			}
		}
		node *tmp=new node(Entry(key,value),elem[index]);
		elem[index]=tmp;
		++Size;
		if (Size>capa) rehash();
	}

    /**
     * TODO Removes the mapping for the specified key from this map if present.
     * If there is no mapping for the specified key, throws ElementNotExist exception.
     * @throw ElementNotExist
     */
    void remove(const K &key) {
		int index=hash(key);
		if (!containsKey(key)) throw ElementNotExist("\nElement Not Exist\n");
		node *last=NULL;
		for (node *tmp=elem[index]; tmp!=NULL; last=tmp, tmp=tmp->next) {
			if (tmp->data.getKey()==key) {
				if (last!=NULL) {
					last->next=tmp->next;
				} else {
					elem[index]=tmp->next;
				}
				--Size;
				delete tmp;
				return;
			}
		}
	}

    /**
     * TODO Returns the number of key-value mappings in this map.
     */
    int size() const {
		return Size;
	}
};

#endif
