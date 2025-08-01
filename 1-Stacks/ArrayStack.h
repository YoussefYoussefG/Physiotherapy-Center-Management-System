//	This is an updated version of code originally
//  created by Frank M. Carrano and Timothy M. Henry.
//  Copyright (c) 2017 Pearson Education, Hoboken, New Jersey.

/** ADT stack: Array-based implementation.
 @file ArrayStack.h */

#ifndef ARRAY_STACK_
#define ARRAY_STACK_

#include "StackADT.h"
#include <iostream>

//Unless spesificed by the stack user, the default size is 100
template<typename T>
class ArrayStack : public StackADT<T>
{
	enum { MAX_SIZE = 100 };
private:
	T items[MAX_SIZE];		// Array of stack items
	int top;                   // Index to top of stack
	int count;                // Number of items in stack
	
public:

	ArrayStack() : top(-1), count(0) {
		// Initialize items array to default values
		for (int i = 0; i < MAX_SIZE; i++) {
			items[i] = T();
		}
	}  // end default constructor

	bool isEmpty() const
	{
		return top == -1;	
	}  // end isEmpty

	bool push(const T& newEntry)
	{
		if( top == MAX_SIZE-1 ) return false;	//Stack is FULL

		top++;
		items[top] = newEntry;   
		count++;
		return true;
	}  // end push

	bool pop(T& TopEntry)
	{
		if (isEmpty()) return false;
		
		TopEntry = items[top];		 
		top--;
		count--;
		return true;
	}  // end pop
	
	bool peek(T& TopEntry) const
	{
		if (isEmpty()) return false;
		
		TopEntry = items[top];		 
		return true;
	}  // end peek

	int getCount() const { return count; }
	void print(std::ostream& out = std::cout) const {
		// Print in LIFO order (most recently added first)
		for (int i = top; i >= 0; i--) {
			out << items[i];
			if (i > 0) out << ", ";
		}
	}

}; // end ArrayStack

// Stream operator overload for ArrayStack
template <typename T>
std::ostream& operator<<(std::ostream& out, const ArrayStack<T>& stack) {
	stack.print(out);
	return out;
}

#endif
