#include "simplify.h"


Simplify::Simplify()
{
}

Simplify::Simplify(MyMesh *my_mesh_)
{
	my_mesh = my_mesh_;
}

Simplify::~Simplify()
{
}

void Simplify::compute_QEM_of_face()//一次计算完成，储存起来，方面下一个函数的调用。
{
	Eigen::Matrix4d M;
	for (auto it = my_mesh->faces_begin(); it != my_mesh->faces_end(); ++it)
	{
		auto norm = my_mesh->normal(it.handle());
		auto _vvh = my_mesh->fv_begin(it.handle()).handle();
		auto vert = my_mesh->point(_vvh);

		Eigen::Vector4d v;
		v[0] = norm[0]; v[1] = norm[1]; v[2] = norm[2]; 
		v[3] = - norm | vert;
		v.normalize();
		//v默认是列向量。
		//M = v.transpose()*v;
		//有可能出问题
		M = v*v.transpose();
		vmf.push_back(M);
	}
}

void Simplify::compute_QEM_of_vertex()
{
	Eigen::Matrix4d M;
	M.resize(4);
	for (auto it = my_mesh->vertices_begin(); it != my_mesh->vertices_end(); ++it)
	{
		for (auto it1 = my_mesh->vf_begin(*it); it1 != my_mesh->vf_end(*it); ++it1)
			M += vmf.at(it1.handle().idx);
		vmv.push_back(M);
	}
}

void compute_error_and_target_vertex_per_edge(const OpenMesh::EdgeHandle &_eh, double &qem_error)
{

}

void Simplify::compute_QEM_of_edge()
{
	Eigen::Matrix4d M;
	M.resize(4);
	double qem_error;
	Eigen::Vector4d v;
	for (auto it = my_mesh->edges_begin(); it != my_mesh->edges_end(); ++it)
	{
		auto _heh = my_mesh->halfedge_handle(it.handle(), 0);
		auto vert1 = my_mesh->from_vertex_handle(_heh);
		auto vert2 = my_mesh->to_vertex_handle(_heh);
		M += vmv.at(vert1.idx());
		M += vmv.at(vert2.idx());
		vme.push_back(M);
		
		//在这里留一个借口，用于计算最优的vertex的点
		//compute_error_and_target_vertex_per_edge(it.handle(),qem_error);
		//暂且使用中点来代替。
		auto mid_point = (my_mesh->point(vert1) + my_mesh->point(vert2)) / 2;
		v[0] = mid_point[0]; v[1] = mid_point[1]; v[2] = mid_point[2]; v[3] = 1.;
		double qem_error = v.dot(M*v);
		//以上的作用本来该由函数compute_error_and_target_vertex_per_edge(it.handle(),qem_error);来承担

		vme_error.push_back(qem_error);
	}
}

//step 2
//void select_valid_pairs();//利用什么数据结构来存储
void Simplify::compute_the_EdgeQEM()
{
	e_QEM eq;
	for (auto it = my_mesh->edges_begin(); it != my_mesh->edges_end(); ++it)
	{
		//申请一个新的结构体指针，并为其分配内存。
		eq = new EdgeQEM();
		auto _eh = it.handle();
		int id = _eh.idx();
		eq->_eh = _eh;
		eq->qem_error = vme_error.at(id);

		qpq->push(eq);
	}
}
void Simplify::initialization()
{
	compute_QEM_of_face();
	compute_QEM_of_vertex();
	compute_QEM_of_edge();
	compute_the_EdgeQEM();
}

//step 3
//void Simplify::compute_cost_and_target_vertex_per_pair(/* parameters */);//这里参数是pairs,如何来表示？//利用map应该可以解决这个问题
//void Simplify::compute_cost_and_target_vertex_of_all_pairs();//如何遍历pairs？//利用堆 //pairs如何来存储？

//利用while循环来移除pairs
void Simplify::simplify_mesh()
{

}
