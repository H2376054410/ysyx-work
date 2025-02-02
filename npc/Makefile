all:
	@echo "Write this Makefile by your self."

sim:
#	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "Write this Makefile by yourself."
	verilator --cc vsrc/top.v --exe csrc/main.cpp -Wno-fatal -CFLAGS "-std=c++11" --trace
	make -j -C obj_dir/ -f Vtop.mk Vtop
	./obj_dir/Vtop +trace
# 正确的目标调用函数
rtl:
	git add .
	git commit -m "write my own makefile"
include ../Makefile

