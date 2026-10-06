#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

std::map<std::string, std::string> parse_snapshot(const std::string& text) {
    std::map<std::string, std::string> values;
    std::istringstream input(text);
    std::string line;
    while (std::getline(input, line)) {
        const auto split = line.find('=');
        if (split != std::string::npos) values[line.substr(0, split)] = line.substr(split + 1);
    }
    return values;
}

static const char* demo_snapshot =
    "cpu_count=8\nonline_cpus=8\nmem_total_kb=16252928\nmem_free_kb=6359040\nuptime_seconds=4312\n";

int display(const std::string& snapshot, const std::string& source) {
    const auto v = parse_snapshot(snapshot);
    for (const char* key : {"cpu_count", "online_cpus", "mem_total_kb", "mem_free_kb", "uptime_seconds"})
        if (!v.count(key)) { std::cerr << "Invalid snapshot: missing " << key << '\n'; return 1; }
    std::cout << "Linux Device Information Monitor\n---------------------------------\n"
              << "Source        : " << source << '\n'
              << "CPU cores     : " << v.at("cpu_count") << '\n'
              << "Online CPUs   : " << v.at("online_cpus") << '\n'
              << "Memory total  : " << std::stoll(v.at("mem_total_kb")) / 1024 << " MB\n"
              << "Memory free   : " << std::stoll(v.at("mem_free_kb")) / 1024 << " MB\n"
              << "Kernel uptime : " << v.at("uptime_seconds") << " seconds\n";
    return 0;
}

int main(int argc, char** argv) {
    if (argc > 1 && std::string(argv[1]) == "--demo") return display(demo_snapshot, "built-in demo data");
    const int fd = open("/dev/hw_health", O_RDONLY);
    if (fd < 0) { std::cerr << "Cannot open /dev/hw_health: " << std::strerror(errno) << "\nLoad the module or run with --demo.\n"; return 1; }
    char buffer[512]{}; const ssize_t n = read(fd, buffer, sizeof(buffer) - 1); close(fd);
    if (n < 0) { std::cerr << "read failed: " << std::strerror(errno) << '\n'; return 1; }
    return display(std::string(buffer, static_cast<size_t>(n)), "kernel module (/dev/hw_health)");
}
