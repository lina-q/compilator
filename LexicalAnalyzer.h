#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include "HashTable.h"
#include "Token.h"

// ����������� ����������
class LexicalAnalyzer
{
public:
    // ����������� � ��������� ����� ����� � ���-�������
    LexicalAnalyzer(const std::string& filename, HashTable& hashTable)
        : position(0), hashTable(hashTable) // ������������� ������� � ������ �� ���-�������
    {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: ���� �� ������ " << filename << std::endl;
            return;
        }
        input.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
    }

    // �����������
    void tokenize()
    {
        Token lastToken = { TokenType::UNKNOWN, "" }; // ������ ��������� �����
       

        while (position < input.size())
        {
            char currentChar = input[position];

            if (currentChar == '\n') ++line;
            
            // ���� ��� ������
            if (isspace(currentChar))
            {
                ++position;
                continue;
            }

            // ��������� �� �����
            if (isalpha(currentChar))
            {
                Token currentToken = parseIdentifierOrKeyword(); 
                hashTable.insert(currentToken); 
                tokens.push_back(std::make_pair(currentToken, line));
                lastToken = currentToken;
            }
            else if (isdigit(currentChar))
            {
                Token currentToken = parseNumber();
                hashTable.insert(currentToken);
                tokens.push_back(std::make_pair(currentToken, line));
                lastToken = currentToken;
            }
            else
            {
                Token currentToken = parseOperator();
                hashTable.insert(currentToken);
                tokens.push_back(std::make_pair(currentToken, line));
                lastToken = currentToken;
            }
        }
    }
    std::vector<std::pair<Token, int>> getTokens() {
        return tokens; // ���������� ������ �������
    }

private:
    std::string input; 
    size_t position; 
    HashTable& hashTable; 
    std::vector<std::pair<Token, int>> tokens; // ������ ��� �������� �������
    std::vector<int> lines;
    int line = 1;
    

    Token parseIdentifierOrKeyword()
    {
        size_t start = position;
        bool hasDigit = false; // ������� �����

        while (position < input.size() &&
           (isalnum(input[position]) || input[position] == '_')) // ������ ��������� ����� � �����
    {
        if (isdigit(input[position])) {
            hasDigit = true;
        }
        ++position;
    }

        std::string value = input.substr(start, position - start);

        // ���� ������������� �������� �����, ������� ������
        if (hasDigit) {
            std::cerr << "Error: ������������� '" << value << "' �� ����� ��������� �����." << std::endl;
            return { TokenType::UNKNOWN, value }; // ���������� UNKNOWN �����
        }

        TokenType type = identifyKeyword(value);
        return { type, value };
    }
    //��������, �������� �� ��������� ��������
    TokenType identifyKeyword(const std::string& value)
    {
        static std::unordered_map<std::string, TokenType> keywords =
        {
            
            {"int", TokenType::TYPE_INT}, {"float", TokenType::TYPE_FLOAT},
            {"return", TokenType::RETURN}, {"itof", TokenType::ITOF},
            {"ftoi", TokenType::FTOI}
        };
        auto it = keywords.find(value);
        if (it != keywords.end())
        {
            return it->second;
        }
        return TokenType::ID;
    }

    
    // ��������, �������� �� ��� ������
    Token parseNumber()
    {
        size_t start = position; 
        bool hasDecimalPoint = false; //������� ���������� �����

        while (position < input.size() && isdigit(input[position]))
        {
            position++;
        }

        // ��������� �� ������� ���������� �����
        if (position < input.size() && input[position] == '.')
        {
            hasDecimalPoint = true; 
            position++; 

            while (position < input.size() && isdigit(input[position]))
            {
                position++;
            }

            //���������, ���� �� ��� ���� ����� ����� ����
            if (position < input.size() && input[position] == '.')
            {
                position++;
                std::cerr << "Error: ������������ ������ ����� '" << input.substr(start, position - start + 1) << "'." << std::endl;
                return { TokenType::UNKNOWN, input.substr(start, position - start +1) }; // ���������� UNKNOWN �����
            }

            return { TokenType::FLOAT_NUM, input.substr(start, position - start) };
        }

        // ���� �� ���� ���������� �����, ���������� ����� �����
        return { TokenType::INT_NUM, input.substr(start, position - start) }; 
    }

    // �������� ����������
    Token parseOperator()
    {
        char currentChar = input[position++];
        switch (currentChar)
        {
        case '=': return { TokenType::ASSIGN, "=" };
        case '+': return { TokenType::PLUS, "+" };
        case '-': return { TokenType::MINUS, "-" };
        case ',': return { TokenType::COMMA, "," };
        case ';': return { TokenType::SEMICOLON, ";" };
        case '(': return { TokenType::LPAREN, "(" };
        case ')': return { TokenType::RPAREN, ")" };
        case '{': return { TokenType::LBRACE, "{" };
        case '}': return { TokenType::RBRACE, "}" };
        default: return { TokenType::UNKNOWN, std::string(1, currentChar) };
        }
    }
};