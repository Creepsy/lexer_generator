#pragma once

#include <string>
#include <vector>
#include <limits>
#include <cstddef>

namespace templates {
    const std::vector<std::string> LEXER_HEADER = {
        "#pragma once\n"
        "\n"
        "#include <cstddef>\n"
        "#include <string>\n"
        "#include <istream>\n"
        "\n"
        "struct position {\n"
        "\tsize_t start_line;\n"
        "\tsize_t start_column;\n"
        "\tsize_t end_line;\n"
        "\tsize_t end_column;\n"
        "};\n"
        "\n"
        "struct token {\n"
        "\tenum token_type {\n",
        
        "\t};\n"
        "\n"
        "\tstd::string identifier;\n"
        "\ttoken_type type;\n"
        "\tposition pos;\n"
        "};\n"
        "\n"
        "class lexer {\n"
        "\tprivate:\n"
        "\t\tstd::istream& stream;\n"
        "\n"
        "\t\tsize_t curr_line;\n"
        "\t\tsize_t curr_column;\n"
        "\n"
        "\t\tchar curr;\n"
        "\n"
        "\t\tvoid next();\n"
        "   public:\n"
        "\t\tlexer(std::istream& stream);\n"
        "\t\tbool end();\n"
        "\t\ttoken next_token();\n"
        "\t\ttoken next_unignored_token();\n"
        "\t\t~lexer();\n"
        "};\n"
    };

    const std::vector<std::string> LEXER_SOURCE = {
        "#include \"",
        
        "\"\n"
        "\n"
        "#include <set>\n"
        "\n"
        "const std::set<token::token_type> IGNORED_TOKENS {\n",

        "};\n"
        "\n"
        "lexer::lexer(std::istream& stream) : stream(stream), curr_line(0), curr_column(0), curr('\\0') {\n"
        "\tthis->next();\n"
        "\tthis->curr_column--;\n"
        "}\n"
        "\n"
        "bool lexer::end() {\n"
        "\treturn this->stream.fail();\n"
        "}\n"
        "\n"
        "token lexer::next_token() {\n"
        "\tsize_t start_line = this->curr_line;\n"
        "\tsize_t start_column = this->curr_column;\n"
        "\tif(this->end()) return token{\"EOF\", token::END_OF_FILE, position{start_line, start_column, start_line, start_column}};\n"
        "\n"
        "\tstd::string identifier = \"\";\n"
        "\tsize_t state = 0;\n"
        "\n"
        "\twhile(true) {\n"
        "\t\tswitch(state) {\n",

        "\t\t}\n"
        "\n"
        "\t\tidentifier += this->curr;\n"
        "\t\tthis->next();\n"
        "\t}\n"
        "\treturn token{\"\", token::UNDEFINED, position{}};\n"
        "}\n"
        "\n"
        "token lexer::next_unignored_token() {\n"
        "\ttoken t = this->next_token();\n"
        "\twhile(t.type != token::UNDEFINED && t.type != token::END_OF_FILE && IGNORED_TOKENS.find(t.type) != IGNORED_TOKENS.end()) {\n"
        "\t\tt = this->next_token();\n"
        "\t}\n"
        "\treturn t;\n"
        "}\n"
        "\n"
        "lexer::~lexer() {\n"
        "}\n"
        "\n"
        "//private\n"
        "\n"
        "void lexer::next() {\n"
        "\tthis->curr_column++;\n"
        "\tif(this->curr == '\\n') {\n"
        "\t\tthis->curr_column = 0;\n"
        "\t\tthis->curr_line++;\n"
        "\t}\n"
        "\n"
        "\tthis->curr = this->stream.get();\n"
        "}\n"
    };

    const std::string ERROR_NODE = 
        "\t\t\tdefault:\n"
        "\t\t\t\treturn token{identifier, token::UNDEFINED, position{start_line, start_column, this->curr_line, this->curr_column}};\n";

    const std::string ERROR_CHAR = 
        "\t\t\t\t\tdefault:\n"
        "\t\t\t\t\t\tstate = " + std::to_string(std::numeric_limits<size_t>::max()) + "ULL;\n"
        "\t\t\t\t\t\tbreak;\n";
};