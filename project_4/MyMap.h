// MyMap.h

#ifndef MYMAP_H
#define MYMAP_H
//#include "support.h"

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap();
	~MyMap() { destroyMap(m_head); }
	void clear() { destroyMap(m_head); }
	int size() const { return m_count; }
	void associate(const KeyType& key, const ValueType& value);

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

	struct Node {
		KeyType m_key;
		ValueType m_value;
		Node* m_left;
		Node* m_right;
		Node(KeyType key, ValueType value) {
			m_key = key;
			m_value = value;
			m_left = nullptr;
			m_right = nullptr;
		}
	};
private:
	int m_count;
	Node* m_head;
	void destroyMap(Node* subtree);
	ValueType* search(Node* subtree, const KeyType& key) const;
	bool insert(const KeyType& key, const ValueType& value, Node* subtree);
};

template <typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap() {
	m_count = 0;
	m_head = nullptr;
}             

template <typename KeyType, typename ValueType>
bool MyMap<KeyType, ValueType>::insert(const KeyType& key, const ValueType& value, Node* subtree) {
	if (key < subtree->m_key) {
		if (subtree->m_left != nullptr)
			return insert(key, value, subtree->m_left);
		else {
			subtree->m_left = new Node(key, value);
			return true;
		}
	}
	else if (key == subtree->m_key) {
		subtree->m_value = value;
		return false;
	}
	else if (key > subtree->m_key) {
		if (subtree->m_right != nullptr)
			return insert(key, value, subtree->m_right);
		else {
			subtree->m_right = new Node(key, value);
			return true;
		}
	}
	return false;
}

template <typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value) {
	if (m_head == nullptr) {
		m_head = new Node(key, value);
		m_count++;
		return;
	}
	if (insert(key, value, m_head) == true) {
		m_count++;
		return;
	}
	return;
} 

template <typename KeyType, typename ValueType>
ValueType* MyMap<KeyType, ValueType>::search(Node* subtree, const KeyType& key) const {
	if (subtree != nullptr) {
		if (key == subtree->m_key)
			return &(subtree->m_value);
		if (key < subtree->m_key)
			return search(subtree->m_left, key);
		else
			return search(subtree->m_right, key);
	}
	else
		return nullptr;
}

template <typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::find(const KeyType& key) const {
	return search(m_head, key); 
}

template <typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::destroyMap(Node* subtree) {
	if (subtree != NULL)
	{
		destroyMap(subtree->m_left);
		destroyMap(subtree->m_right);
		delete subtree;
	}
}

#endif