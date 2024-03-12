#include "sandbox.h"

char* ToString(int number) {
    if (number == 0){
        char* result = new char[1];
        result[0] = '0';
        return result;
    }
  int number1 = number;
  int8_t pow = 0;

  while(number1 > 0) {
    pow += 1;
    number1 /= 10;
  }
  char* result = new char[pow];
  pow -= 1;
  number1 = number;
  while(pow >= 0) {
    result[pow] = char((number1 % 10) + int('0'));
    --pow;
    number1 /= 10;
  }

  return result;
}

char* GetFileName(int iter) {
  char* number = ToString(iter);
  size_t len = 0;
  while (number[len] != '\0'){
      len++;
  }
  size_t size = len + strlen(".bmp");
  char* result = new char[size];
  size_t i = 0;

  for(i; i < len; ++i){
    result[i] = number[i];
  }
  for(i; i < size; ++i) {
    result[i] = ".bmp"[i-len];
  }
  delete number;
  return result;
}

uint64_t** ArrayConvert(uint64_t* mas, uint16_t width, uint16_t height) {
  auto new_mas = new uint64_t*[height];
  for(uint16_t i = 0;i <height; ++i) {
    new_mas[i] = new uint64_t[width];
  }
  uint32_t k = 0;
  for(uint16_t i = 0; i < height; ++i) {
    for(uint16_t j = 0; j < width; ++j) {
      new_mas[i][j] = mas[k];
      ++k;
    }
  }
  return new_mas;
}

void StartModel(ProgramParameters &Arguments) {
  FieldManager my_field = ReadFromTSV(Arguments.input_path);
  Queue queue = my_field.GetQueue();

  uint64_t counter_iter = 0;
  uint64_t bmp_counter = 1;

  if(Arguments.max_iterations == 0 or Arguments.freq == 0) {
    while(!queue.IsEmpty()) {
      queue = my_field.NextQueue(queue);
    }
    CreateBMP(strcat(Arguments.output_path, "1.bmp"), my_field, ArrayConvert(my_field.mas,my_field.width, my_field.height));
  } else {
    while(!queue.IsEmpty() and counter_iter < Arguments.max_iterations) {
      queue = my_field.NextQueue(queue);
      if(counter_iter % Arguments.freq == 0) {
        char* temp = new char[200];
        strcpy(temp,Arguments.output_path);
        char* filename = GetFileName(counter_iter/Arguments.freq);
        strcat(temp, filename);
        CreateBMP(temp, my_field, ArrayConvert(my_field.mas, my_field.width,my_field.height));
        delete[] filename;
        delete[] temp;
        filename = nullptr, temp = nullptr;
      }
      counter_iter++;
      bmp_counter++;
    }
    if(counter_iter >= Arguments.max_iterations) {
      while(!queue.IsEmpty()) {
        queue = my_field.NextQueue(queue);
      }
      counter_iter++;
      char* temp = new char[200];
      strcpy(temp,Arguments.output_path);
      char* filename = GetFileName(counter_iter/Arguments.freq);
      strcat(temp, filename);
      CreateBMP(temp, my_field, ArrayConvert(my_field.mas, my_field.width,my_field.height));
      delete[] temp;
      delete[] filename;
    }

  }

}