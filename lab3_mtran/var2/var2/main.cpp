#include <iostream>
#include "Token.h"
#include "LexicalAnalizer.h"
#include <fstream>
#include <list>
#include <string>
#include "TextTable.h"
#include "SyntaxAnalizer.h"
#include "AstNodePrinter.h"

//Функция формирования и вывода таблиц
void PrintTokensDictionary(std::map<std::string, Token> dictionary)
{
	TextTable t('-', '|', '+');

	t.add("TOKEN");
	t.add("DESCRIPTION");
	t.endOfRow();

	for (auto it = dictionary.begin(); it != dictionary.end(); ++it)
	{
		t.add(it->second.GetValue());
		t.add(it->second.DescriptionString());
		t.endOfRow();
	}

	t.setAlignment(2, TextTable::Alignment::RIGHT);
	std::cout << t;
}

//Функция подсчета пробелов в начале строки
int CountSpacesBegin(std::string line)
{
	int count = 0;
	for (auto ch : line)
		if (ch == ' ')
			++count;
		else
			break;
	return count;
}

int main()
{
	std::map<std::string, Token> constants;
	std::map<std::string, Token> variables;
	std::map<std::string, Token> operators;
	std::map<std::string, Token> keywords;

	std::list<LexicalError> lexicalErrors;
	std::list<SyntaxError> syntaxErrors;

	AstNode abstractSyntaxTree(TokenTypes::PROGRAM);

	std::string FILENAME = "C:/Users/Mary/Desktop/lab3_mtran/test2.py";

	//Чтение строк из файла
	std::ifstream file(FILENAME);
	std::string str;
	std::list<std::string> codeLines;
	while (std::getline(file, str))
	{
		codeLines.push_back(str);
	}
	file.close();

	LexicalAnalizer lexicalAnalizer;

	int lineNumber = 0;

	for (auto line : codeLines)
	{
		Construction construction = lexicalAnalizer.AnaliseLine(line, lineNumber);
		if (construction.GetTokens().size() == 0)
		{
			++lineNumber;
			continue;
		}

		//Если def, то заносим название функции в словарь переменных
		Token firstToken = construction.GetTokens().front();
		TokenTypes firstTokenType = firstToken.GetTokenType();
		if (firstTokenType == TokenTypes::FUNCTION_DEFINITION)
		{
			Token secondToken = *std::next(construction.GetTokens().begin(), 1);
			if (secondToken.GetTokenType() == TokenTypes::ID)
			{
				variables.emplace(secondToken.GetValue(), secondToken);
			}
		}

		for (int i = 0; i < construction.GetTokens().size(); ++i)
		{//Обработка остальных строк, запись элементов в словари
			Token token = *std::next(construction.GetTokens().begin(), i);

			if (token.GetTokenType() == TokenTypes::COMMA || token.GetTokenType() == TokenTypes::DOT)
				continue;

			if (token.IsReservedIdToken())
				keywords.emplace(token.GetValue(), token);
			else if (token.IsOperation())
				operators.emplace(token.GetValue(), token);
			else if (token.IsConstant())
				constants.emplace(token.GetValue(), token);
			else if (token.GetTokenType() != TokenTypes::UNKNOWN)
				variables.emplace(token.GetValue(), token);
		}

		//Добавление обнаруженых ошибок в общий список
		auto errorsBuff = construction.GetErrors();
		if (construction.HasErrors())
			lexicalErrors.insert(lexicalErrors.end(), errorsBuff.begin(), errorsBuff.end());

		//std::cout << line << std::endl;
		SyntaxAnalize syntaxAnalize;
		syntaxAnalize.LineAnalize(abstractSyntaxTree, construction, line);
		auto errorsSyntaxBuff = syntaxAnalize.GetErrors();
		if (syntaxAnalize.HasErrors())
			syntaxErrors.insert(syntaxErrors.end(), errorsSyntaxBuff.begin(), errorsSyntaxBuff.end());

		lineNumber++;
	}

	//Вывод ошибок при их наличии
	if (lexicalErrors.size() > 0)
	{
		std::cout << "ERRORS" << std::endl;

		for (LexicalError error : lexicalErrors)
		{
			std::cout << "line " << error.GetCodeLineNumber() + 1 << " char" << error.GetIndexInCodeLine() + 1 << " :: " << LexicalError::GetErrorTypesStr(error.GetErrorType()) << std::endl;
			std::cout << (error.GetDescription()) << std::endl;
		}
	}
	else if (syntaxErrors.size() > 0 && lexicalErrors.size() == 0)
	{
		std::cout << "ERRORS" << std::endl;

		for (SyntaxError error : syntaxErrors)
		{
			std::cout << "line " << error.GetCodeLineNumber() + 1 << " char" << error.GetIndexInCodeLine() + 1 << " :: " << SyntaxError::GetErrorTypesStr(error.GetErrorType()) << std::endl;
			std::cout << (error.GetDescription()) << std::endl;
		}
	}
	else
	{
		AstNodePrinter t;
		t.Print(abstractSyntaxTree);
	}

	//Вывод таблиц
	/*std::cout << "\nCONSTANTS" << std::endl;
	PrintTokensDictionary(constants);

	std::cout << "\nVARIABLES" << std::endl;
	PrintTokensDictionary(variables);

	std::cout << "\nKEYWORS" << std::endl;
	PrintTokensDictionary(keywords);

	std::cout << "\nOPERATORS" << std::endl;
	PrintTokensDictionary(operators);*/

	return 0;
}