---
date: "2021/12/26"
title: "Part 3: Memory and pointers I: definitions"
series: "C for Python programmers"
---

# Memory and pointers I

Pointers are perhaps the most dreaded concept of the C language. It is immediately associated with hard-to-find bugs, unsafety and `segmentation fault (core dumped)`.

The key to understand pointers is to treat them for what they fundamentally are: **memory addresses**. It is based on this very bare definition that we can understand the versatility of the concept and how they are used in many different contexts. But first, we need to get a clear mental representation of how memory is represented and handled by modern computers.

The C language allows us to manipulate memory at (almost) the same level of precision that assembly code would give us. Memory is viewed as a **contiguous** space of addresses which all contain 1 byte of data.

As a Python analogy you can imagine that memory is a dictionary mapping memory addresses to bytes. Contrarily to Python dictionaries, this mapping between memory addresses and byte data happens (almost) at hardware level which gives to the C language (almost) maximal speed in its read/write interaction with memory data.

## Table of contents

<details markdown="1">
<summary>Show table of contents</summary>
[TOC]
</details>

## Memory addresses

As a first approximation, any piece of data processed by a C program lives at a given address in Random Access Memory (RAM): it is the core principle of Random Access Memory that you can access data at any "random" address.

For instance, the program:

```C
unsigned char my_byte = 3;
printf("%p", &my_byte);
```

Will output something such as `0x00007ffe45a73aa4` which is a 64-bit memory address expressed in hexadecimal (two hex digits represent 1 byte, sixteen hex digits represent 8 bytes = 64 bits). 64-bit computers manipulate 64-bit memory addresses.

The unary operator `&` allows to access the memory address of any variable. Here, `my_byte` is 3 and `&my_byte` is `0x00007ffe45a73aa4`: the value 3 lives at address `0x00007ffe45a73aa4`.

A memory address is always the address of a **byte** (8 bits, `unsigned char`). If we ask the memory address of an `int` variable (usually 4 bytes):

```C
int my_int = 1024;
printf("%p", &my_int);
```

We get the address of the **first byte** of a **contiguous** memory chunk of the 4 bytes that constitutes `my_int`. Indeed, `&my_int`, `&my_int+1`, `&my_int+2`, `&my_int+3` will be the memory addresses of the four bytes which constitute `my_int`.

This linear model of contiguous memory addresses, that we can manipulate with arithmetical operations (`&my_int+1`, `&my_int+2` etc...), is a fundamental characteristic of modern computer architectures.

## Physical vs Virtual memory

Above, we said that RAM memory addresses are written on 64 bits. In theory that allows for \\(2^{64} \simeq 1.8 \times 10^{19} \simeq 10^{10} \times 10^9\\) different addresses which is far beyond the \\(8 \times 10^9\\) addresses that the usual 8 Gb of RAM of our computers offers.

That's where the OS (mac, Linux, Windows) comes into play: the OS allows each program to access a range of _virtual_ 64-bit addresses which it then internally translates to _physical_ RAM addresses.

This layer of abstraction provided by the OS is key to the contiguous model of memory that we described above: in the _virtual address space_ (which is the one that we see in our programs), the program's data is guaranteed to be stored contiguously while actually, in the _physical address space_, the data is stored wherever there is room left. It is the OS that is responsible for mapping _virtual_ addresses to _physical_ ones.

If we were writing our programs in assembly code instead of C we would also be manipulating virtual addresses. Only the OS's kernel manipulates physical addresses[^0].

[^0]: [On that subject](https://stackoverflow.com/questions/41429622/are-addresses-in-x86-assembly-virtual-or-physical).


## Dereference operator

Accessing the value stored at a given address is called **dereferencing**. The dereference operator in C is unary[^1] `*`:

[^1]: _Unary_ operator as opposed to the _binary_ operator `*` which is multiplication.

```C
int val = 34;
printf("%d\n", *(&val));
```

Will output `34` as we access the value stored at the address `&val`.

## Pointer types

What if we want to store the address of variable `a` in an other variable `b`? What should be the type of variable `b`? In other words: what is the type of `&a`?

Theoretically, since memory addresses are stored on 8 bytes (on 64-bit computers), the type of `&a` should for instance be `uint64_t` (defined in `<stdint.h>`) as it is a type guaranteed to be encoded on 64 bits.

However, solely having the memory address of `a` is not very useful, because when we **dereference** that address (i.e. access the data stored at the address) the program needs to know the type of `a` in order to reconstruct the information. For instance, if `a` is an `int`, the program will know that the bytes constituting `a` are stored between `&a` and `&a+sizeof(int)` excluded[^2] and make sense of the data accordingly.

[^2]: Generally `sizeof(int) = 4`, i.e `int` are stored on 4 bytes.

This is why in C, pointer types relate to the type of the object being referenced:

```C
int val = 389;
int* pointer_on_val = &val;
printf("%p %d", pointer_on_val, *pointer_on_val);
```

The type of `pointer_on_val` is `int*`[^3] because the type of `val` is `int`. That way we convey two pieces of information:

[^3]: When defining pointers it is very common to see `int *pointer_on_val` instead of `int* pointer_on_val` (it is even imposed by some styling conventions). I personally find `int* pointer_on_val` much clearer. This only makes a difference [when trying to define several pointers in the same declaration](https://stackoverflow.com/questions/558474/what-makes-more-sense-char-string-or-char-string) which you should avoid.

1. `pointer_on_val` is a memory address.
2. When dereferencing the address `pointer_on_val` by doing `*pointer_on_val` we are left with an `int`.

The program will output the memory address of `val` such as `0x7fffc27fad0c` and the `int` value stored at `0x7fffc27fad0c`, which is the value of `val`: 389.

## What is the use of pointers?

Given the fundamental nature of pointers asking the question "what is the use of pointers?" is potentially as broad as asking "what is the use of oxygen?". Pointers can be used in many different ways and contexts: they are versatile.

We can however describe two cases where pointers are very useful:

1. Passing an object to a function through its address
2. Dynamic memory allocation

### Passing an object to a function: read

Pointers allow us to do something very natural. When passing data to a function, instead of
passing a copy of that data we can rather give the address of the data to the function and let the function dereference it.

```C
#include <stdio.h>
#include <stdbool.h>

typedef struct {
  int health;
  int strength;
  int agility;
  int intelligence;
  // ...
} Character;

bool is_character_dead(Character* character) {
  // Same as:
  // return (*character).health <= 0;
  return character->health <= 0;
}

int main() {
    Character hero = {.health = 30, .strength = 10,
                      .agility = 2, .intelligence = 20};

    printf("%ld\n", sizeof(hero));

    if (is_character_dead(&hero)) printf("Dead :(\n");
    else printf("Alive!\n");
}
```

Here, imagine that you encode all the attributes of a RPG character in a struct `Character` similarly to what we saw in [part 2](part 2) of this series. On my computer, the data of `Character` costs me 16 bytes[^4] and we can imagine that, with a lot more characteristics, the description of a character could be way bigger.

[^4]: `sizeof(hero)` must be at least `4*sizeof(int)` since we use 4 `int` but it can be more because of [struct packing](https://stackoverflow.com/questions/4306186/structure-padding-and-packing).

Hence, instead of copying that data each time we call a function that works with `Character` (a _method_ in Pythonic terms) we can simply give a pointer to it and access the character's data by dereferencing.

Avoiding copy when working with big objects leads to better performances.

### Passing an object to a function: write

In the above example we only have been reading data but we can also write data using pointers:

```C
void fight(Character* characterA, Character* characterB) {
  if (characterA->strength > characterB->strength) {
    characterB->health = 0; // Kill B if less strong
  } else if (characterA->strength < characterB->strength) {
    characterA->health = 0; // Kill A if less strong
  }
  // do nothing if same strength
}
```

This principle can also be used to return several values in a function:

```C
int sum_and_diff(int a, int b, int* ret_diff) {
  *ret_diff = a-b;
  return a + b;
}

void main() {
  int a = 2;
  int b = 3;
  int diff = 0;

  int sum = sum_and_product(a,b,&diff);

  printf("%d %d\n", sum, diff);
}
```

### Read-only protection

Note that if a function should not write but only read the dereferenced value of a pointer you can use the keyword `const` as a protection:

```
bool is_character_dead(const Character* character) {

  // ERROR: will not compile because of const
  character->health = -9;

  return character->health <= 0;
}
```

### Dynamic memory allocation

This is the topic of [part 4](part4) :)

## The danger of pointers

Pointers are memory addresses together with some type information. Hence, the danger of pointers is the same as the danger of sending letters by the Post: an address can be or become invalid.

### Invalid addresses

Here is a very simple example of an invalid address:

```C
int* p = NULL;
printf("%d", *p);
```

The value `NULL` is used as a default/initialization value for pointers to mean **I am not pointing to anything**. The program will crash with the infamous `segmentation fault (core dumped)` as we are trying to access data at an invalid address.

`NULL` pointers are often used for error management in C. For instance if we try to open a file that does not exists:

```C
#include <stdio.h>

int main() {
  FILE* file_ptr = fopen("inexistant_file.txt", "r");

  if (file_ptr == NULL) {
    printf("Couldn't open the file!");
    return -1; // return error code
  }

  fclose(file_ptr);
  return 0;
}
```

### Dangling pointer

The example above was quite simple in the sense the address stored by the pointer was invalid but easy to recognize and to test for.

There are cases, much trickier, where an address is at some point valid but then becomes invalid:

```C
int* f() {
  int val = 3;
  return &val;
}

void main() {
  int* pointer_to_val = f();
  printf("%d\n", *pointer_to_val);
}
```

This program crashes with `segmentation fault (core dumped)`. The reason is because the variable `val` only lives **within the scope of function f**. Hence, as soon as the function returns, the address `&val`, becomes invalid[^5]. In the main function we are dereferencing an invalid address, which crashes the program.

[^5]: We'll see why in more details in [part 4](part4).

`pointer_to_val` is called a **dangling pointer**: it is a memory address that is no longer valid, it is pointing to a now _forbidden_ (_deallocated_) chunk of memory.

## Conclusion

In modern computer architectures, memory is seen as a contiguous space of addressable bytes. Nowadays memory addresses are represented on 64 bits.

This model of memory is enforced by the OS which, under the hood, translates contiguous _virtual memory addresses_ into, potentially non-contiguous, _physical memory addresses_ in RAM.

C pointers are memory addresses together with the type of the object that is pointed at. Communicating the address of an object instead of the object itself to a function allows to avoid inefficient copies of the data. By **dereferencing** pointers we can either read or write the data they are pointing at.

The concept of pointers inherently comes with the risk of manipulating invalid addresses or worse, manipulating addresses that _become_ invalid through time. This issue is a source of bugs that can be arbitrarily hard to discover or fix.

So far, it is not evident that the concept of pointers is absolutely needed. They represent a convenient feature but we could question whether they are truly necessary: are there things that we really cannot do without pointers?

The answer is **yes**. We crucially need pointers as soon as we want to work with big chunks of memory (> 8Mb) or that we want to work with data whose size is only known at runtime and potentially unbounded. Imagine a software such as Photoshop for working with images: you cannot know in advance (at compile time) the size of the images that your users are going to load in the software and you will need to **dynamically allocate** the required memory on the fly, at runtime.

[Part 4](part4) of this series is about Dynamic Memory Allocation and will present two concepts that we have carefully avoided so far: the Stack and the Heap.

## Exercises

### A. Matrices

Consider the code:

```C
#include <stdio.h>
#include <stdint.h>

typedef struct {
  uint8_t h, w;
  int m[100][100];
} Matrix;

void main() {
  Matrix A;
  A.h = 10;
  A.w = 7;

  for (int i = 0; i < A.h; i += 1)
    for (int j = 0; j < A.w; j += 1)
      A.m[i][j] = i * j;
}
```

1. Write a function `void print_matrix(const Matrix* A)` which prints a matrix;

2. Write a function `Matrix multiply_scalar(const Matrix* A, int scalar)` which returns the matrix \\(s\times A\\) with \\(s\\) an `int` scalar. Why is `const` used?

3. Write a function `void multiply_scalar_inplace(Matrix* A, int scalar)` which computes scalar multiplication in place (it modifies `A` directly).

4. Same as 2. and 3. but compute matrix transposition.

5. What happens if you change your code with `int m[10000][10000];` instead of `int m[100][100];`? Does it still work? Why? Answer to this question in [Part 4](part4) :)

<details>
  <summary>Partial solution</summary>

```C
#include <stdint.h>
#include <stdio.h>

typedef struct {
  uint8_t h, w;
  int m[100][100];
} Matrix;

void print_matrix(const Matrix* A) {
  for (int i = 0; i < A->h; i += 1) {
    for (int j = 0; j < A->w; j += 1)
      printf("%d ", A->m[i][j]);
    printf("\n");
  }
}

Matrix multiply_scalar(const Matrix* A, int scalar) {
  Matrix to_return;
  to_return.h = A->h;
  to_return.w = A->w;

  for (int i = 0; i < A->h; i += 1)
    for (int j = 0; j < A->w; j += 1)
      to_return.m[i][j] = A->m[i][j] * scalar;

  return to_return;
}

void multiply_scalar_inplace(Matrix* A, int scalar) {
  for (int i = 0; i < A->h; i += 1)
    for (int j = 0; j < A->w; j += 1)
      A->m[i][j] *= scalar;
}

void main() {
  Matrix A;
  A.h = 10;
  A.w = 7;

  for (int i = 0; i < A.h; i += 1)
    for (int j = 0; j < A.w; j += 1)
      A.m[i][j] = i * j;

  print_matrix(&A);
  printf("\n");

  Matrix A_times_two = multiply_scalar(&A, 2);
  print_matrix(&A_times_two);
  printf("\n");

  multiply_scalar_inplace(&A, 3);
  print_matrix(&A);
}
```

</details>

### B. Read the bytes of an int

1. Write a function which outputs all the bytes of an `int` in hex. _(hint: cast `int*`to`unsigned char *`)_.

2. Test your function with values such as 12, 256 and `INT_MAX` (defined in `<limits.h>`). What you will see will depend on the [endianness](https://www.geeksforgeeks.org/little-and-big-endian-mystery/) of your system.

3. Why, assuming 4 bytes `int`, the doesn't output `ff ff ff ff` for `INT_MAX`?

<details markdown = "1">
  <summary>Solution</summary>

1. and 2.

```C
#include <limits.h>
#include <stdio.h>

void print_bytes_of_int(int x) {
  unsigned char* b = (unsigned char*)(&x);
  for (int i = 0; i < sizeof(x); i += 1) {
    printf("%x ", *(b + i));
  }
  printf("\n");
}

void main() {
  print_bytes_of_int(12);
  print_bytes_of_int(256);
  print_bytes_of_int(INT_MAX);  // <limits.h>
}
```

3. On a _little endian_ (i.e. less significant byte first) with 4 bytes `int` the hex representation of the bytes of `INT_MAX` outputted by the program is `ff ff ff 7f` (it would be `7f ff ff ff` on a _big endian_ system). Hence the corresponding total hex representation of `INT_MAX` is `0x7fffffff`. It is not `0xffffffff` because `INT_MAX` is signed and half of the 4-byte space is used to represent negative numbers (which start with an initial bit equal to 1). `0xffffffff` is the representation of `UINT_MAX` which is the biggest `unsigned int`.

</details>
