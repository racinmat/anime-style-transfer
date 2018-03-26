.SUFFIXES: .o .cpp

SOURCES=$(wildcard *.cpp)
export OBJFOLDER:=$(OBJFOLDER)/ibeosdk
OBJECTS:=$(SOURCES:%.cpp=$(OBJFOLDER)/%.o)

FOLDER_MISC=misc
FOLDER_DATABLOCKS=datablocks
FOLDER_DEVICES=devices
FOLDER_LISTENER=listener

ifeq ($(WITHJPEGSUPPORT), on)
	FOLDER_JPEGSUPPORT=jpegsupport
endif

ifeq ($(WITHMONGODB), on)
	FOLDER_DATABASE=database
endif

MY_TARGETS = $(FOLDER_MISC) $(FOLDER_DATABLOCKS) $(FOLDER_DEVICES) $(FOLDER_LISTENER) $(FOLDER_JPEGSUPPORT) $(FOLDER_DATABASE)

ifeq ($(WITHJPEGSUPPORT), on)
	FOLDER_JPEGSUPPORT=jpegsupport
endif

.PHONY : $(MY_TARGETS) all clean

all: $(MY_TARGETS) $(OBJECTS)

$(MY_TARGETS): CXXFLAGS+=$(LIBOBJCXXFLAGS)
$(MY_TARGETS):
	@if [ ! -d "$(OBJFOLDER)/$@" ]; then mkdir -p $(OBJFOLDER)/$@; fi;
	$(MAKE) -C $@ all

$(OBJFOLDER)/%.d: %.cpp
	@$(SHELL) -ec '$(CXX) -MM $(CPPFLAGS) $< \
                      | sed '\''s|\($*\)\.o[ :]*|\1.o $@ : |g'\'' \
                      | sed '\''s|$*\.o|$(OBJFOLDER)/$*.o|g'\'' > $@; \
                      [ -s $@ ] || rm -f $@'

clean:
	@if [ -d $(OBJFOLDER)/$(FOLDER_MISC) ];       then $(MAKE) -C $(FOLDER_MISC) clean; fi
	@if [ -d $(OBJFOLDER)/$(FOLDER_DATABLOCKS) ]; then $(MAKE) -C $(FOLDER_DATABLOCKS) clean; fi
	@if [ -d $(OBJFOLDER)/$(FOLDER_DEVICES) ];    then $(MAKE) -C $(FOLDER_DEVICES) clean; fi
	@if [ -d $(OBJFOLDER)/$(FOLDER_LISTENER) ];   then $(MAKE) -C $(FOLDER_LISTENER) clean; fi
	@if [ -d $(OBJFOLDER)/$(FOLDER_JPEGSUPPORT) ];   then $(MAKE) -C $(FOLDER_JPEGSUPPORT) clean; fi
	@if [ -d $(OBJFOLDER)/$(FOLDER_DATABASE) ];   then $(MAKE) -C $(FOLDER_DATABASE) clean; fi
	@if [ -d $(OBJFOLDER)/$(FOLDER_JPEGSUPPORT) ];   then $(MAKE) -C $(FOLDER_JPEGSUPPORT) clean; fi
	@echo "   Remove .o and .d files from directory $(OBJFOLDER)"
	@rm -f $(OBJFOLDER)/*.o $(OBJFOLDER)/*.d
#	$(MAKE) -C $(FOLDER_UNITTESTS) clean



#unittests:
#	$(MAKE) all
#	$(MAKE) -C $(FOLDER_UNITTESTS)

-include $(OBJECTS:.o=.d)

$(OBJFOLDER)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(LIBOBJCXXFLAGS) -o $@ -c $<

