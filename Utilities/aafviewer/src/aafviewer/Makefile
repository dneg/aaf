# GNU makefile for aafviewer


JDK		:= /usr/java/j2sdk1.4.1_02
JAVAC		:= $(JDK)/bin/javac
JAR		:= $(JDK)/bin/jar

THIS_DIR	:= $(shell pwd)
SRC_DIR		:= $(THIS_DIR)/src
OBJ_DIR		:= $(THIS_DIR)/classes
LIB_DIR		:= $(THIS_DIR)/lib
JAR_FILE	:= $(THIS_DIR)/aafviewer.jar

SOURCES		:= $(shell find $(SRC_DIR) -name \*.java -print)
OBJECTS		:= $(SOURCES:$(SRC_DIR)/%.java=$(OBJ_DIR)/%.class)

CLASSPATH	:= $(LIB_DIR)/xercesImpl.jar:$(LIB_DIR)/xmlParserAPIs.jar:$(LIB_DIR)/zvtm.jar


# default rule
.PHONY : all
all : $(OBJ_DIR) $(OBJECTS)

# make obj directory if does not exist
$(OBJ_DIR) :
	mkdir -p $(OBJ_DIR)
	
# compile all .java files
$(OBJ_DIR)/%.class : $(SRC_DIR)/%.java
	$(JAVAC) -O -classpath $(CLASSPATH) -sourcepath $(SRC_DIR) -d $(OBJ_DIR) $<

# archive the .class files
jar : 
	cd $(OBJ_DIR) ; $(JAR) cvf $(JAR_FILE) bbc
	cd $(OBJ_DIR) ; $(JAR) uvf $(JAR_FILE) com

# remove the class files
clean :
	rm -rf `find $(OBJ_DIR) -name \*.class -print`

