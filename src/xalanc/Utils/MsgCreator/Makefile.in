#
# Copyright 1999-2004 The Apache Software Foundation.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# Output directory
OUTDIR := ../../../..

include $(XALANCROOT)/version.incl
include ../../../../Makefile.incl

.PHONY: doClean prepare install

MSG_CREATOR_OBJECTS= $(XSL_OBJ_DIR)/MsgFileOutputStream.o $(XSL_OBJ_DIR)/ICUResHandler.o $(XSL_OBJ_DIR)/InMemHandler.o $(XSL_OBJ_DIR)/MsgCreator.o $(XSL_OBJ_DIR)/NLSHandler.o $(XSL_OBJ_DIR)/SAX2Handler.o 

MSG_CREATOR_SRC=$(XALANCROOT)/src/xalanc/Utils/MsgCreator


$(XSL_BIN_DIR)/MsgCreator : $(MSG_CREATOR_OBJECTS)
	${LINK} $(XSL_BUILD_OPTIONS) ${PLATFORM_LIB_LINK_OPTIONS} ${LIBRARY_SEARCH_PATHS} \
	${EXTRA_LINK_OPTIONS} $(LIBS) $^ -o $@ $(XERCES_LIB) $(LOC_OTHER_LINK_PARAMETERS)
	
$(XSL_OBJ_DIR)/%.o : $(MSG_CREATOR_SRC)/%.cpp    
	$(CC1) $(XSL_BUILD_OPTIONS) -c $(XSL_INCL) $(EXTRA_COMPILE_OPTIONS) -o $@ $<

$(XSL_OBJ_DIR)/%.o : $(MSG_CREATOR_SRC)/%.hpp    
	$(CC1) $(XSL_BUILD_OPTIONS) -c $(XSL_INCL) $(EXTRA_COMPILE_OPTIONS) -o $@ $<


prepare:

install:

doClean:
	rm -f $(XSL_BIN_DIR)/MsgCreator$(EXT)
	rm -f $(MSG_CREATOR_OBJECTS)
