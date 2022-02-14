#pragma once

#include <string>
#include <vector>
#include <limits>
#include <cstddef>

namespace templates {
    const std::vector<std::string> LEXER_HEADER {
        "#pragma once\n"
        "\n"
        "#include <cstddef>\n"
        "#include <string>\n"
        "#include <istream>\n"
        "\n"
        "namespace ",

        " {\n"
        "\tstruct position {\n"
        "\t\tsize_t start_line;\n"
        "\t\tsize_t start_column;\n"
        "\t\tsize_t end_line;\n"
        "\t\tsize_t end_column;\n"
        "\t};\n"
        "\t\n"
        "\tstruct token {\n"
        "\t\tenum token_type {\n",
        
        "\t\t};\n"
        "\n"
        "\t\tstd::string identifier;\n"
        "\t\ttoken_type type;\n"
        "\t\tposition pos;\n"
        "\t};\n"
        "\n"
        "\tclass ",
        
        "\t {\n"
        "\t\tprivate:\n"
        "\t\t\tstd::istream& stream;\n"
        "\n"
        "\t\t\tsize_t curr_line;\n"
        "\t\t\tsize_t curr_column;\n"
        "\n"
        "\t\t\tchar curr;\n"
        "\n"
        "\t\t\tvoid next();\n"
        "\t   public:\n"
        "\t\t\t",
        
        "(std::istream& stream);\n"
        "\t\t\tbool end();\n"
        "\t\t\ttoken next_token();\n"
        "\t\t\ttoken next_unignored_token();\n"
        "\t\t\t~",
        
        "();\n"
        "\t};\n"
        "}\n"
    };

    const std::vector<std::string> LEXER_SOURCE {
        "#include \"",
        
        "\"\n"
        "\n"
        "#include <set>\n"
        "using namespace ",

        ";\n"
        "\n"
        "const std::set<token::token_type> IGNORED_TOKENS {\n",

        "};\n"
        "\n",

        "::",
        
        "(std::istream& stream) : stream(stream), curr_line(0), curr_column(0), curr('\\0') {\n"
        "\tthis->next();\n"
        "\tthis->curr_column--;\n"
        "}\n"
        "\n"
        "bool ",

        "::end() {\n"
        "\treturn this->stream.fail();\n"
        "}\n"
        "\n"
        "token ",
        
        "::next_token() {\n"
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
        "token ",
        
        "::next_unignored_token() {\n"
        "\ttoken t = this->next_token();\n"
        "\twhile(t.type != token::UNDEFINED && t.type != token::END_OF_FILE && IGNORED_TOKENS.find(t.type) != IGNORED_TOKENS.end()) {\n"
        "\t\tt = this->next_token();\n"
        "\t}\n"
        "\treturn t;\n"
        "}\n"
        "\n",

        "::~",
        
        "() {\n"
        "}\n"
        "\n"
        "//private\n"
        "\n"
        "void ",
        
        "::next() {\n"
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