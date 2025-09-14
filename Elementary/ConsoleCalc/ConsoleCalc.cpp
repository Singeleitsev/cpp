#include <iostream>

char esc = 0, operation = 0;
int num[3] = { 0 };

int main()
{
    while (esc != 'q')
    {
        std::cin >> num[0];
        std::cin >> operation;
        std::cin >> num[1];
        std::cin.sync(); // clear the input buffer

        switch (operation)
        {
        case '+': num[2] = num[0] + num[1]; break;
        case '-': num[2] = num[0] - num[1]; break;
        case '*': num[2] = num[0] * num[1]; break;
        case '/': num[2] = num[0] / num[1]; break;
        default: 
            num[2] = 0;
            std::cout << "Wrong operation\n";
        }
        std::cout << "Result: " << num[2];
        std::cout << "\nPress Q to quit or any Key to repeat\n";
        std::cin >> esc;
        std::cin.sync(); // clear the input buffer
    }
}
