#pragma once

#include <iostream>
#include <memory>
#include <unordered_map>
#include <stdlib.h>
#include <time.h> 

#include "Node.hpp"

#include <boost/optional/optional.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string.hpp>

class Var : public Node
{
    std::shared_ptr< boost::property_tree::ptree > varTag;

    enum class Type { Char, Number, Float, String };
    Type varType;

    std::unordered_map< std::string, Type> typeMap = {
        {"Char", Type::Char},
        {"Number", Type::Number},
        {"Float", Type::Float},
        {"String", Type::String}
    };

    enum class LexicalRange { abc, ABC, special, all };
    LexicalRange lexicalRange;

    static std::vector<char> abcRange;
    static std::vector<char> ABCRange;
    static std::vector<char> specialRange;

    std::pair< int, int > range;

    int length;

    void FindParameters( const boost::optional< boost::property_tree::ptree& >& attributes );

    bool FindType( const boost::optional< boost::property_tree::ptree& >& attributes );
    void FindNumericRange( const boost::optional< boost::property_tree::ptree& >& attributes );
    void FindLexicalRange( const boost::optional< boost::property_tree::ptree& >& attributes );
    void FindLength( const boost::optional< boost::property_tree::ptree& >& attributes );

    char GenerateRandomChar();
    int GenerateRandomNumber();
    std::string GetReverseNumber( int generatedNumber );

public:
    Var();
    Var( std::shared_ptr< boost::property_tree::ptree > );

    void Print() override;
};