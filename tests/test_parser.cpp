#define main project_main
#include "../app/main.cpp"
#undef main
#include <cassert>
int main() {
    const auto values = parse_snapshot("cpu_count=4\nonline_cpus=3\nmem_total_kb=1024\n");
    assert(values.at("cpu_count") == "4");
    assert(values.at("online_cpus") == "3");
    assert(values.at("mem_total_kb") == "1024");
    return 0;
}
