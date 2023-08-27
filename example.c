#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CSV_H_IMPLEMENTATION
#include "csv.h"

#define TEXT_1 "1,2,3\n4,5,6\n7,8,9"
#define TEXT_2 "a,b,c\n 1, 2,3\n4,5 , 6 "
#define TEXT_3 "a,b\n1,1.2\n2,2.4"
#define TEXT_4 "a,b\n\"1,2\",3\n4,\"5,6\"\n\"7\"\",8\",9"

void print_result(int res, int len, char *start)
{
   if (res == CSV_OK) {
        printf("[%d] %.*s\n", len, len, start);
    } else if (res == CSV_END_OF_ROW) {
        printf("[0] CSV_END_OF_ROW\n");
    } else if (res == CSV_END_OF_STRING) {
        printf("[0] CSV_END_OF_STRING\n");
    } else {
        printf("INVALID RESULT\n");
    }
}

int main()
{
    int n;
    int flags = 0;
    int len;
    char *start;
    CSV_Parser p;

    {
        printf("\n[*] Parsing:\n\n%s\n\n", TEXT_1);
        csv_parser_init(&p, TEXT_1, strlen(TEXT_1), ',', flags);

        do {
            n = csv_parser_next(&p, &start, &len);
            print_result(n, len, start);
        } while(n != CSV_END_OF_STRING);
    }

    {
        printf("\n[*] Parsing:\n\n%s\n\n", TEXT_2);
        csv_parser_init(&p, TEXT_2, strlen(TEXT_2), ',', flags);

        printf("Skipping first row\n");
        csv_parser_skip_row(&p);

        do {
            n = csv_parser_next(&p, &start, &len);
            print_result(n, len, start);
        } while(n != CSV_END_OF_STRING);
    }

    {
        printf("\n[*] Parsing:\n\n%s\n\n", TEXT_3);
        csv_parser_init(&p, TEXT_3, strlen(TEXT_3), ',', flags);

        printf("Skipping first row\n");
        csv_parser_skip_row(&p);

        long ival;
        double fval;

        do {
            if (csv_parser_next_long(&p, &ival) != CSV_OK) break;
            if (csv_parser_next_double(&p, &fval) != CSV_OK) break;
            printf("| %ld | %lf |\n", ival, fval);
            if (csv_parser_next(&p, &start, &len) != CSV_END_OF_ROW) break;
        } while(1);
    }

    {
        printf("\n[*] Parsing:\n\n%s\n\n", TEXT_4);
        csv_parser_init(&p, TEXT_4, strlen(TEXT_4), ',', flags);

        printf("Skipping first row\n");
        csv_parser_skip_row(&p);

        do {
            n = csv_parser_next(&p, &start, &len);
            print_result(n, len, start);
        } while(n != CSV_END_OF_STRING);
    }

    return 0;
}
