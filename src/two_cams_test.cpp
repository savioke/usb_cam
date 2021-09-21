/**
 * ROS-independent executable for
 * testing alternate streaming from
 * two cameras.
 */
#include <ros/ros.h>
#include <usb_cam/usb_cam.h>
#include <sstream>


using namespace usb_cam;


static const UsbCam::io_method io_method = UsbCam::IO_METHOD_MMAP;
static const UsbCam::pixel_format pixel_format = UsbCam::PIXEL_FORMAT_MJPEG;
static const int image_width = 1280;
static const int image_height = 720;
static const int framerate = 25;

int main(int argc, char **argv)
{
  ros::Time::init();
  std::string device1("/dev/video0");
  std::string device2("/dev/video2");
  if (argc > 1)
  {
    device1 = argv[1];
  }
  if (argc > 2)
  {
    device2 = argv[2];
  }
  UsbCam cam1;
  ROS_INFO_STREAM("Starting device " << device1);
  cam1.start(device1.c_str(), io_method, pixel_format,
             image_width, image_height, framerate);
  cam1.stop_capturing();

  UsbCam cam2;
  ROS_INFO_STREAM("Starting device " << device2);
  cam2.start(device2.c_str(), io_method, pixel_format,
             image_width, image_height, framerate);
  cam2.stop_capturing();


  sensor_msgs::Image img1, img2;

  ros::Rate loop_rate(10);
  while (true)
  {
    cam1.start_capturing();
    cam1.grab_image(&img1);
    ROS_INFO_STREAM("C1: Grabbed image: " << img1.width << "x" << img1.height);
    cam1.stop_capturing();
    cam2.start_capturing();
    cam2.grab_image(&img2);
    cam2.stop_capturing();
    ROS_INFO_STREAM("C2: Grabbed image: " << img2.width << "x" << img2.height);
    loop_rate.sleep();
  }
  cam1.shutdown();
  cam2.shutdown();

  return EXIT_SUCCESS;
}
