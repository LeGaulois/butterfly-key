#ifndef TOOLS_H
#define TOOLS_H


void erase_string(char **str);
int erase_file(char *filename);
int read_ecc_key_file(bdevice *bd, int type, int number);
unsigned char* uint16_t_to_char(uint16_t u16);

#endif
