PROJECT=Principal

TARGET_FOLDER=objects

CLASSES=$(wildcard *.java)

OBJ=$(addprefix $(TARGET_FOLDER)/, $(subst .java,.class,$(CLASSES)))

JAVA_FLAGS=--class-path

all: objFolder $(CLASSES) $(PROJECT) 

$(PROJECT): $(OBJ)
	@ echo 'Running binary using JAVA: $@'
	@ echo ' '
	@ java $(JAVA_FLAGS) $(TARGET_FOLDER) $@

$(TARGET_FOLDER)/%.class: %.java
	@ echo 'Building target using JAVAC compiler: $<'
	@ javac $< -d $(TARGET_FOLDER)

objFolder:
	@ mkdir -p objects

clean:
	@ rm $(TARGET_FOLDER)/*.class
