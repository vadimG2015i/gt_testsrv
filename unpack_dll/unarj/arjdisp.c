/*
 * $Id: arjdisp.c,v 1.1.1.1 2002/03/28 00:01:19 andrew_belov Exp $
 * ---------------------------------------------------------------------------
 * This is a sample graphical interface program.
 *
 */

#include "arj.h"

#include <stdlib.h>

DEBUGHDR(__FILE__)                      /* Debug information block */

/* Local variables */

static char *archive_name;
static char *filename;
static long uncompsize;
static long bytes;
static long compsize;
static char cmd_verb;
static char msg_lf[]="\n";
static char strform[]="%s";

/* Pseudographical controls */

#if TARGET==UNIX
static char indo[]="*";
static char win_top   []="/----------------------------------------------------------------------------\\";
static char win_border[]="|";
static char win_bottom[]="\\----------------------------------------------------------------------------/";
static char ind_top   []="+--------------------------------------------------+";
static char ind_middle[]="|..................................................|";
static char ind_bottom[]="+--------------------------------------------------+";
#else
static char indo[]="�";
static char win_top   []="������������������������������������������������������������������������������";
static char win_border[]="�";
static char win_bottom[]="������������������������������������������������������������������������������";
static char ind_top   []="����������������������������������������������������";
static char ind_middle[]="����������������������������������������������������";
static char ind_bottom[]="����������������������������������������������������";
#endif

/* Shows the initial screen */

static void show_init_scrn()
{
 int i;
 char *t;

 textbackground(1);
 textcolor(7);
 clrscr();
 gotoxy(2, 2);
 wprintf(win_top);
 for(i=3; i<24; i++)
 {
  gotoxy(2, i); wprintf(win_border);
  gotoxy(79, i); wprintf(win_border);
 }
 gotoxy(2, 24); wprintf(win_bottom);
 gotoxy(10, 5);
 wprintf(M_ARJDISP_COPYRIGHT);
 gotoxy(10, 6);
 wprintf(M_ARJDISP_DISTRIBUTION);
 gotoxy(10, 7);
 wprintf(M_ARJDISP_LICENSE);
 gotoxy(16, 10);
 wprintf(M_PROCESSING_ARCHIVE, archive_name);
 t=strtok(M_ARJDISP_INFO, msg_lf);
 i=13;
 while(t!=NULL&&i<=23)
 {
  gotoxy(10, i++);
  wprintf(strform, t);
  t=strtok(NULL, msg_lf);
 }
 gotoxy(16, 20);
 wprintf(M_PRESS_ANY_KEY);
 uni_getch();
 gotoxy(1, 24);
}

/* Archive processing screen */

static void show_proc_scrn()
{
 int i;

 textbackground(1);
 textcolor(7);
 if(bytes==0L)
 {
  clrscr();
  gotoxy(2, 2);
  wprintf(win_top);
  for(i=3; i<24; i++)
  {
   gotoxy(2, i); wprintf(win_border);
   gotoxy(79, i); wprintf(win_border);
  }
  gotoxy(2, 24); wprintf(win_bottom);
  gotoxy(10, 5);
  wprintf(M_ARJDISP_COPYRIGHT);
  gotoxy(10, 6);
  wprintf(M_ARJDISP_DISTRIBUTION);
  gotoxy(10, 7);
  wprintf(M_ARJDISP_LICENSE);
  gotoxy(16, 10);
  wprintf(M_PROCESSING_ARCHIVE, archive_name);
  gotoxy(16, 12);
  switch(cmd_verb)
  {
   case ARJ_CMD_ADD:
   case ARJ_CMD_FRESHEN:
   case ARJ_CMD_MOVE:
   case ARJ_CMD_UPDATE:
    wprintf(M_ADDING_FILE, filename);
    break;
   case ARJ_CMD_TEST:
    wprintf(M_TESTING_FILE, filename);
    break;
   case ARJ_CMD_EXTR_NP:
   case ARJ_CMD_EXTRACT:
    wprintf(M_EXTRACTING_FILE, filename);
    break;
   default:
    wprintf(M_PROCESSING_FILE, filename);
    break;
  }
  gotoxy(15, 14);
  wprintf(ind_top);
  gotoxy(15, 15);
  wprintf(ind_middle);
  gotoxy(15, 16);
  wprintf(ind_bottom);
  gotoxy(16, 18);
  wprintf(M_ARJDISP_CTR_START);
 }
 else
 {
  gotoxy(16, 15);
  i=calc_percentage(bytes, uncompsize)/20;
  while(i--)
   wprintf(indo);
  gotoxy(16, 18);
  wprintf(M_ARJDISP_CTR, calc_percentage(bytes, uncompsize)/10);
 }
 gotoxy(16, 15);
 if(bytes==uncompsize)
  gotoxy(1, 24);
}

/* Ending screen */

static void show_ending_scrn()
{
 int i;

 textbackground(1);
 textcolor(7);
 clrscr();
 gotoxy(2, 2);
 wprintf(win_top);
 for(i=3; i<24; i++)
 {
  gotoxy(2, i); wprintf(win_border);
  gotoxy(79, i); wprintf(win_border);
 }
 gotoxy(2, 24); wprintf(win_bottom);
 gotoxy(10, 5);
 wprintf(M_ARJDISP_COPYRIGHT);
 gotoxy(10, 6);
 wprintf(M_ARJDISP_DISTRIBUTION);
 gotoxy(10, 7);
 wprintf(M_ARJDISP_LICENSE);
 gotoxy(16, 10);
 wprintf(M_FINISHED_PROCESSING, archive_name);
 gotoxy(1, 24);
 scrn_reset();
 delay(2);
}

/* Main routine */

void main(int argc, char **argv)
{
 if(argc==2)
 {
  if(!strcmp(argv[1], "test"))
   system("arj t arj*" EXE_EXTENSION " -hep -*");
 }
 if(argc!=7)
  error(M_ARJDISP_BANNER);
 archive_name=argv[1];
 filename=argv[2];
 uncompsize=atol(argv[3]);
 bytes=atol(argv[4]);
 compsize=atol(argv[5]);
 cmd_verb=argv[6][0];
 if(cmd_verb=='+')
  show_init_scrn();
 else if(cmd_verb=='-')
  show_ending_scrn();
 else
  show_proc_scrn();
 return (0);
}

