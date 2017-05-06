#pragma once
#include <iostream>
#include <OpenMesh\Core\Mesh\PolyConnectivity.hh>
#include <OpenMesh\Core\IO\MeshIO.hh>
#include <OpenMesh\Core\Mesh\TriMesh_ArrayKernelT.hh>

#include <queue>
#include <functional>

#include <Eigen\Dense>
#include <Eigen\SparseLU>

typedef OpenMesh::TriMesh_ArrayKernelT<>  MyMesh;

typedef struct EdgeQEM *e_QEM;
struct EdgeQEM{
	MyMesh::EdgeHandle _eh;
	double qem_error;
	//��������operator<,�����
	inline friend bool operator<(const EdgeQEM & lhs, const EdgeQEM & rhs) { return (lhs.qem > rhs.qem); }
	inline friend bool operator>(const EdgeQEM & lhs, const EdgeQEM & rhs) { return (lhs.qem < rhs.qem); }
};

class Simplify
{
public:
	Simplify();
	Simplify(MyMesh *);
	~Simplify();

public:
	//step 1
	void compute_QEM_of_face();//һ�μ�����ɣ�����������������һ�������ĵ��á�
	void compute_QEM_of_vertex();//��������Ҫ����
	void compute_error_and_target_vertex_per_edge(const OpenMesh::EdgeHandle &, double &);
	void compute_QEM_of_edge();

	//step 2
	//void select_valid_pairs();//����ʲô���ݽṹ���洢
	void compute_the_EdgeQEM();
	void initialization();

	//step 3
	//void compute_cost_and_target_vertex_per_pair(/* parameters */);//���������pairs,�������ʾ��//����mapӦ�ÿ��Խ���������
	//void compute_cost_and_target_vertex_of_all_pairs();//��α���pairs��//���ö� //pairs������洢��

	//����whileѭ�����Ƴ�pairs
	void simplify_mesh();

private:
	MyMesh								*my_mesh;
	std::vector<Eigen::Matrix4d>        vmf;
	std::vector<Eigen::Matrix4d>        vmv;
	std::vector<Eigen::Matrix4d>        vme;
	std::vector<double>					vme_error;
	//���ﶨ����һ��ָ���������е�ָ�롣
	//���ָ�룬��������ʹ��const
	std::priority_queue<e_QEM>			*qpq;
};

