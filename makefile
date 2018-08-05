all: code_jam common distributed_code_jam hackerrank tester

code_jam: google/code_jam/**
	cd google/code_jam && make -j 16

common: common/**
	cd common && make -j 16

distributed_code_jam: google/distributed_code_jam/**
	cd google/distributed_code_jam && make -j 16

hackerrank: hackerrank/**
	cd hackerrank && make -j 16

tester: tester/**
	cd tester && make -j 16

clean_code_jam:
	cd google/code_jam/ && make -j 16 clean

clean_common:
	cd common && make -j 16 clean

clean_distributed_code_jam:
	cd google/distributed_code_jam/ && make -j 16 clean

clean_hackerrank:
	cd hackerrank && make -j 16 clean

clean_tester:
	cd tester && make -j 16 clean

clean: clean_code_jam clean_common clean_distributed_code_jam clean_hackerrank clean_tester
