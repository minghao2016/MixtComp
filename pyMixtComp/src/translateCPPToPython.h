//
//  translateCPPtoPython.hpp
//  MixtComp
//
//  Created by Leo Perard on 09/01/2019.
//
//

#ifndef PYMIXTCOMP_SRC_TRANSLATECPPTOPYTHON_H
#define PYMIXTCOMP_SRC_TRANSLATECPPTOPYTHON_H

#include <iostream>
#include <boost/python.hpp>
#include <boost/python/dict.hpp>
#include <IO/NamedAlgebra.h>
#include <LinAlg/LinAlg.h>
#include <LinAlg/names.h>
#include <boost/python/numpy.hpp>

namespace mixt {

template <typename InType>
void translateCPPToPython(const InType& in, const std::string& name, boost::python::dict& out) {
	out[name] = in;
}



template <typename T>
void translateCPPToPython(const NamedVector<T>& in, const std::string& name, boost::python::dict& out) {
	out[name] = boost::python::dict();

	Index nrow = in.vec_.size();

	boost::python::numpy::dtype dt = boost::python::numpy::dtype::get_builtin<T>();
	boost::python::tuple shape = boost::python::make_tuple(nrow);
	boost::python::tuple stride = boost::python::make_tuple(sizeof(T));
	boost::python::object own;
	out[name]["data"] = boost::python::numpy::from_data(in.vec_.data(), dt, shape, stride, own);

	boost::python::list rowNames;
	for(auto& s: in.rowNames_){
		rowNames.append(s);
	}

	out[name]["rowNames"] = boost::python::numpy::array(rowNames);
	out[name]["nrow"] = nrow;
	out[name]["dtype"] = names<T>::name;
	out[name]["ctype"] = "Vector";
}


template <typename T>
void translateCPPToPython(const NamedMatrix<T>& in, const std::string& name, boost::python::dict& out) {
	out[name] = boost::python::dict();
	Index nrow = in.mat_.rows();
	Index ncol = in.mat_.cols();
	std::vector<std::vector<T>> data(nrow, std::vector<T>(ncol));

	for (Index i = 0; i < nrow; ++i) {
		for (Index j = 0; j < ncol; ++j) {
			data[i][j] = in.mat_(i, j);
		}
	}

	out[name]["colNames"] = in.colNames_;
	out[name]["ctype"] = "Matrix";
	out[name]["data"] = data;
	out[name]["dtype"] = names<T>::name;
	out[name]["rowNames"] = in.rowNames_;
	out[name]["ncol"] = ncol;
	out[name]["nrow"] = nrow;
}


}  // namespace mixt

#endif /* PYMIXTCOMP_SRC_TRANSLATECPPTOPYTHON_H */