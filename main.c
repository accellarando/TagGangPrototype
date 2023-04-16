/*
 * This is some example source code to do some basic skeleton tracking to control a stepper motor.
 *
 * Much of this source code is taken from Skeltrack's test-kinect example, and joaquimrocha's Skeltrack-Desktop-Control code.
 *
 * @author Ella Moss
 */
#include <gfreenect.h>
#include <skeltrack.h>
#include <math.h>
#include <string.h>
#include <glib-object.h>
#include <clutter/clutter.h>
#include <clutter/clutter-keysyms.h>
#include <stdio.h>

static SkeltrackSkeleton *skeleton = NULL;

static GFreenectDevice *kinect = NULL;
static ClutterActor *info_text;
static ClutterActor *depth_tex;
static ClutterActor *video_tex;
static ClutterContent *depth_canvas;
static ClutterContent *depth_image = NULL;
static SkeltrackJointList list = NULL;

static gboolean SHOW_SKELETON = TRUE;
static gboolean ENABLE_SMOOTHING = TRUE;
static gfloat SMOOTHING_FACTOR = .0;

/* Adjust this value to increase or decrease the threshold. */
static guint THRESHOLD_BEGIN = 500;
static guint THRESHOLD_END   = 1500;

typedef struct
{
	guint16 *reduced_buffer;
	gint width;
	gint height;
	gint reduced_width;
	gint reduced_height;
} BufferInfo;

static void set_info_text (void)
{
  gchar *title;
  title = g_strdup_printf ("<b>Current View:</b> %s\n"
                           "<b>Threshold:</b> %d",
                           SHOW_SKELETON ? "Skeleton" : "Point Cloud",
                           THRESHOLD_END);
  clutter_text_set_markup (CLUTTER_TEXT (info_text), title);
  g_free (title);
}

static void on_new_kinect_device (GObject *obj, GAsyncResult *res, gpointer user_data){
	ClutterActor *stage, *instructions;
	GError *error = NULL;
	gint width = 640;
	gint height = 480;

	kinect = gfreenect_device_new_finish (res, &error);
	if (kinect == NULL)
	{
		/*
		printf("Kinect was null: %s\n",error->message);
		g_error_free (error);
		clutter_main_quit ();
		return;
		*/
	}

	printf("Kinect created!\n");


	stage = clutter_stage_get_default ();
	clutter_stage_set_title (CLUTTER_STAGE (stage), "TagGang Prototype");
	clutter_actor_set_size (stage, width, height + 220);
	clutter_stage_set_user_resizable (CLUTTER_STAGE (stage), TRUE);

  info_text = clutter_text_new ();
  set_info_text ();
  clutter_actor_set_position (info_text, 50, height + 20);
  clutter_container_add_actor (CLUTTER_CONTAINER (stage), info_text);

	clutter_actor_show_all(stage);
}

int main(int argc, char* argv[]){
	printf("Hello world!\n");


	if (clutter_init (&argc, &argv) != CLUTTER_INIT_SUCCESS)
    {
      return -1;
    }
	gfreenect_device_new(0,
			GFREENECT_SUBDEVICE_CAMERA,
			NULL,
			on_new_kinect_device,
			NULL);

	clutter_main();

	return 0;
}
