#include <iostream>

class SchoolClass {
	int *ClassData;
	int ClassDataIndex;
public:
	SchoolClass(int NumberStudents);
	~SchoolClass();
	void AddScore(int Score);
	int GetScore(int Index);
	float AverageScore();
};

SchoolClass::SchoolClass(int NumberStudents)
{
	ClassData = new int[NumberStudents];
	ClassDataIndex = 0;
}

SchoolClass::~SchoolClass()
{
	delete ClassData;
}

void SchoolClass::AddScore(int Score)
{
	ClassData[ClassDataIndex++] = Score;
}

int SchoolClass::GetScore(int Index)
{
	if (Index <= ClassDataIndex)
	{
		return ClassData[Index];
	}
	else
	{
		return -1;
	}
}

float SchoolClass::AverageScore()
{
	float Sum = 0;
	if (ClassDataIndex == 0)
	{
		return -1;
	}

	for (int loop_index = 0; loop_index < ClassDataIndex; loop_index++)
	{
		Sum += (float)ClassData[loop_index];
	}
	return Sum / (float)ClassDataIndex;
}

int main()
{
	SchoolClass HistoryClass(9);
	SchoolClass EnglishClass(10);

	HistoryClass.AddScore(80);
	HistoryClass.AddScore(81);
	HistoryClass.AddScore(82);
	HistoryClass.AddScore(83);
	HistoryClass.AddScore(84);
	HistoryClass.AddScore(85);
	HistoryClass.AddScore(86);
	HistoryClass.AddScore(87);
	HistoryClass.AddScore(88);

	EnglishClass.AddScore(91);
	EnglishClass.AddScore(92);
	EnglishClass.AddScore(93);
	EnglishClass.AddScore(94);
	EnglishClass.AddScore(95);
	EnglishClass.AddScore(96);
	EnglishClass.AddScore(97);
	EnglishClass.AddScore(98);
	EnglishClass.AddScore(99);
	EnglishClass.AddScore(100);

	std::cout << "Average score for History Class: "
		<< HistoryClass.AverageScore() << "\n";
	std::cout << "Average score for English Class: "
		<< EnglishClass.AverageScore() << "\n";
}