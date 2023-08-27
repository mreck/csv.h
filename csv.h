#ifndef CSV_H_
#define CSV_H_

#define CSV_OK            0
#define CSV_END_OF_ROW    1
#define CSV_END_OF_STRING 2
#define CSV_INVALID       3

#define CSV_TRIM_WHITESPACE 1 << 0;

typedef struct {
    char *p;
    char *end;
    char  sep;
    int   flags;
} CSV_Parser;

void csv_parser_init(CSV_Parser *parser, char *start, unsigned long length, char sep, int flags);
int csv_parser_next(CSV_Parser *parser, char **start, int *length);
int csv_parser_next_long(CSV_Parser *parser, long *value);
int csv_parser_next_double(CSV_Parser *parser, double *value);
int csv_parser_skip_row(CSV_Parser *parser);

#endif // CSV_H_

#ifdef CSV_H_IMPLEMENTATION

void csv_parser_init(CSV_Parser *parser, char *start, unsigned long length, char sep, int flags)
{
    parser->p     = start;
    parser->end   = start + length;
    parser->sep   = sep;
    parser->flags = flags;
}

int csv_parser_next(CSV_Parser *parser, char **start, int *length)
{
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
        if (parser->p[0] == parser->sep) break;
        if (parser->p[0] == '\n') break;
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

int csv_parser_skip_row(CSV_Parser *parser)
{
    int n, l;
    char *s;
    do { n = csv_parser_next(parser, &s, &l); }
    while (n == CSV_OK);
    return n;
}

#endif // CSV_H_IMPLEMENTATION
