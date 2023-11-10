#include <iostream>
#include <cstdint>

namespace zspace {

    template<typename T>
    class ZMaxrix 
    {
        private:
            T **data;
            std::uint64_t rows;
            std::uint64_t cols;

        public:

            // Constructor
            ZMaxrix(std::uint64_t rows, std::uint64_t cols) noexcept{
                this -> rows = rows;
                this -> cols = cols;

                data = (T **)malloc(rows * sizeof(T *));
                for (std::uint64_t i = 0; i < cols; i++){
                    data[i] = (T *)malloc(rows * sizeof(T));
                }
            }

            ZMaxrix(std::uint64_t rows, std::uint64_t cols, T pattern) noexcept{
                this -> rows = rows;
                this -> cols = cols;

                data = (T **)malloc(rows * sizeof(T *));
                for (std::uint64_t i = 0; i < cols; i++){
                    data[i] = (T *)malloc(rows * sizeof(T));
                }
                for (std::uint64_t i = 0; i < rows; i++){
                    for (std::uint64_t j = 0; j < cols; j++){
                        data[i][j] = pattern;
                    }
                }
            }

            // Destructor
            ~ZMaxrix() noexcept{
                for (std::uint64_t i = 0; i < cols; i++){
                    free(data[i]);
                }
                free(data);
            }
            
            // Getters
            ZMaxrix& print_matrix(void) noexcept{
                for (std::uint64_t i = 0; i < rows; i++){
                    for (std::uint64_t j = 0; j < cols; j++){
                        std::cout << data[i][j] << " ";
                    }
                    std::cout << std::endl;
                }
                return *this;
            }

            auto& operator [] (std::uint64_t index) {
                return data[index];
            }



            
    };
}