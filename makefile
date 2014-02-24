# SOURCES = src/main.cpp
# MAKE = g++ -g -o ./main $(SOURCES) -lGL -lglfw

# all:
# 	$(MAKE) -O2
# debug:
# 	$(MAKE) -O0
# clean:
# 	rm -f ./main && all

######################################################################################################

# taken from http://ubuntuforums.org/showthread.php?t=1204739

app = main

srcExt = cpp
srcDir = src
objDir = out/obj
binDir = .
inc = out/inc

debug = 1

CFlags = -Wall -Wextra -O2 -std=c++11
LDFlags =
libs = GL glfw
libDir =

#************************ DO NOT EDIT BELOW THIS LINE! ************************

ifeq ($(debug),1)
	debug= -g3
else
	debug= -g1
endif
inc := $(addprefix -I,$(inc))
libs := $(addprefix -l,$(libs))
libDir := $(addprefix -L,$(libDir))
CFlags += -c $(debug) $(inc) $(libDir) $(libs)
sources := $(shell find $(srcDir) -name '*.$(srcExt)')
srcDirs := $(shell find . -name '*.$(srcExt)' -exec dirname {} \; | uniq)
objects := $(patsubst %.$(srcExt),$(objDir)/%.o,$(sources))
LDFlags += $(libs)

ifeq ($(srcExt), cpp)
	CC = $(CXX)
	# CC = clang++
else
	CFlags += -std=gnu99
endif

dependencies := $(objects:.o=.d)

.phony: all clean distclean


all: distclean $(binDir)/$(app)

$(binDir)/$(app): buildrepo $(objects)
	@echo ""
	@echo "Linking Flags:"
	@echo "	" $(LDFlags)

	@mkdir -p `dirname $@`
	@echo ""
	@echo "Linking $@..."
	@echo "	"$(objects) $(LDFlags) -o $@
	@$(CC) $(objects) $(LDFlags) -o $@

	@echo ""

$(objDir)/%.o: %.$(srcExt)
	@echo ""
	@echo "Compiler Flags:"
	@echo "	" $(CFlags) $<

	@echo ""
	@echo "Generating dependencies for $<..."
	@$(call make-depend,$<,$@,$(subst .o,.d,$@))
	@echo "Compiling $<..."
	@$(CC) $(CFlags) $< -o $@

clean:
	$(RM) -R -v $(objDir)
	$(RM) -R -v $(binDir)/$(app)

distclean:
	$(RM) -R $(binDir)/$(app)

buildrepo:
	@$(call make-repo)

define make-repo
   for dir in $(srcDirs); \
   do \
	mkdir -p $(objDir)/$$dir; \
   done
endef


# usage: $(call make-depend,source-file,object-file,depend-file)
define make-depend
  $(CC) -MM       \
        -MF $3    \
        -MP       \
        -MT $2    \
        $(CFlags) \
        $1
endef

# needed to notice changes in header and recompile all dependent sources
-include $(dependencies)
