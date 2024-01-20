#pragma once

class Sphere {
public:
	template <typename Vertex>
	static IndexedTriangleList<Vertex> GetPlainSphere(float radius = 0.5f, int segments = 16, int rings = 8) {
		const float latitude_angle_step = PI / rings;
		const float longitude_angle_step = PI * 2.0f / segments;

		const int total_vertices = segments * (rings - 1) + 2;
		std::vector<Vertex> vertices;
		vertices.reserve(total_vertices);

		// top vertex
		vertices.emplace_back(Vec3{ 0.0f, 0.0f, radius });

		// middle vertices
		for (size_t i = 1; i < rings; i++) {
			const float latitude = i * latitude_angle_step + PI / 2;
			const float z = sin(latitude) * radius;
			const float ring_radius = cos(latitude) * radius;

			for (size_t i = 0; i < segments; i++) {
				const float longitude = i * longitude_angle_step;
				vertices.emplace_back(Vec3{ cos(longitude) * ring_radius, sin(longitude) * ring_radius, z });
			}
		}
		// bottom vertex
		vertices.emplace_back(Vec3{ 0.0f, 0.0f, -radius });

		std::vector<size_t> indices;

		auto calculateIndex = [segments](int iLat, int iLong) {
			return std::max(0, (iLat - 1) * segments + iLong);
		};

		// distribute faces
		for (int iLat = 1; iLat < rings - 1; iLat++) {
			for (int iLong = 1; iLong < segments; iLong++) {
				indices.emplace_back(calculateIndex(iLat, iLong));
				indices.emplace_back(calculateIndex(iLat + 1, iLong));
				indices.emplace_back(calculateIndex(iLat + 1, iLong + 1));

				indices.emplace_back(calculateIndex(iLat, iLong));
				indices.emplace_back(calculateIndex(iLat + 1, iLong + 1));
				indices.emplace_back(calculateIndex(iLat, iLong + 1));
			}
			indices.emplace_back(calculateIndex(iLat + 1, 1));
			indices.emplace_back(calculateIndex(iLat, 1));
			indices.emplace_back(calculateIndex(iLat, segments));

			indices.emplace_back(calculateIndex(iLat + 1, segments));
			indices.emplace_back(calculateIndex(iLat + 1, 1));
			indices.emplace_back(calculateIndex(iLat, segments));
		}

		// sphere top indices
		const int first_index = 0;
		for (int i = 1; i < segments; i++) {
			indices.emplace_back(first_index);
			indices.emplace_back(i);
			indices.emplace_back(i + 1);
		}
		indices.emplace_back(first_index);
		indices.emplace_back(segments);
		indices.emplace_back(1);

		// sphere bottom indices
		const int last_index = total_vertices - 1;
		const int bottom_ring_first_index = segments * (rings - 2) + 1;
		const int bottom_ring_last_index = last_index - 1;
		for (int i = bottom_ring_first_index; i < bottom_ring_last_index; i++) {
			indices.emplace_back(i + 1);
			indices.emplace_back(i);
			indices.emplace_back(last_index);
		}
		indices.emplace_back(last_index);
		indices.emplace_back(bottom_ring_first_index);
		indices.emplace_back(bottom_ring_last_index);

		const Mat3 rot = Mat3::RotationX(PI / 2);
		for (auto& v : vertices) {
			v.pos *= rot;
		}

		return { std::move(vertices) , std::move(indices) };
	}
};