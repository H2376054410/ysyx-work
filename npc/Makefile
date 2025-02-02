# 定义变量
VERILATOR = verilator
CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2
LDFLAGS =

# 目标可执行文件
TARGET = obj_dir/Vtop

# Verilog 文件路径
VERILOG_FILES = vsrc/top.v

# C++ 仿真文件路径
CPP_FILES = csrc/sim_main.cpp

# 生成的波形文件
WAVEFORM_FILE = wave.vcd
# 默认目标
all: $(TARGET)
	$(TARGET) +trace
	gtkwave $(WAVEFORM_FILE)
# 生成 C++ 模型并编译成可执行文件
$(TARGET):
	$(VERILATOR) --cc $(VERILOG_FILES) --exe $(CPP_FILES) -Wno-fatal $(CXXFLAGS) --trace
	make -j -C obj_dir/ -f Vtop.mk Vtop
sim:
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "Write this Makefile by your self."


# 清理生成的文件
clean:
	rm -rf obj_dir/ $(WAVEFORM_FILE)

.PHONY: all clean
include ../Makefile





