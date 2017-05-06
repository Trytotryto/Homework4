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

void Simplify::compute_QEM_of_face()//һ�μ�����ɣ�����������������һ�������ĵ��á�
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
		//vĬ������������
		//M = v.transpose()*v;
		//�п��ܳ�����
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
		
		//��������һ����ڣ����ڼ������ŵ�vertex�ĵ�
		//compute_error_and_target_vertex_per_edge(it.handle(),qem_error);
		//����ʹ���е������档
		auto mid_point = (my_mesh->point(vert1) + my_mesh->point(vert2)) / 2;
		v[0] = mid_point[0]; v[1] = mid_point[1]; v[2] = mid_point[2]; v[3] = 1.;
		double qem_error = v.dot(M*v);
		//���ϵ����ñ������ɺ���compute_error_and_target_vertex_per_edge(it.handle(),qem_error);���е�

		vme_error.push_back(qem_error);
	}
}

//step 2
//void select_valid_pairs();//����ʲô���ݽṹ���洢
void Simplify::compute_the_EdgeQEM()
{
	e_QEM eq;
	for (auto it = my_mesh->edges_begin(); it != my_mesh->edges_end(); ++it)
	{
		//����һ���µĽṹ��ָ�룬��Ϊ������ڴ档
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
//void Simplify::compute_cost_and_target_vertex_per_pair(/* parameters */);//���������pairs,�������ʾ��//����mapӦ�ÿ��Խ���������
//void Simplify::compute_cost_and_target_vertex_of_all_pairs();//��α���pairs��//���ö� //pairs������洢��

//����whileѭ�����Ƴ�pairs
void Simplify::simplify_mesh()
{

}
