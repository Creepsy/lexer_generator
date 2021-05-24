#pragma once

#include <vector>
#include <string>
#include <cstddef>

namespace ast {
    struct char_range {
        char start;
        char end;

        bool negated = false;

        std::vector<char_range> intersection(const char_range& other) const;
        std::vector<char_range> without_negation() const;
        static std::vector<char_range> simplify(std::vector<char_range> ranges);
    };

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
            const std::vector<branch*>& get_elements();
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
            branch* get_child();
            std::string to_string() override;

            ~quantifier_branch();

            quantifier_branch& operator=(const quantifier_branch& other) = delete;
            quantifier_branch& operator=(quantifier_branch&& other) = delete;
    };

    class character_set_branch : public branch {
        private:
            std::vector<char_range> characters;
            
            const bool negated;
        public:
            character_set_branch(const std::vector<char_range>& characters, const bool negated);
            character_set_branch(const bool negated);
            
            bool is_negated();
            void add_character_range(const char_range range);
            void add_character_ranges(const std::vector<char_range>& characters);
            const std::vector<char_range>& get_characters();
            std::string to_string() override;

            ~character_set_branch();
    };
}