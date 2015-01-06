/*
 * main.cpp
 *
 *  Created on: Jan 5, 2015
 *      Author: Greg Silverman
 */

namespace std {};

using namespace std;

#include <iostream>
#include <memory>
#include <vector>

// 0 1 2 3 4 5 6
namespace gbs
{


	template<typename T>
	class PivotMethodA
	{
	public:
		virtual ~PivotMethodA() {}
		virtual int operator()(T *p_a,int p_lo,int p_hi)=0;
	};

	template<typename T>
	class MidpointPivotMethod : public PivotMethodA<T>
	{
	public:
		virtual ~MidpointPivotMethod() {}
		virtual int operator()(T *p_a,int p_lo,int p_hi)
		{
			return p_lo+(p_hi-p_lo)/2;
		}
	};

	template<typename T>
	class EndpointPivotMethod : public PivotMethodA<T>
	{
	public:
		virtual ~EndpointPivotMethod() {}
		virtual int operator()(T *p_a,int p_lo,int p_hi)
		{
			return p_lo;
		}
	};

	template<typename T>
	class BeginningpointPivotMethod : public PivotMethodA<T>
	{
	public:
		virtual ~BeginningpointPivotMethod() {}
		virtual int operator()(T *p_a,int p_lo,int p_hi)
		{
			return p_hi;
		}
	};

	template<typename T>
	class PartitionMethodA
	{
	public:
		virtual ~PartitionMethodA() {}
		virtual int operator()(T* p_a,int p_lo,int p_hi,PivotMethodA<T>& p_pivot_method) = 0;
	};

	template<typename T>
	class PartitionMethod1 : public PartitionMethodA<T>
	{
	public:
		virtual ~PartitionMethod1() {}
		virtual int operator()(T* p_a,int p_lo,int p_hi,PivotMethodA<T>& p_pm)
		{
			int pivot_index = p_pm(p_a,p_lo,p_hi);
			T   pivot_value = p_a[pivot_index];

			T temp = p_a[pivot_index];
			p_a[pivot_index] = p_a[p_hi];
			p_a[p_hi]        = temp;

			int store_index = p_lo;

			for(int i=p_lo;i<(p_hi);++i)
			{
				if(p_a[i]<pivot_value)
				{
					T temp = p_a[i];
					p_a[i] = p_a[store_index];
					p_a[store_index] = temp;
					++store_index;
				}
			}

			temp = p_a[store_index];
			p_a[store_index] = p_a[p_hi];
			p_a[p_hi] = temp;

			return store_index;
		}
	};

	template<typename T>
	class PartitionMethod2 : public PartitionMethodA<T>
	{
	public:
		virtual ~PartitionMethod2() {}
		virtual int operator()(T* p_a,int p_lo,int p_hi,PivotMethodA<T>& p_pivot_method)
		{
			int pivot_index = p_pivot_method(p_a,p_lo,p_hi);
			T   pivot_value = p_a[pivot_index];

			int l=p_lo;
			int u=p_hi;
			while(l<u)
			{
				while(l<pivot_index && p_a[l]<pivot_value)
				{
					++l;
				}
				while(u>pivot_index && p_a[u]>pivot_value)
				{
					--u;
				}
				T tmp = p_a[l];
				p_a[l] = p_a[u];
				p_a[u] = tmp;
				++l;
				--u;
			}

			return pivot_index;
		}
	};

	template<typename T>
	void
	quicksort
		(T* p_a,int p_lo,int p_hi,PartitionMethodA<T>& p_partition_method,PivotMethodA<T>& p_pivot_method)
	{
		if(p_lo<p_hi)
		{
			int p = p_partition_method(p_a,p_lo,p_hi,p_pivot_method);
			quicksort(p_a,p_lo,p-1,p_partition_method,p_pivot_method);
			quicksort(p_a,p+1,p_hi,p_partition_method,p_pivot_method);

		}
	}
}

int
main
  ()
{
	using g_par = gbs::PartitionMethodA<int>;
	using s_par = shared_ptr<g_par>;
	vector<s_par> partition_methods;

	using g_piv = gbs::PivotMethodA<int>;
	using s_piv = shared_ptr<g_piv>;
	vector<s_piv> pivot_methods;

	partition_methods.push_back(s_par(new gbs::PartitionMethod1<int>()));
	partition_methods.push_back(s_par(new gbs::PartitionMethod2<int>()));

	pivot_methods.push_back(s_piv(new gbs::BeginningpointPivotMethod<int>()));
	pivot_methods.push_back(s_piv(new gbs::MidpointPivotMethod<int>()));
	pivot_methods.push_back(s_piv(new gbs::EndpointPivotMethod<int>()));


	for(size_t par=0;par<partition_methods.size();++par)
	{
		for(size_t piv=0;piv<pivot_methods.size();++piv)
		{
			const int N=6;
			int a[6];
			for(int i=0;i<N;++i)
			{
				a[i] = N-i-1;
			}
			gbs::quicksort(&a[0],0,N-1,*partition_methods[par],*pivot_methods[piv]);
			for(int i=0;i<N;++i)
			{
				cout << a[i] << ", ";
			}
			cout << endl;

		}
	}
	return 0;
}


