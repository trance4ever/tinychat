# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Produce verbose output by default.
VERBOSE = 1

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/trance/tinychat

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/trance/tinychat

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/trance/tinychat/CMakeFiles /home/trance/tinychat//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/trance/tinychat/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named tinychat

# Build rule for target.
tinychat: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 tinychat
.PHONY : tinychat

# fast build rule for target.
tinychat/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tinychat.dir/build.make CMakeFiles/tinychat.dir/build
.PHONY : tinychat/fast

#=============================================================================
# Target rules for targets named test_init

# Build rule for target.
test_init: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 test_init
.PHONY : test_init

# fast build rule for target.
test_init/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_init.dir/build.make CMakeFiles/test_init.dir/build
.PHONY : test_init/fast

src/easyconfig/config.o: src/easyconfig/config.cc.o
.PHONY : src/easyconfig/config.o

# target to build an object file
src/easyconfig/config.cc.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tinychat.dir/build.make CMakeFiles/tinychat.dir/src/easyconfig/config.cc.o
.PHONY : src/easyconfig/config.cc.o

src/easyconfig/config.i: src/easyconfig/config.cc.i
.PHONY : src/easyconfig/config.i

# target to preprocess a source file
src/easyconfig/config.cc.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tinychat.dir/build.make CMakeFiles/tinychat.dir/src/easyconfig/config.cc.i
.PHONY : src/easyconfig/config.cc.i

src/easyconfig/config.s: src/easyconfig/config.cc.s
.PHONY : src/easyconfig/config.s

# target to generate assembly for a file
src/easyconfig/config.cc.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tinychat.dir/build.make CMakeFiles/tinychat.dir/src/easyconfig/config.cc.s
.PHONY : src/easyconfig/config.cc.s

src/easyconfig/parse.o: src/easyconfig/parse.cc.o
.PHONY : src/easyconfig/parse.o

# target to build an object file
src/easyconfig/parse.cc.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tinychat.dir/build.make CMakeFiles/tinychat.dir/src/easyconfig/parse.cc.o
.PHONY : src/easyconfig/parse.cc.o

src/easyconfig/parse.i: src/easyconfig/parse.cc.i
.PHONY : src/easyconfig/parse.i

# target to preprocess a source file
src/easyconfig/parse.cc.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tinychat.dir/build.make CMakeFiles/tinychat.dir/src/easyconfig/parse.cc.i
.PHONY : src/easyconfig/parse.cc.i

src/easyconfig/parse.s: src/easyconfig/parse.cc.s
.PHONY : src/easyconfig/parse.s

# target to generate assembly for a file
src/easyconfig/parse.cc.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tinychat.dir/build.make CMakeFiles/tinychat.dir/src/easyconfig/parse.cc.s
.PHONY : src/easyconfig/parse.cc.s

src/init.o: src/init.cc.o
.PHONY : src/init.o

# target to build an object file
src/init.cc.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tinychat.dir/build.make CMakeFiles/tinychat.dir/src/init.cc.o
.PHONY : src/init.cc.o

src/init.i: src/init.cc.i
.PHONY : src/init.i

# target to preprocess a source file
src/init.cc.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tinychat.dir/build.make CMakeFiles/tinychat.dir/src/init.cc.i
.PHONY : src/init.cc.i

src/init.s: src/init.cc.s
.PHONY : src/init.s

# target to generate assembly for a file
src/init.cc.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tinychat.dir/build.make CMakeFiles/tinychat.dir/src/init.cc.s
.PHONY : src/init.cc.s

src/log.o: src/log.cc.o
.PHONY : src/log.o

# target to build an object file
src/log.cc.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tinychat.dir/build.make CMakeFiles/tinychat.dir/src/log.cc.o
.PHONY : src/log.cc.o

src/log.i: src/log.cc.i
.PHONY : src/log.i

# target to preprocess a source file
src/log.cc.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tinychat.dir/build.make CMakeFiles/tinychat.dir/src/log.cc.i
.PHONY : src/log.cc.i

src/log.s: src/log.cc.s
.PHONY : src/log.s

# target to generate assembly for a file
src/log.cc.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tinychat.dir/build.make CMakeFiles/tinychat.dir/src/log.cc.s
.PHONY : src/log.cc.s

src/util.o: src/util.cc.o
.PHONY : src/util.o

# target to build an object file
src/util.cc.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tinychat.dir/build.make CMakeFiles/tinychat.dir/src/util.cc.o
.PHONY : src/util.cc.o

src/util.i: src/util.cc.i
.PHONY : src/util.i

# target to preprocess a source file
src/util.cc.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tinychat.dir/build.make CMakeFiles/tinychat.dir/src/util.cc.i
.PHONY : src/util.cc.i

src/util.s: src/util.cc.s
.PHONY : src/util.s

# target to generate assembly for a file
src/util.cc.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/tinychat.dir/build.make CMakeFiles/tinychat.dir/src/util.cc.s
.PHONY : src/util.cc.s

test/test_init.o: test/test_init.cc.o
.PHONY : test/test_init.o

# target to build an object file
test/test_init.cc.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_init.dir/build.make CMakeFiles/test_init.dir/test/test_init.cc.o
.PHONY : test/test_init.cc.o

test/test_init.i: test/test_init.cc.i
.PHONY : test/test_init.i

# target to preprocess a source file
test/test_init.cc.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_init.dir/build.make CMakeFiles/test_init.dir/test/test_init.cc.i
.PHONY : test/test_init.cc.i

test/test_init.s: test/test_init.cc.s
.PHONY : test/test_init.s

# target to generate assembly for a file
test/test_init.cc.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_init.dir/build.make CMakeFiles/test_init.dir/test/test_init.cc.s
.PHONY : test/test_init.cc.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... test_init"
	@echo "... tinychat"
	@echo "... src/easyconfig/config.o"
	@echo "... src/easyconfig/config.i"
	@echo "... src/easyconfig/config.s"
	@echo "... src/easyconfig/parse.o"
	@echo "... src/easyconfig/parse.i"
	@echo "... src/easyconfig/parse.s"
	@echo "... src/init.o"
	@echo "... src/init.i"
	@echo "... src/init.s"
	@echo "... src/log.o"
	@echo "... src/log.i"
	@echo "... src/log.s"
	@echo "... src/util.o"
	@echo "... src/util.i"
	@echo "... src/util.s"
	@echo "... test/test_init.o"
	@echo "... test/test_init.i"
	@echo "... test/test_init.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

