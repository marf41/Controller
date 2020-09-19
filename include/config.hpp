#pragma once

#ifdef DEBUG
#define log Serial.printf
#define done() Serial.println("DONE")
#else
#define log
#define done()
#endif