#include <iostream>
#include <string>
#include "Token.h"
#include "LexicalAnalyzer.h"
#include "HashTable.h"
#include "SyntaxAnalyzer.h"
#include <fstream>

int main()
{
    setlocale(LC_ALL, "ru");
    std::string filename = "input.txt";
    std::string outputFilename = "output.txt"; // ��� ����� ��� ������ ������ �������

    HashTable hashTable;
    LexicalAnalyzer lexicalAnalyzer(filename, hashTable);

    lexicalAnalyzer.tokenize(); 

    hashTable.print(); 

  
    SyntaxAnalyzer syntaxAnalyzer(lexicalAnalyzer, outputFilename);

   
    syntaxAnalyzer.parse(); 

    return 0;
}