#include "InvokeCommands.h"

#define CONTROL(c) ((c) ^ 0x40)

typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;

/* Supported actions */
enum action {
	SEL_BACK = 1,
	SEL_NEXT,
	SEL_PREV,
	SEL_HOME,
	SEL_END,
	SEL_CDHOME,
	SEL_SEARCH,
	SEL_CDQUIT,
	SEL_QUIT,
};

#define ESC          27
#define INSERT       50
#define DELETE       51
#define PGUP         53
#define PGDN         54
#define ARROWRIGHT   67
#define ARROWLEFT    68
#define END          70
#define HOME         72
#define OTHER        79
#define BRACKETLEFT  91
#define TILDE       126
#define BACKSPACE   127
#define ARROWUP     65
#define ARROWDOWN   66
#define COLUMNWIDTH 80
#define TRUE 1
#define FALSE 0

#define SIZE         30
#define STARTTINGROW 5
#define STARTTINGROWDATA 7
#define MAXIMUMROWNUMBER 32


/* Associate a pressed key to an action */
struct key {
	int sym;         /* Key pressed */
	enum action act; /* Action */
	char *run;       /* Program to run or program option */
	char *env;       /* Environment variable to run */
};

/* Extension pattern and mime combination */
struct assoc {
	char *regex; /* Regex to match on filename */
	char *mime;  /* File type */
};
