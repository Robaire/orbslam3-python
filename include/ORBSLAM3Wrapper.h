#ifndef ORB_SLAM3_PYTHON_H
#define ORB_SLAM3_PYTHON_H

#include <memory>
#include <System.h>
#include <Tracking.h>
#include <pybind11/pybind11.h>

class ORBSLAM3Python
{
public:
    ORBSLAM3Python(std::string vocabFile, std::string settingsFile,
                   ORB_SLAM3::System::eSensor sensorMode = ORB_SLAM3::System::eSensor::RGBD);
    ~ORBSLAM3Python();

    bool initialize();
    pybind11::object processMono(cv::Mat image, double timestamp);
    pybind11::object processStereo(cv::Mat leftImage, cv::Mat rightImage, double timestamp);
    pybind11::object processStereoIMU(cv::Mat leftImage, cv::Mat rightImage, double timestamp, std::vector<std::vector<float>> imuData);
    void reset();
    void shutdown();
    bool isRunning();
    bool isLost();
    bool mapChanged();
    int getTrackingState();
    void setUseViewer(bool useViewer);
    std::vector<Eigen::Matrix4f> getTrajectory() const;
    pybind11::object getCurrentPose() const;

private:
    std::string vocabluaryFile;
    std::string settingsFile;
    ORB_SLAM3::System::eSensor sensorMode;
    std::shared_ptr<ORB_SLAM3::System> system;
    bool bUseViewer;
    bool bUseRGB;
};

#endif // ORB_SLAM3_PYTHON_H