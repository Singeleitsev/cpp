#include <iostream>

class Vehicle {
public:
	void Start();
	void Move();
};

class Helicopter : public Vehicle {
public:
	void Move();
};

void Vehicle::Start() { std::cout << "Started!\n"; }
void Vehicle::Move() { std::cout << "Driving...\n"; }
void Helicopter::Move() { std::cout << "Flying...\n"; }

int main()
{
	Helicopter Whirly;
	Whirly.Start();
	Whirly.Move();
}

