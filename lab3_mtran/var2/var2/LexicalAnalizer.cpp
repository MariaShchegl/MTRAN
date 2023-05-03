#include "LexicalAnalizer.h"

std::string trim_before(std::string src)
{
    std::string res = src;
    res.erase(res.begin(), std::find_if_not(res.begin(), res.end(), ::isspace));
    return res;
}

std::string trim(std::string src)
{
    std::string res = src;
    res.erase(std::find_if_not(res.rbegin(), res.rend(), ::isspace).base(), res.end());
    res.erase(res.begin(), std::find_if_not(res.begin(), res.end(), ::isspace));
    return res;
}

const std::map <ErrorTypes, std::string> ErrorTypesStr = {
    {ErrorTypes::UNDEFINED_FUNCTION, "UNDEFINED_FUNCTION"},
    {ErrorTypes::UNEXPECTED_TOKEN, "UNEXPECTED_TOKEN"},
};

std::string LexicalError::GetErrorTypesStr(ErrorTypes errorType)
{
    return ErrorTypesStr.at(errorType);
}

std::regex _regex(R"((?:(#.*)|((?:f?\"[^\"\\]+(?:\\.[^\"\\]*)*\"([\s]*)?)|(?:f?'[^'\\]+(?:\\.[^'\\]*)*'([\s]*)?))|([0-9]+\.[0-9]*([\s]*)?)|(\d+([\s]*)?)|((?:[(){}\[\]]([\s]*)?)|(?:[+\-\/*<=>!%:]+([\s]*)?))|(,([\s]*)?)|(\.([\s]*)?)|(\w+([\s]*)?)|(.+\s?)))", std::regex_constants::icase);
std::regex regexString(R"((?:f?\"[^\"\\]+(?:\\.[^\"\\]*)*\"([\s]*)?)|(?:f?'[^'\\]+(?:\\.[^'\\]*)*'([\s]*)?))");
std::regex regexComment("(#.*)");
std::regex regexFloat(R"([0-9]+\.[0-9]*([\s]*)?)");
std::regex regexInt(R"(\d+([\s]*)?)");
std::regex regexOperator(R"([+\-\/*<=>!%(){}\[\]:]+([\s]*)?)");
std::regex regexComma(R"(,([\s]*)?)");
std::regex regexDot(R"(\.([\s]*)?)");
std::regex regexID(R"(\w+([\s]*)?)");
std::regex regexOther(R"(.+\s?)");

//Получение TokenType по match
TokenTypes LexicalAnalizer::GetTokenType(std::string match)
{
    if (std::regex_match(std::string{ match }, regexComment))
        return TokenTypes::COMMENT;
    else if (std::regex_match(std::string{ match }, regexString))
        return TokenTypes::STRING_CONST;
    else if (std::regex_match(std::string{ match }, regexFloat))
        return TokenTypes::FLOAT_NUM;
    else if (std::regex_match(std::string{ match }, regexInt))
        return TokenTypes::INT_NUM;
    else if (std::regex_match(std::string{ match }, regexOperator))
        return GetOperatorTokenType(match);
    else if (std::regex_match(std::string{ match }, regexComma))
        return TokenTypes::COMMA;
    else if (std::regex_match(std::string{ match }, regexDot))
        return TokenTypes::DOT;
    else if (std::regex_match(std::string{ match }, regexID))
        return GetIDTokenType(match);
    else
        return TokenTypes::UNKNOWN;
}

//Получение группы принадлежности match
std::string LexicalAnalizer::GetTokenTypeGroup(std::string match)
{
    if (std::regex_match(std::string{ match }, regexComment))
        return "Comment";
    else if (std::regex_match(std::string{ match }, regexString))
        return "String";
    else if (std::regex_match(std::string{ match }, regexFloat))
        return "Float";
    else if (std::regex_match(std::string{ match }, regexInt))
        return "Integer";
    else if (std::regex_match(std::string{ match }, regexOperator))
        return "Operator";
    else if (std::regex_match(std::string{ match }, regexComma))
        return "Comma";
    else if (std::regex_match(std::string{ match }, regexDot))
        return "Dot";
    else if (std::regex_match(std::string{ match }, regexID))
        return "ID";
    else
        return "Other";
}

//Проверка TokenTypes::Operator на принадлежность к корректным операторам
TokenTypes LexicalAnalizer::GetOperatorTokenType(std::string value)
{
    if (Token::ExistingOperators.contains(value))
    {
        return Token::ExistingOperators.at(value);
    }
    return TokenTypes::UNKNOWN;
}

//Проверка с TokenTypes::ID на принадлежность к зарезервированным словам
TokenTypes LexicalAnalizer::GetIDTokenType(std::string value)
{
    if (Token::ReservedIDs.contains(value))
    {
        return Token::ReservedIDs.at(value);
    }
    return TokenTypes::ID;
}

//Парсинг строки
std::tuple<std::list<Token>, std::list<LexicalError>> LexicalAnalizer::ParseLine(std::string codeLine, int lineNumber)
{
    std::list<Token> tokens;
    std::list<LexicalError> errors;

    std::vector<std::ssub_match> matches;

    //Разбиваем строки по регулярке
    std::copy(std::sregex_token_iterator{ codeLine.begin(), codeLine.end(), _regex }, {}, std::back_inserter(matches));

    for (size_t index = 0; auto match : matches)// for index, match in enumerate(matches)
    {
        if (trim(match).length() == 0)
            break;

        TokenTypes type = GetTokenType(trim(match));

        if (type == TokenTypes::UNKNOWN)
        {//Если тип не определен, то ошибку формируем
            LexicalError error = LexicalError(lineNumber, match, index, match.length(), ErrorTypes::UNEXPECTED_TOKEN);
            error.CreateAndSetDescription(codeLine);
            errors.push_back(error);
        }

        if (type == TokenTypes::COMMENT)
            continue;

        tokens.push_back(Token(trim(match), GetTokenTypeGroup(match), lineNumber, index, match.length(), type));

        index += match.length();
    }

    return std::make_tuple(tokens, errors);
}

//Функция первоначальной подготовки строки
Construction LexicalAnalizer::AnaliseLine(std::string codeLine, int lineNumber)
{
    std::string trimedLine = trim_before(codeLine); //Удаляем пробелы вначале строки и в конце
    int spaces = codeLine.length() - trimedLine.length();
    std::list<Token> tokens;
    std::list<LexicalError> errors;
    tie(tokens, errors) = ParseLine(trim(codeLine), lineNumber);

    return Construction(tokens, errors, spaces);
}