#include <iostream>

class DisplayClass
{
public:
	void Display(char Character);
	void Display(const char *String);
};

void DisplayClass::Display(char Character)
{
	std::cout << Character;
}

void DisplayClass::Display(const char *String)
{
	std::cout << String;
}

int main()
{
	DisplayClass DisplayObject;

	DisplayObject.Display('H');

	DisplayObject.Display("ello");
	//const char *szString1 = "ello";
	//DisplayObject.Display(szString1);

	char cChar1 = '!';
	DisplayObject.Display(cChar1);
	std::cout << "\n";
}