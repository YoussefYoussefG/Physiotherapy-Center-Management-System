#include "LinkedQueue.h"
#include <iostream>		//To use C++ console I/O functionality
						//, we need to include iostream which contains necessary declarations.
using namespace std;	//This statement enables us to use classes and objects in the standard C++ library
						//without prefixing them with "std::"

//A function that prints queue contents

template <typename T>
void PrintQueue(LinkedQueue<T> Q) //Pass by value
{
	//For this function to work properly, the LikedQueue class MUST
	//have  a copy constructor (pass by value)
	T K;
	cout<<"\nQueue contents: ";
	while(Q.dequeue(K))
		cout << K << " ";
	cout<<endl;
}

/*
Function: removeFirstZeros
	A function the removes the first zero items found in a queue
	The function stops either because a non-zero item is encounted or queue becomes empty
*/
void removeFirstZeros(LinkedQueue<int>& Q) //pass by ref
{
	int x;
	while (Q.peek(x))	//Have a look first before dequeuing it
		if (x != 0)	//if item is not zero, don't dequeue and end the function
			return;
		else
			Q.dequeue(x);	//it is a zero ==> dequeue it
}

//A function that returns the sum of an integer queue
int QueueSum(LinkedQueue<int> Q) 
{
	int s=0;
	int x;
	while(Q.dequeue(x))
		s += x;
	return s;
}

//This is the program starting point.
int main()
{
	//Declare a queue of integers named "queue1"
	LinkedQueue<int> queue1;
	int x, n;

	cout<<"\nEnter no. of values to enqueue:";
	cin>>n;

	//Test the queue by enqueuing n values then poping
	cout << "Filling queue 1:" << endl;
	cout << "Enqueuing "<<n<<" values:" << endl;

	cout<<"Please Enter the "<<n<<" values to enqueue in queue1 :";
	for(int i = 0; i < n; i++)
	{
		cin>>x;
		queue1.enqueue(x); 
	}
	PrintQueue(queue1);
	cout<<"\nSum of queue items = "<<QueueSum(queue1);

	cout << "\nRemoving first zero items:";
	removeFirstZeros(queue1);

	cout << "After removal: ";	
	PrintQueue(queue1);
	
	cout<<endl;

	return 0;
}
  
