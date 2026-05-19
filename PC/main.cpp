#include "PointCloud.h"
#include "crop_box_filter/crop_box_filter.hpp"

#include <iostream>
#include <string>
#include <memory>

template<PointCloudType T>
std::ostream& operator<<(std::ostream& stream, const PointCloud<T>& pc) {
	stream << "Point cloud (" << pc.getTypeString() << "): " << std::endl;
	for (size_t i = 0; i < pc.getSize(); ++i) {
		for (size_t j = 0; j < pc.getPointSize(); ++j) {
			stream << pc.getPoints()[i * pc.getPointSize() + j] << " ";
		}
		stream << std::endl;
	}
	return stream;
}

int main() {
	try {
		PointCloud<PointCloudType::XYZIR> pc_xyzir;
		PointCloud<PointCloudType::XYZIRDAT> pc_xyzirdat;
		FillPointCloud(&pc_xyzir, 3,
			{ 5.0, 1.2, 2.1, 0.5, 1.0,
			 -3.2, 0.2, 1.1, 0.7, 1.0,
			 2.2, 2.1, 7.0, 0.1, 2.0 });

		std::cout << "We have: " << pc_xyzir << std::endl;

		std::unique_ptr<pointcloud_preprocessor::Filter<PointCloudType::XYZIR>> cbf = std::make_unique<pointcloud_preprocessor::CropBoxFilter<PointCloudType::XYZIR>>();
		cbf->SetParams(pointcloud_preprocessor::FilterParametr({
			{"min_x", -4.0}, {"max_x", -2.0},
			{"min_y", 0.1}, {"max_y", 2.0},
			{"min_z", 0.5}, {"max_z", 3.0}
		}));
		std::unique_ptr<PointCloud<PointCloudType::XYZIR>> out = cbf->Apply(&pc_xyzir);

		std::cout << "after CBF: " << *out << std::endl;
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}