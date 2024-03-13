#ifndef LABWORK3_CLOSEDCANDIES_SANDBOX_H
#define LABWORK3_CLOSEDCANDIES_SANDBOX_H

#include <cinttypes>
#include <cstring>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <limits>
#include <string>
///---------------------------FIELD CONTROL---------------------------------

struct Node {
    uint64_t value;
    int16_t x;
    int16_t y;
    Node* next;

    Node(uint64_t _val, int16_t _y, int16_t _x) {
        value = _val;
        x = _x;
        y = _y;
        next = nullptr;
    }
};

struct Queue {
    Node* head;
    Node* tail = nullptr;

    Queue() {
        head = nullptr;
        tail = nullptr;
    }

    Queue(const Queue& other);
    ~Queue();
    Queue& operator=(const Queue& other);

    void Push(uint64_t value, int16_t y , int16_t x);

    void Pop();

    Node* GetNode();

    bool IsEmpty() const;

    void Print();


};


struct FieldManager {
    uint64_t *mas;
    uint16_t height;
    uint16_t width;
    uint32_t size;
    int16_t x_max = INT16_MIN;
    int16_t x_min = INT16_MAX;
    int16_t y_max = INT16_MIN;
    int16_t y_min = INT16_MAX;
    Queue* first_queue;
    Queue* second_queue;

    void AddToBottom();

    void AddToTop();

    void AddToRight();

    void AddToLeft();

    void GetQueue();

    void NextQueue();

    void CheckExtension(Node *cell);

    ~FieldManager() {
        std::cout << this << std::endl;
        delete[] mas;
    }

    FieldManager() = default;

    FieldManager(const FieldManager& other);

    FieldManager& operator=(const FieldManager& other);
};

FieldManager ReadFromTSV(const char* path);

///------------------------------DRAW CONTROL-------------------------------------

#pragma pack(push, 1)
struct BITMAPFILEHEADER {
    uint16_t bfType = 0x4D12;
    uint32_t bfSize = 0;
    uint16_t bfReserved1 = 0;
    uint16_t bfReserved2 = 0;
    uint32_t bfOffBits = 0;
};
#pragma pack(pop)

struct BITMAPINFOHEADER {
    uint32_t bSize = 0;
    int32_t bWidth = 0;
    int32_t bHeight = 0;
    uint16_t bPlanes = 0;
    uint16_t bBitCount = 0;
    uint32_t bCompression = 0;
    uint32_t bSizeImage = 0;
    int32_t bXPelsPerMeter = 0;
    int32_t bYPelsPerMeter = 0;
    uint32_t bClrUsed = 0;
    uint32_t bClrImportant = 0;
} ;

struct RGBQUAD {
    uint8_t rgbBlue = 0;
    uint8_t rgbGreen = 0;
    uint8_t rgbRed = 0;
    uint8_t rgbReserved = 0;
};

void CreateBMP(const std::string& output_path, FieldManager& my_vector, uint64_t** mas);

///----------------------------ARGUMENT PARSER--------------------------------------

struct ProgramParameters {
    char* input_path;
    char* output_path = "";
    uint64_t max_iterations = std::numeric_limits<uint64_t>::max();
    bool max_iterations_flag = false;
    uint64_t freq = 1;
    bool freq_flag = false;
};

static const char* st_input = "--input=";
static const char* st_output = "--output=";
static const char* st_freq = "--freq=";
static const char* st_max_iter = "--max=";

void InputError();

void ParseArguments(char *argv[], int argc, ProgramParameters& Arguments);

///--------------------------OTHER FUNCTIONS----------------------------------------

std::string GetFileName(int iter);
uint64_t** ArrayConvert(uint64_t* mas, uint16_t width, uint16_t height);
char* ToString(int number);
void StartModel(ProgramParameters &Arguments);
int GetColore(int value);


#endif //LABWORK3_CLOSEDCANDIES_SANDBOX_H
