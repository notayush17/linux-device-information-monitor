CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2
BUILD := build

.PHONY: all app demo module test clean

all: app

demo: app
	./build/hw_monitor --demo

app: $(BUILD)/hw_monitor $(BUILD)/test_parser

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/hw_monitor: app/main.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD)/test_parser: tests/test_parser.cpp app/main.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -DUNIT_TEST $< -o $@

module: | $(BUILD)
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(CURDIR)/driver modules
	cp driver/hw_health.ko $(BUILD)/hw_health.ko

test: app
	./build/test_parser
	./tests/smoke_test.sh

clean:
	rm -rf $(BUILD)
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(CURDIR)/driver clean 2>/dev/null || true
