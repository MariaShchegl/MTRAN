#pragma once
#include "Token.h"
#include<list>
#include <regex>

enum class ErrorTypes {
    UNEXPECTED_TOKEN, UNDEFINED_FUNCTION
};

class LexicalError
{
    ErrorTypes ErrorType;
    std::string Value;
    int CodeLineNumber;
    int IndexInCodeLine;
    int Length;
    std::string _decription = "Unexpected token";
public:

    LexicalError(int lineNumber, std::string value, int index, int length, ErrorTypes errorType)
    {
        this->CodeLineNumber = lineNumber;
        this->Value = value;
        this->IndexInCodeLine = index;
        this->Length = length;
        this->ErrorType = errorType;
    }

    LexicalError(std::string value, int lineNumber, int index, ErrorTypes errorType)
    {
        this->CodeLineNumber = lineNumber;
        this->Value = value;
        this->IndexInCodeLine = index;
        this->ErrorType = errorType;
    }

    static std::string GetErrorTypesStr(ErrorTypes);

    ErrorTypes GetErrorType() { return this->ErrorType; }
    std::string GetValue() { return this->Value; }
    int GetCodeLineNumber() { return this->CodeLineNumber; }
    int GetIndexInCodeLine() { return this->IndexInCodeLine; }
    int GetLength() { return this->Length; }

    void SetErrorType(ErrorTypes errorType) { this->ErrorType = errorType; }
    void SetValue(std::string value) { this->Value = value; }
    void SetCodeLineNumber(int codeLineNumber) { this->CodeLineNumber = codeLineNumber; }
    void SetIndexInCodeLine(int indexInCodeLine) { this->IndexInCodeLine = indexInCodeLine; }
    void SetLength(int length) { this->Length = length; }

    std::string GetDescription() { return this->_decription; }

    void CreateAndSetDescription(std::string codeLine)
    {
        std::string str1 = codeLine;
        str1.append("\n");
        str1.append(std::string(this->IndexInCodeLine, ' '));
        str1.append("^");
        this->_decription = str1;
    }
};

class Construction
{
    std::list<Token> Tokens;
    std::list<LexicalError> Errors;
    int Indentation;
public:
    Construction()
        : Indentation(0), Tokens(std::list<Token>()), Errors(std::list<LexicalError>())
    {
    }

    Construction(std::list<Token> tokens, std::list<LexicalError> errors, int indentation) {
        this->Tokens = tokens;
        this->Errors = errors;
        this->Indentation = indentation;
    }

    std::list<Token> GetTokens() { return this->Tokens; }
    std::list<LexicalError> GetErrors() { return this->Errors; }

    void SetTokens(std::list<Token> tokens) { this->Tokens = tokens; }
    void SetErrors(std::list<LexicalError> errors) { this->Errors = errors; }

    int GetIndentation() { return this->Indentation; }

    void SetIndentation(int indentation) { this->Indentation = indentation; }

    bool HasErrors() { return this->Errors.size() > 0; }
};

class LexicalAnalizer
{
    std::list<Token> _tokens;
    std::list<LexicalError> _error;

public:
    std::tuple<std::list<Token>, std::list<LexicalError>> ParseLine(std::string, int);

    Construction AnaliseLine(std::string, int);

    static TokenTypes GetTokenType(std::string);

    static std::string GetTokenTypeGroup(std::string);

    static TokenTypes GetOperatorTokenType(std::string);

    static TokenTypes GetIDTokenType(std::string);

};