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

// Base Tokens
TOKEN(eof)
TOKEN(unknown)
TOKEN(identifier)
TOKEN(num_const)
TOKEN(char_const)
TOKEN(string_literal)

// Organizational tokens
TOKEN_PUNCTUATOR(o_paren, "(")
TOKEN_PUNCTUATOR(c_paren, ")")
TOKEN_PUNCTUATOR(o_brace, "{")
TOKEN_PUNCTUATOR(c_brace, "}")
TOKEN_PUNCTUATOR(o_bracket, "[")
TOKEN_PUNCTUATOR(c_bracket, "]")
TOKEN_PUNCTUATOR(semi_colon, ";")
TOKEN_PUNCTUATOR(comma, ",")
TOKEN_PUNCTUATOR(quote, "\"")
TOKEN_PUNCTUATOR(apost, "\'")

// Operators
TOKEN_PUNCTUATOR(plus, "+")
TOKEN_PUNCTUATOR(minus, "-")
TOKEN_PUNCTUATOR(star, "*")
TOKEN_PUNCTUATOR(slash, "/")
TOKEN_PUNCTUATOR(percent, "%")
TOKEN_PUNCTUATOR(hat, "^")
TOKEN_PUNCTUATOR(equal, "=")
TOKEN_PUNCTUATOR(plus_equal, "+=")
TOKEN_PUNCTUATOR(minus_equal, "-=")
TOKEN_PUNCTUATOR(star_equal, "*=")
TOKEN_PUNCTUATOR(slash_equal, "/=")
TOKEN_PUNCTUATOR(plus_plus, "++")
TOKEN_PUNCTUATOR(minus_minus, "--")
TOKEN_PUNCTUATOR(pipe_pipe, "||")
TOKEN_PUNCTUATOR(amp_amp, "&&")
TOKEN_PUNCTUATOR(bang, "!")
TOKEN_PUNCTUATOR(less_equal, "<=")
TOKEN_PUNCTUATOR(less, "<")
TOKEN_PUNCTUATOR(greater, ">")
TOKEN_PUNCTUATOR(greater_equal, ">=")
TOKEN_PUNCTUATOR(equal_equal, "==")
TOKEN_PUNCTUATOR(bang_equal, "!=")

// Keywords
TOKEN_KEYWORD(int)
TOKEN_KEYWORD(char)
TOKEN_KEYWORD(bool)
TOKEN_KEYWORD(void)
TOKEN_KEYWORD(true)
TOKEN_KEYWORD(false)
TOKEN_KEYWORD(if)
TOKEN_KEYWORD(else)
TOKEN_KEYWORD(while)
TOKEN_KEYWORD(for)
TOKEN_KEYWORD(return)

#undef TOKEN
#undef TOKEN_PUNCTUATOR
#undef TOKEN_KEYWORD
