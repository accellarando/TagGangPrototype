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
	guint16 *original_buffer;
	gint width;
	gint height;
	gint reduced_width;
	gint reduced_height;
} BufferInfo;


typedef struct
{
	gint x;
	gint y;
	gint z;
} Point;


static gint GESTURE_THRESHOLD = 250;

static gboolean hand_is_active (SkeltrackJoint *head, SkeltrackJoint *hand)
{
	return hand != NULL && ABS (head->z - hand->z) > GESTURE_THRESHOLD;
}

static void grayscale_buffer_set_value (guchar *buffer, gint index, guchar value)
{
  buffer[index * 3] = value;
  buffer[index * 3 + 1] = value;
  buffer[index * 3 + 2] = value;
}

static void paint_joint (cairo_t *cairo,
             SkeltrackJoint *joint,
             gint radius,
             const gchar *color_str)
{
	printf("In paint joint\n");
  ClutterColor *color;

  if (joint == NULL)
    return;

  color = clutter_color_new (0, 0, 0, 200);
  clutter_color_from_string (color, color_str);

  cairo_set_line_width (cairo, 10);
  clutter_cairo_set_source_color (cairo, color);
  cairo_arc (cairo,
             joint->screen_x,
             joint->screen_y,
             radius * (THRESHOLD_END - THRESHOLD_BEGIN) / joint->z,
             0,
             G_PI * 2);
  cairo_fill (cairo);
  clutter_color_free (color);
}
static guchar * create_grayscale_buffer (BufferInfo *buffer_info, gint dimension_reduction)
{
  gint i, j;
  gint size;
  guchar *grayscale_buffer;
  guint16 *reduced_buffer;

  reduced_buffer = buffer_info->reduced_buffer;

  size = buffer_info->width * buffer_info->height * sizeof (guchar) * 3;
  grayscale_buffer = g_slice_alloc (size);
  /* Paint it white */
  memset (grayscale_buffer, 255, size);

  for (i = 0; i < buffer_info->reduced_width; i++)
    {
      for (j = 0; j < buffer_info->reduced_height; j++)
        {
          if (reduced_buffer[j * buffer_info->reduced_width + i] != 0)
            {
              gint index = j * dimension_reduction * buffer_info->width +
                i * dimension_reduction;
              grayscale_buffer_set_value (grayscale_buffer, index, 0);
            }
        }
    }

  return grayscale_buffer;
}
static BufferInfo * process_buffer (guint16 *buffer,
                guint width,
                guint height,
                guint dimension_factor,
                guint threshold_begin,
                guint threshold_end)
{
  BufferInfo *buffer_info;
  gint i, j, reduced_width, reduced_height;
  guint16 *reduced_buffer;

  g_return_val_if_fail (buffer != NULL, NULL);

  reduced_width = (width - width % dimension_factor) / dimension_factor;
  reduced_height = (height - height % dimension_factor) / dimension_factor;

  reduced_buffer = g_slice_alloc0 (reduced_width * reduced_height *
                                   sizeof (guint16));

  for (i = 0; i < reduced_width; i++)
    {
      for (j = 0; j < reduced_height; j++)
        {
          gint index;
          guint16 value = 0;

          index = j * width * dimension_factor + i * dimension_factor;
          value = buffer[index];

          if (value < threshold_begin || value > threshold_end)
            {
              reduced_buffer[j * reduced_width + i] = 0;
              continue;
            }

          reduced_buffer[j * reduced_width + i] = value;
        }
    }

  buffer_info = g_slice_new0 (BufferInfo);
  buffer_info->reduced_buffer = reduced_buffer;
  buffer_info->original_buffer = buffer;
  buffer_info->reduced_width = reduced_width;
  buffer_info->reduced_height = reduced_height;
  buffer_info->width = width;
  buffer_info->height = height;

  return buffer_info;
}
static Point* smooth_point (guint16 *buffer, guint width, guint height, SkeltrackJoint *joint)
{
	if (joint == NULL)
		return NULL;

	Point *closest;
	gint i, j, x, y, radius, min, count;
	radius = 16;
	x = joint->screen_x;
	y = joint->screen_y;

	if (x >= width || y >= height)
		return NULL;

	closest = g_slice_new0 (Point);

	closest->x = x;
	closest->y = y;
	closest->z = joint->z;
	min = closest->z - 50;

	count = 1;

	for (i = x - radius; i < x + radius; i+=2)
	{
		if (i < 0 || i >= width)
			continue;
		for (j = y - radius; j < y + radius; j+=2)
		{
			guint16 current;
			if (j < 0 || j >= height || (j == y && i == x))
				continue;

			current = buffer[j * width + i];
			if (current < closest->z && current >= min)
			{
				closest->x += x;
				closest->y += y;
				count++;
			}
		}
	}

	closest->x /= count;
	closest->y /= count;

	return closest;
}
static void interpret_gestures (SkeltrackJointList joint_list,
		guint16 *buffer,
		guint width,
		guint height)
{
	gint64 time;
	Point *left_point, *right_point, *single_point;
	SkeltrackJoint *head, *left_hand, *right_hand;

	if (joint_list == NULL){
		printf("Null joint list\n");
		return;
	}

	head = skeltrack_joint_list_get_joint (joint_list,
			SKELTRACK_JOINT_ID_HEAD);
	left_hand = skeltrack_joint_list_get_joint (joint_list,
			SKELTRACK_JOINT_ID_LEFT_HAND);
	right_hand = skeltrack_joint_list_get_joint (joint_list,
			SKELTRACK_JOINT_ID_RIGHT_HAND);

	if (head == NULL)
		printf("Null head\n");
		return;

	left_point = NULL;
	right_point = NULL;
	single_point = NULL;

	if (hand_is_active (head, left_hand))
	{
		left_point = smooth_point (buffer, width, height, left_hand);
		single_point = left_point;
		if (hand_is_active (head, right_hand))
		{
			right_point = smooth_point (buffer, width, height, right_hand);
			single_point = NULL;
		}
	}
	else if (hand_is_active (head, right_hand))
	{
		right_point = smooth_point (buffer, width, height, right_hand);
		single_point = right_point;
	}
	printf("right point: (%d,%d,%d), left point:(%d,%d,%d)",
			right_point->x,right_point->y, right_point->z,
			left_point->x, left_point->y, left_point->z);
}
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

static void on_track_joints (GObject      *obj,
		GAsyncResult *res,
		gpointer      user_data)
{
	BufferInfo *buffer_info;
	guint16 *reduced, *original;
	gint width, height;
	GError *error = NULL;

	buffer_info = (BufferInfo *) user_data;
	reduced = (guint16 *) buffer_info->reduced_buffer;
	original = (guint16 *) buffer_info->original_buffer;
	width = buffer_info->width;
	height = buffer_info->height;

	list = skeltrack_skeleton_track_joints_finish (skeleton,
			res,
			&error);

	if (error == NULL)
	{
		printf("Joint list: %d\n", list);
		interpret_gestures (list, original, width, height);
		/*
		if (SHOW_SKELETON)
			clutter_cairo_texture_invalidate (CLUTTER_CAIRO_TEXTURE (depth_tex));
			*/
	}
	else
	{
		g_error_free (error);
	}

	g_slice_free1 (width * height * sizeof (guint16), reduced);
	g_slice_free (BufferInfo, buffer_info);
}

static void on_depth_frame (GFreenectDevice *kinect, gpointer user_data)
{
	gint width, height;
	gint dimension_factor;
	guchar *grayscale_buffer;
	guint16 *depth;
	BufferInfo *buffer_info;
	gsize len;
	GError *error = NULL;
	GFreenectFrameMode frame_mode;

	depth = (guint16 *) gfreenect_device_get_depth_frame_raw (kinect,
			&len,
			&frame_mode);

	width = frame_mode.width;
	height = frame_mode.height;

	g_object_get (skeleton, "dimension-reduction", &dimension_factor, NULL);

	buffer_info = process_buffer (depth,
			width,
			height,
			dimension_factor,
			THRESHOLD_BEGIN,
			THRESHOLD_END);

	skeltrack_skeleton_track_joints (skeleton,
			buffer_info->reduced_buffer,
			buffer_info->reduced_width,
			buffer_info->reduced_height,
			NULL,
			on_track_joints,
			buffer_info);


	if (!SHOW_SKELETON)
	{
		grayscale_buffer = create_grayscale_buffer (buffer_info,
				dimension_factor);
		if (! clutter_texture_set_from_rgb_data (CLUTTER_TEXTURE (depth_tex),
					grayscale_buffer,
					FALSE,
					width, height,
					0,
					3,
					CLUTTER_TEXTURE_NONE,
					&error))
		{
			g_error_free (error);
		}
		g_slice_free1 (width * height * sizeof (guchar) * 3,
				grayscale_buffer);
	}
}

static void on_texture_draw (ClutterCairoTexture *texture,
		cairo_t *cairo,
		gpointer user_data)
{
	guint width, height;
	ClutterColor *color;
	SkeltrackJoint *head, *left_hand, *right_hand;
	if (list == NULL)
		return;

	head = skeltrack_joint_list_get_joint (list,
			SKELTRACK_JOINT_ID_HEAD);
	left_hand = skeltrack_joint_list_get_joint (list,
			SKELTRACK_JOINT_ID_LEFT_HAND);
	right_hand = skeltrack_joint_list_get_joint (list,
			SKELTRACK_JOINT_ID_RIGHT_HAND);

	/* Paint it white */
	clutter_cairo_texture_clear (texture);
	clutter_cairo_texture_get_surface_size (texture, &width, &height);
	color = clutter_color_new (255, 255, 255, 255);
	clutter_cairo_set_source_color (cairo, color);
	cairo_rectangle (cairo, 0, 0, width, height);
	cairo_fill (cairo);
	clutter_color_free (color);

	paint_joint (cairo, head, 50, "#FFF800");
	paint_joint (cairo, left_hand, 30, "#C2FF00");
	paint_joint (cairo, right_hand, 30, "#00FAFF");

	skeltrack_joint_list_free (list);
	list = NULL;
}

static void on_new_kinect_device (GObject *obj, GAsyncResult *res, gpointer user_data){
	ClutterActor *stage, *instructions;
	GError *error = NULL;
	gint width = 640;
	gint height = 480;

	kinect = gfreenect_device_new_finish (res, &error);
	if (kinect == NULL)
	{
		printf("Kinect was null: %s\n",error->message);
		g_error_free (error);
		clutter_main_quit ();
		return;
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


	skeleton = SKELTRACK_SKELETON (skeltrack_skeleton_new ());

	g_signal_connect (kinect,
			"depth-frame",
			G_CALLBACK (on_depth_frame),
			NULL);

	g_signal_connect (depth_tex,
			"draw",
			G_CALLBACK (on_texture_draw),
			NULL);

	gfreenect_device_set_tilt_angle (kinect, 0, NULL, NULL, NULL);

	gfreenect_device_start_depth_stream (kinect,
			GFREENECT_DEPTH_FORMAT_MM,
			NULL);
}

static void quit (gint signale)
{
	signal (SIGINT, 0);

	clutter_main_quit ();
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

	signal (SIGINT, quit);

	clutter_main();

	if (kinect != NULL)
		g_object_unref (kinect);

	if (skeleton != NULL)
	{
		g_object_unref (skeleton);
	}

	return 0;
}
