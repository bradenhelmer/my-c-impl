// Statement Parsing Imlpementations
#include "Parser.h"

std::unique_ptr<BlockStmtAST> Parser::parseBlockStmt() {
  std::vector<std::unique_ptr<StmtAST>> stmtList;
  while (currKind() != c_brace) {
    if (isPrimitive(currKind())) {
      TokenKind currDeclKind = currTok->kind;
      advanceCurrent();
      stmtList.push_back(parseVarDecl(currDeclKind, lex.getIdentifier()));
    } else {
      switch (currKind()) {
	case kw_if:
	  stmtList.push_back(parseCondStatement());
	  break;
	case kw_for:
	case kw_while:
	  stmtList.push_back(parseIterStmt());
	  break;
	case kw_return:
	  advanceCurrent();
	  stmtList.push_back(parseReturnStmt());
      }
    }
  }
  advanceCurrent();
  return std::make_unique<BlockStmtAST>(std::move(stmtList));
}

std::unique_ptr<BlockStmtAST> Parser::parseCondStatement() {}

std::unique_ptr<BlockStmtAST> Parser::parseIterStmt() {}

std::unique_ptr<ReturnStmtAST> Parser::parseReturnStmt() {
  if (currKind() == semi_colon) {
    advanceCurrent();
    return std::make_unique<ReturnStmtAST>(nullptr);
  }
  return std::make_unique<ReturnStmtAST>(parseExpr());
}
