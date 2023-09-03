#include <benchmark/benchmark.h>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>
#include <iostream>
#include <stduuid/uuid.h>
#include <thread>

template <typename generator>
class BenchMarker {
public:
    BenchMarker()
        : generator_()
    {
    }

    template <bool use_replace>
    inline std::string gen_string()
    {

        std::string str = boost::uuids::to_string(generator_());
        if constexpr (use_replace) {
            std::replace(str.begin(), str.end(), '-', '_');
        } else {
            str[8] = str[13] = str[18] = str[23] = '_';
        }
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

auto get_seed()
{
    auto thread_hash = std::hash<std::thread::id>()(std::this_thread::get_id());
    auto tick = std::chrono::steady_clock::now().time_since_epoch().count();
    auto rand = thread_hash ^ tick;
    std::cout << thread_hash << " ^ " << tick << " = " << rand << "\n";
    return rand;
}

template <bool use_replace>
static void BM_normal_gen(benchmark::State& state)
{
    BenchMarker<boost::uuids::random_generator> normal_uuid;

    for (auto _ : state) {
        benchmark::DoNotOptimize(
            normal_uuid.gen_string<use_replace>());
    }
}
BENCHMARK_TEMPLATE(BM_normal_gen, true)->Name("boost::uuids::random_generator (use std::replace)");
BENCHMARK_TEMPLATE(BM_normal_gen, false)->Name("boost::uuids::random_generator");

template <bool use_replace>
static void BM_mt19937_gen(benchmark::State& state)
{
    BenchMarker<boost::uuids::random_generator_mt19937> mt19937_uuid;

    for (auto _ : state) {
        benchmark::DoNotOptimize(
            mt19937_uuid.gen_string<use_replace>());
    }
}

BENCHMARK_TEMPLATE(BM_mt19937_gen, true)->Name("boost::uuids::random_generator_mt19937 (use std::replace)");
BENCHMARK_TEMPLATE(BM_mt19937_gen, false)->Name("boost::uuids::random_generator_mt19937");

template <bool use_replace>
static void BM_stduuid_gen(benchmark::State& state)
{
    std::random_device rd;
    auto seed_data = std::array<int, std::mt19937::state_size> {};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    std::mt19937 generator(seq);

    for (auto _ : state) {
        auto str = uuids::to_string(
            uuids::uuid_random_generator {generator}());

        if constexpr (use_replace) {
            std::replace(str.begin(), str.end(), '-', '_');
        } else {
            str[8] = str[13] = str[18] = str[23] = '_';
        }

        benchmark::DoNotOptimize(
            str);
    }
}

BENCHMARK_TEMPLATE(BM_stduuid_gen, true)->Name("uuids::uuid_system_generator (use std::replace)");
BENCHMARK_TEMPLATE(BM_stduuid_gen, false)->Name("uuids::uuid_system_generator");

BENCHMARK_MAIN();