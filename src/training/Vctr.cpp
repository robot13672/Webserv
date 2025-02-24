#include <iostream>
#include <vector>

int main()
{
	std::vector <int> myVector = {1, 2, 3, 4, 5};
	std::cout << "capacity(): " << myVector.capacity() << std::endl;
	myVector.push_back(2);
	myVector.push_back(44);
	myVector.push_back(77);
	myVector.push_back(9);
	myVector.reserve(77);
	std::cout << "capacity() after reserve(77): " << myVector.capacity() << std::endl;
	std::cout << "empty(): " << myVector.empty()  << std::endl;
	
	std::cout << "Quantity of elements in myVector: " << myVector.size() << "; max_size() = " << myVector.max_size() << std::endl;
	std::cout << "capacity(): " << myVector.capacity() << std::endl;
	for (int i = 0; i < myVector.size(); i++)
		std::cout << myVector[i] << std::endl;
	
		
		std::cout <<  "myVector[1] = myVector.at(1) = " << myVector.at(1) << std::endl;
		
		std::cout << std::endl << "Umpredicted behaviour, direct addressing: ";
		myVector[999] = 999;
		std::cout << myVector[myVector.size() + 999] << std::endl; // beyand the vector range

		std::cout << "Predicted behaviour, addressing via method at(): " <<  std::endl;
		try
		{
			std::cout << myVector.at(999) << std::endl;
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
		
		myVector.pop_back();
		std::cout << std::endl << "After pop_back()" << std::endl;
		std::cout << "Quantity of elements in myVector: " << myVector.size() << "; max_size() = " << myVector.max_size() << std::endl;
		for (int i = 0; i < myVector.size(); i++)
		std::cout << myVector[i] << std::endl;
		
		myVector.clear();
		std::cout << std::endl;
		std::cout << "Quantity of elements in myVector after clear(): " << myVector.size() << "; max_size() = " << myVector.max_size() << std::endl;
		std::cout << "capacity(): " << myVector.capacity() << std::endl;
		myVector.shrink_to_fit();
		std::cout << "capacity() after shrink_to_fit(): " << myVector.capacity() << std::endl;
		std::cout << "empty(): " << myVector.empty()  << std::endl;

}