#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include "xdpy.h"

struct _XDpy {
  Display *dpy;
  Window win;
};

XDpy *
new_xdpy(char *display_name)
{
  XDpy *self;
  Display *dpy;

  self = malloc(sizeof(XDpy));
  if (self == NULL) return NULL;

  dpy = self->dpy = XOpenDisplay(display_name);
  if (dpy == NULL) {
    free(self);
    return NULL;
  }
  self->win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 1, 1, 0, 0, WhitePixel(dpy, 0));
  return self;
}

void
delete_xdpy(XDpy *self)
{
  XDestroyWindow(self->dpy, self->win);
  XCloseDisplay(self->dpy);
  free(self);
}

static Atom select_prop = None;
static Atom text_type = None;

char *
retrieve_selection(XDpy *self, char *selection_name)
{
  Window cursel;
  XEvent ev;
  unsigned char *return_str;
  char *r = NULL;
  Atom return_type, selection_atom;
  int return_format;
  unsigned long nitems, byte_left;
  
  selection_atom = XInternAtom(self->dpy, selection_name, 1);
  
  if (select_prop == None) {
    select_prop = XInternAtom(self->dpy, "select", False);
  }
  if (text_type == None) {
    text_type = XInternAtom(self->dpy, "TEXT", False);
  }

  cursel = XGetSelectionOwner(self->dpy, selection_atom);
  if (cursel) {
    XConvertSelection(self->dpy, selection_atom, text_type, 
		      select_prop, self->win, CurrentTime);
    XSelectInput(self->dpy, self->win, 0);
    XFlush(self->dpy);
    while (1) {
      XNextEvent(self->dpy, &ev);
      if (ev.type == SelectionNotify) break;
    }
    XGetWindowProperty(self->dpy, self->win, select_prop, 0, 0x400,
		       False, AnyPropertyType, &return_type,
		       &return_format, &nitems,
		       &byte_left, &return_str);
    /* TODO: change encoding */
    r = malloc(nitems + 1);
    memcpy(r, return_str, nitems);
    r[nitems] = 0;
    XFree(return_str);
  } 
  return r;
}
