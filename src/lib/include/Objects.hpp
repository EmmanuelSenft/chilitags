
#ifndef Objects_HPP
#define Objects_HPP

#include <string>
#include <map>
#include <unordered_map>

#ifdef WITH_YAML
#include <yaml-cpp/yaml.h>
#endif

#include <opencv2/core/core.hpp>

#include "ObjectConfig.hpp"
#include "Estimator.hpp"


namespace chilitags {

static const float DEFAULT_GAIN=0.9;

class Objects {

public:
    Objects(cv::InputArray cameraMatrix, 
            cv::InputArray distCoeffs, 
            float size,
            float gain = DEFAULT_GAIN);

#ifdef WITH_YAML

    Objects(cv::InputArray cameraMatrix,
            cv::InputArray distCoeffs,
            const std::string& configuration, 
            float defaultSize = 0,
            float gain = DEFAULT_GAIN);

    /**
     *
     * \param defaultSize: default size of markers, to be used for markers 
     * not associated to an object (ie, markers that are not used in the 
     * configuration file). The special value '0.0' (default value) means that
     * ONLY the markers used in the configuration file are tracked.
     */
    Objects(cv::InputArray cameraMatrix,
            cv::InputArray distCoeffs,
            const YAML::Node& configuration, 
            float defaultSize = 0,
            float gain = DEFAULT_GAIN);

#endif

    /** Returns the list of all detected objects with
     * their transformation matrices, in the camera
     * frame.
     */
    std::map<std::string, cv::Matx44d> all() const;

private:
    void init(float size);

    bool isCalibrated;
    mutable double calibError;
    mutable std::vector<std::vector<cv::Point3f>> calibCorners;
    mutable std::vector<std::vector<cv::Point2f>> calibImagePoints;
    cv::InputArray cameraMatrix;
    cv::InputArray distCoeffs;

    void computeTransformation(const std::string& name,
                               const std::vector<cv::Point3f>& corners,
                               const std::vector<cv::Point2f>& imagePoints,
                               std::map<std::string, cv::Matx44d>& objects) const;

    cv::Matx44d transformationMatrix(const cv::Mat& tvec, const cv::Mat& rvec) const;

    float gain;
    bool hasObjectConfiguration;
    std::vector<cv::Point3f> defaultMarkerCorners;

    ObjectConfig _config;
    
    // store, for each tag, an estimator
    mutable std::unordered_map<std::string, Estimator<cv::Mat>> estimatedTranslations;
    mutable std::unordered_map<std::string, Estimator<cv::Mat>> estimatedRotations;

};

}

#endif
