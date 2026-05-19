

#ifndef POINTCLOUD_PREPROCESSOR__CROP_BOX_FILTER_HPP_
#define POINTCLOUD_PREPROCESSOR__CROP_BOX_FILTER_HPP_

#include "../filter.hpp"

#include <vector>
#include <cstring>

namespace pointcloud_preprocessor
{
template<PointCloudType T>
class CropBoxFilter : public Filter<T>
{

private:
  struct CropBoxParam
  {
    double min_x{ 0.0 };
    double max_x{ 0.0 };
    double min_y{ 0.0 };
    double max_y{ 0.0 };
    double min_z{ 0.0 };
    double max_z{ 0.0 };
    bool negative{false};
  } param_;

public:
  CropBoxFilter()
  : Filter<T>("CropBoxFilter")
  {
  }


  std::unique_ptr<PointCloud<T>> Apply(PointCloud<T>* pc)
  {
    size_t output_size = 0, output_points_count = 0;
    std::vector<double> output(pc->getSize() * pc->getPointSize());

    for (size_t global_offset = 0; global_offset + pc->getPointSize() <= pc->getSize() * pc->getPointSize();
        global_offset += pc->getPointSize()) {
      std::vector<double> point(3);
      std::memcpy(&point[0], &pc->getPoints()[global_offset + 0], sizeof(double));
      std::memcpy(&point[1], &pc->getPoints()[global_offset + 1], sizeof(double));
      std::memcpy(&point[2], &pc->getPoints()[global_offset + 2], sizeof(double));

      if (!std::isfinite(point[0]) || !std::isfinite(point[1]) || !std::isfinite(point[2])) {
        this->logger_.log("Ignoring point containing NaN values");
        continue;
      }

      bool point_is_inside = point[2] > param_.min_z && point[2] < param_.max_z &&
                            point[1] > param_.min_y && point[1] < param_.max_y &&
                            point[0] > param_.min_x && point[0] < param_.max_x;
      if ((!param_.negative && point_is_inside) || (param_.negative && !point_is_inside)) {
        memcpy(&output[output_size], &pc->getPoints()[global_offset], pc->getPointSize() * sizeof(double));
        output_size += pc->getPointSize();
        output_points_count += 1;
      }
    }

    output.resize(output_size);
    auto output_pc = std::make_unique<PointCloud<T>>();
    output_pc->Init(output_points_count);
    output_pc->setPoints(std::move(output));
    return output_pc;
  }



  void SetParams(const FilterParametr& param){
    CropBoxParam new_param{};

    new_param.min_x = param.GetParam("min_x", new_param.min_x);
    new_param.max_x = param.GetParam("max_x", new_param.max_x);
    new_param.min_y = param.GetParam("min_y", new_param.min_y);
    new_param.max_y = param.GetParam("max_y", new_param.max_y);
    new_param.min_z = param.GetParam("min_z", new_param.min_z);
    new_param.max_z = param.GetParam("max_z", new_param.max_z);
    new_param.negative = static_cast<bool>(param.GetParam("negative", param_.negative ? 1 : 0));
    if (
      new_param.min_x < new_param.max_x ||
      new_param.min_y < new_param.max_y ||
      new_param.min_z < new_param.max_z) {
      if (
        param_.min_x != new_param.min_x || param_.max_x != new_param.max_x ||
        param_.min_y != new_param.min_y || param_.max_y != new_param.max_y ||
        param_.min_z != new_param.min_z || param_.max_z != new_param.max_z ||
        param_.negative != new_param.negative) {
        this->logger_.log("[paramCallback] Setting the minimum point to: " +
          std::to_string(new_param.min_x) + " " + std::to_string(new_param.min_y) + " " + std::to_string(new_param.min_z));
        this->logger_.log("[paramCallback] Setting the maximum point to: " +
          std::to_string(new_param.max_x) + " " + std::to_string(new_param.max_y) + " " + std::to_string(new_param.max_z));
        this->logger_.log("[paramCallback] Setting the filter negative flag to: " + std::string(new_param.negative ? "true" : "false"));
        this->param_ = new_param;
      }
    }
    Filter<T>::SetParams(param);
  }
};
}  // namespace pointcloud_preprocessor

#endif  // POINTCLOUD_PREPROCESSOR__CROP_BOX_FILTER_HPP_
