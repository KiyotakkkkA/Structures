#include <iostream>
#include <cstdint>

//солнце встало, я устала

namespace zspace {
    template <typename T>
    class ZVector
    {
        private:
            std::size_t length;
            std::size_t capacity;
            T *data;

            // Helper functions
            std::int64_t partition(std::int64_t start, std::int64_t end) noexcept{

                T pivot = data[end];
                std::int64_t ind = start;

                for (std::size_t i = start; i < end; i++){
                    if (data[i] <= pivot){
                        swap(i, ind);
                        ind++;
                    }
                }

                swap(ind, end);

                return ind;
            }

        public:
            // Constructors
            ZVector(void) noexcept
            {
                this -> length = 0;
                this -> capacity = 1;

                this -> data = (T *)malloc(sizeof(T) * this -> capacity);
            }

            ZVector(const std::size_t &length) noexcept{
                this -> length = length;
                this -> capacity = 1;

                while (this -> capacity < this -> length){
                    this -> capacity *= 2;
                }
                this -> data = (T *)malloc(sizeof(T) * this -> capacity);
            }

            ZVector(const std::size_t &length, const T &value) noexcept{
                this -> length = length;
                this -> capacity = 1;

                while (this -> capacity < this -> length){
                    this -> capacity *= 2;
                }
                this -> data = (T *)malloc(sizeof(T) * this -> capacity);
                for (std::size_t i = 0; i < this -> length; i++){
                    this -> data[i] = value;
                }
            }

            ZVector(const std::initializer_list<T> &list) noexcept{
                std::size_t sz = list.size();
                std::size_t counter = 0;

                this -> length = sz;
                this -> capacity = 1;

                while (this -> capacity < this -> length){
                    this -> capacity *= 2;
                }
                this -> data = (T *)malloc(sizeof(T) * this -> capacity);

                for (T elem: list){
                    data[counter++] = elem;
                }
            }

            // Destructor
            ~ZVector(void) noexcept{
                free(this -> data);
            }

            // Getters
            ZVector& print_vector(void) noexcept{
                for (std::size_t i = 0; i < this -> length; i++){
                    std::cout << this -> data[i] << " ";
                }
                std::cout << std::endl;
                return *this;
            }

            std::size_t size(void) const noexcept{
                return length;
            }

            std::size_t capacity_size(void) const noexcept{
                return capacity;
            }

            T get(const std::size_t &index) const{
                if (index >= length){
                    throw std::out_of_range("ERROR! List index out of range");
                }
                return data[index];
            };

            std::size_t index(const std::size_t &value) const noexcept{
                for (std::size_t i = 0; i < length; i++){
                    if (data[i] == value){
                        return i;
                    }
                }
                return -1;
            };

            // Setters
            ZVector& reserve(const std::size_t &reserve_amount) noexcept{
                if (capacity < reserve_amount){

                    capacity = reserve_amount;
                    T *temp = (T *)malloc(sizeof(T) * capacity);

                    for (std::size_t i = 0; i < length; i++){
                        temp[i] = data[i];
                    }

                    free(data);
                    data = temp;

                }
                return *this;
            }

            ZVector& resize(const std::size_t &new_size) noexcept{
                if (length == capacity) return *this;

                capacity = new_size;
                T *temp = (T *)malloc(sizeof(T) * capacity);

                if (length < capacity){
                    for (std::size_t i = 0; i < length; i++){
                        temp[i] = data[i];
                    }
                    for (std::size_t j = length; j < capacity; j++){
                        temp[j] = T();
                    }
                }
                else if (length > capacity){
                    for (std::size_t i = 0; i < capacity; i++){
                        temp[i] = data[i];
                    }
                }
                free(data);

                data = temp;
                length = new_size;

                return *this;
            }

            // Override operators
            T& operator [] (const std::size_t &index) noexcept{
                return data[index];
            }

            // Insertion
            ZVector& insert(const std::size_t &index, const T &value){
                if (index > this -> length){
                    throw std::out_of_range("ERROR! List index out of range");
                    return *this;
                }
                if (this -> length == 0){
                    this -> data[0] = value;
                    this -> length++;
                    return *this;
                }
                if (length + 1 <= capacity){
                    T tmp1, tmp2;
                    for (std::size_t i = 0; i < length+1; i++){
                        if (i == index){
                            tmp1 = data[i];
                            data[i] = value;
                        }
                        else if (i > index){
                            tmp2 = data[i];
                            data[i] = tmp1;
                            tmp1 = tmp2;
                        }
                    }
                    length++;
                    return *this;
                }
                else{
                    this -> capacity *= 2;

                    std::size_t counter = 0;
                    T *temp = (T *)malloc(sizeof(T) * this -> capacity);
                    for (std::size_t i = 0; i < this -> length + 1; i++){
                        if (i != index){
                            temp[i] = data[counter++];
                        }
                        else{
                            temp[i] = value;
                        }
                    }
                    this -> length++;
                    free(this -> data);

                    data = temp;

                    return *this;
                }
            }        
            
            ZVector& push_back(const T &value){
                insert(length, value);
                return *this;
            }

            ZVector& push(const T &value){
                insert(0, value);
                return *this;
            }

            // Remove section
            T pop(const std::size_t index){
                if (index >= length){
                    throw std::out_of_range("ERROR! List index out of range");
                }
                std::size_t counter = 0;
                T exception;
                T *temp = (T *)malloc((length - 1)*sizeof(T));
                for (std::size_t i = 0; i < length; i++){
                    if (i != index){
                        temp[counter++] = data[i];
                    }
                    else exception = data[i];
                }
                length--;

                free(data);
                data = temp;

                return exception;
            }

            ZVector& erase(const std::size_t index){
                pop(index);
                return *this;
            }

            // Utility methods
            ZVector& swap(const std::size_t index1, const std::size_t index2){
                T tmp = data[index1];
                data[index1] = data[index2];
                data[index2] = tmp;
                return *this;
            }

            ZVector& reverse(void) noexcept {
                T *temp = (T *)malloc(sizeof(T) * length);
                for (std::size_t i = 0; i < length; i++){
                    temp[i] = data[i];
                }
                for (std::size_t i = 0; i < length; i++){
                    data[i] = temp[length - i - 1];
                }
                free(temp);
                return *this;
            }

            ZVector& quicksort(std::int64_t start, std::int64_t end) noexcept{
                // Quick sort algorithm
                // This algorithm is based on the division of the array into two parts and recursive calls to sort each of them
                // The difficulty of this algorithm is O(n log n)

                if (start < end){
                    T pivot = data[end];

                    std::int64_t position = partition(start, end);

                    quicksort(start, position - 1);
                    quicksort(position + 1, end);
                }
                return *this;

            }

    };
}


// захар зайчик