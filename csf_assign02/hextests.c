// Unit tests for hex functions
// These tests should work for both your C implementations and your
// assembly language implementations

#include <stdio.h>
#include <stdlib.h>
#include "tctest.h"
#include "hexfuncs.h"

// test fixture object
typedef struct {
  char test_data_1[16];
  //added in our test data
  char test_data_2[16];
} TestObjs;

// setup function (to create the test fixture)
TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));
  strcpy(objs->test_data_1, "Hello, world!\n");
  strcpy(objs->test_data_2, "It's Party Time");
  return objs;
}

// cleanup function (to destroy the test fixture)
void cleanup(TestObjs *objs) {
  free(objs);
}

// Prototypes for test functions
void testFormatOffset(TestObjs *objs);
void testFormatByteAsHex(TestObjs *objs);
void testHexToPrintable(TestObjs *objs);

int main(int argc, char **argv) {
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(testFormatOffset);
  TEST(testFormatByteAsHex);
  TEST(testHexToPrintable);

  TEST_FINI();

  return 0;
}

void testFormatOffset(TestObjs *objs) {
  (void) objs; // suppress warning about unused parameter
  char buf[16];
  
  hex_format_offset(1L, buf);
  ASSERT(0 == strcmp(buf, "00000001"));
  
  //Our Tests
  hex_format_offset(3L, buf);
  ASSERT(0 == strcmp(buf, "00000003"));

  hex_format_offset(30L, buf);
  ASSERT(0 == strcmp(buf, "0000001e"));

  hex_format_offset(400L, buf);
  ASSERT(0 == strcmp(buf, "00000190"));

  hex_format_offset(77L, buf);
  ASSERT(0 == strcmp(buf, "0000004d"));

  hex_format_offset(9213L, buf);
  ASSERT(0 == strcmp(buf, "000023fd"));

  hex_format_offset(679213L, buf);
  ASSERT(0 == strcmp(buf, "000a5d2d"));

  hex_format_offset(3938789L, buf);
  ASSERT(0 == strcmp(buf, "003c19e5"));

  hex_format_offset(78679562L, buf);
  ASSERT(0 == strcmp(buf, "04b08e0a"));

  hex_format_offset(986571987L, buf);
  ASSERT(0 == strcmp(buf, "3acde4d3"));
}

void testFormatByteAsHex(TestObjs *objs) {
  char buf[8];
  hex_format_byte_as_hex(objs->test_data_1[0], buf);
  ASSERT(0 == strcmp(buf, "48"));

  //Our tests

  hex_format_byte_as_hex(objs->test_data_1[3], buf);
  ASSERT(0 == strcmp(buf, "6c"));

  hex_format_byte_as_hex(objs->test_data_1[8], buf);
  ASSERT(0 == strcmp(buf, "6f"));

  hex_format_byte_as_hex(objs->test_data_1[12], buf);
  ASSERT(0 == strcmp(buf, "21"));

  hex_format_byte_as_hex(objs->test_data_1[2], buf);
  ASSERT(0 == strcmp(buf, "6c"));

  hex_format_byte_as_hex(objs->test_data_1[7], buf);
  ASSERT(0 == strcmp(buf, "77"));
  
  hex_format_byte_as_hex(objs->test_data_2[0], buf);
  ASSERT(0 == strcmp(buf, "49"));

  hex_format_byte_as_hex(objs->test_data_2[2], buf);
  ASSERT(0 == strcmp(buf, "27"));

  hex_format_byte_as_hex(objs->test_data_2[3], buf);
  ASSERT(0 == strcmp(buf, "73"));

  hex_format_byte_as_hex(objs->test_data_2[11], buf);
  ASSERT(0 == strcmp(buf, "54"));

  hex_format_byte_as_hex(objs->test_data_2[13], buf);
  ASSERT(0 == strcmp(buf, "6d"));
}

void testHexToPrintable(TestObjs *objs) {
  ASSERT('H' == hex_to_printable(objs->test_data_1[0]));
  ASSERT('.' == hex_to_printable(objs->test_data_1[13]));

  //our tests
  ASSERT('e' == hex_to_printable(objs->test_data_1[1]));
  ASSERT('d' == hex_to_printable(objs->test_data_1[11]));

  ASSERT('.' == hex_to_printable(objs->test_data_1[13]));

  ASSERT('s' == hex_to_printable(objs->test_data_2[3]));
  ASSERT('m' == hex_to_printable(objs->test_data_2[13]));

  ASSERT('t' == hex_to_printable(objs->test_data_2[1]));
  ASSERT('T' == hex_to_printable(objs->test_data_2[11]));
  
  ASSERT('I' == hex_to_printable(objs->test_data_2[0]));
  ASSERT('i' == hex_to_printable(objs->test_data_2[12]));
}
