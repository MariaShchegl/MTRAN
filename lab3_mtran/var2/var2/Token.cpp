#include "Token.h"

//Словарь зарезервированных слов
std::map <std::string, TokenTypes> Token::ReservedIDs = {
        {"and", TokenTypes::AND},
        {"or", TokenTypes::OR},
        {"not", TokenTypes::NOT},
        {"for", TokenTypes::FOR},
        {"in", TokenTypes::IN},
        {"while", TokenTypes::WHILE},
        {"if", TokenTypes::IF},
        {"elif", TokenTypes::ELIF},
        {"else", TokenTypes::ELSE},
        {"raise", TokenTypes::RAISE},
        {"import", TokenTypes::IMPORT},
        {"def", TokenTypes::FUNCTION_DEFINITION},
        {"return", TokenTypes::RETURN},
        {"print", TokenTypes::BUILT_IN_FUNCTION},
        {"input", TokenTypes::BUILT_IN_FUNCTION},
        {"range", TokenTypes::BUILT_IN_FUNCTION},
        {"type", TokenTypes::BUILT_IN_FUNCTION},
        {"abs", TokenTypes::BUILT_IN_FUNCTION},
        {"max", TokenTypes::BUILT_IN_FUNCTION},
        {"min", TokenTypes::BUILT_IN_FUNCTION},
        {"int", TokenTypes::BUILT_IN_FUNCTION},
        {"float", TokenTypes::BUILT_IN_FUNCTION},
        {"count", TokenTypes::BUILT_IN_FUNCTION},
        {"len", TokenTypes::BUILT_IN_FUNCTION},
        {"str", TokenTypes::BUILT_IN_FUNCTION},
        {"list", TokenTypes::BUILT_IN_FUNCTION},
};

//Словарь операторов
std::map <std::string, TokenTypes> Token::ExistingOperators = {
        {"=", TokenTypes::ASSIGN},
        {"+", TokenTypes::PLUS},
        {"-", TokenTypes::MINUS},
        {"/", TokenTypes::DIVISION},
        {"*", TokenTypes::MULTIPLICATION},
        {"%", TokenTypes::MODULE},
        {":", TokenTypes::COLON},
        {"<", TokenTypes::LOWER},
        {"<=", TokenTypes::LOWER_OR_EQUAL},
        {">", TokenTypes::GREATER},
        {">=", TokenTypes::GREATER_OR_EQUAL},
        {"==", TokenTypes::EQUAL},
        {"!=", TokenTypes::NOT_EQUAL},
        {"(", TokenTypes::OPENING_ROUND_BRACKET},
        {")", TokenTypes::CLOSING_ROUND_BRACKET},
        {"[", TokenTypes::OPENING_SQUARE_BRACKET},
        {"]", TokenTypes::CLOSING_SQUARE_BRACKET},
        {"{", TokenTypes::OPENING_CURLY_BRACKET},
        {"}", TokenTypes::CLOSING_CURLY_BRACKET},
        {".", TokenTypes::DOT},
};

//Словарь соотношения типов токенов и строк
const std::map <TokenTypes, std::string> TokenTypesStr = {
    {TokenTypes::UNKNOWN, "UNKNOWN"},
    {TokenTypes::COMMENT, "COMMENT"},
    {TokenTypes::STRING_CONST, "STRING_CONST"},
    {TokenTypes::FLOAT_NUM, "FLOAT_NUM"},
    {TokenTypes::INT_NUM, "INT_NUM"},
    {TokenTypes::ID, "ID"},
    {TokenTypes::DOT, "DOT"},
    {TokenTypes::COLON, "COLON"},
    {TokenTypes::COMMA, "COMMA"},
    {TokenTypes::OPENING_SQUARE_BRACKET, "OPENING_SQUARE_BRACKET"},
    {TokenTypes::CLOSING_SQUARE_BRACKET, "CLOSING_SQUARE_BRACKET"},
    {TokenTypes::OPENING_ROUND_BRACKET, "OPENING_ROUND_BRACKET"},
    {TokenTypes::CLOSING_ROUND_BRACKET, "CLOSING_ROUND_BRACKET"},
    {TokenTypes::OPENING_CURLY_BRACKET, "OPENING_CURLY_BRACKET"},
    {TokenTypes::CLOSING_CURLY_BRACKET, "CLOSING_CURLY_BRACKET"},
    {TokenTypes::AND, "AND"},
    {TokenTypes::OR, "OR"},
    {TokenTypes::NOT, "NOT"},
    {TokenTypes::ASSIGN, "ASSIGN"},
    {TokenTypes::PLUS, "PLUS"},
    {TokenTypes::MINUS, "MINUS"},
    {TokenTypes::DIVISION, "DIVISION"},
    {TokenTypes::MULTIPLICATION, "MULTIPLICATION"},
    {TokenTypes::MODULE, "MODULE"},
    {TokenTypes::GREATER, "GREATER"},
    {TokenTypes::GREATER_OR_EQUAL, "GREATER_OR_EQUAL"},
    {TokenTypes::LOWER, "LOWER"},
    {TokenTypes::LOWER_OR_EQUAL, "LOWER_OR_EQUAL"},
    {TokenTypes::EQUAL, "EQUAL"},
    {TokenTypes::NOT_EQUAL, "NOT_EQUAL"},
    {TokenTypes::FOR, "FOR"},
    {TokenTypes::IN, "IN"},
    {TokenTypes::WHILE, "WHILE"},
    {TokenTypes::IF, "IF"},
    {TokenTypes::ELIF, "ELIF"},
    {TokenTypes::ELSE, "ELSE"},
    {TokenTypes::BUILT_IN_FUNCTION, "BUILT_IN_FUNCTION"},
    {TokenTypes::RAISE, "RAISE"},
    {TokenTypes::RETURN, "RETURN"},
    {TokenTypes::IMPORT, "IMPORT"},
    {TokenTypes::FUNCTION_DEFINITION, "FUNCTION_DEFINITION"},
    {TokenTypes::PROGRAM, "PROGRAM"}
};

//Функция получения строки по типу токена
std::string Token::GetTokenTypesStr(TokenTypes tokenType)
{
    return TokenTypesStr.at(tokenType);
}

//Проверка на принадлежность типа токена к зарезервированным словам
bool Token::IsReservedIdToken() {
    return std::find_if(ReservedIDs.begin(), ReservedIDs.end(), [v = this->TokenType](const auto& mo) {return mo.second == v; }) != ReservedIDs.end();
}

//Проверка на принадлежность типа токена к операторам
bool Token::IsOperation() {
    return std::find_if(ExistingOperators.begin(), ExistingOperators.end(), [v = this->TokenType](const auto& mo) {return mo.second == v; }) != ExistingOperators.end();
}

//Проверка на принадлежность типа токена к константам
bool Token::IsConstant() {
    return this->TokenType == TokenTypes::STRING_CONST
        || this->TokenType == TokenTypes::INT_NUM
        || this->TokenType == TokenTypes::FLOAT_NUM;
}

bool Token::IsBlockOpeningToken()
{
    return this->TokenType == TokenTypes::IF
        || this->TokenType == TokenTypes::ELIF
        || this->TokenType == TokenTypes::ELSE
        || this->TokenType == TokenTypes::FOR
        || this->TokenType == TokenTypes::WHILE
        || this->TokenType == TokenTypes::FUNCTION_DEFINITION;
}

bool Token::IsCloseBracket()
{
    return this->TokenType == TokenTypes::CLOSING_CURLY_BRACKET
        || this->TokenType == TokenTypes::CLOSING_ROUND_BRACKET
        || this->TokenType == TokenTypes::CLOSING_SQUARE_BRACKET;
}

bool Token::IsOpenBracket()
{
    return this->TokenType == TokenTypes::OPENING_CURLY_BRACKET
        || this->TokenType == TokenTypes::OPENING_ROUND_BRACKET
        || this->TokenType == TokenTypes::OPENING_SQUARE_BRACKET;
}

bool Token::IsBracket()
{
    return this->IsOpenBracket() || this->IsCloseBracket();
}

bool Token::IsLogicalOperations()
{
    return this->TokenType == TokenTypes::LOWER
        || this->TokenType == TokenTypes::LOWER_OR_EQUAL
        || this->TokenType == TokenTypes::GREATER_OR_EQUAL
        || this->TokenType == TokenTypes::GREATER
        || this->TokenType == TokenTypes::EQUAL
        || this->TokenType == TokenTypes::NOT_EQUAL;
}

//Функция описания токена для таблицы
std::string Token::DescriptionString() {
    if (this->IsReservedIdToken() && !this->IsOperation())
        return "Reserved keyword " + Token::GetTokenTypesStr(this->TokenType);

    if (this->IsOperation())
        return "Operation " + Token::GetTokenTypesStr(this->TokenType);

    if (this->IsConstant())
        return Token::GetTokenTypesStr(this->TokenType) + " constant";

    if (this->TokenType == TokenTypes::COMMENT)
        return "is # comment";

    return "is " + Token::GetTokenTypesStr(this->TokenType);
}