#include <boost/token_functions.hpp>
#include <iostream>
#include <stack>
#include <boost/tokenizer.hpp>
#include <vector>
#include "lex.yy.c" 


/*
 * Token types:
 * 0 -> error
 * 1 -> Noun
 * 2 -> verb
 * 3 -> adword
 * 4 -> punctuation
 * 5 -> described noun
 * 6 -> described verb
 */


const std::vector<std::string> token_types = {"error", "noun", "verb", "adword", "punctuation", "described noun", "described verb"};

class Token {
    public:
        std::string data;
        int type;
        int modifier;

        // Modifiers:
        // noun ->  0 = Subject
        //          1 = object
        //
        // adword ->    0 = adjective
        //              1 = adverb

};

class Sentence {
    public:
        std::vector<Token> tokens;
        bool validity;
        bool validity_checked;

        bool isvalid() {
            if (validity_checked) return validity;
            else validity_checked = true;
            std::stack<int> st;
            for (int i = 0; i < tokens.size(); i++) {
                int prev;
                if (!st.empty()) prev = st.top();
                st.push(tokens[i].type);
                if (!st.empty()) {
                    if (tokens[i].type == 1 && prev == 3) {
                        st.pop();
                        st.pop();
                        st.push(5);
                    }

                    else if (tokens[i].type == 2 && prev == 3) {
                        st.pop();
                        st.pop();
                        st.push(6);
                        tokens[i-1].modifier = 1;
                    }
                }
            }

            //valid = 4 (2/6) (1/5)
            //      = 4 (1/5) (2/6) (1/5)

            validity = true;

            if (st.top() != 4) {
                //std::cout << "No Ending punctuation" << std::endl;
                return validity = false;
            }
            st.pop();

            switch(st.top()) {
                case 1:
                case 5:
                    st.pop();
                    break;

                case 2:
                case 6:
                    break;

                default:
                    //std::cout << "Last word wrong" << std::endl;
                    return validity = false;
            }

            if (st.top() != 2 && st.top() != 6) {
                //std::cout << st.top() << std::endl;
                //std::cout << "Missing verb" << std::endl;
                return validity = false;
            }
            st.pop();
            if (!st.empty()) {
                if (st.top() == 1 || st.top() == 5) {
                    st.pop();
                    if (st.empty()) return validity = true;
                    else return validity = false;
                }

                else {
                    //std::cout << "Missing first noun" << std::endl;
                    return validity = false;
                }
            }
            else return validity = false;
        }


        void show() {
            for (int i = 0; i < tokens.size(); i++) {
                std::cout << tokens[i].data << " ";
            }
            std::cout << std::endl;
        }



        int token_fixer() {
            if (!isvalid()) return -1;
            else {
                for (int i = 0; i < tokens.size(); i++) {
                    if (tokens[i].type == 1) {
                        if (i >= 2) tokens[i].modifier = 1;
                    }
                }
                return 1;
            }
        }


        void display_tokens() {
            int status = token_fixer();
            if (status == -1) {
                std::cout << "Malformed sentence" << std::endl;
                return;
            }

            for (int i = 0; i < tokens.size(); i++) {
                if (tokens[i].type == 1 && tokens[i].modifier == 0) std::cout << "(subject noun: " << tokens[i].data << ") ";
                else if (tokens[i].type == 1 && tokens[i].modifier == 1) std::cout << "(object noun: " << tokens[i].data << ") ";
                else if (tokens[i].type == 2) std::cout << "(verb: " << tokens[i].data << ") ";
                else if (tokens[i].type == 3 && tokens[i].modifier == 0) std::cout << "(adjective: " << tokens[i].data << ") ";
                else if (tokens[i].type == 3 && tokens[i].modifier == 1) std::cout << "(adverb: " << tokens[i].data << ") ";
                else if (tokens[i].type == 4) std::cout << "(punctuation: " << tokens[i].data << ") ";
            }
            std::cout << std::endl;
        }
};


class Paragraph {
    public:
        std::vector<Sentence> sentences;
        bool validity;
        bool validity_checked;

        bool is_valid() {
            if (validity_checked) return validity;
            else validity_checked = true;

            for (int i = 0; i < sentences.size(); i++) {
                if (sentences[i].isvalid() == 0) return validity = false;
            }

            return validity = true;
        }


        void show() {
            for (int i = 0; i < sentences.size(); i++) {
                sentences[i].show();
            }
        }
};




Paragraph parse(std::string test) {
    //std::cout << test << std::endl << std::endl;
    std::string buffer = "";
    for (int i = 0; i < test.size(); i++) {
        if (i >= 1) {
            if ((test[i] == '?' || test[i] == '!' || test[i] == '.') && test[i-1] != ' ') buffer.push_back(' ');
            else if ((test[i] != ' ') && (test[i-1] == '?' || test[i-1] == '!' || test[i-1] == '.')) buffer.push_back(' ');
        }
        buffer.push_back(test[i]);
    }


    boost::char_separator<char> sep(" \n");
    boost::tokenizer<boost::char_separator<char>> tok(buffer, sep);
    std::vector<std::string> tmp;
    for(boost::tokenizer<boost::char_separator<char>>::iterator it=tok.begin(); it!=tok.end(); ++it){
       tmp.push_back(*it);
    }

    std::vector<Token> tokens;
    for (int i = 0; i < tmp.size(); i++) {
        Token new_token;
        new_token.data = tmp[i];
        new_token.modifier = 0;
        //std::cout << tmp[i] << std::endl;
        char *toke = tmp[i].data();
        YY_BUFFER_STATE bf = yy_scan_string(toke);
        new_token.type = yylex();
        yy_delete_buffer(bf);
        tokens.push_back(new_token);
    }

    tmp.clear();

    for (int i = 0; i < tokens.size(); i++) {
        //std::cout << tokens[i].data << " " << token_types[tokens[i].type] << std::endl;
        //std::cout << tokens[i].type << " ";
    }

    //std::cout << std::endl;
    //std::cout << tokens.size();

    Paragraph p;
    Sentence new_sentence;
    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i].type != 4) new_sentence.tokens.push_back(tokens[i]);

        else {
            new_sentence.tokens.push_back(tokens[i]);
            p.sentences.push_back(new_sentence);
            new_sentence.tokens.clear();
        }
    }
    return p;
}

