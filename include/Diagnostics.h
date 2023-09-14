// Class for diagnosing errors are compile time
#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <string>
#include <unordered_map>

class Diagnostic {
 public:
  enum DiagCode : unsigned short {
    token_error,
    syntax_error,
    parse_error,
    type_error,
    invalid_modifier_error,
    initialization_error
  };

  static void runDiagnostic(DiagCode code, const std::string &message) {
    fprintf(stderr, "%s: %s. Exiting...\n", diagCodeStrings.at(code).c_str(),
            message.c_str());
    exit(1);
  }

 private:
  static inline const std::unordered_map<DiagCode, const std::string>
      diagCodeStrings = {{token_error, "TOKEN ERROR"},
                         {syntax_error, "SYNTAX ERROR"},
                         {type_error, "TYPE ERROR"},
                         {invalid_modifier_error, "INVALID MODIFIER ERROR"},
                         {initialization_error, "INITIALIZATION ERROR"}};
};

#endif  // DIAGNOSTICS_H
