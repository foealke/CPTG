#include <iostream>
#include <stdlib.h>
#include <time.h> 

#include "CPTG/nodes/Var.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/property_tree/xml_parser.hpp>

Var::Var() : varType(Type::Number) { }

Var::Var(std::shared_ptr< ptree > tag) : varTag(tag)
{
	srand(time(NULL));
	boost::optional< ptree& > attributes = varTag->get_child_optional("<xmlattr>");

	if(attributes != boost::none)
	{
		FindParameters(attributes.get());
	}
	else
	{
		varType = Type::Number;
		range = { 0, 9 };
	}
}

void Var::FindParameters(const ptree& attributes)
{
	if(!FindType(attributes))
	{
		std::cout << "Error: Unknown variable type provided\nUsing default instead" << std::endl;
		return;
	}

	// Find range depending on variable type
	if(varType == Type::Number || varType == Type::Float)
	{
		FindNumericRange(attributes);
	}
	else
	{
		FindLexicalRange(attributes);
	}
	
	// Determin the length for text types
	if(varType == Type::String || varType == Type::Float)
	{
		FindLength(attributes);
	}
	else if(varType == Type::Char)
	{
		length = 1;
	}
}

bool Var::FindType(const ptree& attributes)
{
	if(attributes.count("type") == 0)
	{
		// It is permited to not specify type
		varType = Type::Number;
		return true; 
	}

	const auto type = attributes.get<std::string>("type");
	if(typeMap.find(type) != typeMap.end())
	{
		varType = typeMap[type];
		return true;
	}
	
	return false;
}

void Var::FindNumericRange(const ptree& attributes)
{
	if(!attributes.count("range"))
	{
		range = {0, 9};
		return;
	}

	std::string rangeStr = attributes.get<std::string>("range");
	std::vector< std::string > splitRange;

	boost::split(splitRange, rangeStr, boost::is_any_of(":"));

	if(std::stoi(splitRange[0]) > std::stoi(splitRange[1]))
	{
		std::cout << "Error: Numeric range is not valid\nUsing default instead" << std::endl;
		range = {0, 9};
		return;
	}

	range = {std::stoi(splitRange[0]), std::stoi(splitRange[1])};
}

void Var::FindLexicalRange(const ptree& attributes)
{
	if(!attributes.count("range"))
	{
		lexicalRange = LexicalRange::abc;
		return;
	}

	std::string rangeKey = varTag->get<std::string>("<xmlattr>.range");

	if(rangeKey == "abc")
	{
		lexicalRange = LexicalRange::abc;
	}
	if(rangeKey == "ABC")
	{
		lexicalRange = LexicalRange::ABC;
	}
	else if(rangeKey == "special")
	{
		// TODO: range does not cover all characters
		lexicalRange = LexicalRange::special;
	}
	else if(rangeKey == "all")
	{
		lexicalRange = LexicalRange::all;
	}
	else
	{
		std::cout << "Error: Unknown lexical range\nUsed default instead" << std::endl;
		lexicalRange = LexicalRange::abc;
	}
}

void Var::FindLength(const ptree& attributes)
{
	if(!attributes.count("length"))
	{
		length = 3;
		return;
	}

	int readLength = varTag->get<int>("<xmlattr>.length");
		
	if(readLength >= 0)
	{
		length = readLength;
	}
	else
	{
		std::cout << "Error: Length must be positive\nUsing default length instead" << std::endl;
		length = 3;
	}
}

void Var::Print()
{
	if(varType == Type::Number || varType == Type::Float)
	{
		int randomNumber = GenerateRandomNumber();
		std::cout << randomNumber;

		if(varType == Type::Float)
		{
			std::cout << GetReverseNumber(randomNumber);
		}
	}
	else if(varType == Type::String || varType == Type::Char)
	{
		for(int i = 0; i < length; i++)
		{
			std::cout << GenerateRandomChar();
		}
	}
	
	std::cout << " ";
}

char Var::GenerateRandomChar()
{
	char randomChars[3] = { 
		abcRange[rand() % abcRange.size()],
		ABCRange[rand() % ABCRange.size()],
		specialRange[rand() % specialRange.size()]
	};

	if(lexicalRange == LexicalRange::all)
	{
		// TODO: generated characters are not evenly distributed
		return randomChars[rand() % 3];
	}

	return randomChars[int(lexicalRange)];
}

int Var::GenerateRandomNumber()
{
	return rand() % (range.second - range.first + 1) + range.first;
}

std::string Var::GetReverseNumber(int generatedNumber)
{
	// To avoid exceeding provided range
	if(generatedNumber == range.second)
	{
		return "";
	}

	std::stringstream ss, tempStream;
	
	for(int i = 0; i < length; i++)
	{
		tempStream << char('0' + rand() % 10);
		if(tempStream.str().back() != '0')
		{
			ss << tempStream.str();
			tempStream.str("");
		}
	}

	if(!ss.str().empty())
	{
		ss << '.';
	}

	std::string str = ss.str();
	std::reverse(str.begin(), str.end());
	return str;
}

// Define ranges for VAR type
const std::vector<char> Var::abcRange = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
const std::vector<char> Var::ABCRange = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
const std::vector<char> Var::specialRange = { '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '-', '_', '+', '=', '{', '}', '[', ']', '|', '\\', ':', ';', '"', '\'', '<', ',', '>', '.', '?', '/' };
