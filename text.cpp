#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

#include <boost/variant.hpp>
#include <mysql.h>
#include <mysql_connection.h>

using namespace std;
using namespace cgicc;



#define EMPTY_STRING ""
#define ZERO_SYMBOL '\0'
#define LENGTH_SHINGLE 3
#define LENGTH_MIN_WORD 3
#define LENGTH_MAX_WORD 32
#define LENGTH_EXCLUSIONS 12
#define LENGTH_MAX_FRAGMENT 256
#define SEPARATORS "./,+-#:`~@;$%^&*№([{< >}])=|!?–\'\"\n"
#define ENG_LETTERS "AaBCcEeHKkMOoPpTXx"
#define RUS_LETTERS "\u0410\u0430\u0412\u0421\u0441\u0415\u0435\u041d\u041a\u043a\u041c\u041e\u043e\u0420\u0440\u0422\u0425\u0445"
#define RUS_LETTERS_LOWER_CASE "\u0430\u0431\u0432\u0433\u0434\u0435\u0451\u0436\u0437\u0438\u0439\u043a\u043b\u043c\u043d\u043e\u043f\u0440\u0441\u0442\u0443\u0444\u0445\u0446\u0448\u0449\u044a\u044b\u044c\u044d\u044e\u044f"
#define RUS_LETTERS_UPPER_CASE "\u0410\u0411\u0412\u0413\u0414\u0415\u0401\u0416\u0417\u0418\u0419\u041a\u041b\u041c\u041d\u041e\u041f\u0420\u0421\u0422\u0423\u0424\u0425\u0426\u0428\u0429\u042a\u042b\u042c\u042d\u042e\u042f"

const string EXCLUSIONS[] = { "чтд", "либо", "или", "что", "чтобы", "как", "нибудь", "только", "зато", "также", "когда", "чем"};

const char* server = "eu-cdbr-west-01.cleardb.com";
const char* user = "bf9a5ee7e63428";
const char* password = "b74c4f1c";
const char* database = "heroku_b26ccddac4641f9";
const char* sqlQueryForTextAI = "SELECT * FROM texts";

string getDB();

double getShinglesMatchingsPersentage(const string &text, const string parsedFragment[], const int &wordsCount);
double antiPlagiarism(string text, string fragment);
string getSubstring(const string &originString, const int &startPosition, const int &length);

int getShinglesTotalCount(const int &wordsTotalCount);
int getMaxStringLength(const string &string1, const string &string2);
int getStringLength(const string &originString);
int getWordsCounter(const string wordsArr[]);
int compareStrings(const string &str1, const string &str2);

void replaceUppercaseLetters(string &word, const int &length);
void writeWordInShingle(string shingle[], int &wordPointer, const string &word, const int &length);
void replaceEngLetters(string &word, const int &length);
void replaceLetter(string &word, const int &length, const char oldLetter, const char newLetter);
void shiftQueue(string queue[], const int &queueLength, const string &newElement);
void findWord(string &str, const string &text, int &startPosition, const int &length);
void parseFragment(const string &fragment, string outputArr[]);

bool isMatchesInFragment(const string shingle[], const string textFragment[], const int &shinglesCount, bool isShinglesChecked[]);
bool isEqualShingles(const string shingle[], const string textFragment[], const int &startPosition);
bool isEmptyWord(const string &word);
bool isExclusion(const string &word);
bool isRepeated(const string &word, const string shingle[], const int &currentWordPointer);
bool isNumber(const string &string, const int &length);
bool isSeparator(char symbol);
bool isNumeral(char symbol);



int main()
{
    setlocale(LC_ALL, "Russian");

    Cgicc form;
    string name;

    cout << "Content-type:text/html\r\n\r\n";

    cout << getDB() << "\n";
    
    return 0;
}

string getDB(){
    MYSQL* conn;
    MYSQL_RES* res;
    MYSQL_ROW row;

    int qstate;
    int num_fields = 0;

    string sumOfTexts = "";

    conn = mysql_init(NULL);

    if (conn != NULL) {
        conn = mysql_real_connect(conn, server, user, password, database, 3306, NULL, 0);
    }

    qstate = mysql_query(conn, sqlQueryForTextAI);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);
        while (row)
        {
            sumOfTexts.append(" ");
            sumOfTexts.append(row[1]);

            row = mysql_fetch_row(res);
        }
    }

    mysql_free_result(res);
    mysql_close(conn);

    return sumOfTexts;
}

double antiPlagiarism(string text, string fragment)
{
    string parsedFragment[LENGTH_MAX_FRAGMENT];
    parseFragment(fragment, parsedFragment);
    int wordsTotalCount = getWordsCounter(parsedFragment);
    
    return (wordsTotalCount < LENGTH_SHINGLE)
        ? 0
        : getShinglesMatchingsPersentage(text, parsedFragment, wordsTotalCount);
}

double getShinglesMatchingsPersentage(const string &text, const string parsedFragment[], const int &wordsCount)
{
    int shinglesTotalCount = getShinglesTotalCount(wordsCount),
        textLength = getStringLength(text),
        wordPointer = 0,
        textPointer = 0,
        sameShinglesCounter = 0;
    string shingle[LENGTH_SHINGLE];
    bool checkedShingles[LENGTH_MAX_FRAGMENT] = {0};

    while (textPointer < textLength)
    {
        string word(LENGTH_MAX_WORD, ZERO_SYMBOL);
        findWord(word, text, textPointer, textLength);
        int length = getStringLength(word);

        if (isEmptyWord(word) || length < LENGTH_MIN_WORD || isNumber(word, length))
        {
            continue;
        }
        replaceEngLetters(word, length);
        replaceUppercaseLetters(word, length);
        if (isExclusion(word) || isRepeated(word, shingle, wordPointer))
        {
            continue;
        }
        writeWordInShingle(shingle, wordPointer, word, length);
        if (wordPointer >= LENGTH_SHINGLE && isMatchesInFragment(shingle, parsedFragment, shinglesTotalCount, checkedShingles))
        {
            sameShinglesCounter++;
        }
    }
    return sameShinglesCounter * 100.0 / shinglesTotalCount;
}

string getSubstring(const string &originString, const int &startPosition, const int &length)
{
    string outputString(length, '\0');
    for (int i = 0, j = startPosition; i < length; i++, j++)
    {
        outputString[i] = originString[j];
    }
    return outputString;
}

int getShinglesTotalCount(const int &wordsTotalCount)
{
    return wordsTotalCount - LENGTH_SHINGLE + 1;
}

int getMaxStringLength(const string &string1, const string &string2)
{
    int stringLength1 = getStringLength(string1),
        stringLength2 = getStringLength(string2);
    return (stringLength1 > stringLength2) ? stringLength1 : stringLength2;
}

int getStringLength(const string &originString)
{
    int counter = 0;
    for (int i = 0; originString[i] != '\0'; i++)
    {
        counter++;
    }
    return counter;
}

int getWordsCounter(const string wordsArr[])
{
    int wordsTotalCount = 0;
    for (int i = 0; i < LENGTH_MAX_FRAGMENT && wordsArr[i] != EMPTY_STRING; i++)
    {
        wordsTotalCount++;        
    }
    return wordsTotalCount;
}

int compareStrings(const string &string1, const string &string2)
{
    int maxLength = getMaxStringLength(string1, string2);

    for (int i = 0; i < maxLength; i++)
    {
        if (string1[i] > string2[i])
        {
            return 1;
        }
        else if (string1[i] < string2[i])
        {
            return -1;
        }
    }
    return 0;
}

void replaceUppercaseLetters(string &word, const int &length)
{
    for (int i = 0; RUS_LETTERS_UPPER_CASE[i] != '\0'; i++)
    {
        replaceLetter(word, length, RUS_LETTERS_UPPER_CASE[i], RUS_LETTERS_LOWER_CASE[i]);
    }
}

void writeWordInShingle(string shingle[], int &wordPointer, const string &word, const int &length)
{
    if (wordPointer < LENGTH_SHINGLE)
    {
        shingle[wordPointer++] = getSubstring(word, 0, length);
    }
    else
    {
        shiftQueue(shingle, LENGTH_SHINGLE, word);
    }
}

void replaceEngLetters(string &word, const int &length)
{
    for (int i = 0; ENG_LETTERS[i] != '\0'; i++)
    {
        replaceLetter(word, length, ENG_LETTERS[i], RUS_LETTERS[i]);
    }
}

void replaceLetter(string &word, const int &length, const char oldLetter, const char newLetter)
{
    for (int i = 0; i < length; i++)
    {
        if (word[i] == oldLetter)
        {
            word[i] = newLetter;
        }
    }
}

void shiftQueue(string queue[], const int &queueLength, const string &newElement)
{
    for (int i = 1; i < queueLength; i++)
    {
        queue[i - 1] = queue[i];
    }
    queue[queueLength - 1] = newElement;
}

void findWord(string &str, const string &text, int &startPosition, const int &length)
{
    int wordBegin = -1,
        wordLength = 0;
    bool isWordStarted = false;

    for (; startPosition < length; startPosition++)
    {
        if (isWordStarted)
        {
            if (isSeparator(text[startPosition]))
            {
                break;
            }
            else
            {
                wordLength++;
            }
        }
        else if (!isSeparator(text[startPosition]))
        {
            isWordStarted = true;
            wordLength++;
            wordBegin = startPosition;
        }
    }
    str = getSubstring(text, wordBegin, wordLength);
}

void parseFragment(const string &fragment, string outputArr[])
{
    int textLength = getStringLength(fragment),
        textPointer = 0,
        wordPointer = 0;

    while (textPointer < textLength)
    {
        string word(LENGTH_MAX_WORD, ZERO_SYMBOL);
        findWord(word, fragment, textPointer, textLength);
        int length = getStringLength(word);

        if (isEmptyWord(word) || length < LENGTH_MIN_WORD || isNumber(word, length))
        {
            continue;
        }
        replaceEngLetters(word, length);
        replaceUppercaseLetters(word, length);
        if (isExclusion(word))
        {
            continue;
        }
        outputArr[wordPointer++] = getSubstring(word, 0, length);
    }
}

bool isMatchesInFragment(const string shingle[], const string textFragment[], const int &shinglesCount, bool isShinglesChecked[])
{
    for (int i = 0; i < shinglesCount; i++)
    {
        if (isShinglesChecked[i])
        {
            continue;
        }
        else if (isEqualShingles(shingle, textFragment, i))
        {
            isShinglesChecked[i] = true;
            return true;
        }
    }
    return false;
}

bool isEqualShingles(const string shingle[], const string textFragment[], const int &startPosition)
{
    for (int j = 0; j < LENGTH_SHINGLE; j++)
    {
        if (compareStrings(shingle[j], textFragment[startPosition + j]))
        {
            return false;
        }
    }
    return true;
}

bool isEmptyWord(const string &word)
{
    return word[0] == '\0';
}

bool isExclusion(const string &word)
{
    for (int i = 0; i < LENGTH_EXCLUSIONS; i++)
    {
        if (!compareStrings(word, EXCLUSIONS[i]))
        {
            return true;
        }
    }
    return false;
}

bool isRepeated(const string &word, const string shingle[], const int &currentWordPointer)
{
    return currentWordPointer > 0 && !compareStrings(shingle[currentWordPointer - 1], word);
}

bool isNumber(const string &string, const int &length)
{
    for (int i = 0; i < length; i++)
    {
        if (!isNumeral(string[i]))
        {
            return false;
        }
    }
    return true;
}

bool isSeparator(char symbol)
{
    for (int i = 0; SEPARATORS[i] != '\0'; i++)
    {
        if (symbol == SEPARATORS[i])
        {
            return true;
        }
    }
    return false;
}

bool isNumeral(char symbol)
{
    return symbol >= '0' && symbol <= '9';
}