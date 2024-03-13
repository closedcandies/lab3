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

std::string GetFileName(int iter) {
  return std::to_string(iter);
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
  std::cout << &my_field << std::endl;
  my_field.GetQueue();
  std::cout << my_field.height << " " << my_field.width << " " << my_field.size << " "<< my_field.y_max << " " << my_field.y_min << " " << my_field.x_max << " " << my_field.x_min << std::endl;

  uint64_t counter_iter = 0;
  uint64_t bmp_counter = 1;
  uint64_t** my_mas;
  if(Arguments.max_iterations == 0 or Arguments.freq == 0) {
    while(!my_field.first_queue->IsEmpty()) {
      my_field.NextQueue();
    }
    my_mas = ArrayConvert(my_field.mas, my_field.width, my_field.height);
    CreateBMP(std::string(Arguments.output_path) + "1.bmp", my_field, my_mas);
    for(uint16_t i = 0; i < my_field.height; ++i) {
      delete[] my_mas[i];
    }
    delete[] my_mas;
  } else {
    do {
      my_field.NextQueue();
      if(counter_iter % Arguments.freq == 0) {
        std::string path{Arguments.output_path};

        std::string filename = GetFileName(counter_iter/Arguments.freq);
        path += filename;
        std::cout << path << std::endl;
        my_mas = ArrayConvert(my_field.mas, my_field.width, my_field.height);
        CreateBMP(path + ".bmp", my_field, my_mas);
        for(uint16_t i = 0; i < my_field.height; ++i) {
          delete[] my_mas[i];
        }
        delete[] my_mas;
      }
      counter_iter++;
      bmp_counter++;
      std::cout << counter_iter << std::endl;
    } while(!my_field.first_queue->IsEmpty() and counter_iter < Arguments.max_iterations);
    if (counter_iter >= Arguments.max_iterations) {
      while(!my_field.first_queue->IsEmpty()) {
        my_field.NextQueue();
      }
      counter_iter++;
      std::string temp{Arguments.output_path};
      std::string filename = GetFileName(counter_iter/Arguments.freq);
      temp += filename;
      std::cout << temp << std::endl;
      my_mas = ArrayConvert(my_field.mas, my_field.width, my_field.height);
      CreateBMP(temp, my_field, my_mas);
     for(uint16_t i = 0; i < my_field.height; ++i) {
       delete[] my_mas[i];
     }
      delete[] my_mas;
    }
  }

}