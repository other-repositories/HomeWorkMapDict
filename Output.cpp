#include <iostream>
#include <map>
#include <chrono>
#include <fstream>

int main()
{
    std::ofstream out("map_time.txt");
    std::ofstream out2("map_memory.txt");

    for (int i = 1; i <= 10'000'000; i *= 10)
    {
        std::map<int, int> m;

        auto start = std::chrono::steady_clock::now();
        for (int j = 0; j < i; ++j)
        {
            m[j] = j;
        }
        auto end = std::chrono::steady_clock::now();

        auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        out << i << " " << float(time)/10'000'000 << std::endl;

        out2 << i << " " << m.size() * sizeof(int) * 2 << std::endl;
    }
    out.close();
    out2.close();
    return 0;
}