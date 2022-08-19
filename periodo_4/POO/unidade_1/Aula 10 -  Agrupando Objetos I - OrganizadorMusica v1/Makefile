PROJECT=Principal

TARGET_FOLDER=objects

CLASSES=$(shell find . -name "*.java")

OBJ=$(addprefix $(TARGET_FOLDER)/, $(subst .java,.class,$(CLASSES)))
JAR=$(wildcard +libs/*.jar)

JAVA_FLAGS=--class-path

all: objFolder $(CLASSES) $(PROJECT) 

$(PROJECT): $(OBJ)
	@ echo 'Running binary using JAVA: $@'
	@ echo ' '
	java $@ -cp $(JAR):.

$(TARGET_FOLDER)/%.class: %.java
	@ echo 'Building target using JAVAC compiler: $< $(JAR)'
	javac $< -cp $(JAR):.

objFolder:
	@ mkdir -p objects

clean:
	@ rm $(TARGET_FOLDER)/*.class
