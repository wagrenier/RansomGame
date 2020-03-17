// SimpleGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

// Simple value that the external program will read
int hidden_value = 10;

int main()
{
    // For the moment it is an int to simplify my memory reading
    int input = hidden_value;
    std::cout << "Your system was hacked, find the hidden value to decrypt your system!" << std::endl;
    while (true)
    {
        std::cin >> input;
        std::cout << "Your input: " << input << std::endl;
    }

    return 0;
}