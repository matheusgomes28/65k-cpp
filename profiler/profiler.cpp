module;

#include <chrono>
#include <concepts>
#include <iostream>
#include <memory>
#include <source_location>
#include <string>

export module profiler;

export namespace profiler
{

    // Constrait for collections that can be used to store
    // a profile result
    template <typename T, typename FuncName, typename Measure>
    concept Bookeper = std::convertible_to<FuncName, std::string> && std::convertible_to<Measure, double>
                       && requires(T v, FuncName func_name, Measure measure) {
                              {
                                  v.update(func_name, measure)
                              } -> std::same_as<bool>;
                          };

    template <typename T, typename FuncName, typename Measure>
        requires Bookeper<T, FuncName, Measure>
    class FunctionProfiler
    {
    public:
        FunctionProfiler(std::shared_ptr<T> books, const std::source_location& sl = std::source_location::current())
            : _unit_name{sl.function_name()}, _start{std::chrono::high_resolution_clock::now()}, _books{books}
        {
        }

        ~FunctionProfiler()
        {
            using namespace std::chrono;
            auto const now       = std::chrono::high_resolution_clock::now();
            auto const time_diff = duration_cast<duration<double>>(now - _start);

            // TODO : Save the info in a map or something
            if (!_books)
            {
                std::cout << "cannot store profile as books is null\n";
                return;
            }

            _books->update(_unit_name, time_diff.count());
        }

    private:
        std::string _unit_name;
        std::chrono::time_point<std::chrono::system_clock> _start;
        std::shared_ptr<T> _books;
    };
} // namespace profiler
