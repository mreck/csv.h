// CSV.h - v0.1
// A CSV utility library in a single header file.

#ifndef CSV_H_
#define CSV_H_

#define CSV_OK            0
#define CSV_END_OF_ROW    1
#define CSV_END_OF_STRING 2
#define CSV_INVALID       3

typedef struct {
    char *p;
    char *end;
    char  sep;
} CSV_Parser;

void csv_parser_init(CSV_Parser *parser, char *start, unsigned long length, char sep);
int csv_parser_next(CSV_Parser *parser, char **start, int *length);
int csv_parser_next_long(CSV_Parser *parser, long *value);
int csv_parser_next_double(CSV_Parser *parser, double *value);
int csv_parser_next_string(CSV_Parser *parser, char *buff, int cap, int *len);
int csv_parser_skip_row(CSV_Parser *parser);

#endif // CSV_H_

#ifdef CSV_H_IMPLEMENTATION

void csv_parser_init(CSV_Parser *parser, char *start, unsigned long length, char sep)
{
    parser->p   = start;
    parser->end = start + length;
    parser->sep = sep;
}

int csv_parser_next(CSV_Parser *parser, char **start, int *length)
{
    int q_count = 0;

    *length = 0;

    if (parser->p >= parser->end || parser->p[0] == '\0') {
        return CSV_END_OF_STRING;
    } else if (parser->p[0] == parser->sep) {
        ++parser->p;
    } else if (parser->p[0] == '\n') {
        ++parser->p;
        return CSV_END_OF_ROW;
    }

    *start = parser->p;

    while (parser->p < parser->end) {
        if (parser->p[0] == '\0') break;
        if ((q_count + 1) % 2) {
            if (parser->p[0] == parser->sep) break;
            if (parser->p[0] == '\n') break;
        }
        if (parser->p[0] == '"') ++q_count;
        ++parser->p;
    }

    *length = parser->p - *start;
    return CSV_OK;
}

int csv_parser_next_long(CSV_Parser *parser, long *value)
{
    int n, l;
    char *s, *end;
    long v;

    n = csv_parser_next(parser, &s, &l);
    if (n != CSV_OK) return n;

    v = strtol(s, &end, 10);
    if (s == end) return CSV_INVALID;
    *value = v;
    return CSV_OK;
}

int csv_parser_next_double(CSV_Parser *parser, double *value)
{
    int n, l;
    char *s, *end;
    double v;

    n = csv_parser_next(parser, &s, &l);
    if (n != CSV_OK) return n;

    v = strtod(s, &end);
    if (s == end) return CSV_INVALID;
    *value = v;
    return CSV_OK;
}

int csv_parser_next_string(CSV_Parser *parser, char *buff, int cap, int *len)
{
    int n, l;
    char *s;

    buff[0] = '\0';
    *len = 0;

    n = csv_parser_next(parser, &s, &l);
    if (n != CSV_OK) return n;

    while (isspace(s[0])) { ++s; --l; }
    if (s[0] == '"') { ++s; --l; }
    while (isspace(s[l-1])) { --l; }
    if (s[l-1] == '"') { --l; }

    int j = 0;
    for (int i = 0; i < l; i++) {
        if (j >= cap) break;
        if (s[i] == '"' && i > 0 && s[i-1] == '"') {}
        else { buff[j++] = s[i]; }
    }
    buff[j] = '\0';

    *len = j;

    return CSV_OK;
}

int csv_parser_skip_row(CSV_Parser *parser)
{
    int n, l;
    char *s;
    do { n = csv_parser_next(parser, &s, &l); }
    while (n == CSV_OK);
    return n;
}

#endif // CSV_H_IMPLEMENTATION
