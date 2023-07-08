// Token Macro Defintions
#ifndef TOKEN
#define TOKEN(X)
#endif

#ifndef TOKEN_PUNCTUATOR
#define TOKEN_PUNCTUATOR(NAME, SEQ) TOKEN(NAME)
#endif

#ifndef TOKEN_KEYWORD
#define TOKEN_KEYWORD(X) TOKEN(kw_##X)
#endif

#ifndef TOKEN_KEYWORD_TYPE
#define TOKEN_KEYWORD_TYPE(X) TOKEN(kwt_##X)
#endif

#undef EOF
TOKEN(eof)
TOKEN(identifier)
TOKEN(numeric_literal)
TOKEN(unknown)

TOKEN_PUNCTUATOR(o_paren, '(')
TOKEN_PUNCTUATOR(c_paren, ')')
TOKEN_PUNCTUATOR(o_brace, '{')
TOKEN_PUNCTUATOR(c_brace, '}')
TOKEN_PUNCTUATOR(comma, ',')
TOKEN_PUNCTUATOR(semi_colon, ';')
TOKEN_PUNCTUATOR(equals, '=')
TOKEN_PUNCTUATOR(add, '+')
TOKEN_PUNCTUATOR(sub, '-')
TOKEN_PUNCTUATOR(mult, '*')
TOKEN_PUNCTUATOR(divide, '/')
TOKEN_PUNCTUATOR(modulo, '%')

TOKEN_KEYWORD_TYPE(char)
TOKEN_KEYWORD_TYPE(int)

#undef TOKEN
#undef TOKEN_PUNCTUATOR
#undef TOKEN_KEYWORD
#undef TOKEN_KEYWORD_TYPE
