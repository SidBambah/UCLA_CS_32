#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <string>

typedef std::string ItemType;

//Class Declaration

class Sequence {
public:
	Sequence();
	~Sequence();
	Sequence(const Sequence& other);
	Sequence& operator=(const Sequence& other);
	bool empty() const;
	int size() const;
	bool insert(int pos, const ItemType& value);
	int insert(const ItemType& value);
	bool erase(int pos);
	int remove(const ItemType& value);
	bool get(int pos, ItemType& value) const;
	bool set(int pos, const ItemType& value);
	int find(const ItemType& value) const;
	void swap(Sequence& other);
private:
	void addToFront(const ItemType& value);
	void addToRear(const ItemType& value);
	int nItems;
	struct Node {
		ItemType value;          //Basis for the linked list
		Node* next;
		Node* prev;
	};
	Node* head;
	Node* tail;
};

int subsequence(const Sequence& seq1, const Sequence& seq2);
void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result);

#endif