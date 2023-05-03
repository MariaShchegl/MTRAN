#pragma once
#include<iostream>
#include<map>
#include <algorithm>

enum class TokenTypes {
    UNKNOWN = 0, COMMENT, STRING_CONST, FLOAT_NUM, INT_NUM,
    ID, DOT, COLON, COMMA,
    OPENING_SQUARE_BRACKET, CLOSING_SQUARE_BRACKET, OPENING_ROUND_BRACKET, CLOSING_ROUND_BRACKET, OPENING_CURLY_BRACKET, CLOSING_CURLY_BRACKET, AND,
    OR, NOT, ASSIGN, PLUS, MINUS,
    DIVISION, MULTIPLICATION, MODULE, GREATER,
    GREATER_OR_EQUAL, LOWER, LOWER_OR_EQUAL, EQUAL,
    NOT_EQUAL, FOR,
    IN, WHILE, IF, ELIF, ELSE,
    BUILT_IN_FUNCTION, RAISE, RETURN,
    IMPORT, FUNCTION_DEFINITION, PROGRAM
};

class Token
{
    std::string Value;
    std::string Group;
    TokenTypes TokenType;
    int CodeLineNumber;
    int CodeLineIndex;
    int Length;
public:

    Token(std::string value, std::string groupNames, int lineNumber, int index, int length, TokenTypes type)
    {
        this->Value = value;
        this->Group = groupNames;
        this->CodeLineNumber = lineNumber;
        this->CodeLineIndex = index;
        this->Length = length;
        this->TokenType = type;
    }

    Token() 
        : Value(""), Group(""), TokenType(TokenTypes::UNKNOWN), CodeLineNumber(0), CodeLineIndex(0), Length(0)
    {
    }

    std::string GetValue() { return this->Value; }
    std::string GetGroup() { return this->Group; }
    TokenTypes GetTokenType() { return this->TokenType; }
    int GetCodeLineNumber() { return this->CodeLineNumber; }
    int GetCodeLineIndex() { return this->CodeLineIndex; }
    int GetLength() { return this->Length; }

    void SetValue(std::string value) { this->Value = value; }
    void SetGroup(std::string group) { this->Group = group; }
    void SetTokenType(TokenTypes tokenType) { this->TokenType = tokenType; }
    void SetCodeLineNumber(int codeLineNumber) { this->CodeLineNumber = codeLineNumber; }
    void SetCodeLineIndex(int codeLineIndex) { this->CodeLineIndex = codeLineIndex; }
    void SetLength(int length) { this->Length = length; }
    
    static std::string GetTokenTypesStr(TokenTypes);

    static std::map <std::string, TokenTypes> ReservedIDs;

    static std::map <std::string, TokenTypes> ExistingOperators;

    
    bool IsBlockOpeningToken();
    bool IsBracket();
    bool IsOpenBracket();
    bool IsCloseBracket();
    bool IsReservedIdToken();
    bool IsOperation();
    bool IsLogicalOperations();
    bool IsConstant();
    std::string DescriptionString();

};