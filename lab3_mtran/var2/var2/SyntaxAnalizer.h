#pragma once
#include "AstNode.h"
#include <list>
#include <stack>
#include "LexicalAnalizer.h"

enum class SyntaxErrorTypes {
    COLON_EXPECTED, EXPECTED_NUMBER, EXPECTED_ROUND_BRACKET, EXPECTED_ASSIGN, EXPECTED_OPERATION, EXPECTED_TOKEN, EXPECTED_BRACKET,
    EXPECTED_EXPRESSION, UNEXPECTED_EXPRESSION, EXPECTED_SQUARE_BRACKET, EXPECTED_FUNC_PROPERTY
};

class SyntaxError
{
    SyntaxErrorTypes ErrorType;
    std::string Value;
    int CodeLineNumber;
    int IndexInCodeLine;
    int Length;
    std::string _decription = "Unexpected token";
public:

    SyntaxError(int lineNumber, std::string value, int index, int length, SyntaxErrorTypes errorType)
    {
        this->CodeLineNumber = lineNumber;
        this->Value = value;
        this->IndexInCodeLine = index;
        this->Length = length;
        this->ErrorType = errorType;
    }

    SyntaxError(std::string value, int lineNumber, int index, SyntaxErrorTypes errorType)
    {
        this->CodeLineNumber = lineNumber;
        this->Value = value;
        this->IndexInCodeLine = index;
        this->ErrorType = errorType;
    }

    static std::string GetErrorTypesStr(SyntaxErrorTypes);

    SyntaxErrorTypes GetErrorType() { return this->ErrorType; }
    std::string GetValue() { return this->Value; }
    int GetCodeLineNumber() { return this->CodeLineNumber; }
    int GetIndexInCodeLine() { return this->IndexInCodeLine; }
    int GetLength() { return this->Length; }

    void SetErrorType(SyntaxErrorTypes errorType) { this->ErrorType = errorType; }
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

class SyntaxAnalize
{
    Construction construction;
    AstNode* tree;
    int currentToken;
    std::list<SyntaxError> Errors;
    std::string codeLine;
    std::stack<TokenTypes> brackets;
    bool IsFunc;

public:

    SyntaxAnalize()
        : brackets(std::stack<TokenTypes>()), construction(Construction()), tree(nullptr), currentToken(0), Errors(std::list<SyntaxError>()), codeLine(""), IsFunc(false)
    {
    }

    std::list<SyntaxError> GetErrors() { return this->Errors; }

    void LineAnalize(AstNode&, Construction, std::string);
    void Expr(AstNode&);
    void Comma(AstNode&);
    void LogicBind(AstNode&);
    void Logic(AstNode&);
    void Add(AstNode&);
    void Mult(AstNode&);
    void Group(AstNode&);
    void BracketCheck(Token token);

    bool HasErrors() { return this->Errors.size() > 0; }
    bool IsNotEnd() { return this->currentToken < this->construction.GetTokens().size(); }
};