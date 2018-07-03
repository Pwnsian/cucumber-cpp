#ifndef _UTILITY_TYPES_H
#define _UTILITY_TYPES_H

#include <memory>
#include <string>

//Gherkin-c forward declarations
class GherkinDocument;
class FileReader;
class TokenScanner;
class TokenMatcher;
class Builder;
class Parser;

typedef std::shared_ptr<const GherkinDocument> GherkinDocumentPtr;

#endif
