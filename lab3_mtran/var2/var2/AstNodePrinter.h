#pragma once
#include "AstNode.h"

class AstNodePrinter
{
    char ConnectChar = '|';
    char MiddleNodeChar = char(195);
    char LastNodeChar = char(192);

public:

    std::string getStringSubTree(AstNode* node,
        std::string indent, bool root)
    {
        if (node == nullptr)
            return "";
        std::string result = indent;
        if (!root)
            if (node->Index() < node->GetParent()->GetChildCount() - 1)
            {
                result += MiddleNodeChar;
                if (node->GetParent()->GetTokenType() == TokenTypes::PROGRAM)
                    for (int i = 0; i < node->GetIndentation(); ++i)
                        result += ConnectChar;
                result += " ";
                indent += ConnectChar;
                indent += " ";
                if (node->GetParent()->GetTokenType() == TokenTypes::PROGRAM)
                    for (int i = 0; i < node->GetIndentation() - 1; ++i)
                        indent += " ";
            }
            else
            {
                result += LastNodeChar;
                result += " ";
                indent += " ";
            }
        result += node->GetText() + "\n";
        for (int i = 0; i < node->GetChildCount(); i++)
            result += getStringSubTree(node->GetChild(i), indent, false);
        return result;
    }

    std::string astNodeToAdvancedDosStringTree(AstNode& node)
    {

        return getStringSubTree(&node, "", true);
    }

    void Print(AstNode& node)
    {
        std::string tree = astNodeToAdvancedDosStringTree(node);
        std::cout << tree << std::endl;
    }
};