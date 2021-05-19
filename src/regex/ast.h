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

    class sequence_branch : public branch {
        private:
            std::vector<branch*> elements;
            const bool option;
        public:
            sequence_branch(const bool option);
            sequence_branch(const sequence_branch& other) = delete;
            sequence_branch(sequence_branch&& other) = delete;
            sequence_branch(std::vector<branch*>& elements, const bool option);

            void add_element(branch* element);
            bool is_option();
            std::string to_string() override;

            ~sequence_branch();

            sequence_branch& operator=(const sequence_branch& other) = delete;
            sequence_branch& operator=(sequence_branch&& other) = delete;
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

    class character_range_branch : public branch {
        private:
            const char begin;
            const char end;

            const bool negated;
        public:
            character_range_branch(const char begin, const char end, bool negated);
            character_range_branch(const character_range_branch& other) = delete;
            character_range_branch(character_range_branch&& other) = delete;

            bool is_negated();
            char get_begin();
            char get_end();
            std::string to_string() override;

            ~character_range_branch();

            character_range_branch& operator=(const character_range_branch& other) = delete;
            character_range_branch& operator=(character_range_branch&& other) = delete;
    };

    class character_set_branch : public branch {
        private:
            std::vector<char> characters;
            const bool negated;
        public:
            character_set_branch(const std::vector<char>& characters, const bool negated);
            character_set_branch(const character_set_branch& other) = delete;
            character_set_branch(character_set_branch&& other) = delete;

            bool is_negated();
            const std::vector<char>& get_characters();
            std::string to_string() override;

            ~character_set_branch();

            character_set_branch& operator=(const character_set_branch& other) = delete;
            character_set_branch& operator=(character_set_branch&& other) = delete;
    };
}