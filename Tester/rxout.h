#ifndef RXOUT_H
#define RXOUT_H

#define WM_REFRESH          WM_USER+20      // RxOut: Fenster-Update


BOOL PostTextToWindow (PHWND pHwnd, PSZ szText, PSZ szTitle, LONG clrBackgnd);

#endif /* RXOUT_H */
