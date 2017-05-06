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
	//关于重载operator<,不清楚
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
	void compute_QEM_of_face();//一次计算完成，储存起来，方面下一个函数的调用。
	void compute_QEM_of_vertex();//至于是需要计算
	void compute_error_and_target_vertex_per_edge(const OpenMesh::EdgeHandle &, double &);
	void compute_QEM_of_edge();

	//step 2
	//void select_valid_pairs();//利用什么数据结构来存储
	void compute_the_EdgeQEM();
	void initialization();

	//step 3
	//void compute_cost_and_target_vertex_per_pair(/* parameters */);//这里参数是pairs,如何来表示？//利用map应该可以解决这个问题
	//void compute_cost_and_target_vertex_of_all_pairs();//如何遍历pairs？//利用堆 //pairs如何来存储？

	//利用while循环来移除pairs
	void simplify_mesh();

private:
	MyMesh								*my_mesh;
	std::vector<Eigen::Matrix4d>        vmf;
	std::vector<Eigen::Matrix4d>        vmv;
	std::vector<Eigen::Matrix4d>        vme;
	std::vector<double>					vme_error;
	//这里定义了一个指向容器队列的指针。
	//面对指针，不可随意使用const
	std::priority_queue<e_QEM>			*qpq;
};

