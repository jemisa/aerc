#ifndef _UI_H
#define _UI_H
#include <stdbool.h>

void init_ui();
void teardown_ui();
void rerender();
bool ui_tick();

#endif