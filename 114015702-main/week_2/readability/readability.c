#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    bool isNotStartSpace = false;
    bool isNotEndSpace = false;
    bool containsMultipleSpacesIAR = false;
    int countWords = 0;
    int countSent = 0;
    int countLetters = 0;
    do
    {
        string text = get_stirng("Text: ");
        int len = strlen(text);
        isNotStartSpace = text[0] == ' ';
        isNotEndSpace = text[len - 1] == ' ';
        for (int i = 0; i < len-1; i++)
        {

            else if (text[i] == ' ' && text[i + 1] == ' ')
            {
                containsMultipleSpacesIAR = true;
                break;
            }
            else if (text[i] == ' ' || text[i] == ',')
            {
                countWords++;
            }
            else if (text[i] == '.' && text[i+] == ' ')
            {

            }

        }
    }
    while (isNotStartSpace);

    if (text[i] > 'a' && text[i] < 'z' || text[i] > 'A' && text[i] < 'Z')
            {
                countLetters++;
            }
    
}