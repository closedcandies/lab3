
#include "sandbox.h"

int GetColore(int value){
    if (value > 3){
        return 4;   // TODO: make not vzhux
    }
    else{
        return value;
    }
}


void CreateBMP(const std::string& output_path, FieldManager& my_field, uint64_t** mas) {
  BITMAPFILEHEADER bmp_file_header;
  BITMAPINFOHEADER bmp_info_header;

  std::ofstream output_file(output_path, std::ios::out|std::ios::binary);
  if (!output_file.is_open())
    InputError();

  RGBQUAD Palette[16];
  //white
  Palette[0].rgbRed = 255;
  Palette[0].rgbGreen = 255;
  Palette[0].rgbBlue = 255;
  //green
  Palette[1].rgbRed = 0;
  Palette[1].rgbGreen = 255;
  Palette[1].rgbBlue = 0;
  //yellow
  Palette[2].rgbRed = 255;
  Palette[2].rgbGreen = 255;
  Palette[2].rgbBlue = 0;
  //violet
  Palette[3].rgbRed = 139;
  Palette[3].rgbGreen = 0;
  Palette[3].rgbBlue = 255;
  //black
  for(int i = 4; i < 16; i++) {
    Palette[i].rgbRed = 0;
    Palette[i].rgbGreen = 0;
    Palette[i].rgbBlue = 0;
  }

  bmp_file_header.bfType = 0x4D42;
  bmp_file_header.bfOffBits = sizeof(bmp_file_header) + sizeof(bmp_info_header) + sizeof(Palette);
  bmp_file_header.bfSize = bmp_file_header.bfOffBits +
      my_field.width * my_field.height/2 +
      my_field.height * ((4 - (my_field.width / 2) % 4) % 4);
  bmp_info_header.bSize = sizeof(bmp_info_header);
  bmp_info_header.bBitCount = 4;
  bmp_info_header.bHeight = my_field.height;
  bmp_info_header.bWidth = my_field.width;
  bmp_info_header.bPlanes = 1;

  output_file.write(reinterpret_cast <char*>(&bmp_file_header), sizeof(bmp_file_header));
  output_file.write(reinterpret_cast <char*>(&bmp_info_header), sizeof(bmp_info_header));
  output_file.write(reinterpret_cast <char*> (&Palette), sizeof(Palette));

  uint8_t byte_for_padding = 0;
  for (int i = my_field.y_min; i <= my_field.y_max; ++i) {

    for (int j = my_field.x_min; j <= my_field.x_max; j+=2) {
      uint8_t byte_for_2_pix = (GetColore(mas[i-my_field.y_min][j-my_field.x_min]) << 4 | GetColore(mas[i - my_field.y_min][j+1 - my_field.x_min]));
      output_file.write(reinterpret_cast <char*> (&byte_for_2_pix), sizeof(byte_for_2_pix));
    }
    for (int a = 0; a < ( (4-(my_field.width/2)%4) %4); ++a) {
      output_file.write(reinterpret_cast <char*> (&byte_for_padding), sizeof(byte_for_padding));
    }
  }

  output_file.close();
}
