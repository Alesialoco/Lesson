#ifndef POINTCLOUD_H_
#define POINTCLOUD_H_

#include <string>
#include <stdexcept>
#include <vector>

// The lidar's pointcloud contains coordinates of point (X, Y and Z) and Intencity of ret beam for point (I) and beam ring (R)
// Also it can contains Distance (D), Azimuth (A) and Timestamp (T)

enum class PointCloudType : size_t {
    XYZIR = 5,
    XYZIRDAT = 8
};

template<PointCloudType T>
class PointCloud {
public:
	PointCloud() = default;
    PointCloud(const PointCloud&) = default;
    PointCloud(PointCloud&&) noexcept = default;
    PointCloud& operator=(const PointCloud&) = default;
    PointCloud& operator=(PointCloud&&) noexcept = default;
	~PointCloud() = default;

	void Init(size_t points_count) {
        size_ = points_count;
        point_size_ = static_cast<size_t>(T);
        points_ = std::vector<double>(size_ * point_size_);
    }


	void AddPoint(const std::vector<double>& point, size_t ind) {
        if (point.size() != point_size_) {
            throw std::runtime_error("Point has unsuported size = " + std::to_string(point.size()) + " for type " + std::to_string(point_size_));
        }
        
        for (size_t i = 0; i < point.size(); ++i) {
			points_[point_size_ * ind + i] = point[i];
		}
    }


	PointCloudType getType() const { 
		return T; 
	}
    size_t getPointSize() const { 
		return static_cast<size_t>(T); 
	}
    const std::string getTypeString() const {
        if (T == PointCloudType::XYZIR) return "XYZIR";
		else return "XYZIRDAT";
    }

    size_t getSize() const { 
		return size_; 
	}
    const std::vector<double>& getPoints() const { 
		return points_; 
	}

	void setSize(size_t size) {
        size_ = size;
        points_.resize(size_ * point_size_);
    }
    
    void setPoints(std::vector<double>&& points) {
        if (points.size() != size_ * point_size_) {
            throw std::runtime_error("Points size did not match");
        }
        points_ = std::move(points);
    }

private:
	size_t size_ = 0;
	size_t point_size_ = static_cast<size_t>(T);
	std::vector<double> points_;
};

// For tests
template<PointCloudType T>
inline void FillPointCloud(PointCloud<T>* pc, size_t points_count, const std::vector<double>& data) {
	size_t point_size = static_cast<size_t>(T);
	pc->Init(points_count);
	size_t ind = 0;
	for (size_t i = 0; i < data.size(); i += point_size) {
		pc->AddPoint(std::vector<double>{data.begin() + point_size * ind, data.begin() + point_size * (ind + 1)}, ind);
		ind++;
	}
}

#endif //POINTCLOUD_H_