#include "sandbox.h"

FieldManager ReadFromTSV(const char *path) {
  std::ifstream TSVFile(path, std::ios::binary);
  if (!TSVFile.is_open())
    InputError();

  FieldManager my_field;
  my_field.first_queue = new Queue();
  my_field.second_queue = new Queue();
  int16_t x, y;
  uint64_t number;

  while (!TSVFile.eof()) {
    TSVFile >> y >> x >> number;
    if (x > my_field.x_max) my_field.x_max = x;
    if (x < my_field.x_min) my_field.x_min = x;
    if (y > my_field.y_max) my_field.y_max = y;
    if (y < my_field.y_min) my_field.y_min = y;
  }
  TSVFile.close();

  if ((my_field.x_max - my_field.x_min + 1) % 2 == 1) {
    my_field.x_max++;
  }

  my_field.width = my_field.x_max - my_field.x_min + 1;
  my_field.height = my_field.y_max - my_field.y_min + 1;
  my_field.size = my_field.width * my_field.height;

  my_field.mas = new uint64_t[my_field.size];
  for (uint32_t i = 0; i < my_field.size; ++i) {
    my_field.mas[i] = 0;
  }

  TSVFile.open(path, std::ios::binary);
  while (!TSVFile.eof()) {
    TSVFile >> y >> x >> number;
    my_field.mas[my_field.width * (y - my_field.y_min) + (x - my_field.x_min)] = number;
  }

  return my_field;
}

void FieldManager::AddToTop() {
  uint64_t *new_mas = new uint64_t[size + width]{0};

  for (uint32_t i = 0; i < size; ++i) {
    new_mas[i] = mas[i];
  }

  delete[]  mas;
  mas = new_mas;
  size += width;
  height++;
  y_max++;
}

void FieldManager::AddToBottom() {
  uint64_t *new_mas = new uint64_t[size + width]{0};
  for (uint32_t i = width; i < size + width; ++i) {
    new_mas[i] = mas[i - width];
  }

  delete[] mas;
  mas = new_mas;
  size += width;
  height++;
  y_min--;
}

void FieldManager::AddToRight() {
  uint64_t *new_mas = new uint64_t[size + height]{0};
  uint32_t c = 0;
  for (uint32_t i = 0; i < height; ++i) {
    for (uint32_t j = 0; j < width; ++j) {
      new_mas[i * width + j + c] = mas[i * width + j];
    }
    c++;
  }

  delete[] mas;
  mas = new_mas;
  size += height;
  width++;
  x_max++;
}

void FieldManager::AddToLeft() {
  uint64_t *new_mas = new uint64_t[size + height]{0};
  for (uint32_t i = 0; i < height; ++i) {
    for (uint32_t j = 0; j < width; ++j) {
      new_mas[i * (width + 1) + j + 1] = mas[i * width + j];
    }
  }

  delete[] mas;
  mas = new_mas;
  size += height;
  width++;
  x_min--;
}

Queue::Queue(const Queue &other) {
  if(!other.IsEmpty()) {
    Node *prev = new Node(other.head->value, other.head->y, other.head->x);
    head = prev;
    Node *curr = other.head->next;
    while (curr != other.tail) {
      Node *new_curr = new Node(curr->value, curr->y, curr->x);
      prev->next = new_curr;
      prev = new_curr;
      curr = curr->next;
    }
    curr = other.tail;
    prev->next = new Node(curr->value, curr->y, curr->x);
    tail = prev->next;
  }
}

Queue::~Queue() {
  if (!this->IsEmpty()) {
    Node *curr = head, *next = head->next;
    while (curr != tail) {
      delete curr;
      curr = next;
      next = curr->next;
    }
    delete curr;
  }
}

Queue &Queue::operator=(const Queue &other) {
  if (this == &other) {
    return *this;
  }

  Node *curr;
  if (!this->IsEmpty()) {
    curr = head;
    Node *next = head->next;
    while (curr != tail) {
      delete curr;
      curr = next;
      next = curr->next;
    }
    delete curr;
  }

  if (!other.IsEmpty()) {
    Node *prev = new Node(other.head->value, other.head->y, other.head->x);
    head = prev;
    curr = other.head->next;
    while (curr != nullptr) {
      Node *new_curr = new Node(curr->value, curr->y, curr->x);
      prev->next = new_curr;
      prev = new_curr;
      curr = curr->next;
    }
    curr = other.tail;
    Node *new_tail = new Node(curr->value, curr->y, curr->x);
    prev->next = new_tail;
    tail = new_tail;
  }
  return *this;
}

void Queue::Push(uint64_t value, int16_t y, int16_t x) {
  if (head == nullptr) {
    head = new Node(value, y, x);
    tail = head;
  } else {
    Node *new_node = new Node(value, y, x);
    tail->next = new_node;
    tail = new_node;
  }
}

void Queue::Pop() {
  if (IsEmpty()) {
    std::cerr << "Empty";
    return;
  }
  Node *saved = head;
  head = head->next;
  if (head == nullptr) {
    tail = nullptr;
  }
  if (head != nullptr)
    delete saved;
}

Node *Queue::GetNode() {
  if (IsEmpty()) {
    std::cout << "empty";
    return nullptr;
  }
  return head;
}

bool Queue::IsEmpty() const {
  return head == nullptr;
}

void FieldManager::GetQueue() {
  for (uint32_t i = 0; i < size; ++i) {
    if (mas[i] >= 4) {
      first_queue->Push(mas[i], i / width + y_min, i % width + x_min);
    }
  }

}

void Queue::Print() {
  if (IsEmpty()) {
    std::cout << "empty";
    return;
  }

  Node *it = head;

  while (it != nullptr) {
    std::cout << it->y << " " << it->x << " " << it->value << "\n";
    it = it->next;
  }
}

void FieldManager::NextQueue() {
  while (!first_queue->IsEmpty()) {
    Node *cell = first_queue->GetNode();
    CheckExtension(cell);
    mas[(cell->y - y_min) * width + (cell->x - x_min)] -= 4;
    cell->value -= 4;

    mas[(cell->y - y_min) * width + (cell->x - x_min) + 1]++;
    if (mas[(cell->y - y_min) * width + (cell->x - x_min) + 1] == 4) {
      second_queue->Push(mas[(cell->y - y_min) * width + (cell->x - x_min) + 1], cell->y, cell->x + 1);
    }

    mas[(cell->y - y_min) * width + (cell->x - x_min) - 1]++;
    if (mas[(cell->y - y_min) * width + (cell->x - x_min) - 1] == 4) {
      second_queue->Push(mas[(cell->y - y_min) * width + (cell->x - x_min) - 1], cell->y, cell->x - 1);
    }

    mas[(cell->y - y_min) * width + (cell->x - x_min) + width]++;
    if (mas[(cell->y - y_min) * width + (cell->x - x_min) + width] == 4) {
      second_queue->Push(mas[(cell->y - y_min) * width + (cell->x - x_min) + width], cell->y + 1, cell->x);
    }

    mas[(cell->y - y_min) * width + (cell->x - x_min) - width]++;
    if (mas[(cell->y - y_min) * width + (cell->x - x_min) - width] == 4) {
      second_queue->Push(mas[(cell->y - y_min) * width + (cell->x - x_min) - width], cell->y - 1, cell->x);
    }

    if (mas[(cell->y - y_min) * width + (cell->x - x_min)] >= 4) {
      second_queue->Push(cell->value, cell->y, cell->x);
    }

    first_queue->Pop();
  }

  first_queue = second_queue;
  second_queue = new Queue;
}

void FieldManager::CheckExtension(Node *cell) {
  if (cell->y == y_min) {
    AddToBottom();
  }
  if (cell->y == y_max) {
    AddToTop();
  }
  if (cell->x == x_max) {
    AddToRight();
    AddToRight();
  }
  if (cell->x == x_min) {
    AddToLeft();
    AddToLeft();
  }
}

FieldManager::FieldManager(const FieldManager &other) {
  this->width = other.width;
  this->size = other.size;
  this->height = other.height;
  this->x_min = other.x_min;
  this->y_min = other.y_min;
  this->x_max = other.x_max;
  this->y_max = other.y_max;
  this->mas = new uint64_t[this->size];
  if (other.mas != nullptr) {
    for (int i = 0; i < this->size; i++) {
      this->mas[i] = other.mas[i];
    }
  }
}

FieldManager &FieldManager::operator=(const FieldManager &other) {
  if (this == &other)
    return *this;

  delete[] mas;
  size = other.size;
  height = other.height;
  width = other.width;
  x_max = other.x_max;
  x_min = other.x_min;
  y_max = other.y_max;
  y_min = other.x_min;
  mas = new uint64_t[other.size];
  if (other.mas != nullptr) {
    for (size_t i = 0; i < other.size; ++i)
      mas[i] = other.mas[i];
  }
  return *this;
}



