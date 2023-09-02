#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <benchmark/benchmark.h>
#include <chrono>
#include <iostream>
#include <thread>

template <typename generator>
class BenchMarker {
public:
    BenchMarker()
        : generator_()
    {
    }

    inline std::string gen_string()
    {
        std::string str = boost::uuids::to_string(generator_());
        std::replace(str.begin(), str.end(), '-', '_');
        return str;
    }

    std::chrono::nanoseconds bench_ns_per_gen(std::chrono::steady_clock& clock, int iters)
    {
        begin = clock.now();

        for (int i = 0; i < iters; i++) {
            gen_string();
        }

        auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(clock.now() - begin);

        return elapsed_ns / iters;
    }

private:
    mutable generator generator_;
    std::chrono::steady_clock::time_point begin;
};

auto get_seed(){
    auto thread_hash = std::hash<std::thread::id>()(std::this_thread::get_id());
    auto tick = std::chrono::steady_clock::now().time_since_epoch().count();
    auto rand = thread_hash ^ tick;
    std::cout << thread_hash << " ^ " << tick << " = " << rand << "\n";
    return rand;
}

static void BM_normal_gen(benchmark::State &state) {
    BenchMarker<boost::uuids::random_generator> normal_uuid;

    for(auto _: state){
        normal_uuid.gen_string();
    }
}
BENCHMARK(BM_normal_gen);

static void BM_mt19937_gen(benchmark::State &state) {
    BenchMarker<boost::uuids::random_generator_mt19937> mt19937_uuid;

    for(auto _: state){
        mt19937_uuid.gen_string();
    }
}
BENCHMARK(BM_mt19937_gen);

BENCHMARK_MAIN();