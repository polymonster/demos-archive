#include <iostream.h>

int main()
{

int myIntegerVariable;
myIntegerVariable = 13;
cout << myIntegerVariable << " ";

const double myDoubleConstant = 12.65;
cout << myDoubleConstant;

int *myIntegerPointer;

int arraySize = 20;

myIntegerPointer = &myIntegerVariable;

cout << *myIntegerPointer;

char myStaticCharArray[arraySize];

for(int i=0;i<20;i++)
{
	myStaticCharArray[i] = i;
}

return 1;

}