#include "Shell.h"
#include <Arduino.h>

Shell::Shell() {

}

void Shell::init() {
  reset_in_msg();
  reset_out_msg();
}

uint8_t Shell::read(char c) {
  // Check the message lenght and return 1 if it is overcome
  if (in_msgIndex >= maxSizeMsg) {
    reset_in_msg();

    return 1;
  }

  if (c == '\0') {
    in_msgComplete = true;
    in_msg[in_msgIndex] = c;

    return 0;
  }

  in_msg[in_msgIndex] = c;
  in_msgIndex++;

  return 0;
}

uint8_t Shell::write(char c) {
  // Check the message lenght and return 1 if it is overcome
  if (out_msgIndex >= maxSizeMsg || out_msgComplete == true) {
    //reset_outn_msg();

    return 1;
  }

  if (c == '\0') {
    out_msgComplete = true;
    out_msg[out_msgIndex] = c;

    return 0;
  }

  out_msg[out_msgIndex] = c;
  out_msgIndex++;

  return 0;
}

uint8_t Shell::write(const char *buff) {

  int i = 0, error = 0;

  while (1) {
    if (buff[i] == '\0') break;
    error = write(buff[i]);
    i++;
  }

  return error;
}

void Shell::interpret_msg() {
  if (in_msgComplete) {

    Serial.print("\nMsg readed:");
    Serial.print(in_msg);

    split_line();
    execute();

    reset_in_msg();
  }
}

void Shell::split_line() {

  char *dup = strdup(in_msg);

  //Serial.println(dup);

  int i = 0;
  char *p = strtok (dup, " ");

  while (p != NULL && i < maxNumArgs) {
    args[i++] = p;
    p = strtok (NULL, " ");
  }

  while (i < maxNumArgs) {
    args[i++] = NULL;
  }

  /*for (int j = 0; j < maxNumArgs; j++) {
    Serial.print('\n');
    Serial.print("Args[");
    Serial.print(j);
    Serial.print("]:");
    Serial.print(args[j]);
  }*/
}

void Shell::execute() {
  reset_out_msg();

  for (int i = 0; i < maxNumFunctions; i++) {
    if (builtin_str[i] != NULL) {
      if (strcmp(args[0], builtin_str[i]) == 0) {
        (this->*builtin_func[i])(args);
        return;
      }
    }
  }
}

uint8_t Shell::request(char *buff[maxSizeMsg]) {
  *buff = out_msg;

  if (out_msgComplete == false) return 1;

  return 0;
}

void Shell::reset_in_msg() {
  in_msgComplete = false;
  in_msgIndex = 0;
  memset(in_msg, 0, maxSizeMsg);
}

void Shell::reset_out_msg() {
  out_msgComplete = false;
  out_msgIndex = 0;
  memset(out_msg, 0, maxSizeMsg);
}

void Shell::test(char **args) {
  Serial.println("\nTest!");
  write("Test!");
  write('\0');
}

void Shell::echo(char **args) {
  Serial.println("\nEcho!");

  if (args[1] == NULL) {
    write("ERROR: No argument passed.");
    write('\0');
    return;
  }

  write(args[1]);

  write('\0');
}
