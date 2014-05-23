/** @file */
#ifndef __TREEMAP_H
#define __TREEMAP_H

#include "ElementNotExist.h"

/**
 * TreeMap is the balanced-tree implementation of map. The iterators must
 * iterate through the map in the natural order (operator<) of the key.
 */
template<class K, class V>
class TreeMap
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

	struct node {
		node *son[2], *fa;
		Entry data;
		int tag;
		node (node *fa, Entry data, int tag) : fa(fa), data(data), tag(tag) {
			son[0]=son[1]=NULL;
		}
	} *root;
	int Size;

	int child(node *cur) {
		return cur->fa->son[0]!=cur;
	}

	void rotate(node *cur) {
		int idx=child(cur);
		node *tmp=cur->fa;
		tmp->son[idx]=cur->son[1^idx];
		if (cur->son[1^idx]!=NULL) cur->son[1^idx]->fa=tmp;
		cur->fa=tmp->fa;
		if (tmp->fa!=NULL) {
			int k=child(tmp);
			tmp->fa->son[k]=cur;
		} else {
			root=cur;
		}
		tmp->fa=cur;
		cur->son[1^idx]=tmp;
	}

	void clean(node *cur) {
		if (cur->son[0]!=NULL) clean(cur->son[0]);
		if (cur->son[1]!=NULL) clean(cur->son[1]);
		delete cur;
	}

	void dfs(node *cur, node *x) {
		node *tmp;
		if (cur==NULL) {
			tmp=new node(NULL,x->data,x->tag);
			root=tmp;
		} else {
			tmp=new node(cur,x->data,x->tag);
			cur->son[child(x)]=tmp;
		}
		if (x->son[0]!=NULL) dfs(tmp,x->son[0]);
		if (x->son[1]!=NULL) dfs(tmp,x->son[1]);
	}

	bool find(node *cur, const K &key) const {
		if (cur==NULL) return 0;
		if (cur->data.getKey()==key) return 1;
		if (cur->data.getKey()<key) {
			return find(cur->son[1],key);
		} else {
			return find(cur->son[0],key);
		}
	}

	bool find(node *cur, const V &value) const {
		if (cur==NULL) return 0;
		if (cur->data.getValue()==value) return 1;
		return find(cur->son[0],value)|find(cur->son[1],value);
	}

	V &keyValue(node *cur, K key) const {
		if (cur->data.getKey()==key) return cur->data.GetValue();
		if (key<cur->data.getKey()) {
			return keyValue(cur->son[0],key);
		} else {
			return keyValue(cur->son[1],key);
		}
	}

	void insert(node *cur, Entry x) {
		node *tmp=NULL;
		if (cur==NULL) {
			tmp=new node(NULL,x,rand());
			root=tmp;
		} else {
			if (cur->data.getKey()>x.getKey()) {
				if (cur->son[0]==NULL) {
					tmp=new node(cur,x,rand());
					cur->son[0]=tmp;
				} else {
					insert(cur->son[0],x);
				}
			} else if (cur->data.getKey()<x.getKey()) {
				if (cur->son[1]==NULL) {
					tmp=new node(cur,x,rand());
					cur->son[1]=tmp;
				} else {
					insert(cur->son[1],x);
				}
			} else {
				cur->data=x;
			}
		}
		if (tmp!=NULL) {
			while (tmp->fa!=NULL) {
				if (tmp->tag>tmp->fa->tag) {
					rotate(tmp);
				} else {
					break;
				}
			}
		}
	}

	void del(node *cur, K key) {
		if (cur->data.getKey()==key) {
			while (cur->son[0]!=NULL||cur->son[1]!=NULL) {
				if (cur->son[0]!=NULL&&(cur->son[1]==NULL||cur->son[1]->tag<cur->son[0]->tag)) {
					rotate(cur->son[0]);
				} else {
					rotate(cur->son[1]);
				}
			}
			if (cur->fa!=NULL) {
				cur->fa->son[child(cur)]=NULL;
			} else {
				root=NULL;
			}
			delete cur;
		} else if (cur->data.getKey()>key) {
			del(cur->son[0],key);
		} else {
			del(cur->son[1],key);
		}
	}

    class Iterator
    {
		node *root, *cur;
    public:
		Iterator(node *root) : root(root) {
			cur=NULL;
		}
		int child(node *cur) {
			return cur->fa->son[0]!=cur;
		}
        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {
			node *tmp=cur;
			if (tmp==NULL) {
				if (root==NULL) {
					return 0;
				} else {
					return 1;
				}
			}
			if (tmp->son[1]!=NULL) return 1;
			while (tmp->fa!=NULL&&child(tmp)==1) {
				tmp=tmp->fa;
			}
			if (tmp->fa!=NULL) return 1;
			return 0;
		}

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const Entry &next() {
			if (!hasNext()) throw ElementNotExist("\nElement Not Exist\n");
			if (cur==NULL) {
				cur=root;
				while (cur->son[0]!=NULL) cur=cur->son[0];
				return cur->data;
			}
			if (cur->son[1]!=NULL) {
				cur=cur->son[1];
				while (cur->son[0]!=NULL) cur=cur->son[0];
				return cur->data;
			}
			while (cur->fa!=NULL&&child(cur)==1) {
				cur=cur->fa;
			}
			cur=cur->fa;
			return cur->data;
		}
    };

    /**
     * TODO Constructs an empty tree map.
     */
    TreeMap() {
		srand(time(0));
		Size=0;
		root=NULL;
	}
    /**
     * TODO Destructor
     */
    ~TreeMap() {
		if (root!=NULL) clean(root);
	}

    /**
     * TODO Assignment operator
     */
    TreeMap &operator=(const TreeMap &x) {
		if (root!=NULL) clean(root);
		root=NULL;
		if (x.root!=NULL) dfs(root,x.root);
		Size=x.Size;
		return *this;
	}

    /**
     * TODO Copy-constructor
     */
    TreeMap(const TreeMap &x) {
		root=NULL;
		if (x.root!=NULL) dfs(root,x.root);
		Size=x.Size;
	}

    /**
     * TODO Returns an iterator over the elements in this map.
     */
    Iterator iterator() const {
		return Iterator(root);
	}

    /**
     * TODO Removes all of the mappings from this map.
     */
    void clear() {
		if (root!=NULL) clean(root);
		root=NULL;
		Size=0;
	}

    /**
     * TODO Returns true if this map contains a mapping for the specified key.
     */
    bool containsKey(const K &key) const {
		return find(root,key);
	}

    /**
     * TODO Returns true if this map maps one or more keys to the specified value.
     */
    bool containsValue(const V &value) const {
		return find(root,value);
	}

    /**
     * TODO Returns a const reference to the value to which the specified key is mapped.
     * If the key is not present in this map, this function should throw ElementNotExist exception.
     * @throw ElementNotExist
     */
    const V &get(const K &key) const {
		if (!containsKey(key)) throw ElementNotExist("\nElement Not Exist\n");
		return keyValue(root,key);
	}

    /**
     * TODO Returns true if this map contains no key-value mappings.
     */
    bool isEmpty() const {
		return Size==0;
	}

    /**
     * TODO Associates the specified value with the specified key in this map.
     */
    void put(const K &key, const V &value) {
		insert(root,Entry(key,value));
		++Size;
	}

    /**
     * TODO Removes the mapping for the specified key from this map if present.
     * If there is no mapping for the specified key, throws ElementNotExist exception.
     * @throw ElementNotExist
     */
    void remove(const K &key) {
		if (!containsKey(key)) throw ElementNotExist("\nElement Not Exist\n");
		del(root,key);
		--Size;
	}

    /**
     * TODO Returns the number of key-value mappings in this map.
     */
    int size() const {
		return Size;
	}
};

#endif
