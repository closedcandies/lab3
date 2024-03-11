#include "lib/sandbox.h"
#include <cinttypes>

int main(int argc, char *argv[]) {
  struct ProgramParameters Arguments;
  ParseArguments(argv, argc, Arguments);
  FieldManager field = ReadFromTSV(Arguments.input_path);
  StartModel(Arguments);
}