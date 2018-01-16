#ifndef VAST_HPP_GFX_RES_MESH
#define VAST_HPP_GFX_RES_MESH

// Local
#include <util/result.hpp>

// Lib
#include <glm/glm.hpp>
#include <glbinding/gl/types.h>

// Std
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <initializer_list>

namespace vast::gfx::res
{
	// A single vertex
	struct Vert
	{
		glm::vec3 pos, col, norm;
		glm::vec2 uv;

		Vert() : pos(glm::vec3(0)), col(glm::vec3(0)), norm(glm::vec3(0)), uv(glm::vec2(0)){}
		Vert(glm::vec3 pos, glm::vec3 col, glm::vec3 norm, glm::vec2 uv) :
			pos(pos),
			col(col),
			norm(norm),
			uv(uv)
		{}
	};

	// A single polygon
	struct Poly
	{
		Vert v0, v1, v2;

		void compute_normal()
		{
			glm::vec3 n = glm::cross(this->v1.pos - this->v0.pos, this->v2.pos - this->v0.pos);
			glm::normalize(n);
			this->v0.norm = this->v1.norm = this->v2.norm = n;
		}

		Poly() {}
		Poly(Vert v0, Vert v1, Vert v2) : v0(v0), v1(v1), v2(v2) {}
	};

	struct OBJMesh
	{
		std::vector<glm::vec3> verts;
		std::vector<glm::vec3> norms;
		std::vector<glm::vec2> uvs;
		std::vector<std::array<glm::ivec3, 3>> polys;

		enum class Error { CANNOT_OPEN_FILE, INVALID_LINE };

		size_t size() { return this->polys.size(); }

		Poly get_polygon(long i)
		{
			std::array<glm::ivec3, 3> ind = this->polys[i];

			Poly p;

			// Set positions
			if (ind[0].x >= 0) p.v0.pos = this->verts[ind[0].x];
			if (ind[0].y >= 0) p.v1.pos = this->verts[ind[0].y];
			if (ind[0].z >= 0) p.v2.pos = this->verts[ind[0].z];

			// Set texture coordinates
			if (ind[1].x >= 0) p.v0.uv = this->uvs[ind[1].x];
			if (ind[1].y >= 0) p.v1.uv = this->uvs[ind[1].y];
			if (ind[1].z >= 0) p.v2.uv = this->uvs[ind[1].z];

			// Set normals
			if (ind[2].x >= 0) p.v0.norm = this->norms[ind[2].x];
			if (ind[2].y >= 0) p.v1.norm = this->norms[ind[2].y];
			if (ind[2].z >= 0) p.v2.norm = this->norms[ind[2].z];

			p.v0.col = p.v1.col = p.v2.col = glm::vec3(1);

			return p;
		}

		util::Status<Error> parse(std::string const& line)
		{
			float f0, f1, f2;
			long l0, l1, l2, l3, l4, l5, l6, l7, l8;

			if (line[0] == '#')
				return util::Status<Error>::success();
				// Try vertex coords format
			else if (std::sscanf(line.c_str(), "v %f %f %f", &f0, &f1, &f2) >= 3)
				this->verts.push_back(glm::vec3(f0, f1, f2));
			// Try vertex textcoords format
			// else if (std::sscanf(line.c_str(), "vt %f %f %f", &f0, &f1, &f2) >= 3)
			// 	this->uvs.push_back(glm::vec3(f0, f1, f2));
			// Try vertex textcoords format
			else if (std::sscanf(line.c_str(), "vt %f %f", &f0, &f1) >= 2)
				this->uvs.push_back(glm::vec2(f0, f1));
			// Try vertex normal format
			else if (std::sscanf(line.c_str(), "vn %f %f %f", &f0, &f1, &f2) >= 3)
				this->norms.push_back(glm::vec3(f0, f1, f2));
			// Try face format 1
			else if (std::sscanf(line.c_str(), "f %li %li %li", &l0, &l1, &l2) >= 3)
				this->polys.push_back({glm::ivec3(l0 - 1, l1 - 1, l2 - 1), glm::ivec3(-1, -1, -1), glm::ivec3(-1, -1, -1)});
			// Try face format 2
			else if (std::sscanf(line.c_str(), "f %li/%li/%li %li/%li/%li %li/%li/%li", &l0, &l1, &l2, &l3, &l4, &l5, &l6, &l7, &l8) >= 3)
				this->polys.push_back({glm::ivec3(l0 - 1, l3 - 1, l6 - 1), glm::ivec3(l1 - 1, l4 - 1, l7 - 1), glm::ivec3(l2 - 1, l5 - 1, l8 - 1)});
			// Try face format 3
			else if (std::sscanf(line.c_str(), "f %li//%li %li//%li %li//%li", &l0, &l1, &l2, &l3, &l4, &l5) >= 6)
				this->polys.push_back({glm::ivec3(l0 - 1, l2 - 1, l4 - 1), glm::ivec3(-1, -1, -1), glm::ivec3(l1 - 1, l3 - 1, l5 - 1)});
			// Try face format 4
			else if (std::sscanf(line.c_str(), "f %li/%li %li/%li %li/%li", &l0, &l1, &l2, &l3, &l4, &l5) >= 6)
				this->polys.push_back({glm::ivec3(l0 - 1, l2 - 1, l4 - 1), glm::ivec3(l1 - 1, l3 - 1, l5 - 1), glm::ivec3(-1, -1, -1)});

			return util::Status<Error>::success();
		}

		util::Status<OBJMesh::Error> read_from(std::string const& filename)
		{
			std::ifstream filestream(filename);
			std::stringstream line;

			if (!filestream.is_open())
				return util::Status<OBJMesh::Error>::failure(Error::CANNOT_OPEN_FILE);

			while (filestream.good())
			{
				char c = filestream.get();

				switch (c)
				{
				case '\n':
					if (auto r = this->parse(line.str()))
					{
						line.str(std::string());
						break;
					}
					else
						return util::Status<OBJMesh::Error>::failure(Error::INVALID_LINE);

				default:
					line << c;
					break;
				}
			}

			return util::Status<OBJMesh::Error>::success();
		}
	};

	struct Mesh
	{
		std::vector<Poly> _polys;

		size_t size() const { return this->_polys.size(); }

		void add(Poly p) { this->_polys.push_back(p); }
		void add(std::initializer_list<Poly> polys)
		{
			for (auto p : polys)
				this->_polys.push_back(p);
		}

		// TODO: Make this nicer
		gl::GLfloat const* get_data() const { return reinterpret_cast<gl::GLfloat const*>(&this->_polys[0]); }
		size_t get_vertex_count() const { return this->_polys.size() * 3; }

		Mesh() {}
		Mesh(OBJMesh& objmesh)
		{
			for (size_t i = 0; i < objmesh.size(); i ++)
			{
				// TODO: Remove this
				auto p = objmesh.get_polygon(i);
				p.compute_normal();
				this->add(p);
			}
		}

		static util::Result<Mesh, OBJMesh::Error> from(std::string const& filename)
		{
			OBJMesh obj;

			if (auto r = obj.read_from(filename))
				return util::Result<Mesh, OBJMesh::Error>::success(Mesh(obj));
			else
				return util::Result<Mesh, OBJMesh::Error>::failure(r.get_error());
		}
	};
}

#endif
