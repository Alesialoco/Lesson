#ifndef POINTCLOUD_PREPROCESSOR__FILTER_HPP_
#define POINTCLOUD_PREPROCESSOR__FILTER_HPP_

#include "Logger.h"
#include "PointCloud.h"

#include <string>
#include <cmath>
#include <unordered_map>
#include <memory>

namespace pointcloud_preprocessor
{

class FilterParametr {
public:
    FilterParametr() {}
    FilterParametr(std::unordered_map<std::string, double> params) : params_(std::move(params)) {}
    
    // Add new param or change exsistent
    void ChangeParam(const std::string& param_name, double param_val) {
        params_[param_name] = param_val;
    }

    double GetParam(const std::string& param_name) const {
        return params_.at(param_name);
    }

    double GetParam(const std::string& param_name, double default_val) const noexcept {
        auto it = params_.find(param_name);
        if (it == params_.end())
            return default_val;

        return it->second;
    }
private:
    std::unordered_map<std::string, double> params_;
};

/** \brief @b Filter represents the base filter class. 
 */
template<PointCloudType T>
class Filter
{
public:
  Filter(const std::string & filter_name = "pointcloud_preprocessor_filter") : filter_name_(filter_name), logger_(filter_name_) {}
  
  virtual ~Filter() = default;

  const std::string& GetFilterName() const { return filter_name_; }

  virtual std::unique_ptr<PointCloud<T>> Apply(PointCloud<T>* pc) = 0;
  
  virtual void SetParams(const FilterParametr& param) {
      params_ = std::move(param);
  }

  const FilterParametr& GetFilterParam() const {
      return params_;
  }
protected:
    std::string filter_name_ = "pointcloud_preprocessor_filter";
    Logger logger_;
    FilterParametr params_;
};

template<PointCloudType T>
class WithDistanceAzimuthFilter : public Filter<T> {
public:
    WithDistanceAzimuthFilter(const std::string& filter_name = "DistanceAzimuthFilter")
        : Filter<T>(filter_name) {}
    
    virtual ~WithDistanceAzimuthFilter() = default;
    
    double GetDistance(const PointCloud<T>* pc, size_t index) const {
      if (T == PointCloudType::XYZIR) {
          double x = pc->getPoints()[index * pc->getPointSize() + 0];
          double y = pc->getPoints()[index * pc->getPointSize() + 1];
          double z = pc->getPoints()[index * pc->getPointSize() + 2];
          return std::hypot(x, y, z);
      }
      else
        return pc->getPoints()[index * pc->getPointSize() + 5];
    }
    
    double GetAzimuth(const PointCloud<T>* pc, size_t index) const {
      if (T == PointCloudType::XYZIR) {
          double x = pc->getPoints()[index * pc->getPointSize() + 0];
          double y = pc->getPoints()[index * pc->getPointSize() + 1];
          return std::atan2(y, x);
      }
      else
        return pc->getPoints()[index * pc->getPointSize() + 6];
    }
};

}  // namespace pointcloud_preprocessor

#endif  // POINTCLOUD_PREPROCESSOR__FILTER_HPP_
