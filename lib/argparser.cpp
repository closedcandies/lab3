#include "sandbox.h"

void InputError() {
  std::cerr << "Incorrect input";
  exit(EXIT_FAILURE);
}

void CheckArguments(ProgramParameters& Arguments) {
  if (strcmp(Arguments.input_path, "") == 0
      or strcmp(Arguments.output_path, "") == 0) {
    InputError();
  } else {
    std::cout << Arguments.input_path << " " << Arguments.output_path << " max iter = " <<
              Arguments.max_iterations << " max iter flag: " << Arguments.max_iterations_flag <<
              " freq = " << Arguments.freq << " freq flag: " << Arguments.freq_flag << '\n';
  }

}

void ParseArguments(char *argv[], int argc, ProgramParameters& Arguments) {
  for(int i = 1; i < argc; ++i) {
    char* st = argv[i];

    if (strcmp(st, "-i") == 0) {
      if (i + 1 >= argc)
        InputError();

      Arguments.input_path = argv[i+1];
      std::ifstream file(Arguments.input_path);
      ++i;
      if (!file.is_open())
        InputError();
    }

    else if (strncmp(st, st_input, strlen(st_input)) == 0 ) {
      char* new_st = (st+strlen(st_input));
      Arguments.input_path = new_st;
      std::ifstream file(Arguments.input_path);

      if (!file.is_open())
        InputError();
    }

    else  if (strcmp(st, "-o") == 0) {
      if (i + 1 >= argc)
        InputError();

      Arguments.output_path = argv[i+1];
      ++i;


      if (!std::filesystem::is_directory(Arguments.output_path))
        InputError();

    }

    else if (strncmp(st, st_output, strlen(st_output)) == 0) {
      char* new_st = (st+strlen(st_output));
      Arguments.output_path = new_st;

      if (!std::filesystem::is_directory(Arguments.output_path))
        InputError();
    }

    else if (strcmp(st, "-m" )== 0) {
      if (i + 1 >= argc)
        InputError();

      Arguments.max_iterations = static_cast <uint64_t> (strtoll(argv[i+1], nullptr, 10));
      Arguments.max_iterations_flag = true;
      ++i;
    }

    else if (strncmp(st, st_max_iter, strlen(st_max_iter)) == 0) {
      char* new_st = (st + strlen(st_max_iter));
      Arguments.max_iterations_flag = true;
      Arguments.max_iterations = static_cast <uint64_t> (strtoll(new_st, nullptr, 10));
    }

    else if (strcmp(st, "-f" )== 0) {
      if (i + 1 >= argc)
        InputError();
      Arguments.freq = static_cast <uint64_t> (strtoll(argv[i+1], nullptr, 10));
      Arguments.freq_flag = true;
      ++i;
    }

    else if (strncmp(st, st_freq, strlen(st_freq)) == 0) {
      char* new_st = (st + strlen(st_freq));
      Arguments.freq_flag = true;
      Arguments.freq = static_cast <uint64_t> (strtoll(new_st, nullptr, 10));
    }
  }

  CheckArguments(Arguments);
}
