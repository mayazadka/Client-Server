#include "help.h"

int main(void)
{
    char buf[100];
    gets(buf);
    Strings strings = split(buf);
    char* query = appendStrings(5, "SELECT COUNT(car_id) FROM customer_car WHERE car_id = '", strings.strings[1], "' AND customer_id = '", strings.strings[2], "';");
    puts(query);
    free(query);
    freeStrings(strings);
}