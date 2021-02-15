#include "help.h"

int main(void)
{
    char buf[100];
    Strings strings = split(buf);
    for(int i=0;i<strings.size;i++)
        puts(strings.strings[i]);
    freeStrings(strings);
}