// ----------------------------------------------------------------------------
// -                        Open3D: www.open3d.org                            -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2015 Qianyi Zhou <Qianyi.Zhou@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#pragma once

#include <vector>
#include <Eigen/Core>
#include <Core/Geometry/Geometry2D.h>
#include <Core/Geometry/Image.h>

namespace three {

class PointCloud;
class ViewControl;
class SelectionPolygonVolume;

/// A 2D polygon used for selection on screen
/// It is an utility class for Visualization
/// The coordinates in SelectionPolygon are lower-left corner based (the OpenGL
/// convention).
class SelectionPolygon : public Geometry2D
{
public:
	enum SectionPolygonType {
		POLYGON_UNFILLED = 0,
		POLYGON_RECTANGLE = 1,
		POLYGON_POLYGON = 2,
	};

public:
	SelectionPolygon() : Geometry2D(GEOMETRY_UNSPECIFIED) {}
	~SelectionPolygon() override {}

public:
	void Clear() override;
	bool IsEmpty() const override;
	Eigen::Vector2d GetMinBound() const final;
	Eigen::Vector2d GetMaxBound() const final;
	void FillPolygon(int width, int height);
	void CropGeometry(const Geometry &input, const ViewControl &view,
			Geometry &output);
	std::shared_ptr<SelectionPolygonVolume> CreateSelectionPolygonVolume(
			const ViewControl &view);

private:
	void CropPointCloudInRectangle(const PointCloud &input,
			const ViewControl &view, PointCloud &output);
	void CropPointCloudInPolygon(const PointCloud &input,
			const ViewControl &view, PointCloud &output);
	void CropInRectangle(const std::vector<Eigen::Vector3d> &input,
			const ViewControl &view, std::vector<size_t> &output_index);
	void CropInPolygon(const std::vector<Eigen::Vector3d> &input,
			const ViewControl &view, std::vector<size_t> &output_index);

public:
	std::vector<Eigen::Vector2d> polygon_;
	bool is_closed_ = false;
	Image polygon_interior_mask_;
	SectionPolygonType polygon_type_ = POLYGON_UNFILLED;
};

}	// namespace three