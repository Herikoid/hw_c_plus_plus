#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>

int main()
{
    std::string input_name;
    int input = 0, ncol, irow, icol;
    std::vector<std::string> names;
    std::vector<std::vector<double>> matrix;
    std::vector<double> row;
    std::string tmp_str, tmp_el;

    // выбор одного из тестовых файлов
    std::cout << "Type:" << std::endl <<
        "1 for correct file" << std::endl <<
        "2 for non-double elements" << std::endl <<
        "3 for file with too few elemnts in row" << std::endl <<
        "4 for file with too many elements in row" << std::endl <<
        "5 for non-existent file" << std::endl;

    std::cin >> input;

    if (input > 5 && input < 1) {
        return 1;
    }

    std::ostringstream oss;
    oss << input;

    input_name = "test1.txt";
    input_name[4] = oss.str()[0];

    std::ifstream file;
    std::stringstream ss;

    try {
        file.open(input_name, std::ios_base::in);

        if (!file.is_open()) { // проверяем на возможность открыть файл
            throw std::domain_error("Could not open the file");
        }

        std::getline(file, tmp_str);

        ss.str(tmp_str);
        while (std::getline(ss, tmp_el, '\t')) { // записываем имена столбцов
            names.push_back(tmp_el);
        }

        ncol = names.size();
        icol = 0;
        while (std::getline(file, tmp_str)) { // считываем матрицу
            ++icol;

            ss.clear();
            ss.str(tmp_str);
            
            irow = 0;
            try {
                while (std::getline(ss, tmp_el, '\t')) { // считываем строку
                    if (++irow > ncol) { // проверяем, не слишком ли много элементов в строке
                        throw std::length_error("Too many values in row " + std::to_string(icol));
                    }
                    row.push_back(stod(tmp_el));
                }
                if (irow < ncol) {  // проверяем, достаточно ли элементов в строке
                    throw std::length_error("Too few values in row " + std::to_string(icol));
                }
            }
            catch (const std::invalid_argument& ia) { // обрабатываем случай, когда элемент некорректного типа
                throw std::invalid_argument(std::to_string(irow) + "th value in row " + std::to_string(icol) + " is not double");
            }

            matrix.push_back(row);
            row.clear();
        }
        // вывод матрицы на печать
        for (const auto& name : names) {
            std::cout << name << '\t';
        }
        std::cout << std::endl;

        for (int i = 0; i < matrix.size(); ++i) {
            for (int value : matrix[i]) {
                std::cout << value << '\t';
            }
            std::cout << std::endl;
        }
    }
    catch(const std::exception& e) {
        std::cout << "There was an exception: " << e.what() << std::endl;
    }

    return 0;
}