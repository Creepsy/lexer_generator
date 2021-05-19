#pragma once

#include <vector>
#include <string>
#include <cstddef>

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
            option_branch(const option_branch& other) = delete;
            option_branch(option_branch&& other) = delete;
            option_branch(std::vector<branch*>& options);

            void add_option(branch* option);
            std::string to_string() override;

            ~option_branch();

            option_branch& operator=(const option_branch& other) = delete;
            option_branch& operator=(option_branch&& other) = delete;
    };
    
    class quantifier_branch : public branch {
        private:
            const size_t min;
            const size_t max;

            const bool limited;

            branch* child;

        public:
            quantifier_branch(branch* child, const size_t min, const size_t max, const bool limited = true);
            quantifier_branch(const quantifier_branch& other) = delete;
            quantifier_branch(quantifier_branch&& other) = delete;

            size_t get_minimum();
            size_t get_maximum();
            bool is_limited();
            std::string to_string() override;

            ~quantifier_branch();

            quantifier_branch& operator=(const quantifier_branch& other) = delete;
            quantifier_branch& operator=(quantifier_branch&& other) = delete;
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