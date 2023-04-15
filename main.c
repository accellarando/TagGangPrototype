/*
 * This is some example source code to do some basic skeleton tracking to control a stepper motor.
 *
 * Much of this source code is taken from Skeltrack's test-kinect example.
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

static void on_new_kinect_device (GObject      *obj,
                      GAsyncResult *res,
                      gpointer      user_data)
{
  ClutterActor *stage, *instructions;
  GError *error = NULL;
  gint width = 640;
  gint height = 480;

  kinect = gfreenect_device_new_finish (res, &error);
  if (kinect == NULL)
    {
      g_debug ("Failed to created kinect device: %s", error->message);
      g_error_free (error);
      clutter_main_quit ();
      return;
    }

  g_debug ("Kinect device created!");

  stage = clutter_stage_new ();
  clutter_stage_set_title (CLUTTER_STAGE (stage), "Kinect Test");
  clutter_actor_set_size (stage, width * 2, height + 250);
  clutter_stage_set_user_resizable (CLUTTER_STAGE (stage), TRUE);

  g_signal_connect (stage, "destroy", G_CALLBACK (on_destroy), kinect);
  g_signal_connect (stage,
                    "key-release-event",
                    G_CALLBACK (on_key_release),
                    kinect);

  depth_tex = clutter_actor_new ();
  depth_canvas = clutter_canvas_new ();
  clutter_actor_set_content (depth_tex, depth_canvas);
  clutter_canvas_set_size (CLUTTER_CANVAS (depth_canvas), width, height);
  clutter_actor_set_size (depth_tex, width, height);
  clutter_actor_add_child (stage, depth_tex);

  video_tex = clutter_actor_new ();
  clutter_actor_set_content (video_tex, clutter_image_new ());
  clutter_actor_set_size (video_tex, width, height);
  clutter_actor_set_position (video_tex, width, 0.0);
  clutter_actor_add_child (stage, video_tex);

  info_text = clutter_text_new ();
  clutter_actor_set_position (info_text, 50, height + 20);
  clutter_actor_add_child (stage, info_text);

  instructions = create_instructions ();
  clutter_actor_set_position (instructions, 50, height + 70);
  clutter_actor_add_child (stage, instructions);

  skeleton = skeltrack_skeleton_new ();
  g_object_get (skeleton, "smoothing-factor", &SMOOTHING_FACTOR, NULL);

  set_info_text ();

  g_signal_connect (kinect,
                    "depth-frame",
                    G_CALLBACK (on_depth_frame),
                    NULL);

  g_signal_connect (kinect,
                    "video-frame",
                    G_CALLBACK (on_video_frame),
                    NULL);

  g_signal_connect (depth_canvas,
                    "draw",
                    G_CALLBACK (on_skeleton_draw),
                    NULL);

  gfreenect_device_set_tilt_angle (kinect, 0, NULL, NULL, NULL);

  gfreenect_device_start_depth_stream (kinect,
                                       GFREENECT_DEPTH_FORMAT_MM,
                                       NULL);

  gfreenect_device_start_video_stream (kinect,
                                       GFREENECT_RESOLUTION_MEDIUM,
                                       GFREENECT_VIDEO_FORMAT_RGB, NULL);

  clutter_actor_show (stage);
}
int main(int argc, char* argv[]){
	printf("Hello world!\n");

	gfreenect_device_new(0,
			GFREENECT_SUBDEVICE_CAMERA,
			NULL,
			on_new_kinect_device,
			NULL);
	return 0;
}
