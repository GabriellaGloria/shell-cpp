#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>

void enableRawMode();
void disableRawMode();
void readInputWithTabSupport(std::string &input);
void handleDoubleTab(const std::string &input);
bool handleTabPress(std::string &input);

#endif // TERMINAL_H
