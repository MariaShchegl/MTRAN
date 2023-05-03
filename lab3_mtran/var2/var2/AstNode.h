#pragma once
#include<iostream>
#include "Token.h"
#include<vector>

class AstNode
{
    TokenTypes Type;    // ��� ����
    std::string Text;   // �����, ��������� � �����
    int Indentation;    // ������
    AstNode *parent = nullptr;     // ������������ ���� ��� ������� ���� ������
    std::vector<AstNode*> childs;   // ������� (�����) ������� ���� ������

public:
    
    TokenTypes GetTokenType() { return this->Type; };
    void SetTokenType(TokenTypes type) { this->Type = type; };
       
    std::string GetText() { return this->Text; };
    void SetText(std::string text) { this->Text = text; };

    int GetIndentation() { return this->Indentation; }
    void SetIndentation(int indentation) { this->Indentation = indentation; };
        
    // ����� ��������� ��������� ���� �� �������
    AstNode* GetChild(int index) { return this->childs[index]; }

    std::vector<AstNode*> GetChilds() { return this->childs; }

    // ����� ��������� ���������� �������� �����
    int GetChildCount() { return childs.size(); };

    // ������������ ���� (��������)
    AstNode* GetParent() { return this->parent; };
    void SetParent(AstNode* parent) { parent->AddChild(this); };
    void Setparent(AstNode* parent) { this->parent = parent; };

    // ������������ � ���������� ����������� (��� ��������)
    AstNode(TokenTypes type, std::string text, AstNode* child1, AstNode* child2, int  indentation)
        : Type(type), Text(text), Indentation(indentation)
    {
        this->parent = nullptr;
        if (child1 != nullptr)
            AddChild(child1);
        if (child2 != nullptr)
            AddChild(child2);
    }
    AstNode(TokenTypes type, std::string text, AstNode* child1, AstNode* child2)
        : Type(type), Text(text), Indentation(0)
    {
        this->parent = nullptr;
        if (child1 != nullptr)
            AddChild(child1);
        if (child2 != nullptr)
            AddChild(child2);
    }
    AstNode(TokenTypes type, AstNode *child1, AstNode *child2)
        : AstNode(type, "", child1, child2)
    {
    }
    AstNode(TokenTypes type, std::string text, std::vector<AstNode*> childs, int indentation)
        : AstNode(type, text, indentation)
    {
        for (int i = 0; i < childs.size(); ++i)
        {
            AstNode temp = *childs[i];
                temp.Setparent(nullptr);
            AddChild(&temp);
        }
    }
    AstNode(TokenTypes type, std::string text, AstNode* child1)
        : AstNode(type, text, child1, nullptr)
    {
    }
    AstNode(TokenTypes type, AstNode *child1)
        : AstNode(type, "", child1, nullptr)
    {
    }
    AstNode(TokenTypes type, std::string text, int indentation)
        : AstNode(type, text, nullptr, nullptr, indentation)
    {
    }
    AstNode(TokenTypes type, std::string text)
        : AstNode(type, text, nullptr, nullptr)
    {
    }
    AstNode(TokenTypes type)
        : AstNode(type, "")
    {
    }
    AstNode()
        : AstNode(TokenTypes::UNKNOWN)
    {
    }

    // ����� ���������� ��������� ����
    void AddChild(AstNode* child)
    {
        if (child->GetParent() != nullptr && child->GetParent()->GetTokenType() != TokenTypes::UNKNOWN)
        {
            auto it = std::find(child->GetParent()->childs.begin(), child->GetParent()->childs.end(), child);
            if (it != child->GetParent()->childs.end())
                child->GetParent()->childs.erase(it);
        }
        auto it = std::find(this->childs.begin(), this->childs.end(), child);
        if (it != this->childs.end())
            childs.erase(it);
        AstNode* buff = new AstNode(child->GetTokenType(), child->GetText(), child->GetChilds(), child->GetIndentation());
        buff->Setparent(this);
        childs.push_back(buff);
    }

    // ����� �������� ��������� ����
    void RemoveChild(AstNode *child)
    {
        auto it = std::find(childs.begin(), childs.end(), child);
        if (it != this->childs.end())
            childs.erase(it);
        if (child->GetParent() == this)
            child->Setparent(nullptr);
    }


    // ������ ������� ���� � �������� ����� ������������� ����
    int Index() 
    { 
        auto ptrParent = GetParent();
        if (ptrParent == nullptr)
            return -1;
        else
        {
            auto it = std::find(ptrParent->childs.begin(), ptrParent->childs.end(), this);
            int index = std::distance(ptrParent->childs.begin(), it);
            return index;
        }
    }
};