#include <iostream>

class DataClass
{
private:
	int PrivateDataMember;
public:
	DataClass(int Value);
	int PublicDataMember;
	int PublicMethod();
};

DataClass::DataClass(int Value)
{
	PrivateDataMember = Value;
}

int DataClass::PublicMethod()
{
	return PrivateDataMember;
}

int main()
{
	DataClass DataObject(1);

	DataObject.PublicDataMember = 2;
	std::cout << "DataObject.PublicDataMember = "
		<< DataObject.PublicDataMember << "\n";
	std::cout << "DataObject.PrivateDataMember = "
		<< DataObject.PublicMethod() << "\n";
}