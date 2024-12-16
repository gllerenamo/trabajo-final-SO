#ifndef SHELL_H
#define SHELL_H

#include "display.h"

void process_command(uint8_t *buffer, Cursor *cursor);
void shell();

#endif