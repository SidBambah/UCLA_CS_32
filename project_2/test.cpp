#include <string>
#include <iostream>
#include <cassert>
#include "Sequence.h"

using namespace std;

int main() {
	//Default constructor
	Sequence s;
	Sequence t;
	//Empty Sequence Operations
	assert(s.size() == 0);          //test size
	assert(t.empty());              //test empty
	assert(t.remove("george") == 0);  //nothing to remove

	cout << "Passed all empty string tests!" << endl;

	//Insert Operations
	assert(s.insert("bob") == 0);    //adding first node
	assert(s.size() == 1);					//check number if number of items updates
	assert(s.insert("jim") == 0);     //checks if adding a second node doesn't break program
	assert(s.size() == 2);         //nItems updates properly
	assert(s.insert(0, "joe") == true);      //insert with overloaded arguments
	assert(s.size() == 3);           //size still updates
	assert(s.insert(2, "jack") == true);
	assert(s.insert(1, "bill") == true);
	assert(s.size() == 5);
	cout << "Passed insert tests!" << endl;

	//Erase and Remove Operations
	assert(s.erase(0) == true);	//checks erase return value
	assert(s.remove("jim"));     //checks remove function
	assert(s.size() == 3);      //size decrements properly

	cout << "Passed erase and remove tests!" << endl;

	//Overloaded Operator
	t = s;
	assert(t.size() == s.size());   //checks that nItems copied properly

									//Get and Set Operations
	ItemType v;         //temp value
	ItemType x;         //another temp value
	assert(s.get(0, v) && v == "bill");    //makes sure value at pos is copied into variable
	assert(t.get(0, x) && x == "bill");    //further check of overloaded operator
	v = "john";
	assert(t.set(1, v));              //sets item at pos 1 to john
	assert(t.get(1, x) && x == v);    //shows value is copied into node

	cout << "Passed get and set tests!" << endl;

	//Find Operation

	assert(s.find("bill") == 0);       //Function returns pos of value
	assert(s.find("johnny") == -1);    //Error because johnny does not exist in sequence

	cout << "Passed find tests!" << endl;

	//Swap Operation
	t.erase(1);          //done so s and t are of diff. size
	swap(s, t);
	assert(s.size() == 2);   //shows that the nItems of t is now in s
	assert(t.size() == 3);   //nItems of s is now in t

	cout << "Passed swap tests!" << endl;

	//Subsequence Operation
	Sequence a;
	Sequence b;
	a.insert(0, "my");         //a has "my actual name is jack"
	a.insert(1, "actual");
	a.insert(2, "name");
	a.insert(3, "is");
	a.insert(4, "jack");
	b.insert(0, "name");     // b has "name is"
	b.insert(1, "is");

	assert(subsequence(a, b) == 2);   //accurately returns the start of the subsequence
	a.insert(1, "name");
	assert(subsequence(a, b) == 3);   //function does not fail if distracted by one similar value

	cout << "Passed subsequence tests!" << endl;

	//Interleave Operation
	Sequence test1;
	Sequence test2;
	Sequence result;
	ItemType t1;          //Vars. to check if items are copied in right order
	ItemType t2;
	test1.insert(0, "1");
	test2.insert(0, "2");
	test1.insert(1, "3");             //test1 and test2 are of same size
	test2.insert(1, "4");
	interleave(test1, test2, result);
	assert(result.size() == 4);               //Accurately joined test1 and test2
	assert(result.get(0, t1) && t1 == "1");   //originally part of tes1
	assert(result.get(1, t2) && t2 == "2");  //originally part of test2
	test1.insert(2, "5");        //Now test1.size() > test2.size()
	interleave(test1, test2, result);    //result will not contain any previous information
	assert(result.get(4, t1) && t1 == "5");   //even though test1 is bigger, remainder is copied in
	test1.erase(2);
	test1.erase(1);
	interleave(test1, test2, result);   //now test1.size() < test2.size()
	assert(result.size() == 3);

	cout << "Passed interleave tests!" << endl;

	cout << "PASSED ALL TESTS!!!!" << endl;
}
