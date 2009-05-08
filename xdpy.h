/* Module to encapsulate Xlib */

struct _XDpy;

typedef struct _XDpy XDpy;

XDpy *new_xdpy(char *display_name);
void delete_xdpy(XDpy *dpy);
char *retrieve_selection(XDpy *self, char *selection_name);
