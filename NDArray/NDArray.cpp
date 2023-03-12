#include <algorithm>
#include <iostream>
#include <array>
#include <vector>
#include <variant>
#include <numeric>

template <class T,size_t SIZE_W,size_t SIZE_H>
class NDArray {
public:
	enum class FillType {
		Null,
		Zero,
		Ones,
		Random
	};

    enum class FastType {
        MIN,
        MAX,
        MED
    };

    enum class RangeType : int{
        ROWS,
        COLUMNS
    };

	NDArray(FillType flags = FillType::Null) {
		int value;
		switch (flags) {
			case FillType::Zero:
				value = 0;
                [[fallthrough]];
			case FillType::Ones:
				value = 1;
				std::fill(std::begin(m_array), std::end(m_array), value);
				break;
			case FillType::Random:
                std::generate(std::begin(m_array), std::end(m_array), [&] {return static_cast<T>(rand()); });
				break;
		}
	}

    T& operator[](int index) {
        return m_array[index];
    }

    const T& operator[](int index) const {
        return m_array[index];
    }

    size_t size() const {
        return SIZE_W * SIZE_H;
    }
    
    NDArray& operator=(const NDArray& nd2) {
        std::copy(std::begin(nd2.m_array) ,std::end(nd2.m_array),std::begin(m_array));
        return *this;
    }

    NDArray<T,SIZE_H,SIZE_W> getTransposive() {
        NDArray<T, SIZE_H, SIZE_W> arr;
        std::copy(std::begin(m_array), std::end(m_array), arr.begin());
        return arr;
    }

    //Доп баллы, новичковый метод
    std::vector<T*> getSlice(int index_from, int index_to) {
        //RVO 17 не произвойдет не одного копирования вектора лишь перемещение в main
        std::vector<T*> out(index_to-index_from+1); 
        std::transform(std::begin(m_array)+ index_from, std::begin(m_array)+ index_to +1,
                            std::begin(out), [](T& elem) {return &elem; });
        return out;
    }

    //Более предпочтительный через итераторы
    auto begin() {
        return std::begin(m_array);
    }
    auto end() {
        return std::end(m_array);
    }

    T mean(FastType f_type) {
        T t = T();
        switch (f_type) {
            case FastType::MAX:
                t = *std::max_element(std::begin(m_array),std::end(m_array));
                break;
            case FastType::MIN:
                t = *std::min_element(std::begin(m_array), std::end(m_array));
                break;
            case FastType::MED:
                t = std::accumulate(std::begin(m_array), std::end(m_array),0)/ (SIZE_H* SIZE_W);
                break;
        }
        return t;
    }
    
    std::variant <std::array<T,SIZE_H>,std::array<T, SIZE_W> > mean(FastType f_type, RangeType r_type) {
        //METAPROGRAMMING ITS TOO LONG FOR THIS TASK I PREFER THIS WAY
        switch (r_type) {
        case RangeType::ROWS: {
            std::array<T, SIZE_H> arr;
            for (int i = 0; i < SIZE_H;i++) {
                switch (f_type) {
                case FastType::MAX: arr[i] = *std::max_element(std::begin(m_array) + i * SIZE_W, std::begin(m_array) + (i + 1) * SIZE_W); break;
                case FastType::MIN: arr[i] = *std::min_element(std::begin(m_array) + i * SIZE_W, std::begin(m_array) + (i + 1) * SIZE_W); break;
                case FastType::MED: arr[i] = std::accumulate(std::begin(m_array) + i * SIZE_W, std::begin(m_array) + (i + 1) * SIZE_W, 0) / (SIZE_W); break;
                }
            }
            return arr;
        }
            break;
        case RangeType::COLUMNS: {
            std::array<T, SIZE_W> arr;
            for (int i = 0; i < SIZE_W; i++) {
                switch (f_type) {
                case FastType::MAX: arr[i] = *std::max_element(std::begin(m_array) + i * SIZE_H, std::begin(m_array) + (i + 1) * SIZE_H); break;
                case FastType::MIN: arr[i] = *std::min_element(std::begin(m_array) + i * SIZE_H, std::begin(m_array) + (i + 1) * SIZE_H); break;
                case FastType::MED: arr[i] = std::accumulate(std::begin(m_array) + i * SIZE_H, std::begin(m_array) + (i + 1) * SIZE_H, 0) / (SIZE_H); break;
                }
            }
            return arr;
        }
        break;
        }
        return {};
    }

    template <size_t SIZE_W_OTHER,size_t SIZE_H_OTHER>
    void multiplyMatrix(const NDArray<T,SIZE_W_OTHER,SIZE_H_OTHER> & mat2) {
        T out;
        T mat_out[SIZE_W * SIZE_H];
        for (int i = 0; i < SIZE_W; i++) {
            for (int z = 0; z < SIZE_H; z++) {
                out = 0;
                for (int j = 0; j < SIZE_H_OTHER; j++) {
                    out += m_array[i* SIZE_W + j] * mat2[j * SIZE_W_OTHER + z];
                }
                mat_out[i*SIZE_W +z] = out;
            }
        }
        std::copy(std::begin(mat_out), std::end(mat_out), std::begin(m_array));
    }
    

    NDArray operator+(const NDArray& other) const {
        NDArray result(*this);
        for (int i = 0; i < result.size(); i++)
            result[i] += other[i];
        return result;
    }

    // Element-wise subtraction
    NDArray operator-(const NDArray& other) const {
        NDArray result(*this);
        for (int i = 0; i < result.size(); i++)
            result[i] -= other[i];
        return result;
    }

    // Element-wise multiplication
    NDArray operator*(const NDArray& other) const {
        NDArray result(*this);
        for (int i = 0; i < result.size(); i++)
            result[i] *= other[i];
        return result;
    }

    // Element-wise division
    NDArray operator/(const NDArray& other) const {
        NDArray result(*this);
        for (int i = 0; i < result.size(); i++)
            result[i] /= other[i];
        return result;
    }

    NDArray operator+(const T& other) const {
        NDArray result(*this);
        for (int i = 0; i < result.size(); i++)
            result[i] += other;
        return result;
    }

    // Element-wise subtraction
    NDArray operator-(const T& other) const {
        NDArray result(*this);
        for (int i = 0; i < result.size(); i++)
            result[i] -= other;
        return result;
    }

    // Element-wise multiplication
    NDArray operator*(const T& other) const {
        NDArray result(*this);
        for (int i = 0; i < result.size(); i++)
            result[i] *= other;
        return result;
    }

    // Element-wise division
    NDArray operator/(const T& other) const {
        NDArray result(*this);
        for (int i = 0; i < result.size(); i++)
            result[i] /= other;
        return result;
    }

    //SUPPORT
    
    void draw() {
        for (int i = 0; i < SIZE_H; i++) {
            for (int j= 0; j < SIZE_W; j++) {
                std::cout << m_array[i * SIZE_W + j] << " ";
            }
            std::cout << '\n';
        }
    }

private:
	T m_array[SIZE_W*SIZE_H];
};

int main() {
	using ArrayType = NDArray<int, 2, 3>;

    //EXAMPLE 1
	    ArrayType arr(ArrayType::FillType::Random); // ZERO ONCE NULL modes includes

    //EXAMPLE 1.5
        auto arr_sum = arr + arr; // - * / also
        arr_sum.draw();
    //EXAMPLE 2
        std::cout << arr.mean(ArrayType::FastType::MED) << std::endl;

    //EXAMPLE 3
        auto out = std::get<static_cast<int>(ArrayType::RangeType::COLUMNS)>(arr.mean(ArrayType::FastType::MED, ArrayType::RangeType::COLUMNS));
        for (auto & elem : out) {
            std::cout << elem << " ";
        }
        std::cout << "\n";

    //EXAMPLE 3 доп баллы
        auto out2 = arr.getSlice(1, 4);
        for (auto elem : out2) {
            *elem = 8;
        }

        arr.draw();
        // OUTPUT x 8  
        //        8 8 
        //        8 x
        //OR
        for (auto& elem : arr) {
            elem = 5;
        }
        arr.draw();
        // OUTPUT 5 5 
        //        5 5 
        //        5 5
        
    //EXAMPLE 4
        auto arr2 = arr.getTransposive();
        arr2.draw();
        // OUTPUT 5 5 5
        //        5 5 5
}