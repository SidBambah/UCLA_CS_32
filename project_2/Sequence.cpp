#include "Sequence.h"
using namespace std;

Sequence::Sequence() {       //Constructs an empty sequence
	head = nullptr;
	tail = nullptr;
	nItems = 0;
}

Sequence::Sequence(const Sequence& other) {
	nItems = 0;
	head = nullptr;
	tail = nullptr;

	for (int i = 0; i < other.size(); i++) {
		ItemType temp;                //Copies each individual node from
		other.get(i, temp);			  // one sequence to another
		insert(i, temp);
	}
}

Sequence::~Sequence() {
	int i = 0;					//Counter that tracks position
	Node* p = head;
	while (i != nItems) {
		Node* killMe = p;		//Removes every node in the sequence
		p = p->next;
		i++;
		delete killMe;
	}
}

Sequence& Sequence::operator=(const Sequence& other) {
	int i = 0;
	Node* p = head;
	while (i != nItems) {
		Node* killMe = p;		//Removes every node in the sequence
		p = p->next;
		i++;
		delete killMe;
	}
	head = nullptr;
	tail = nullptr;
	nItems = 0;
	for (int i = 0; i < other.size(); i++) {
		ItemType temp;                //Copies each individual node from
		other.get(i, temp);			  // one sequence to another
		insert(i, temp);
	}
	return *this;
}

void Sequence::addToFront(const ItemType& value) {
	Node* p = new Node;
	p->value = value;
	p->next = head;        // Moves head pointer and connects
	p->prev = nullptr;     // new node to the following nodes
	head = p;
	if (nItems == 0)
		tail = p;
}

void Sequence::addToRear(const ItemType& value) {
	Node* p = new Node;
	p->next = nullptr;
	p->prev = tail;      //Moves tail pointer nd connects
	tail->next = p;		// new node to previous nodes
	p->value = value;
	tail = p;
}

bool Sequence::empty() const {
	if (nItems == 0)         //Returns true if there are no
		return true;		// items in the sequence
	return false;
}

int Sequence::size() const {
	return nItems;        //Returns how many items are in the sequence
}

bool Sequence::insert(int pos, const ItemType& value) {
	if (pos > nItems || pos < 0)
		return false;            //Fails if pos is out of bounds
	else if (pos == 0) {
		addToFront(value);
		nItems++;				//Adds to front is pos is 0
		return true;
	}
	else if (pos == nItems) {
		addToRear(value);		//Adds to back is pos = number of items
		nItems++;				// in sequence
		return true;
	}
	else {
		int i = 0;
		Node* p = head;
		while (i != pos - 1) {
			p = p->next;
			i++;
		}
		Node* newest = new Node;		//adjusts pointers if new node
		newest->value = value;			//is added in the middle
		newest->next = p->next;
		newest->prev = p;
		p->next = newest;
		newest->next->prev = newest;
		nItems++;
		return true;
	}
}
int Sequence::insert(const ItemType& value) {
	int pos = 0;
	Node* p = head;
	if (nItems == 0) {
		insert(0, value);
	}
	while (p != nullptr) {
		if (p->value <= value) {
			insert(pos, value);
			return pos;			//Finds the position where given 
		}						//to add new node and calls insert 
		pos++;					//function
		p = p->next;
	}
	addToRear(value);
	return nItems - 1;
}

bool Sequence::erase(int pos) {
	if (pos >= nItems || pos < 0)
		return false;				//Error is pos is out of bounds
	else if (pos == 0 && nItems == 1) {
		delete head;
		head = nullptr;
		tail = nullptr;		//If only one node, removes it
		nItems--;
		return true;
	}
	else if (pos == 0) {
		Node* killMe = head;
		head = head->next;
		delete killMe;		//If start of sequence, removes first node
		nItems--;
		return true;
	}
	else if (pos == (nItems - 1)) {
		Node* killMe = tail;
		tail = tail->prev;		//Removes last node
		delete killMe;
		nItems--;
		return true;
	}
	else {
		int i = 0;
		Node* p = head;				//Removes a node in the middle
		while (i != pos - 1) {
			p = p->next;
			i++;
		}
		Node* killMe = p->next;
		p->next = killMe->next;
		killMe->next->prev = p;
		delete killMe;
		nItems--;
		return true;
	}
}

int Sequence::remove(const ItemType & value) {
	ItemType temp;
	int numRemoved = 0;
	for (int i = 0; i != size(); i++) {
		get(i, temp);
		if (temp == value) {
			erase(i);
			i--;
			numRemoved++;
		}
	}
	return numRemoved;
}

bool Sequence::get(int pos, ItemType & value) const {
	if (pos < 0 || pos >= nItems)
		return false;
	Node* p = head;				//Pastes value of node at pos
	int i = 0;                  //into the value reference variable
	while (i != pos) {
		p = p->next;
		i++;
	}
	value = p->value;
	return true;
}

bool Sequence::set(int pos, const ItemType & value) {
	if (pos < 0 || pos >= nItems)
		return false;
	Node* p = head;				//Pastes items in value reference variable
	int i = 0;                  //into the node at pos
	while (i != pos) {
		p = p->next;
		i++;
	}
	p->value = value;
	return true;
}

int Sequence::find(const ItemType & value) const {
	int p = 0;
	Node* temp = head;				//Returns pos of smallest node that contains
	while (temp != nullptr) {       //value
		if (temp->value == value)
			return p;
		p++;
		temp = temp->next;
	}
	p = -1;
	return p;
}

void Sequence::swap(Sequence& other) {
	Node* tempHead = head;
	Node* tempTail = tail;             //Uses copy constructor and overloaded
	int size = nItems;					//operator (=) to swap variables
	head = other.head;
	tail = other.tail;
	other.head = tempHead;
	other.tail = tempTail;
	nItems = other.nItems;
	other.nItems = size;
}

int subsequence(const Sequence& seq1, const Sequence& seq2) {
	if (seq1.size() < seq2.size() || seq2.empty())
		return -1;
	ItemType temp1;
	ItemType temp2;
	for (int i = 0; i != seq1.size(); i++) {
		seq1.get(i, temp1);
		seq2.get(0, temp2);
		if (temp1 == temp2) {
			int z = i;
			int num = 0;
			for (int s = 0; s != seq2.size(); s++) {
				seq1.get(z, temp1);
				seq2.get(s, temp2);
				if (temp1 == temp2)
					num++;
				z++;
			}
			if (num == seq2.size())
				return i;
		}
	}
	return -1;
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result) {
	Sequence temp;
	if (seq1.empty() && !seq2.empty()) {
		temp = seq2;
		result = temp;			//If seq1 is empty, result will be a copy of seq2
		return;
	}
	if (seq2.empty() && !seq1.empty()) {
		temp = seq1;
		result = temp;			//If seq2 is empty, result will be a copy of seq1
		return;
	}
	if (seq1.size() == seq2.size()) {
		Sequence toCopy;
		int pos = 0;
		int i = 0;
		while (pos != seq1.size()) {
			ItemType temp1, temp2;			//If size of seq1 is same as size of seq2
			seq1.get(pos, temp1);				//put values from seq1 and seq2 in temp variables
			seq2.get(pos, temp2);				// to write to result sequence
			toCopy.insert(i, temp1);
			i++;
			toCopy.insert(i, temp2);
			i++;
			pos++;
		}
		result = toCopy;
		return;
	}
	if (seq1.size() < seq2.size()) {
		Sequence toCopy;
		int pos = 0;
		int i = 0;
		while (pos != seq1.size()) {
			ItemType temp1, temp2;
			seq1.get(pos, temp1);
			seq2.get(pos, temp2);
			toCopy.insert(i, temp1);
			i++;
			toCopy.insert(i, temp2);
			i++;
			pos++;
		}

		while (pos != seq2.size()) {
			ItemType temp1;
			seq2.get(pos, temp1);
			toCopy.insert(i, temp1);
			i++;
			pos++;
		}
		result = toCopy;
		return;
	}

	if (seq2.size() < seq1.size()) {
		Sequence toCopy;
		int pos = 0;
		int i = 0;
		while (pos != seq2.size()) {
			ItemType temp1, temp2;
			seq1.get(pos, temp1);
			seq2.get(pos, temp2);
			toCopy.insert(i, temp1);
			i++;
			toCopy.insert(i, temp2);
			i++;
			pos++;
		}
		while (pos != seq1.size()) {
			ItemType temp1;
			seq1.get(pos, temp1);
			toCopy.insert(i, temp1);
			i++;
			pos++;
		}
		result = toCopy;
		return;
	}
}
