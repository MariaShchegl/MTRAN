#include "SyntaxAnalizer.h"

const std::map <SyntaxErrorTypes, std::string> SyntaxErrorTypesStr = {
	{SyntaxErrorTypes::COLON_EXPECTED, "SyntaxError COLON_EXPECTED (:)"},
	{SyntaxErrorTypes::EXPECTED_NUMBER, "SyntaxError EXPECTED_NUMBER_OR_VARIABLE"},
	{SyntaxErrorTypes::EXPECTED_ROUND_BRACKET, "SyntaxError EXPECTED_ROUND_BRACKET"},
	{SyntaxErrorTypes::EXPECTED_SQUARE_BRACKET, "SyntaxError EXPECTED_SQUARE_BRACKET"},
	{SyntaxErrorTypes::EXPECTED_ASSIGN, "SyntaxError EXPECTED_ASSIGN"},
	{SyntaxErrorTypes::EXPECTED_OPERATION, "SyntaxError EXPECTED_OPERATION"},
	{SyntaxErrorTypes::EXPECTED_TOKEN, "SyntaxError EXPECTED_TOKEN_OR_OPERATION"},
	{SyntaxErrorTypes::EXPECTED_BRACKET, "SyntaxError EXPECTED_INCORRECT_SEQUENCE_OF_BRACKETS"},
	{SyntaxErrorTypes::EXPECTED_EXPRESSION, "SyntaxError EXPECTED_EXPRESSION"},
	{SyntaxErrorTypes::UNEXPECTED_EXPRESSION, "SyntaxError UNEXPECTED_EXPRESSION"},
	{SyntaxErrorTypes::EXPECTED_FUNC_PROPERTY, "SyntaxError EXPECTED_FUNCTION_OR_PROPERTY"},
};

std::string SyntaxError::GetErrorTypesStr(SyntaxErrorTypes errorType)
{
	return SyntaxErrorTypesStr.at(errorType);
}

void SyntaxAnalize::LineAnalize(AstNode& tree, Construction construction, std::string codeLine)
{
	this->currentToken = 0;
	this->construction = construction;
	this->tree = &tree;
	this->Errors.clear();
	this->codeLine = codeLine;
	while (!this->brackets.empty())
		this->brackets.pop();

	Token firstToken = this->construction.GetTokens().front();
	if (firstToken.IsBlockOpeningToken())
	{//���� ������ ����� ����������� ����� ���� (if, elif, else, def, while, for)
		Token lastToken = this->construction.GetTokens().back();
		if (lastToken.GetTokenType() != TokenTypes::COLON)
		{//���� � ����� ��� : �� ������
			int indexError = lastToken.GetCodeLineIndex() + lastToken.GetLength() + construction.GetIndentation();
			SyntaxError error = SyntaxError(firstToken.GetCodeLineNumber(), firstToken.GetValue(), indexError, firstToken.GetValue().length(), SyntaxErrorTypes::COLON_EXPECTED);
			error.CreateAndSetDescription(codeLine);
			this->Errors.push_back(error);
		}
		else
		{//���������� ������ �� �������
			if(firstToken.GetTokenType() != TokenTypes::FOR)
				while (this->currentToken < this->construction.GetTokens().size())
				{
					AstNode child;
					Expr(child);
					child.SetIndentation(this->construction.GetIndentation());
					tree.AddChild(&child);
					//this->currentToken++;
				}
		}
		
	}
	else
	{//���������� ������ �� �������
		while (this->IsNotEnd() && !this->HasErrors())
		{
			this->IsFunc = false;
			AstNode child;
			Expr(child);
			child.SetIndentation(this->construction.GetIndentation());
			tree.AddChild(&child);
			//this->currentToken++;
		}
	}
}

void SyntaxAnalize::Expr(AstNode& res)
{
	Token token = *std::next(this->construction.GetTokens().begin(), this->currentToken);

	if (token.GetTokenType() == TokenTypes::BUILT_IN_FUNCTION)
	{//���� ����� ����������� � ������� ���������
		this->currentToken++; //������� � ���������� ������
		this->IsFunc = true;
		AstNode value;
		LogicBind(value); //������ �������� ������������
		if (value.GetTokenType() == TokenTypes::UNKNOWN)
		{
			AstNode res2(token.GetTokenType(), token.GetValue());
			res = res2;
		}
		else
		{
			AstNode res2(token.GetTokenType(), token.GetValue(), &value);
			res = res2;
		}
	}
	else if (token.GetTokenType() == TokenTypes::RETURN || token.GetTokenType() == TokenTypes::IMPORT)
	{
		this->currentToken++; //������� � ���������� ������
		AstNode value;
		LogicBind(value); //������ �������� ������������
		if (value.GetTokenType() == TokenTypes::UNKNOWN)
		{
			int indexError = token.GetCodeLineIndex() + token.GetLength() + this->construction.GetIndentation();
			SyntaxError error = SyntaxError(token.GetCodeLineNumber(), token.GetValue(), indexError, token.GetValue().length(), SyntaxErrorTypes::EXPECTED_EXPRESSION);
			error.CreateAndSetDescription(this->codeLine);
			this->Errors.push_back(error);
			AstNode res2(TokenTypes::UNKNOWN);
			res = res2;
		}
		else
		{
			AstNode res2(token.GetTokenType(), token.GetValue(), &value);
			res = res2;
		}
	}
	else if (token.IsBlockOpeningToken())
	{//���� ��� if, elif, else, def, while
		if (token.GetTokenType() == TokenTypes::ELSE)
		{
			this->currentToken++; //������� � ���������� ������
			if (this->IsNotEnd())
			{
				Token tokenNext = *std::next(this->construction.GetTokens().begin(), this->currentToken);
				this->currentToken++;
				if (tokenNext.GetTokenType() == TokenTypes::COLON)
				{
					AstNode res2(token.GetTokenType(), token.GetValue());
					res = res2;
				}
				else
				{
					int indexError = token.GetCodeLineIndex() + token.GetLength() + this->construction.GetIndentation();
					SyntaxError error = SyntaxError(token.GetCodeLineNumber(), token.GetValue(), indexError, token.GetValue().length(), SyntaxErrorTypes::UNEXPECTED_EXPRESSION);
					error.CreateAndSetDescription(this->codeLine);
					this->Errors.push_back(error);
					AstNode res2(TokenTypes::UNKNOWN);
					res = res2;
				}
			}
			else
			{
				int indexError = token.GetCodeLineIndex() + token.GetLength() + this->construction.GetIndentation();
				SyntaxError error = SyntaxError(token.GetCodeLineNumber(), token.GetValue(), indexError, token.GetValue().length(), SyntaxErrorTypes::COLON_EXPECTED);
				error.CreateAndSetDescription(this->codeLine);
				this->Errors.push_back(error);
				AstNode res2(TokenTypes::UNKNOWN);
				res = res2;
			}
		}
		else
		{
			this->currentToken++; //������� � ���������� ������
			AstNode value;
			LogicBind(value); //������ �������� ������������
			if (value.GetTokenType() == TokenTypes::UNKNOWN)
			{
				int indexError = token.GetCodeLineIndex() + token.GetLength() + this->construction.GetIndentation();
				SyntaxError error = SyntaxError(token.GetCodeLineNumber(), token.GetValue(), indexError, token.GetValue().length(), SyntaxErrorTypes::EXPECTED_EXPRESSION);
				error.CreateAndSetDescription(this->codeLine);
				this->Errors.push_back(error);
				AstNode res2(TokenTypes::UNKNOWN);
				res = res2;
			}
			else
			{
				AstNode res2(token.GetTokenType(), token.GetValue(), &value);
				res = res2;
			}
		}
	}
	else if (token.GetTokenType() == TokenTypes::ID)
	{//���� ����� �������������
		AstNode ident(token.GetTokenType(), token.GetValue());
		this->currentToken++;
		if (this->IsNotEnd())
		{
			Token tokenNext = *std::next(this->construction.GetTokens().begin(), this->currentToken);
			if (tokenNext.GetTokenType() == TokenTypes::ASSIGN)
			{ //���� ��������� ����� = 
				this->currentToken++;
				AstNode value;
				LogicBind(value); //������ �������� ������������
				AstNode res2(TokenTypes::ASSIGN, "=", &ident, &value);
				res = res2;

				if (this->IsNotEnd())
				{
					Token tokenLast = *std::next(this->construction.GetTokens().begin(), this->currentToken);
					int indexError = tokenLast.GetCodeLineIndex() + this->construction.GetIndentation();
					SyntaxError error = SyntaxError(tokenLast.GetCodeLineNumber(), tokenLast.GetValue(), indexError, tokenLast.GetValue().length(), SyntaxErrorTypes::EXPECTED_OPERATION);
					error.CreateAndSetDescription(this->codeLine);
					this->Errors.push_back(error);
				}
				else if (value.GetTokenType() == TokenTypes::UNKNOWN)
				{
					int indexError = tokenNext.GetCodeLineIndex() + this->construction.GetIndentation() + 1;
					SyntaxError error = SyntaxError(tokenNext.GetCodeLineNumber(), tokenNext.GetValue(), indexError, tokenNext.GetValue().length(), SyntaxErrorTypes::EXPECTED_EXPRESSION);
					error.CreateAndSetDescription(this->codeLine);
					this->Errors.push_back(error);
				}
			}
			else if (tokenNext.GetTokenType() == TokenTypes::OPENING_ROUND_BRACKET)
			{ //���� ��������� ����� (
				this->IsFunc = true;
				AstNode value;
				LogicBind(value); //������ �������� ������������
				if (value.GetTokenType() == TokenTypes::UNKNOWN)
				{
					AstNode res2(token.GetTokenType(), token.GetValue());
					res = res2;
				}
				else
				{
					AstNode res2(token.GetTokenType(), token.GetValue(), &value);
					res = res2;
				}
			}
			else if (tokenNext.GetTokenType() == TokenTypes::OPENING_SQUARE_BRACKET)
			{ //���� ��������� ����� [
				AstNode value;
				LogicBind(value); //������ �������� ������������
				if (value.GetTokenType() == TokenTypes::UNKNOWN)
				{
					int indexError = tokenNext.GetCodeLineIndex() + this->construction.GetIndentation() + 1;
					SyntaxError error = SyntaxError(token.GetCodeLineNumber(), token.GetValue(), indexError, token.GetValue().length(), SyntaxErrorTypes::EXPECTED_TOKEN);
					error.CreateAndSetDescription(this->codeLine);
					this->Errors.push_back(error);
				}
				else
				{ 
					AstNode res2(token.GetTokenType(), token.GetValue(), &value);
					res = res2;
				}
			}
			else
			{ //�����
				if (this->currentToken >= 2)
				{ //���� �� ������ ������, �� ������ �� ������� ��������� ����� ��������
					Token tokenBefore = *std::next(this->construction.GetTokens().begin(), (this->currentToken - 2));
					if (tokenBefore.IsBracket())
						BracketCheck(tokenBefore);
					else
					{
						int indexError = tokenBefore.GetCodeLineIndex() + this->construction.GetIndentation() - 1;
						SyntaxError error = SyntaxError(tokenBefore.GetCodeLineNumber(), tokenBefore.GetValue(), indexError, tokenBefore.GetValue().length(), SyntaxErrorTypes::EXPECTED_OPERATION);
						error.CreateAndSetDescription(this->codeLine);
						this->Errors.push_back(error);
					}
					AstNode res2(TokenTypes::UNKNOWN);
					res = res2;
				}
				else
				{ //���� ������ ������ (�.�. ������ �����), �� ��������� = �������
					int indexError = tokenNext.GetCodeLineIndex() + this->construction.GetIndentation();
					SyntaxError error = SyntaxError(tokenNext.GetCodeLineNumber(), tokenNext.GetValue(), indexError, tokenNext.GetValue().length(), SyntaxErrorTypes::EXPECTED_ASSIGN);
					error.CreateAndSetDescription(this->codeLine);
					this->Errors.push_back(error);
					AstNode res2(TokenTypes::UNKNOWN);
					res = res2;
				}
			}
		}
		else
		{ //���� � ������ ��������� �����
			if (this->currentToken == 0)
			{
				AstNode res2(ident.GetTokenType(), ident.GetText());
			    res = res2;
		    }
			//����� ������, ��� return f''kskbcjk ����� �������� ������ 
			else 
			{
				int indexError = token.GetCodeLineIndex() + this->construction.GetIndentation();
				SyntaxError error = SyntaxError(token.GetCodeLineNumber(), token.GetValue(), indexError, token.GetValue().length(), SyntaxErrorTypes::EXPECTED_OPERATION);
				error.CreateAndSetDescription(this->codeLine);
				this->Errors.push_back(error);
				AstNode res2(TokenTypes::UNKNOWN);
				res = res2;
			}
		}
	}
	else if (token.GetTokenType() == TokenTypes::INT_NUM || token.GetTokenType() == TokenTypes::FLOAT_NUM || token.GetTokenType() == TokenTypes::STRING_CONST)
	{
		AstNode ident(token.GetTokenType(), token.GetValue());
		this->currentToken++;
		if (this->IsNotEnd())
		{
			Token tokenNext = *std::next(this->construction.GetTokens().begin(), this->currentToken);
			if (tokenNext.GetTokenType() == TokenTypes::OPENING_SQUARE_BRACKET && token.GetTokenType() == TokenTypes::STRING_CONST)
			{ //���� ��������� ����� [ � ������� ������
				AstNode value;
				LogicBind(value); //������ �������� ������������
				if (value.GetTokenType() == TokenTypes::UNKNOWN)
				{
					int indexError = tokenNext.GetCodeLineIndex() + this->construction.GetIndentation() + 1;
					SyntaxError error = SyntaxError(token.GetCodeLineNumber(), token.GetValue(), indexError, token.GetValue().length(), SyntaxErrorTypes::EXPECTED_TOKEN);
					error.CreateAndSetDescription(this->codeLine);
					this->Errors.push_back(error);
				}
				else
				{
					AstNode res2(token.GetTokenType(), token.GetValue(), &value);
					res = res2;
				}
			}
			else
			{
				int indexError = tokenNext.GetCodeLineIndex() + this->construction.GetIndentation() + 1;
				SyntaxError error = SyntaxError(token.GetCodeLineNumber(), token.GetValue(), indexError, token.GetValue().length(), SyntaxErrorTypes::UNEXPECTED_EXPRESSION);
				error.CreateAndSetDescription(this->codeLine);
				this->Errors.push_back(error);
			}
		}
	}
	else
	{
		int countError = this->Errors.size();
		BracketCheck(token);
		this->currentToken++;
		if (this->Errors.size() == countError)
		{
			int indexError = token.GetCodeLineIndex() + this->construction.GetIndentation();
			SyntaxError error = SyntaxError(token.GetCodeLineNumber(), token.GetValue(), indexError, token.GetValue().length(), SyntaxErrorTypes::EXPECTED_EXPRESSION);
			error.CreateAndSetDescription(this->codeLine);
			this->Errors.push_back(error);
		}
	}
}

void SyntaxAnalize::Comma(AstNode& res)
{
	AstNode* result = new AstNode();
	LogicBind(*result); //����� �������� �� or and
	if (this->IsNotEnd())
	{
		Token token = *std::next(this->construction.GetTokens().begin(), this->currentToken);
		while (token.GetTokenType() == TokenTypes::COMMA)
		{
			this->currentToken++;
			AstNode temp;
			LogicBind(temp); //����� �������� �� < > <= >= == !=

			result = new AstNode(token.GetTokenType(), token.GetValue(), result, &temp);
			if (this->IsNotEnd())
				token = *std::next(this->construction.GetTokens().begin(), this->currentToken);
			else
				break;
		}
	}
	res = *result;
}

void SyntaxAnalize::LogicBind(AstNode& res)
{ //�������� �� and or
	AstNode* result = new AstNode();
	Logic(*result); //����� �������� �� < > <= >= == !=
	if (this->IsNotEnd())
	{
		Token token = *std::next(this->construction.GetTokens().begin(), this->currentToken);
		if (token.GetTokenType() == TokenTypes::COLON)
		{//�������� �� ����� :
			this->currentToken++;
		}
		while (token.GetTokenType() == TokenTypes::OR || token.GetTokenType() == TokenTypes::AND)
		{
			this->currentToken++;
			AstNode temp;
			Logic(temp); //����� �������� �� < > <= >= == !=

			result = new AstNode(token.GetTokenType(), token.GetValue(), result, &temp);
			if (this->IsNotEnd())
				token = *std::next(this->construction.GetTokens().begin(), this->currentToken);
			else
				break;
		}
	}
	res = *result;
}

void SyntaxAnalize::Logic(AstNode& res)
{ ///�������� �� < > <= >= == !=
	AstNode* result = new AstNode();
	Add(*result); //����� �������� �� + -
	if (this->IsNotEnd())
	{
		Token token = *std::next(this->construction.GetTokens().begin(), this->currentToken);
		while (token.IsLogicalOperations())
		{
			this->currentToken++;
			AstNode temp;
			Add(temp); //����� �������� �� + -

			result = new AstNode(token.GetTokenType(), token.GetValue(), result, &temp);
			if (this->IsNotEnd())
				token = *std::next(this->construction.GetTokens().begin(), this->currentToken);
			else
				break;
		}
	}
	res = *result;
}

void SyntaxAnalize::Add(AstNode& res)
{ //�������� �� + -
	AstNode *result = new AstNode();
	Mult(*result); //����� �������� �� * / %
	if (this->IsNotEnd())
	{
		Token token = *std::next(this->construction.GetTokens().begin(), this->currentToken);
		while (token.GetTokenType() == TokenTypes::PLUS || token.GetTokenType() == TokenTypes::MINUS)
		{
			this->currentToken++;
			AstNode temp;
			Mult(temp); //����� �������� �� * / %

			result = new AstNode(token.GetTokenType(), token.GetValue(), result, &temp);
			if (this->IsNotEnd())
				token = *std::next(this->construction.GetTokens().begin(), this->currentToken);
			else
				break;
		}
	}
	res = *result;
}

void SyntaxAnalize::Mult(AstNode& res)
{//�������� �� * / %
	AstNode *result = new AstNode();
	Group(*result); //����� �������� �� ( � ��������� ���� �������
	if (this->IsNotEnd())
	{
		Token token = *std::next(this->construction.GetTokens().begin(), this->currentToken);
		while (token.GetTokenType() == TokenTypes::MULTIPLICATION || token.GetTokenType() == TokenTypes::DIVISION || token.GetTokenType() == TokenTypes::MODULE)
		{
			this->currentToken++;
			AstNode temp;
			Group(temp); //����� �������� �� ( � ��������� ���� �������
			result = new AstNode(token.GetTokenType(), token.GetValue(), result, &temp);
			if (this->IsNotEnd())
				token = *std::next(this->construction.GetTokens().begin(), this->currentToken);
			else
				break;
		}
	}
	res = *result;
}

void SyntaxAnalize::Group(AstNode& res)
{
	if (this->IsNotEnd())
	{
		Token token = *std::next(this->construction.GetTokens().begin(), this->currentToken);

		if (token.GetTokenType() == TokenTypes::COLON)
		{//�������� �� ����� :
			this->currentToken++;

			AstNode res2(TokenTypes::UNKNOWN);
			res = res2;
		}
		else if (token.GetTokenType() == TokenTypes::OPENING_ROUND_BRACKET)
		{//�������� �� ����� (
			BracketCheck(token);
			this->currentToken++;
			AstNode result;

			if (this->IsNotEnd())
			{
				Token tokenNext = *std::next(this->construction.GetTokens().begin(), this->currentToken);
				if (tokenNext.GetTokenType() != TokenTypes::CLOSING_ROUND_BRACKET)
				{
					this->IsFunc = false;
					Comma(result); //LogicBind
				}
			}

			if (this->IsNotEnd())
			{
				Token tokenNext = *std::next(this->construction.GetTokens().begin(), this->currentToken);
				if (tokenNext.GetTokenType() != TokenTypes::CLOSING_ROUND_BRACKET)
				{
					int indexError = token.GetCodeLineIndex() + this->construction.GetIndentation();
					SyntaxError error = SyntaxError(token.GetCodeLineNumber(), token.GetValue(), indexError, token.GetValue().length(), SyntaxErrorTypes::EXPECTED_ROUND_BRACKET);
					error.CreateAndSetDescription(this->codeLine);
					this->Errors.push_back(error);
				}
				BracketCheck(tokenNext);
			}
			else
			{
				while (!this->IsNotEnd())
					this->currentToken--;
				Token tokenLast = *std::next(this->construction.GetTokens().begin(), this->currentToken);
				int indexError = tokenLast.GetCodeLineIndex() + tokenLast.GetLength() + this->construction.GetIndentation();
				SyntaxError error = SyntaxError(tokenLast.GetCodeLineNumber(), tokenLast.GetValue(), indexError, tokenLast.GetValue().length(), SyntaxErrorTypes::EXPECTED_ROUND_BRACKET);
				error.CreateAndSetDescription(this->codeLine);
				this->Errors.push_back(error);
			}
			this->currentToken++;
			res = result;
		}
		else if (token.GetTokenType() == TokenTypes::OPENING_SQUARE_BRACKET)
		{ //�������� �� ����� [
			BracketCheck(token);
			this->currentToken++;
			AstNode result;

			if (this->IsNotEnd())
			{
				Token tokenNext = *std::next(this->construction.GetTokens().begin(), this->currentToken);
				if (tokenNext.GetTokenType() != TokenTypes::CLOSING_SQUARE_BRACKET)
				{
					Comma(result); //LogicBind
				}
			}

			if (this->IsNotEnd())
			{
				Token tokenNext = *std::next(this->construction.GetTokens().begin(), this->currentToken);
				if (tokenNext.GetTokenType() != TokenTypes::CLOSING_SQUARE_BRACKET)
				{
					int indexError = token.GetCodeLineIndex() + this->construction.GetIndentation();
					SyntaxError error = SyntaxError(token.GetCodeLineNumber(), token.GetValue(), indexError, token.GetValue().length(), SyntaxErrorTypes::EXPECTED_SQUARE_BRACKET);
					error.CreateAndSetDescription(this->codeLine);
					this->Errors.push_back(error);
				}
				BracketCheck(tokenNext);
			}
			else
			{
				while (!this->IsNotEnd())
					this->currentToken--;
				Token tokenLast = *std::next(this->construction.GetTokens().begin(), this->currentToken);
				int indexError = tokenLast.GetCodeLineIndex() + tokenLast.GetLength() + this->construction.GetIndentation();
				SyntaxError error = SyntaxError(tokenLast.GetCodeLineNumber(), tokenLast.GetValue(), indexError, tokenLast.GetValue().length(), SyntaxErrorTypes::EXPECTED_SQUARE_BRACKET);
				error.CreateAndSetDescription(this->codeLine);
				this->Errors.push_back(error);
			}
			this->currentToken++;
			res = result;
		}
		else if (this->IsFunc)
		{ //���� ���������� ����� ��� �������� � ��� (
			int indexError = token.GetCodeLineIndex() + this->construction.GetIndentation();
			SyntaxError error = SyntaxError(token.GetCodeLineNumber(), token.GetValue(), indexError, token.GetValue().length(), SyntaxErrorTypes::EXPECTED_ROUND_BRACKET);
			error.CreateAndSetDescription(this->codeLine);
			this->Errors.push_back(error);
			AstNode res2(TokenTypes::UNKNOWN);
			res = res2;
		}
		else if (token.GetTokenType() == TokenTypes::ID || token.GetTokenType() == TokenTypes::INT_NUM || token.GetTokenType() == TokenTypes::FLOAT_NUM || token.GetTokenType() == TokenTypes::STRING_CONST)
		{ //�������� �� ����� ������ �������/����������, ������ ��� �������
			AstNode res2(token.GetTokenType(), token.GetValue());
			this->currentToken++;

			if (this->IsNotEnd())
			{
				Token tokenNext = *std::next(this->construction.GetTokens().begin(), this->currentToken);
				if (tokenNext.GetTokenType() == TokenTypes::OPENING_ROUND_BRACKET)
				{
					this->IsFunc = true;
					AstNode result;
					LogicBind(result);
					if (result.GetTokenType() != TokenTypes::UNKNOWN)
						res2.AddChild(&result);
				}
				else if (tokenNext.GetTokenType() == TokenTypes::DOT)
				{
					this->currentToken++;
					AstNode result;
					LogicBind(result);
					if (result.GetTokenType() != TokenTypes::UNKNOWN)
						res2.AddChild(&result);
					else
					{
						int indexError = token.GetCodeLineIndex() + token.GetLength() + this->construction.GetIndentation() + 1;
						SyntaxError error = SyntaxError(token.GetCodeLineNumber(), token.GetValue(), indexError, token.GetValue().length(), SyntaxErrorTypes::EXPECTED_FUNC_PROPERTY);
						error.CreateAndSetDescription(this->codeLine);
						this->Errors.push_back(error);
					}
				}
			}

			res = res2;
		}
		else if (token.GetTokenType() == TokenTypes::BUILT_IN_FUNCTION)
		{ //���� ����� ���������� �������
			AstNode res2(token.GetTokenType(), token.GetValue());
			this->currentToken++;
			if (this->IsNotEnd())
			{
				AstNode result;
				this->IsFunc = true;
				LogicBind(result);
				if (result.GetTokenType() == TokenTypes::UNKNOWN)
				{
					res = res2;
				}
				else
				{
					AstNode res3(res2.GetTokenType(), res2.GetText(), &result);
					res = res3;
				}
			}
			else
			{ //���� ��� ���������� ������
				while (!this->IsNotEnd())
					this->currentToken--;
				Token tokenLast = *std::next(this->construction.GetTokens().begin(), this->currentToken);
				int indexError = tokenLast.GetCodeLineIndex() + tokenLast.GetLength() + this->construction.GetIndentation();
				SyntaxError error = SyntaxError(tokenLast.GetCodeLineNumber(), tokenLast.GetValue(), indexError, tokenLast.GetValue().length(), SyntaxErrorTypes::EXPECTED_ROUND_BRACKET);
				error.CreateAndSetDescription(this->codeLine);
				this->Errors.push_back(error);
				res = res2;
			}
		}
		else if (token.GetTokenType() == TokenTypes::NOT)
		{
			this->currentToken++;
			AstNode result;
			LogicBind(result);
			if (result.GetTokenType() == TokenTypes::UNKNOWN)
			{
				int indexError = token.GetCodeLineIndex() + this->construction.GetIndentation();
				SyntaxError error = SyntaxError(token.GetCodeLineNumber(), token.GetValue(), indexError, token.GetValue().length(), SyntaxErrorTypes::EXPECTED_EXPRESSION);
				error.CreateAndSetDescription(this->codeLine);
				this->Errors.push_back(error);
				AstNode res2(TokenTypes::UNKNOWN);
				res = res2;
			}
			else
			{
				AstNode res2(token.GetTokenType(), token.GetValue(), &result);
				res = res2;
			}
		}
		else
		{
			BracketCheck(token);
			if (token.GetTokenType() == TokenTypes::MINUS)
			{
				this->currentToken++;
				if (this->IsNotEnd())
				{
					Token tokenNext = *std::next(this->construction.GetTokens().begin(), this->currentToken);
					if (tokenNext.GetTokenType() == TokenTypes::INT_NUM || tokenNext.GetTokenType() == TokenTypes::FLOAT_NUM || tokenNext.GetTokenType() == TokenTypes::ID)
					{
						this->currentToken++;
						AstNode res2(tokenNext.GetTokenType(), "-"+tokenNext.GetValue());
						res = res2;
					}
					else
					{
						int indexError = token.GetCodeLineIndex() + this->construction.GetIndentation();
						SyntaxError error = SyntaxError(token.GetCodeLineNumber(), token.GetValue(), indexError, token.GetValue().length(), SyntaxErrorTypes::EXPECTED_NUMBER);
						error.CreateAndSetDescription(this->codeLine);
						this->Errors.push_back(error);
						AstNode res2(TokenTypes::UNKNOWN);
						res = res2;
					}
				}
				else 
				{
					int indexError = token.GetCodeLineIndex() + this->construction.GetIndentation();
					SyntaxError error = SyntaxError(token.GetCodeLineNumber(), token.GetValue(), indexError, token.GetValue().length(), SyntaxErrorTypes::EXPECTED_NUMBER);
					error.CreateAndSetDescription(this->codeLine);
					this->Errors.push_back(error);
					AstNode res2(TokenTypes::UNKNOWN);
					res = res2;
				}
			}
			else
			{
				int indexError = token.GetCodeLineIndex() + this->construction.GetIndentation();
				SyntaxError error = SyntaxError(token.GetCodeLineNumber(), token.GetValue(), indexError, token.GetValue().length(), SyntaxErrorTypes::EXPECTED_NUMBER);
				error.CreateAndSetDescription(this->codeLine);
				this->Errors.push_back(error);
				AstNode res2(TokenTypes::UNKNOWN);
				res = res2;
			}
		}
	}
	else
	{
	AstNode res2(TokenTypes::UNKNOWN);
	res = res2;
	}
}

void SyntaxAnalize::BracketCheck(Token token)
{
	if (token.IsBracket())
	{
		if (token.IsOpenBracket())
		{
			this->brackets.push(token.GetTokenType());
		}
		else
		{
			if (this->brackets.size() > 0)
			{
				auto br = this->brackets.top();
				if (!((br == TokenTypes::OPENING_CURLY_BRACKET && token.GetTokenType() == TokenTypes::CLOSING_CURLY_BRACKET)
					|| (br == TokenTypes::OPENING_ROUND_BRACKET && token.GetTokenType() == TokenTypes::CLOSING_ROUND_BRACKET)
					|| (br == TokenTypes::OPENING_SQUARE_BRACKET && token.GetTokenType() == TokenTypes::CLOSING_SQUARE_BRACKET)))
				{
					int indexError = token.GetCodeLineIndex() + this->construction.GetIndentation();
					SyntaxError error = SyntaxError(token.GetCodeLineNumber(), token.GetValue(), indexError, token.GetValue().length(), SyntaxErrorTypes::EXPECTED_BRACKET);
					error.CreateAndSetDescription(this->codeLine);
					this->Errors.push_back(error);
				}
				this->brackets.pop();
			}
			else
			{
				int indexError = token.GetCodeLineIndex() + this->construction.GetIndentation();
				SyntaxError error = SyntaxError(token.GetCodeLineNumber(), token.GetValue(), indexError, token.GetValue().length(), SyntaxErrorTypes::EXPECTED_BRACKET);
				error.CreateAndSetDescription(this->codeLine);
				this->Errors.push_back(error);
			}
		}
	}
}



/*Token tokenNext = *std::next(this->construction.GetTokens().begin(), this->currentToken);
			if (tokenNext.GetTokenType() == TokenTypes::OPENING_ROUND_BRACKET)
			{
				this->currentToken++;
				if (this->IsNotEnd())
				{
					Token tokenNext3 = *std::next(this->construction.GetTokens().begin(), this->currentToken);
					if (tokenNext3.GetTokenType() == TokenTypes::CLOSING_ROUND_BRACKET)
					{
						res = res2;
					}
					else
					{
						AstNode result;
						Add(result);
						if (this->IsNotEnd())
						{
							Token tokenNext2 = *std::next(this->construction.GetTokens().begin(), this->currentToken);
							if (tokenNext2.GetTokenType() != TokenTypes::CLOSING_ROUND_BRACKET)
							{
								int indexError = token.GetCodeLineIndex() + this->construction.GetIndentation();
								SyntaxError error = SyntaxError(token.GetCodeLineNumber(), token.GetValue(), indexError, token.GetValue().length(), SyntaxErrorTypes::EXPECTED_ROUND_BRACKET);
								error.CreateAndSetDescription(this->codeLine);
								this->Errors.push_back(error);
							}
						}
						else
						{
							while (!this->IsNotEnd())
								this->currentToken--;
							Token tokenLast = *std::next(this->construction.GetTokens().begin(), this->currentToken);
							int indexError = tokenLast.GetCodeLineIndex() + tokenLast.GetLength() + this->construction.GetIndentation();
							SyntaxError error = SyntaxError(tokenLast.GetCodeLineNumber(), tokenLast.GetValue(), indexError, tokenLast.GetValue().length(), SyntaxErrorTypes::EXPECTED_ROUND_BRACKET);
							error.CreateAndSetDescription(this->codeLine);
							this->Errors.push_back(error);
						}

						this->currentToken++;
						AstNode res3(res2.GetTokenType(), res2.GetText(), &result);
						res = res3;
					}
				}
				else
				{
					while (!this->IsNotEnd())
						this->currentToken--;
					Token tokenLast = *std::next(this->construction.GetTokens().begin(), this->currentToken);
					int indexError = tokenLast.GetCodeLineIndex() + tokenLast.GetLength() + this->construction.GetIndentation();
					SyntaxError error = SyntaxError(tokenLast.GetCodeLineNumber(), tokenLast.GetValue(), indexError, tokenLast.GetValue().length(), SyntaxErrorTypes::EXPECTED_ROUND_BRACKET);
					error.CreateAndSetDescription(this->codeLine);
					this->Errors.push_back(error);
					res = res2;
				}
			}*/