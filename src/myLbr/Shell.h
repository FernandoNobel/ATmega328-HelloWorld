#ifndef SHELL_H_
#define SHELL_H_

#include <inttypes.h>
#include <string.h>

class Shell {
private:
  const static uint8_t maxNumArgs = 10;
  const static uint8_t maxNumFunctions = 10;
  const static uint8_t maxSizeMsg = 128;

  char    in_msg [maxSizeMsg];      // a String to hold incoming message
  uint8_t in_msgIndex;              // where do we added the nex char to de input
  bool    in_msgComplete;           // whether the string is complete

  char    out_msg [maxSizeMsg];     // a String to hold output message
  uint8_t out_msgIndex;             // where do we added the nex char to de output
  bool    out_msgComplete;          // whether the string is complet

  char    *args[maxNumArgs];

  bool    isProcessing;             // Is the shell processing the commad

public:
  Shell();
  void    init();                   // init the shell
  uint8_t read(char c);             // read a single char
  uint8_t write(char c);            // write a single char
  uint8_t write(const char *buff);  // write a char array
  uint8_t write(uint8_t n);         // write a number between 0-9
  uint8_t write(uint32_t n);        // write a number as a char array
  void    check_msg();              // check if in_msg is completed and set to process it
  void    interpret_msg();          // check if message is ready and interpret it
  void    split_line();
  void    execute();
  uint8_t request(char *buff[maxSizeMsg]);
  void    reset_in_msg();           // reset the in_msg to zero
  void    reset_out_msg();          // reset the in_msg to zer
  bool    getIsProcessing() { return isProcessing; };     // return isProcessing

private:
  void test(char **args);
  void echo(char **args);
  void readHz(char **args);

  char const *builtin_str[maxNumFunctions] = {
    "test",
    "echo",
    "readHz"
  };
  void (Shell::*builtin_func[maxNumFunctions])(char **) = {
    &Shell::test,
    &Shell::echo,
    &Shell::readHz
  };

};

#endif /* SHELL_H_ */
