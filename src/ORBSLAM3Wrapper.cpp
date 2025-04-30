#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <Eigen/Dense>

#include <ORB_SLAM3/include/KeyFrame.h>
#include <ORB_SLAM3/include/Converter.h>
#include <ORB_SLAM3/include/Tracking.h>
#include <ORB_SLAM3/include/MapPoint.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "ORBSLAM3Wrapper.h"
#include "NDArrayConverter.h"

namespace py = pybind11;

/// @brief Constructor for ORBSLAM3Python
/// @param vocabFile The path to the vocabulary file
/// @param settingsFile The path to the settings file
/// @param sensorMode The sensor mode
ORBSLAM3Python::ORBSLAM3Python(std::string vocabFile, std::string settingsFile, ORB_SLAM3::System::eSensor sensorMode)
    : vocabluaryFile(vocabFile),
      settingsFile(settingsFile),
      sensorMode(sensorMode),
      system(nullptr),
      bUseViewer(false) {}

/// @brief Destructor for ORBSLAM3Python
ORBSLAM3Python::~ORBSLAM3Python() {}

/// @brief Initialize the ORBSLAM3 system
/// @return True if the system was initialized successfully, false otherwise
bool ORBSLAM3Python::initialize()
{
    system = std::make_shared<ORB_SLAM3::System>(vocabluaryFile, settingsFile, sensorMode, bUseViewer);
    return true;
}

/// @brief Check if the system is running
/// @return True if the system is running, false otherwise
bool ORBSLAM3Python::isRunning() { return system != nullptr; }

/// @brief Reset the system
void ORBSLAM3Python::reset()
{
    if (system)
    {
        system->Reset();
    }
}

/// @brief Process a monocular image
/// @param image The image to process
/// @param timestamp The timestamp of the image
/// @return The pose of the camera
py::object ORBSLAM3Python::processMono(cv::Mat image, double timestamp)
{
    if (!system)
    {
        std::cout << "you must call initialize() first!" << std::endl;
        return py::none();
    }
    if (image.data)
    {
        auto pose = system->TrackMonocular(image, timestamp);
        return getCurrentPose();
        // return py::cast(pose.matrix());
    }
    else
    {
        std::cout << "you must provide an image!" << std::endl;
        return py::none();
    }
}

/// @brief Process a stereo image
/// @param leftImage The left image
/// @param rightImage The right image
/// @param timestamp The timestamp of the image
/// @return The pose of the camera
py::object ORBSLAM3Python::processStereo(cv::Mat leftImage, cv::Mat rightImage, double timestamp)
{
    if (!system)
    {
        std::cout << "you must call initialize() first!" << std::endl;
        return py::none();
    }
    if (leftImage.data && rightImage.data)
    {
        auto pose = system->TrackStereo(leftImage, rightImage, timestamp);
        return getCurrentPose();
        // return py::cast(pose.matrix());
    }
    else
    {
        std::cout << "you must provide both left and right images!" << std::endl;
        return py::none();
    }
}

/// @brief Process a stereo image with IMU data
/// @param leftImage The left image
/// @param rightImage The right image
/// @param timestamp The timestamp of the image
/// @param imuData The IMU data
/// @return The pose of the camera
py::object ORBSLAM3Python::processStereoIMU(cv::Mat leftImage, cv::Mat rightImage, double timestamp, std::vector<std::vector<float>> imuData)
{
    if (!system)
    {
        std::cout << "you must call initialize() first!" << std::endl;
        return py::none();
    }
    if (leftImage.data && rightImage.data)
    {

        // Check the imuData vector
        if (imuData.empty())
        {
            std::cout << "you must provide IMU data!" << std::endl;
            return py::none();
        }

        // Create a list of ORB_SLAM3::IMU::Point objects
        std::vector<ORB_SLAM3::IMU::Point> imuDataList;

        // Iterate through the imuData vector and create a list of ORB_SLAM3::IMU::Point objects
        for (const auto &imuPoint : imuData)
        {
            // Check the size of the imuData vector
            if (imuPoint.size() != 7)
            {
                std::cout << "IMU Data must be [acc_x, acc_y, acc_z, ang_vel_x, ang_vel_y, ang_vel_z, timestamp]" << std::endl;
                return py::none();
            }

            // Create an ORB_SLAM3::IMU::Point object
            imuDataList.push_back(ORB_SLAM3::IMU::Point(imuPoint[0], imuPoint[1], imuPoint[2], imuPoint[3], imuPoint[4], imuPoint[5], imuPoint[6]));
        }

        auto pose = system->TrackStereo(leftImage, rightImage, timestamp, imuDataList);
        return getCurrentPose();
        // return py::cast(pose.matrix());
    }
    else
    {
        std::cout << "you must provide both left and right images!" << std::endl;
        return py::none();
    }
}

/// @brief Check if the system is lost
/// @return True if lost or uninitialized, false otherwise
bool ORBSLAM3Python::isLost()
{
    if (system)
    {
        return system->isLost();
    }
    else
    {
        return true;
    }
}

/// @brief Shutdown the system
void ORBSLAM3Python::shutdown()
{
    if (system)
    {
        system->Shutdown();
    }
}

/// @brief Check if the map has changed
/// @return True if the map has changed, false otherwise
bool ORBSLAM3Python::mapChanged()
{
    if (system)
    {
        return system->MapChanged();
    }
}

/// @brief Get the tracking state
/// @return The tracking state
int ORBSLAM3Python::getTrackingState()
{
    if (system)
    {
        return system->GetTrackingState();
    }
}

/// @brief Get the number of tracked key points from the last frame
/// @return The number of tracked key points
int ORBSLAM3Python::getTrackedKeyPoints()
{
    if (system)
    {
        return system->GetTrackedKeyPointsUn().size();
    }
    else
    {
        return 0;
    }
}

void ORBSLAM3Python::setUseViewer(bool useViewer)
{
    bUseViewer = useViewer;
}

/// @brief Get the camera trajectory
/// @return A vector of 4x4 matrices representing the camera trajectory
/// @note This seems to cause a segfault if orbslam loses tracking
std::vector<Eigen::Matrix4f> ORBSLAM3Python::getTrajectory() const
{
    if (system)
    {
        // TODO: Fix this
        return system->GetCameraTrajectory();
        // return std::vector<Eigen::Matrix4f>();
    }
    else
    {
        return std::vector<Eigen::Matrix4f>();
    }
}

/// @brief Get the current camera pose
/// @return The current camera pose
py::object ORBSLAM3Python::getCurrentPose() const
{
    if (system)
    {
        std::vector<Eigen::Matrix4f> trajectory = system->GetCameraTrajectory();
        if (trajectory.empty())
        {
            return py::none();
        }
        return py::cast(trajectory.back());
    }
    else
    {
        return py::none();
    }
}

PYBIND11_MODULE(orbslam3, m)
{
    NDArrayConverter::init_numpy();
    py::enum_<ORB_SLAM3::Tracking::eTrackingState>(m, "TrackingState")
        .value("SYSTEM_NOT_READY", ORB_SLAM3::Tracking::eTrackingState::SYSTEM_NOT_READY)
        .value("NO_IMAGES_YET", ORB_SLAM3::Tracking::eTrackingState::NO_IMAGES_YET)
        .value("NOT_INITIALIZED", ORB_SLAM3::Tracking::eTrackingState::NOT_INITIALIZED)
        .value("OK", ORB_SLAM3::Tracking::eTrackingState::OK)
        .value("RECENTLY_LOST", ORB_SLAM3::Tracking::eTrackingState::RECENTLY_LOST)
        .value("LOST", ORB_SLAM3::Tracking::eTrackingState::LOST)
        .value("OK_KLT", ORB_SLAM3::Tracking::eTrackingState::OK_KLT);

    py::enum_<ORB_SLAM3::System::eSensor>(m, "Sensor")
        .value("MONOCULAR", ORB_SLAM3::System::eSensor::MONOCULAR)
        .value("STEREO", ORB_SLAM3::System::eSensor::STEREO)
        .value("RGBD", ORB_SLAM3::System::eSensor::RGBD)
        .value("IMU_MONOCULAR", ORB_SLAM3::System::eSensor::IMU_MONOCULAR)
        .value("IMU_STEREO", ORB_SLAM3::System::eSensor::IMU_STEREO)
        .value("IMU_RGBD", ORB_SLAM3::System::eSensor::IMU_RGBD);

    // py::class_<ORB_SLAM3::IMU::Point>(m, "IMUPoint")
    //     .def(py::init<const float &, const float &, const float &,
    //                   const float &, const float &, const float &,
    //                   const double &>(),
    //          py::arg("acc_x"), py::arg("acc_y"), py::arg("acc_z"),
    //          py::arg("ang_vel_x"), py::arg("ang_vel_y"), py::arg("ang_vel_z"),
    //          py::arg("timestamp"))
    //     .def_readwrite("a", &ORB_SLAM3::IMU::Point::a)
    //     .def_readwrite("w", &ORB_SLAM3::IMU::Point::w)
    //     .def_readwrite("t", &ORB_SLAM3::IMU::Point::t);

    py::class_<ORBSLAM3Python>(m, "system")
        .def(py::init<std::string, std::string, ORB_SLAM3::System::eSensor>(), py::arg("vocab_file"), py::arg("settings_file"), py::arg("sensor_type"))
        .def("initialize", &ORBSLAM3Python::initialize)
        .def("process_image_mono", &ORBSLAM3Python::processMono, py::arg("image"), py::arg("time_stamp"))
        .def("process_image_stereo", &ORBSLAM3Python::processStereo, py::arg("left_image"), py::arg("right_image"), py::arg("time_stamp"))
        .def("process_image_stereo_imu", &ORBSLAM3Python::processStereoIMU, py::arg("left_image"), py::arg("right_image"), py::arg("time_stamp"), py::arg("imu_data"))
        .def("shutdown", &ORBSLAM3Python::shutdown)
        .def("is_running", &ORBSLAM3Python::isRunning)
        .def("is_lost", &ORBSLAM3Python::isLost)
        .def("map_changed", &ORBSLAM3Python::mapChanged)
        .def("get_tracking_state", &ORBSLAM3Python::getTrackingState)
        .def("get_tracked_kp_qty", &ORBSLAM3Python::getTrackedKeyPoints)
        .def("reset", &ORBSLAM3Python::reset)
        // .def("set_use_viewer", &ORBSLAM3Python::setUseViewer)
        .def("get_trajectory", &ORBSLAM3Python::getTrajectory)
        .def("get_current_pose", &ORBSLAM3Python::getCurrentPose);
}