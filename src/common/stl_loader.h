#include <npe.h>

#include <string>
#include <fstream>
#include <unordered_map>

#include <igl/readSTL.h>


std::unordered_map<std::string, pybind11::object> load_mesh_stl(const std::string& filename) {
    Eigen::MatrixXd v, n;
    Eigen::MatrixXi f;

    std::ifstream file;
    file.open(filename);
    if (file.fail()) {
        throw pybind11::value_error("Failed to open file " + filename);
    }

    igl::readSTL(file, v, f, n);
    
    std::unordered_map<std::string, pybind11::object> vertex_ret;
    vertex_ret.insert(std::make_pair("positions", npe::move(v)));
    if (n.rows() > 0) {
        vertex_ret.insert(std::make_pair("normals", npe::move(n)));
    }

    std::unordered_map<std::string, pybind11::object> face_ret;
    face_ret.insert(std::make_pair("vertex_ids", npe::move(f)));

    std::unordered_map<std::string, pybind11::object> ret;
    ret["vertex_data"] = pybind11::cast(vertex_ret);
    ret["face_data"] = pybind11::cast(face_ret);   
    ret["textures"] = pybind11::cast(std::vector<std::string>());
    ret["normal_maps"] = pybind11::cast(std::vector<std::string>());

    return ret;
}