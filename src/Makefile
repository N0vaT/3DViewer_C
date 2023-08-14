CP := gcc
CFLAGS := -Wall -Werror -Wextra -std=c11
GCOVFLAGS := -L. --coverage
OS = $(shell uname)
ifeq ($(OS), Linux)
	CHECK_FLAGS = $(shell pkg-config --libs check) -lm
else
	CHECK_FLAGS = -lcheck
endif
SRC_BACKEND := $(wildcard backend/*.c)
SRC_TEST := $(wildcard tests/*.c)
APP_NAME := 3DViewer.app
APP_VERSION := v1.0
DOC_PATH := ../documentation/
DIST_PATH := ../archive

all: test

install: uninstall
	- rm -rf qt_gui/CMakeCache.txt
	- rm -rf qt_gui/CMakeFiles/
	- rm -rf qt_gui/Makefile
	- rm -rf qt_gui/cmake_install.cmake
	- rm -rf qt_gui/qt_gui.app
	- mkdir ../build
	cd qt_gui; cmake .; make;
	cp -R qt_gui/qt_gui.app ../build/$(APP_NAME)
	@chmod +rwx ../build
	@chmod +rwx ../build/$(APP_NAME)

uninstall:
	rm -rf ../build
	rm -rf /tmp/settings

clean:
	- rm -rf test .clang-format report gcov_report *.gcno *.gcda *.info *CPPLINT.cfg
	- rm -rf qt_gui/CMakeCache.txt
	- rm -rf qt_gui/CMakeFiles
	- rm -rf qt_gui/Makefile
	- rm -rf qt_gui/cmake_install.cmake
	- rm -rf qt_gui/CMakeLists.txt.user
	- rm -rf qt_gui/.qt
	- rm -rf qt_gui/qt_gui
	- rm -rf qt_gui/qt_gui.app
	- rm -rf $(DIST_PATH)
	- rm -rf $(DOC_PATH)
	- rm -rf build-qt_gui*
	- rm -rf qt_gui/qt_gui_autogen
	- rm -rf qt_gui/qt_gui.app
	- rm -rf settings
	- rm -rf non_existing_file

dist: install dvi test
	- rm -rf $(DIST_PATH)
	- mkdir $(DIST_PATH)
	- mkdir $(DIST_PATH)/$(APP_NAME)_$(APP_VERSION)
	- cp -r ../src $(DIST_PATH)/$(APP_NAME)_$(APP_VERSION)/src
	- cp -r $(DOC_PATH) ../archive/$(APP_NAME)_$(APP_VERSION)/$(DOC_PATH)
	- cp -r Manual ../archive/$(APP_NAME)_$(APP_VERSION)
	- cd $(DIST_PATH) && tar cvzf Archive_$(APP_NAME)_$(APP_VERSION).tgz * && rm -rf $(APP_NAME)_$(APP_VERSION)
	- rm -rf $(DIST_PATH)/src
	- rm -rf $(DIST_PATH)/documentation

dvi:
	rm -rf $(DOC_PATH)
	mkdir $(DOC_PATH)
	doxygen Doxyfile
	
rename_files:
	@- cp tests/test_files/cube.obj1 tests/test_files/cube.obj
	@- cp tests/test_files/lamp.obj1 tests/test_files/lamp.obj
	@- cp tests/test_files/skull.obj1 tests/test_files/skull.obj
	@- cp tests/test_files/empty.obj1 tests/test_files/empty.obj
	@- cp tests/test_files/test.obj1 tests/test_files/test.obj
	@- cp tests/test_files/test1.obj1 tests/test_files/test1.obj
	@- cp tests/test_files/test2.obj1 tests/test_files/test2.obj

test:
	@- make rename_files
	$(CP) $(CFLAGS) -o test $(SRC_BACKEND) $(SRC_TEST) $^ $(CHECK_FLAGS) -fsanitize=address
	./test

gcov_report:
	@- make rename_files
	$(CP) $(CFLAGS) $(SRC_BACKEND) $(SRC_TEST) $(CHECK_FLAGS) $(GCOVFLAGS) -o $@ 
	./gcov_report
	lcov -t gcov_report -o rep.info -c -d .
	genhtml -o report rep.info
	open ./report/index.html
	rm -rf *.gcda *.gcno *.info

codestyle:
	cp ../materials/linters/.clang-format .
	clang-format -n backend/*.c backend/*.h tests/*.c tests/*.h qt_gui/*.h qt_gui/*.cpp qt_gui/gifmaker/*.c qt_gui/gifmaker/*.cpp qt_gui/gifmaker/*.h
