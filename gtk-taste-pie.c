#include <stdio.h>

#include <gtk/gtk.h>

#include "gdkaddons.h"
#include "gtkpiemenu.h"


GtkWidget	*step, *refinement, *number, *shift;
GtkWidget	*automatic, *submenus, *label;

#if 0
static void
selected_callback (void)
{
  g_message ("Select callback");
}

static void
deselected_callback (void)
{
  g_message ("Deselect callback");
}
#endif

void
number_callback (void)
{
  int   nu, re, reo;

  nu = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (number));
  reo = re = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (refinement));
  
  while (re < 4 && (nu > (1 << re)))
    re++;

  if ((re != reo) && (GTK_TOGGLE_BUTTON (automatic) -> active == 0))
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (refinement), re);
}

GtkWidget *
my_spin_button_new (char *name, gdouble ivalue, gdouble maxim, GtkWidget **sb, GtkSignalFunc adjcallback)
{
  GtkWidget *mybox;
  GtkWidget *label;
  GtkWidget *spinb;
  GtkObject *adjust;


  mybox = gtk_hbox_new (FALSE, 5);

  label = gtk_label_new (name);
  adjust = gtk_adjustment_new (ivalue, 0.0, maxim,
				1.0, 1.0,  0.0);

*sb = spinb = gtk_spin_button_new (GTK_ADJUSTMENT (adjust),
			       0.5, 0);

  gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinb), TRUE);
  gtk_spin_button_set_snap_to_ticks (GTK_SPIN_BUTTON (spinb), TRUE);

  gtk_box_pack_start (GTK_BOX (mybox), label, FALSE, FALSE, 0);
  gtk_box_pack_end   (GTK_BOX (mybox), spinb, FALSE, FALSE, 0);

  gtk_signal_connect (GTK_OBJECT (adjust), "value_changed",
		      adjcallback, NULL);

  return mybox;
}

static GtkWidget *test_pie_menu = NULL;

#if 0
static void
test_my_report (void)
{
  printf ("%d\n", ((GtkPieMenu *) test_pie_menu)->menu_type);
}
#endif

static int
test_callback (GtkWidget *w, gpointer	data)
{
  g_message ("Callback %d. called", (guint) data);
  return TRUE;
}

static int
test_callback_s (GtkWidget *w, gpointer	data)
{
  g_message ("Callback %s. called", (char *) data);
  return TRUE;
}

static GtkWidget*
taste_sub1 (void)
{
  static char *names[] = { "Chocolate", "Apple pie", "Ice cream" };
  static GtkPieCompass directions[] = { GTK_PIE_N, GTK_PIE_SE, GTK_PIE_SW };
  int	i;

  GtkPieMenu	*pm;
  GtkWidget	*im;

  pm = GTK_PIE_MENU (gtk_pie_menu_new ());


  for (i = 0; i < (sizeof (names) / sizeof (char *)); i++)
    {
      im = gtk_menu_item_new_with_label (names [i]);
      gtk_pie_menu_put_with_compass (pm, im, directions [i]);
      gtk_signal_connect (GTK_OBJECT (im), "activate",
			  (GtkSignalFunc) test_callback_s, names [i]);
    }
  
  return GTK_WIDGET (pm);
}

static GtkWidget*
taste_sub2 (void)
{
  static char *tnames[] = { "Gtk+", "Tcl/Tk", "Qt", "Motif", "Athena" };
  static char *names[] = { "North", "South", "West", "East" };
  static GtkPieCompass directions[] = { GTK_PIE_N, GTK_PIE_S, GTK_PIE_W, GTK_PIE_E, GTK_PIE_SE };

  int	i;

  GtkPieMenu	*pm, *pm2;
  GtkWidget	*im;

  pm = GTK_PIE_MENU (gtk_pie_menu_new ());
  pm2 = GTK_PIE_MENU (gtk_pie_menu_new ()); 

  for (i = 0; i < (sizeof (names) / sizeof (char *)); i++)
    {
      im = gtk_menu_item_new_with_label (names [i]);
      gtk_pie_menu_put_with_compass (pm, im, directions [i]);
      gtk_signal_connect (GTK_OBJECT (im), "activate",
			  (GtkSignalFunc) test_callback_s, names [i]);
    }

  gtk_menu_item_set_submenu (GTK_MENU_ITEM (im), GTK_WIDGET (pm2));

  for (i = 0; i < (sizeof (tnames) / sizeof (char *)); i++)
    {
      im = gtk_menu_item_new_with_label (tnames [i]);

      if (i != 0)
	gtk_widget_set_sensitive (im, 0);

      gtk_pie_menu_put_with_compass (pm2, im, directions [i]);
      gtk_signal_connect (GTK_OBJECT (im), "activate",
			  (GtkSignalFunc) test_callback_s, tnames [i]);
    }

  return GTK_WIDGET (pm);

}

void
test_build_menu (void)
{

  int 		istep, irefinement, inumber, ishift, iauto, isub, iisub;
  GtkWidget	*im;
  int 		i, v;
  char		tbuf[32];

  istep = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (step));
  irefinement = 1 << gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (refinement));
  inumber = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (number));
  ishift = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (shift));

  iauto = (GTK_TOGGLE_BUTTON (automatic) -> active) ? 1 : 0;
  isub = (GTK_TOGGLE_BUTTON (submenus) -> active ) ? 1 : 0;

  if (test_pie_menu != NULL)
    gtk_widget_destroy (test_pie_menu);

  test_pie_menu = gtk_pie_menu_new_with_refinement (irefinement);

  for (i = 0, v = ishift; i < inumber; i++, v = (v + istep) & (irefinement - 1))
    {
      iisub = isub & ((i == 1) || (i == ((irefinement / 2) + 1)));

      sprintf (tbuf, (iisub) ? "Test submenu" :
	                       "Test button  [%d]", i);
      im = gtk_menu_item_new_with_label (tbuf);
      gtk_signal_connect (GTK_OBJECT (im), "activate",
			  (GtkSignalFunc) test_callback, (void *) i);

#if 0
      gtk_signal_connect (GTK_OBJECT (im), "select",
			  (GtkSignalFunc) selected_callback, (void *) i);

      gtk_signal_connect (GTK_OBJECT (im), "deselect",
			  (GtkSignalFunc) deselected_callback, (void *) i);
#endif

      if (iauto)
	gtk_pie_menu_append (GTK_PIE_MENU (test_pie_menu), im);
      else
	gtk_pie_menu_put (GTK_PIE_MENU (test_pie_menu), im, v);

      if (iisub)
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (im), 
				   (i == 1 ? taste_sub1 : taste_sub2)());
				   

    }
  gtk_widget_show_all (test_pie_menu);
}

int
test_invoke_pie_menu (GtkWidget *w,
		      GdkEventButton *event,
		      gpointer *data)
{
#if 0
  if (event -> button == 3)
    {
      static GtkWidget *menu = NULL;

      if (! menu)
	{
	  menu = gtk_menu_new ();
	  gtk_menu_append (GTK_MENU (menu), gtk_menu_item_new_with_label ("Boring"));
	  gtk_menu_append (GTK_MENU (menu), gtk_menu_item_new_with_label ("GtkMenu"));
	  gtk_menu_append (GTK_MENU (menu), gtk_menu_item_new ());
	  gtk_menu_append (GTK_MENU (menu), gtk_menu_item_new_with_label ("I am sure"));
	  gtk_menu_append (GTK_MENU (menu), gtk_menu_item_new_with_label ("You will"));
	  gtk_menu_append (GTK_MENU (menu), gtk_menu_item_new_with_label ("NOT use it"));
	  gtk_menu_append (GTK_MENU (menu), gtk_menu_item_new_with_label ("after you trie"));
	  gtk_menu_append (GTK_MENU (menu), gtk_menu_item_new_with_label ("GtkPieMenu!"));
	  gtk_widget_show_all (menu);
	}
      gtk_menu_popup (GTK_MENU (menu),
		      NULL, NULL, NULL, NULL,
		      event -> button, event -> time);
      return TRUE;
    }
#endif

  if (test_pie_menu == NULL)
    test_build_menu ();

  if ((event -> state & GDK_CONTROL_MASK)== 0)
    gtk_pie_menu_popup (GTK_PIE_MENU (test_pie_menu), event->button, event -> time);
  else
    gtk_menu_popup (GTK_MENU (test_pie_menu),
		    NULL,
		    NULL,
		    NULL,
		    NULL,
		    event -> button,
		    event -> time);

  return TRUE;
}


int main (int argc, char **argv)
{
  GtkWidget 	*window;
  GtkWidget     *ebox;
  GtkWidget	*vbox;
  GtkWidget	*button;
  GtkWidget     *frame;

  gtk_init (&argc, &argv);

  gtk_rc_parse ("./gtk-taste-pie-rc");

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Do you like pies?");
  gtk_signal_connect (GTK_OBJECT (window), "destroy",
		      (GtkSignalFunc) gtk_main_quit, NULL);

  vbox = gtk_vbox_new (FALSE, 5);
  gtk_container_add (GTK_CONTAINER (window), vbox);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 5);

  button = my_spin_button_new ("Number", 4, 16, &number, (GtkSignalFunc) number_callback);
  gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

  button = my_spin_button_new ("Refinement", 2, 4, &refinement, (GtkSignalFunc) number_callback);
  gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

  button = my_spin_button_new ("Step", 1, 16, &step, NULL);
  gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

  button = my_spin_button_new ("Shift", 0, 16, &shift, NULL);
  gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

  automatic = gtk_check_button_new_with_label ("Automatic pie bakin'");
  gtk_box_pack_start (GTK_BOX (vbox), automatic, FALSE, FALSE, 0);

  submenus = gtk_check_button_new_with_label ("Taste submenus");
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (submenus), 1);
  gtk_box_pack_start (GTK_BOX (vbox), submenus, FALSE, FALSE, 0);

  frame = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
  gtk_box_pack_start (GTK_BOX (vbox), frame, TRUE, TRUE, 0);

  ebox = gtk_event_box_new ();
  gtk_container_add (GTK_CONTAINER (frame), ebox);
  gtk_widget_set_usize (ebox, -1, 300);

  gtk_widget_add_events (ebox, GDK_BUTTON_PRESS_MASK);
  gtk_signal_connect (GTK_OBJECT (ebox), "button_press_event",
		      (GtkSignalFunc) test_invoke_pie_menu, NULL);


  label = gtk_label_new ("\
Press Mouse Button (MB)\n\
to get pie menu. Do not\n\
release MB and with mouse\n\
 motion choose your direction \n\
- pie menu item. Finally\n\
release MB to activate pie\n\
menu item.\n\
\n\
Ctrl-MB to get same menu\n\
in old layout");

  gtk_container_add (GTK_CONTAINER (ebox), label);
  gtk_misc_set_alignment (GTK_MISC (label), 0.5, 0.05);

  button = gtk_button_new_with_label ("Try this pie!");
  gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      (GtkSignalFunc) test_build_menu, NULL);

  button = gtk_button_new_with_label ("Finish bakin'!");
  gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      (GtkSignalFunc) gtk_main_quit, NULL);

  gtk_widget_show_all (window);

  gtk_main ();

  return 0;
}

