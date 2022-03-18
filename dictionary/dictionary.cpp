#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <cstdlib>

// алфавит
const std::vector<char> alphabet = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                                     'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
// лишние текстовые символы (пунктуация)
const std::vector<char> extra_char = { '.', ',', ':', ';', '!', '?', '(', ')' };


void delete_extra(std::string& word) { // удаляем лишние символы из слова
    for (char c : extra_char) {
        word.erase(std::remove(word.begin(), word.end(), c), word.end());
    }
}

std::vector<std::string> spell_check(std::string& word, std::set<std::string> dict) { // предлагаем опции для испровления опечаток, пользуясь словарём
    std::vector<std::string> options;
    std::string tmp;

    for (int i = 0; i < word.size(); ++i) { // ищем опечатки с лишней буквой
        tmp = word;
        tmp.erase(tmp.begin() + i);
        if (dict.find(tmp) != dict.end()) {
            options.push_back(tmp);
        }
    }

    for (int i = 0; i < word.size(); ++i) { // ищем опечатки с заменённой буквой
        for (char c : alphabet) {
            tmp = word;
            tmp[i] = c;
            if (dict.find(tmp) != dict.end()) {
                options.push_back(tmp);
            }
        }
    }

    for (int i = 0; i < word.size(); ++i) { // ищем опечатки с пропущенной буквой
        for (char c : alphabet) {
            tmp = word;
            tmp.insert(i, 1, c);
            if (dict.find(tmp) != dict.end()) {
                options.push_back(tmp);
            }
        }
    }

    return options;
}


int main() {
    
    std::string buffer;
    std::set<std::string> dict;
    std::vector<std::string> T9_opt;
    int input;

    try {
        std::ifstream dict_in, text_in;
        std::ofstream dict_out, text_out;

        dict_in.open("dict_in.txt");
        
        if (!dict_in.is_open()) { // проверяем, открылся ли словарь
            throw std::domain_error("Could not open the input dictionary file. Check that file named dict_in.txt.");
        }

        text_in.open("text_in.txt");
        
        if (!text_in.is_open()) { // проверяем, открылся ли текст
            throw std::domain_error("Could not open the input text file. Check that file named text_in.txt.");
        }

        while (dict_in >> buffer) { // записываем все слова из входного словаря
            dict.insert(buffer);
        }

        text_out.open("text_out.txt");
        while (text_in >> buffer) { // считываем текст
            std::string word = buffer;

            delete_extra(word);

            if (!word.empty()) {
                if (dict.find(word) == dict.end()) { // обрабатываем новое слово 
                    std::cout << "word: \"" << word << "\"" << std::endl;
                    std::cout << "options:" << std::endl;
                    std::cout << "1. type 1 to add word to the dictionary" << std::endl;
                    if (!(T9_opt = spell_check(word, dict)).empty()) { // проверяем новое слово на опечатки
                        for (int i = 2; i < T9_opt.size() + 2; ++i) {
                            std::cout << i << ". type " << i << " to replace word with \"" << T9_opt[i - 2] << "\"" << std::endl;
                        }
                    }
                    std::cout << "or type another number to leave word as it is" << std::endl;

                    std::cin >> input;

                    if (input == 1) { // добавляем слово в словарь
                        dict.insert(word);
                    }
                    else {
                        if (input > 1 && input < T9_opt.size() + 2) {  // корректируем слово с опечаткой
                            word.replace(0, buffer.size(), T9_opt[input - 2]);
                        }
                    }
                }
                text_out << word << " ";  // записываем слово в исправленный текст
            }
        }

        dict_out.open("dict_out.txt");

        for (const auto& word : dict) { // заполняем итоговый словарь
            dict_out << word << std::endl;
        }
    }
    catch (const std::domain_error& e) {
        std::cout << e.what() << std::endl;
    }


    std::cout << "You can find corrected text in text_out.txt and new dictionary in dict_out.txt" << std::endl;

    return 0;
}