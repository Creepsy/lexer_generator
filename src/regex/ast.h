#pragma once

#include <vector>
#include <string>

namespace ast {
    class branch {
        private:
        public:
            branch();
            virtual std::string to_string() = 0;
            virtual ~branch();
    };

    class option_branch : public branch {
        private:
            std::vector<branch*> options;
        public:
            option_branch();
            option_branch(std::vector<branch*>& options);
            void add_option(branch* option);
            std::string to_string() override;
            ~option_branch();
    };
    
    class quantifier_branch : public branch {
        private:
        public:
            quantifier_branch();
            std::string to_string() override;
            ~quantifier_branch();
    };

    class sequence_branch : public branch {
        private:
        public:
            sequence_branch();
            std::string to_string() override;
            ~sequence_branch();
    };

    class character_branch : public branch {
        private:
        public:
            character_branch();
            std::string to_string() override;
            ~character_branch();
    };
}